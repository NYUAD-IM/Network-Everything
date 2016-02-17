#include <SPI.h>
#include <Adafruit_PN532.h>

// Use this line for a breakout with a hardware SPI connection.  Note that
// the PN532 SCK, MOSI, and MISO pins need to be connected to the Arduino's
// hardware SPI SCK, MOSI, and MISO pins.  On an Arduino Uno these are
// SCK = 13, MOSI = 11, MISO = 12.  The SS line can be any digital IO pin.
Adafruit_PN532 nfc(PN532_SS);

long lastID = 0; // previous UID number

void setup() {
  Serial.begin(115200); // fast serial

  nfc.begin(); // start the reader

  // diagnostic, and checking to see that communication is AOK
  uint32_t versiondata = nfc.getFirmwareVersion(); 
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }

  // configure board to read RFID tags
  nfc.SAMConfig();
}


void loop() {
  int thisID; // the ID we are reading
  int success; // have we read it?
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) {
    // Display some basic information about the card
    if (uidLength == 4)
    {
      // We probably have a Mifare Classic card ...
      unsigned long cardid = uid[0];
      cardid <<= 8;
      cardid |= uid[1];
      cardid <<= 8;
      cardid |= uid[2];
      cardid <<= 8;
      cardid |= uid[3];
      thisID = cardid;
      if (thisID != lastID) {
        Serial.println(cardid); // print the card number
      }
      lastID = thisID;
    }
  }
}

