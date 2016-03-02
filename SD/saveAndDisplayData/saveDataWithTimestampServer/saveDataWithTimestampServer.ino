/*
 * Reads the value of a sensor attached to A0
 * stores it to a data file
 * when requested, it displays the data
 * in a graph rendered by p5.js
 * 
  Based on the following examples :
  Udp NTP Client
  SD card datalogger
  ethernet server
  and a huge thanks to Tim Dicus for the file parsing based on 
  this thread http://forum.arduino.cc/index.php?topic=95723.0
*/

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SD.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

unsigned int localPort = 8888;       // local port to listen for UDP packets
char timeServer[] = "time.nist.gov"; // time.nist.gov NTP server
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

// A UDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

EthernetServer server(80);  // create a server at port 80

unsigned long prevTime = 0;  // variable to hold previous time
const long interval = 60000; // interval at which to save data (milliseconds)

int chipSelect = 4;  // CS pin for SD card

String theTime = ""; // variable to hold time when the sensr is read

char buffer[64];
int index = 0;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

//  Serial.println("Starting Ethernet communication .. ");
  // start Ethernet and UDP
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for (;;)
      ;
  }
  server.begin();           // start to listen for clients
  // print your local IP address:
  printIPAddress();
  Udp.begin(localPort); // start UDP communication on 8888

//  Serial.print("Initializing SD card...");
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
//  Serial.println("card initialized.");
}

void loop() {
  unsigned long currentTime = millis();
  if (currentTime - prevTime >= interval) {
    // save the last time you checked the time and read the sensor
    prevTime = currentTime;
    sendNTPpacket(timeServer); // send an NTP packet to a time server
    readPacket(); // parse the data
    writeValToSD(analogRead(A0)); //read the snesor value and save it to the SD card
  }

  checkClientConnection();  // is anybody out there?
  // send page to cnnected client
  Ethernet.maintain();      // keep current IP address from DHCP server
}

void writeValToSD(int sensorVal) {
  // make a string for assembling the data to log:
  String dataString = String(sensorVal);

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.print(theTime);
    dataFile.print(",");
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
 //   Serial.print(", the sensor value is : ");
 //   Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
//    Serial.println("error opening datalog.txt");
  }
}

void checkClientConnection() {
  EthernetClient client = server.available();  // if a client is connected

  if (client) {  // if sometone is knocking
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {   // client data available to read
        char c = client.read(); // read 1 byte (character) from client
        // last line of client request is blank and ends with \n
        // respond to client only after last line received
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response
          String s = buffer;
          String t;
          int idx = 0;
          if (s.startsWith("GET")) {
            client.println("HTTP/1.0 200 OK");
            if ((idx = s.indexOf(".htm")) > 0  ) {
              client.println("Content-Type: text/html");
            } else if ((idx = s.indexOf(".pjs")) > 0) {
              client.println("Content-Type: application/javascript");
            } else if ((idx = s.indexOf(".txt")) > 0) {
              client.println("Content-Type: text");
            }
            t = s.substring(5, idx + 5);
            client.println();
            sdCheck(client, t);
          }
          client.stop();
          break;
        }
        // every line of text received from the client ends with \r\n
        else if (c == '\r') {
          // you're starting a new line
          currentLineIsBlank = true;
          buffer[index] = '\0';
          index = 0;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
          buffer[index++] = c;
        }
      } // end if (client.available())
    } // end while (client.connected())
  } // end if (client)
}


void sdCheck(EthernetClient client, String file)
{
  char filename[13];
  file.toCharArray(filename, file.length());
  File myFile = SD.open(filename);

  if (myFile) {
    byte buffer[64];
    int count = 0;
    if (myFile.available()) {
      while ((count = myFile.read(buffer, 64)) > 0 ) {
        client.write(buffer, count);
      }
    }
    // close the file:
    myFile.close();
  } else {
    client.println("ERROR");
  }
}


void readPacket() {
  // wait to see if a reply is available
  delay(1000);
  if (Udp.parsePacket()) {
    // We've received a packet, read the data from it
    Udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
    // the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, extract the two words:
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;
    // print the hour, minute and second:
    long hours = (epoch  % 86400L) / 3600;
    long minutes;
    if (((epoch % 3600) / 60) < 10) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      minutes = 0;
    } else {
      minutes = (epoch  % 3600) / 60;
    }
    theTime = "";
    theTime = String(hours);
    theTime += ":";
    theTime += String(minutes);
//    Serial.print("At UTC : ");
//    Serial.print(theTime);
  }
}

// send an NTP request to the time server at the given address
void sendNTPpacket(char* address) {
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

void printIPAddress()
{
//  Serial.print("My IP address is ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print(".");
  }
  Serial.println();
}

