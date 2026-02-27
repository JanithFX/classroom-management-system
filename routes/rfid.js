import express from 'express';
import { v4 as uuidv4 } from 'uuid';

const router = express.Router();

// In-memory storage for last scanned RFID (for real-time sync)
let lastScannedRFID = {
  uid: null,
  timestamp: null
};

// POST: Store scanned RFID (called by ESP32)
router.post('/rfid/scan', async (req, res) => {
  try {
    const { rfidUid } = req.body;
    
    if (!rfidUid) {
      return res.status(400).json({ error: 'rfidUid is required' });
    }

    // Store the last scanned RFID
    lastScannedRFID = {
      uid: rfidUid.toUpperCase(),
      timestamp: new Date().toISOString()
    };

    console.log(`RFID Scanned: ${lastScannedRFID.uid}`);

    res.json({
      success: true,
      message: 'RFID scan received',
      rfidUid: lastScannedRFID.uid,
      timestamp: lastScannedRFID.timestamp
    });
  } catch (err) {
    console.error('Error storing RFID scan:', err);
    res.status(500).json({ error: 'Failed to store RFID scan' });
  }
});

// GET: Get last scanned RFID (called by frontend)
router.get('/rfid/last-scan', (req, res) => {
  if (!lastScannedRFID.uid) {
    return res.json({
      uid: null,
      timestamp: null,
      message: 'No RFID scanned yet'
    });
  }

  const scan = {
    uid: lastScannedRFID.uid,
    timestamp: lastScannedRFID.timestamp
  };

  // Clear after retrieval so duplicate scans don't occur
  lastScannedRFID = { uid: null, timestamp: null };

  res.json(scan);
});

// POST: Register new RFID card
router.post('/rfid/register', async (req, res) => {
  try {
    const { rfidUid, studentName } = req.body;
    const cardId = uuidv4();

    await req.db.run(
      `INSERT INTO rfid_cards (cardId, studentName, rfidUid)
       VALUES (?, ?, ?)`,
      [cardId, studentName, rfidUid]
    );

    res.status(201).json({
      success: true,
      message: 'RFID card registered',
      cardId,
      rfidUid,
      studentName
    });
  } catch (err) {
    console.error('Error registering RFID card:', err);
    res.status(500).json({ error: 'Failed to register RFID card' });
  }
});

// POST: Log RFID card access
router.post('/rfid/log', async (req, res) => {
  try {
    const { rfidUid, action = 'ENTRY' } = req.body;

    // Find card in database
    const card = await req.db.get(
      `SELECT * FROM rfid_cards WHERE rfidUid = ?`,
      [rfidUid]
    );

    if (!card) {
      // Unknown card - log it anyway
      console.warn(`Unknown RFID card detected: ${rfidUid}`);
      return res.status(404).json({
        error: 'Card not registered',
        rfidUid,
        action: 'Register this card first'
      });
    }

    // Log the access
    await req.db.run(
      `INSERT INTO rfid_logs (cardId, action) VALUES (?, ?)`,
      [card.cardId, action]
    );

    res.status(201).json({
      success: true,
      message: 'Access logged',
      cardId: card.cardId,
      studentName: card.studentName,
      action,
      timestamp: new Date().toISOString()
    });
  } catch (err) {
    console.error('Error logging RFID access:', err);
    res.status(500).json({ error: 'Failed to log RFID access' });
  }
});

// GET: All registered RFID cards
router.get('/rfid/cards', async (req, res) => {
  try {
    const cards = await req.db.all(
      `SELECT cardId, studentName, rfidUid, createdAt FROM rfid_cards ORDER BY createdAt DESC`
    );
    res.json(cards);
  } catch (err) {
    console.error('Error fetching RFID cards:', err);
    res.status(500).json({ error: 'Failed to fetch RFID cards' });
  }
});

// GET: RFID access logs
router.get('/rfid/logs', async (req, res) => {
  try {
    const limit = req.query.limit || 50;
    const logs = await req.db.all(
      `SELECT 
        l.id,
        l.timestamp,
        l.action,
        c.studentName,
        c.rfidUid
      FROM rfid_logs l
      JOIN rfid_cards c ON l.cardId = c.cardId
      ORDER BY l.timestamp DESC
      LIMIT ?`,
      [limit]
    );
    res.json(logs);
  } catch (err) {
    console.error('Error fetching RFID logs:', err);
    res.status(500).json({ error: 'Failed to fetch RFID logs' });
  }
});

// GET: Student attendance for today
router.get('/rfid/attendance', async (req, res) => {
  try {
    const attendance = await req.db.all(
      `SELECT 
        c.studentName,
        c.rfidUid,
        COUNT(*) as entries,
        MIN(l.timestamp) as firstEntry,
        MAX(l.timestamp) as lastEntry
      FROM rfid_logs l
      JOIN rfid_cards c ON l.cardId = c.cardId
      WHERE DATE(l.timestamp) = DATE('now')
      GROUP BY c.cardId
      ORDER BY c.studentName`
    );
    res.json(attendance);
  } catch (err) {
    console.error('Error fetching attendance:', err);
    res.status(500).json({ error: 'Failed to fetch attendance' });
  }
});

// DELETE: Remove RFID card
router.delete('/rfid/cards/:cardId', async (req, res) => {
  try {
    const { cardId } = req.params;

    await req.db.run(
      `DELETE FROM rfid_cards WHERE cardId = ?`,
      [cardId]
    );

    res.json({ success: true, message: 'RFID card deleted' });
  } catch (err) {
    console.error('Error deleting RFID card:', err);
    res.status(500).json({ error: 'Failed to delete RFID card' });
  }
});

export default router;
