/*
  connect to server

  This sketch connects to a a web server and makes a request. 
  The page returns a randomly generated nyumber between
  0 and 255, which changes the brightness level of a conected
  LED. The example uses DHCP for addressing.

*/

#include <SPI.h>
#include <Ethernet.h>

// assign a MAC address for the ethernet controller.
// fill in your address here:
byte mac[] = {
  0x90, 0xA2, 0xDA, 0x10, 0x2A, 0x11
};
// fill in an available IP address on your network here,
// for manual configuration:
//IPAddress ip(192, 168, 1, 177);

// initialize the library instance:
EthernetClient client;

char value[] = "0000";
int stringPos = 0;
boolean startRead = false;
char server[] = "neteverything.nyuad.im";

unsigned long lastConnectionTime = 0;             // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 10L * 1000L; // delay between updates, in milliseconds
// the "L" is needed to use long type numbers

void setup() {
  // start serial port:
  Serial.begin(9600);
  // give the ethernet module time to boot up:
  delay(1000);
  // start the Ethernet connection using a fixed IP address and DNS server:
  Ethernet.begin(mac);
  // print the Ethernet board/shield's IP address:
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  // if there's incoming data from the net connection.
  // send it out the serial port.
  if (client.available()) {
    char c = client.read();
    if (c == '+') { // beginning char
      startRead = true; // start reading to string
      for (int i = 0; i < 4; i++) {
        value[i] = ' ';
      }
      c = client.read();
    }
    if (c != '~' && startRead) {
      value[stringPos] = c;
      stringPos++;
    } else if (c == '~' && startRead){
      startRead = false;
      stringPos = 0;
      String outPut(value);
      int ledVal = outPut.toInt();
      Serial.println(ledVal);
      analogWrite(3, ledVal);
    }

  }

  // if ten seconds have passed since your last connection,
  // then connect again and send data:
  if (millis() - lastConnectionTime > postingInterval) {
    httpRequest();
  }
}

// this method makes a HTTP connection to the server:
void httpRequest() {
  client.stop();
  // if there's a successful connection:
  if (client.connect(server, 80)) {
    Serial.println("connected.... ");
    // send the HTTP GET request:
    client.println("GET /ethernetExample/random.php HTTP/1.1");
    client.println("Host: neteverything.nyuad.im");
    client.println("User-Agent: arduino-ethernet");
    client.println("Connection: close");
    client.println();
    // note the time that the connection was made:
    lastConnectionTime = millis();
  } else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
  }
}


