const { SerialPort } = require('serialport');
const { ReadlineParser } = require('@serialport/parser-readline');
const express = require('express');
const http = require('http');
const { Server } = require('socket.io');

const app = express();
const server = http.createServer(app);
const io = new Server(server);

// --- CONFIGURATION SÉRIE ---
// Remplacez 'COM3' par votre port (ex: '/dev/ttyACM0' sur Mac/Linux)
const port = new SerialPort({ path: 'COM5', baudRate: 115200 });
const parser = port.pipe(new ReadlineParser({ delimiter: '\r\n' }));

app.use(express.static(__dirname)); // Sert votre fichier HTML

io.on('connection', (socket) => {
    console.log('Interface Web connectée');

    // Quand l'utilisateur change une note sur le web
    socket.on('changeNote', (data) => {
        // data = { step: 1, track: 5 }
        const cmd = `M${data.step}:${data.track}\n`;
        port.write(cmd);
        console.log('Envoi vers Arduino:', cmd);
    });
});

server.listen(3000, () => {
    console.log('Serveur prêt sur http://localhost:3000');
});