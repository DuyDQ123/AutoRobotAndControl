#include <WiFi.h>
#include <WebServer.h>

// WiFi credentials
const char* ssid = "duy";
const char* password = "11111111";

WebServer server(80);
String sensorData = "";

// Mode: 0 = manual, 1 = auto
int mode = 0;

void handleRoot() {
  String html = R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
      <title>ESP32-CAM Car</title>
      <style>
        button { font-size: 20px; margin: 10px; padding: 10px 20px; }
        .mode { margin-top: 20px; }
        #data { margin-top: 30px; font-family: monospace; }
      </style>
    </head>
    <body>
      <h1>ESP32-CAM Car Control</h1>

      <div>
        <button onclick="sendCommand('F')">Forward (W)</button><br>
        <button onclick="sendCommand('L')">Left (A)</button>
        <button onclick="sendCommand('S')">Stop</button>
        <button onclick="sendCommand('R')">Right (D)</button><br>
        <button onclick="sendCommand('B')">Backward (S)</button>
      </div>

      <div class="mode">
        <button onclick="setMode(0)">Manual Mode</button>
        <button onclick="setMode(1)">Auto Mode</button>
      </div>

      <div id="data">
        <h3>Sensor Data:</h3>
        <pre id="sensor"></pre>
      </div>

      <script>
        document.addEventListener('keydown', function(e) {
          if (e.key === 'w') sendCommand('F');
          if (e.key === 's') sendCommand('B');
          if (e.key === 'a') sendCommand('L');
          if (e.key === 'd') sendCommand('R');
          if (e.key === 'x') sendCommand('S');
        });

        function sendCommand(cmd) {
          fetch('/cmd?c=' + cmd);
        }

        function setMode(m) {
          fetch('/mode?m=' + m);
        }

        setInterval(() => {
          fetch('/sensor').then(r => r.text()).then(data => {
            document.getElementById('sensor').textContent = data;
          });
        }, 1000);
      </script>
    </body>
    </html>
  )rawliteral";

  server.send(200, "text/html", html);
}

void handleCmd() {
  if (server.hasArg("c")) {
    char cmd = server.arg("c")[0];
    Serial.write(cmd);
    server.send(200, "text/plain", "OK");
  }
}

void handleMode() {
  if (server.hasArg("m")) {
    mode = server.arg("m").toInt();
    if (mode == 1) Serial.write('A');
    else Serial.write('M');
  }
  server.send(200, "text/plain", "OK");
}

void handleSensor() {
  server.send(200, "text/plain", sensorData);
}

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi Connected!");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/cmd", handleCmd);
  server.on("/mode", handleMode);
  server.on("/sensor", handleSensor);
  server.begin();
}

void loop() {
  server.handleClient();

  // Receive sensor data from Arduino
  if (Serial.available()) {
    sensorData = Serial.readStringUntil('\n');
  }
}
