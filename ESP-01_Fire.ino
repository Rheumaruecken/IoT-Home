
// download von HEISE.de
// angepasst durch M. Nebel

#include <ESP8266WiFi.h>

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "GEHEIM"
#define WLAN_PASS       "GEHEIM"
#define LED             2

/************************* Server config *************************************/

#define FINGERPRINT     "A1 A1 A1 A1 A1 A1 A1 A1 A1 A1 A1 A1 A1 A1 A1 A1 A1 A1 A1 A1"
#define CERT_NAME       "GEHEIM"   //Common Name

/************************** Global State *************************************/

// WiFiFlientSecure for SSL/TLS support
WiFiClientSecure client;

/*************************** Sketch Code ************************************/

void setup() {
  Serial1.begin(115200);
  Serial1.println();
  Serial1.println("Initializing outputs...");
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  Serial1.println("Starting Wifi connection...");
 
  // set static ip part
  IPAddress ipCLI (192, 168, 100, 111);
  IPAddress ipGTW(192, 168, 100, 1);
  IPAddress ipSUB(255, 255, 255, 0);
  IPAddress ipDNS(192, 168, 100, 1);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  WiFi.config(ipCLI, ipGTW, ipSUB, ipDNS);

  int failcounter = 300;
  Serial1.print("Waiting for a connection");
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial1.print(".");
    if (failcounter <= 0) {
      Serial1.println();
      Serial1.println("Giving up.");
      blinkError();
      shutdown();
    }
    failcounter--;
  }
  Serial1.println();
  digitalWrite(LED, HIGH);
  Serial1.println("Wifi connected.");
  // connect to host
  const char* host = "192.168.100.10";
  int port         = 443;
  if (client.connect(host, port)) {
    Serial1.print("Connection to ");
    Serial1.print(host);
    Serial1.println(" established.");
    // check the fingerprint of the SSL cert
    verifyFingerprint();
    // send GET request
    Serial1.println("Sending a message to the server:");

    client.print(String("GET /ESP-01.php?id=101&sensor=FeuerSchlaf") + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n" +
                 "\r\n");
    Serial1.println(String("GET /") + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n" +
                 "\r\n");

    blinkSent();
    delay(500);
    // get response
    int success = 0;
    failcounter = 50000;
    while (client.connected()) {
      if (client.available()) {
        String line = client.readStringUntil('\n');
        Serial1.println("Server response: " + line);
        if (line[0] == 'O' && line[1] == 'K') {
          digitalWrite(LED, LOW);
          success = 1;
        }
      }
      if (failcounter <= 0) {
        blinkError();
        shutdown();
      }
      delay(100);   // wait
      failcounter--;
    }
    if (success) {
      blinkSuccess();
    } else {
      Serial1.println("Got no success message.");
      blinkError();
    }
    // close connection
    Serial1.println("Successfully closing the connection.");
    client.stop();
    shutdown();
  } else {
    Serial1.print("Unable to connect to ");
    Serial1.print(host);
    Serial1.println(". Sorry.");
    shutdown();
  }
}

void loop() {
  // this should never be reached.
}

void verifyFingerprint() {
  if (! client.verify(FINGERPRINT, CERT_NAME)) {
    Serial1.println("ERROR: Incorrect certificate signature!");
    // Connection insecure!
    blinkError();
    shutdown();
  }
}

void blinkSuccess() {
  for (int i = 4; i < 50; i=(5*i) >> 2) {
    digitalWrite(LED, HIGH);   // turn the LED off
    delay(10*i);               // wait
    digitalWrite(LED, LOW);    // turn the LED on
    delay(10*i);               // wait
  }
}

void blinkError() {
  for (int i = 0; i < 28; i++) {
    digitalWrite(LED, HIGH);   // turn the LED off
    delay(125);                        // wait
    digitalWrite(LED, LOW);    // turn the LED on
    delay(125);                        // wait
  }
}

void blinkSent() {
  for (int i = 0; i < 2; i++) {
    digitalWrite(LED, LOW);   // turn the LED on
    delay(200);                        // wait
    digitalWrite(LED, HIGH);    // turn the LED off
    delay(200);                        // wait
  }
}

void shutdown() {
  Serial1.println("Shutting down.");
  Serial1.println("Going to sleep.");
  ESP.deepSleep(0);
  Serial1.println("Sleep failed.");
  while(1) {
    blinkError();
  }
}
