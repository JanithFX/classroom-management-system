import express from 'express';

const router = express.Router();

// POST: Receive sensor data from ESP32
router.post('/sensor-data', async (req, res) => {
  try {
    const {
      deviceId,
      soundLevel,
      temperature,
      humidity,
      coLevel,
      rfidCard,
      timestamp,
      rssi
    } = req.body;

    // Insert sensor data into database
    await req.db.run(
      `INSERT INTO sensor_data 
       (deviceId, soundLevel, temperature, humidity, coLevel, rssi, timestamp)
       VALUES (?, ?, ?, ?, ?, ?, ?)`,
      [deviceId, soundLevel, temperature, humidity, coLevel, rssi, timestamp]
    );

    // Check for alerts
    await checkAndCreateAlerts(req.db, {
      soundLevel,
      temperature,
      humidity,
      coLevel
    });

    console.log(`Data received from ${deviceId} at ${timestamp}`);

    // if an RFID card UID was included in the sensor payload, create an
    // attendance log entry so the device itself can drive the student
    // attendance management.  This mirrors the behaviour of the dedicated
    // POST /rfid/log endpoint and provides a fallback when the badge is
    // sent along with the regular sensor update.
    if (rfidCard && rfidCard.length > 0) {
      try {
        const card = await req.db.get(
          `SELECT * FROM rfid_cards WHERE rfidUid = ?`,
          [rfidCard]
        );
        if (card) {
          await req.db.run(
            `INSERT INTO rfid_logs (cardId, action) VALUES (?, 'ENTRY')`,
            [card.cardId]
          );
          console.log(`Attendance logged for card ${rfidCard}`);
        } else {
          console.warn(`Unknown RFID card in sensor payload: ${rfidCard}`);
        }
      } catch (logErr) {
        console.error('Error logging attendance from sensor payload:', logErr);
      }
    }

    res.status(201).json({ 
      success: true, 
      message: 'Sensor data recorded',
      data: { deviceId, timestamp }
    });
  } catch (err) {
    console.error('Error receiving sensor data:', err);
    res.status(500).json({ error: 'Failed to record sensor data' });
  }
});

// GET: Latest sensor data
router.get('/sensor-data/latest', async (req, res) => {
  try {
    const data = await req.db.get(
      `SELECT * FROM sensor_data ORDER BY timestamp DESC LIMIT 1`
    );
    res.json(data || {});
  } catch (err) {
    console.error('Error fetching latest sensor data:', err);
    res.status(500).json({ error: 'Failed to fetch sensor data' });
  }
});

// GET: Sensor data history (last N records)
router.get('/sensor-data/history', async (req, res) => {
  try {
    const limit = req.query.limit || 100;
    const data = await req.db.all(
      `SELECT * FROM sensor_data ORDER BY timestamp DESC LIMIT ?`,
      [limit]
    );
    res.json(data);
  } catch (err) {
    console.error('Error fetching sensor history:', err);
    res.status(500).json({ error: 'Failed to fetch sensor history' });
  }
});

// GET: Sensor statistics
router.get('/sensor-stats', async (req, res) => {
  try {
    const stats = await req.db.get(`
      SELECT 
        AVG(soundLevel) as avgSound,
        MAX(soundLevel) as maxSound,
        AVG(temperature) as avgTemp,
        MAX(temperature) as maxTemp,
        MIN(temperature) as minTemp,
        AVG(humidity) as avgHumidity,
        AVG(coLevel) as avgCO,
        MAX(coLevel) as maxCO,
        COUNT(*) as totalReadings
      FROM sensor_data
      WHERE timestamp > datetime('now', '-1 hour')
    `);
    res.json(stats);
  } catch (err) {
    console.error('Error fetching sensor stats:', err);
    res.status(500).json({ error: 'Failed to fetch sensor statistics' });
  }
});

// Helper function to check and create alerts
async function checkAndCreateAlerts(db, sensorData) {
  const { soundLevel, temperature, humidity, coLevel } = sensorData;

  // Thresholds
  const thresholds = {
    soundLevel: 80,      // Alert if > 80%
    tempHigh: 28,        // Alert if > 28°C
    tempLow: 18,         // Alert if < 18°C
    humidityHigh: 70,    // Alert if > 70%
    humidityLow: 30,     // Alert if < 30%
    coLevel: 10          // Alert if > 10 PPM
  };

  try {
    // Sound level alert
    if (soundLevel > thresholds.soundLevel) {
      await db.run(
        `INSERT INTO alerts (type, severity, message, value)
         VALUES (?, ?, ?, ?)`,
        ['SOUND', 'WARNING', `High sound level detected: ${soundLevel.toFixed(2)}%`, soundLevel]
      );
    }

    // Temperature alerts
    if (temperature > thresholds.tempHigh) {
      await db.run(
        `INSERT INTO alerts (type, severity, message, value)
         VALUES (?, ?, ?, ?)`,
        ['TEMPERATURE', 'WARNING', `High temperature: ${temperature.toFixed(2)}°C`, temperature]
      );
    }
    if (temperature < thresholds.tempLow) {
      await db.run(
        `INSERT INTO alerts (type, severity, message, value)
         VALUES (?, ?, ?, ?)`,
        ['TEMPERATURE', 'WARNING', `Low temperature: ${temperature.toFixed(2)}°C`, temperature]
      );
    }

    // Humidity alerts
    if (humidity > thresholds.humidityHigh) {
      await db.run(
        `INSERT INTO alerts (type, severity, message, value)
         VALUES (?, ?, ?, ?)`,
        ['HUMIDITY', 'INFO', `High humidity: ${humidity.toFixed(2)}%`, humidity]
      );
    }
    if (humidity < thresholds.humidityLow) {
      await db.run(
        `INSERT INTO alerts (type, severity, message, value)
         VALUES (?, ?, ?, ?)`,
        ['HUMIDITY', 'INFO', `Low humidity: ${humidity.toFixed(2)}%`, humidity]
      );
    }

    // CO level alert
    if (coLevel > thresholds.coLevel) {
      await db.run(
        `INSERT INTO alerts (type, severity, message, value)
         VALUES (?, ?, ?, ?)`,
        ['CO_LEVEL', 'CRITICAL', `High CO level: ${coLevel.toFixed(2)} PPM`, coLevel]
      );
    }
  } catch (err) {
    console.error('Error creating alerts:', err);
  }
}

// POST: Generate test data (for development/demo only)
router.post('/sensor-data/generate-test-data', async (req, res) => {
  try {
    // Generate last 24 hours of test data (1 sample per hour)
    const testData = [];
    const now = new Date();

    for (let i = 23; i >= 0; i--) {
      const timestamp = new Date(now.getTime() - i * 60 * 60 * 1000);
      
      testData.push({
        deviceId: 'CLASSROOM_01',
        soundLevel: 45 + Math.random() * 30,
        temperature: 22 + Math.random() * 6,
        humidity: 50 + Math.random() * 20,
        coLevel: 5 + Math.random() * 8,
        rssi: -50 - Math.random() * 40,
        timestamp: timestamp.toISOString().replace('T', ' ').substr(0, 19)
      });
    }

    // Insert test data
    for (const data of testData) {
      await req.db.run(
        `INSERT INTO sensor_data 
         (deviceId, soundLevel, temperature, humidity, coLevel, rssi, timestamp)
         VALUES (?, ?, ?, ?, ?, ?, ?)`,
        [data.deviceId, data.soundLevel, data.temperature, data.humidity, data.coLevel, data.rssi, data.timestamp]
      );
    }

    res.json({
      success: true,
      message: `Generated ${testData.length} test data records`,
      records: testData.length
    });
  } catch (err) {
    console.error('Error generating test data:', err);
    res.status(500).json({ error: 'Failed to generate test data' });
  }
});

export default router;
