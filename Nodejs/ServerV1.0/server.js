const express = require('express');
const bodyParser = require('body-parser');
const fs = require('fs');
const app = express();
const port = 3000;

app.use(bodyParser.json());

// Create a variable to track the sender state
let senderState = true; // Assume it's initially ON
let comment = "first loc"; //

app.get('/sender-state', (req, res) => {
  res.json({ senderState, comment});
});

app.post('/toggle-sender', (req, res) => {
  // Toggle the sender state
  senderState = !senderState;
  // Get the comment from the POST request
  const newComment = req.body.comment;
  comment = newComment;
  res.json({ senderState, comment });
});

app.post('/esp32data', (req, res) => {
  const data = req.body;
  console.log('Received data from ESP32:', data);

  if (senderState) {
    // Get the comment from the request
    const comment = req.body.Comment;

    // Save the received data along with the comment to a text file
    const dataToSave = `Data: ${data.Data}, RSSI: ${data.RSSI}, Comment: ${comment}, Timestamp: ${new Date()}\n`;

    fs.appendFile('data.txt', dataToSave, (err) => {
      if (err) {
        console.error('Error saving data:', err);
        res.status(500).send('Error saving data');
      } else {
        res.status(200).send('Data received and saved successfully');
      }
    });
  } else {
    res.status(200).send('Data not saved because sender is OFF.');
  }
});


app.listen(port, () => {
  console.log(`Server is running on port ${port}`);
});
