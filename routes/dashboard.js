import express from 'express';

const router = express.Router();

// GET: Dashboard summary
router.get('/dashboard/summary', async (req, res) => {
  try {
    // Get latest sensor reading
    const latestSensor = await req.db.get(
      `SELECT * FROM sensor_data ORDER BY timestamp DESC LIMIT 1`
    );

    // Get today's attendance
    const attendance = await req.db.get(
      `SELECT COUNT(DISTINCT cardId) as presentStudents FROM rfid_logs 
       WHERE DATE(timestamp) = DATE('now')`
    );

    // Get active alerts
    const alerts = await req.db.all(
      `SELECT * FROM alerts WHERE resolved = 0 AND timestamp > datetime('now', '-1 hour')
       ORDER BY timestamp DESC LIMIT 5`
    );

    // Get statistics
    const stats = await req.db.get(`
      SELECT 
        AVG(soundLevel) as avgSound,
        AVG(temperature) as avgTemp,
        AVG(humidity) as avgHumidity,
        AVG(coLevel) as avgCO
      FROM sensor_data
      WHERE timestamp > datetime('now', '-1 hour')
    `);

    res.json({
      latestSensor,
      attendance: attendance?.presentStudents || 0,
      activeAlerts: alerts.length,
      recentAlerts: alerts,
      stats
    });
  } catch (err) {
    console.error('Error fetching dashboard summary:', err);
    res.status(500).json({ error: 'Failed to fetch dashboard summary' });
  }
});

// GET: Alerts with filtering
router.get('/alerts', async (req, res) => {
  try {
    const type = req.query.type;
    const resolved = req.query.resolved || '0';
    const limit = req.query.limit || 100;

    let query = `SELECT * FROM alerts WHERE resolved = ?`;
    let params = [resolved];

    if (type) {
      query += ` AND type = ?`;
      params.push(type);
    }

    query += ` ORDER BY timestamp DESC LIMIT ?`;
    params.push(limit);

    const alerts = await req.db.all(query, params);
    res.json(alerts);
  } catch (err) {
    console.error('Error fetching alerts:', err);
    res.status(500).json({ error: 'Failed to fetch alerts' });
  }
});

// PUT: Resolve alert
router.put('/alerts/:alertId/resolve', async (req, res) => {
  try {
    const { alertId } = req.params;

    await req.db.run(
      `UPDATE alerts SET resolved = 1 WHERE id = ?`,
      [alertId]
    );

    res.json({ success: true, message: 'Alert resolved' });
  } catch (err) {
    console.error('Error resolving alert:', err);
    res.status(500).json({ error: 'Failed to resolve alert' });
  }
});

// GET: System status
router.get('/system/status', async (req, res) => {
  try {
    // Get latest reading timestamp
    const lastReading = await req.db.get(
      `SELECT timestamp FROM sensor_data ORDER BY timestamp DESC LIMIT 1`
    );

    const lastReadingTime = lastReading?.timestamp ? new Date(lastReading.timestamp) : null;
    const now = new Date();
    const minutesSinceLastReading = lastReadingTime 
      ? Math.floor((now - lastReadingTime) / 60000)
      : -1;

    const isOnline = minutesSinceLastReading !== -1 && minutesSinceLastReading < 2;

    res.json({
      status: isOnline ? 'ONLINE' : 'OFFLINE',
      lastReading: lastReadingTime?.toISOString(),
      minutesSinceLastReading,
      timestamp: now.toISOString()
    });
  } catch (err) {
    console.error('Error fetching system status:', err);
    res.status(500).json({ error: 'Failed to fetch system status' });
  }
});

// GET: Chart data (for frontend charts)
router.get('/dashboard/chart-data', async (req, res) => {
  try {
    const hours = req.query.hours || 24;
    
    // Get temperature and humidity data
    const tempHumidData = await req.db.all(
      `SELECT 
        strftime('%Y-%m-%d %H:00:00', timestamp) as hour,
        AVG(temperature) as avgTemp,
        AVG(humidity) as avgHumidity,
        MAX(temperature) as maxTemp,
        MIN(temperature) as minTemp
      FROM sensor_data
      WHERE timestamp > datetime('now', '-' || ? || ' hours')
      GROUP BY strftime('%Y-%m-%d %H:00:00', timestamp)
      ORDER BY hour ASC`,
      [hours]
    );

    // Get sound and CO data
    const pollutantData = await req.db.all(
      `SELECT 
        strftime('%Y-%m-%d %H:00:00', timestamp) as hour,
        AVG(soundLevel) as avgSound,
        AVG(coLevel) as avgCO
      FROM sensor_data
      WHERE timestamp > datetime('now', '-' || ? || ' hours')
      GROUP BY strftime('%Y-%m-%d %H:00:00', timestamp)
      ORDER BY hour ASC`,
      [hours]
    );

    // Log data availability for debugging
    const totalData = await req.db.get(
      `SELECT COUNT(*) as count FROM sensor_data`
    );
    
    console.log(`Chart data request: ${tempHumidData.length} temp records, ${pollutantData.length} pollutant records (Total DB records: ${totalData.count})`);

    res.json({
      tempHumidData,
      pollutantData,
      period: `${hours} hours`,
      debug: {
        tempRecords: tempHumidData.length,
        pollutantRecords: pollutantData.length,
        totalDatabaseRecords: totalData.count
      }
    });
  } catch (err) {
    console.error('Error fetching chart data:', err);
    res.status(500).json({ error: 'Failed to fetch chart data' });
  }
});

// GET: Export data as CSV
router.get('/dashboard/export', async (req, res) => {
  try {
    const format = req.query.format || 'csv';
    const hours = req.query.hours || 24;

    const data = await req.db.all(
      `SELECT * FROM sensor_data
       WHERE timestamp > datetime('now', '-' || ? || ' hours')
       ORDER BY timestamp DESC`,
      [hours]
    );

    if (format === 'csv') {
      let csv = 'Timestamp,Device ID,Sound Level,Temperature,Humidity,CO Level,Signal Strength\n';
      data.forEach(row => {
        csv += `${row.timestamp},${row.deviceId},${row.soundLevel},${row.temperature},${row.humidity},${row.coLevel},${row.rssi}\n`;
      });
      res.setHeader('Content-Type', 'text/csv');
      res.setHeader('Content-Disposition', 'attachment; filename=sensor_data.csv');
      res.send(csv);
    } else {
      res.json(data);
    }
  } catch (err) {
    console.error('Error exporting data:', err);
    res.status(500).json({ error: 'Failed to export data' });
  }
});

export default router;
