/*********************************************************************
 This is an example for our nRF52 based Bluefruit LE modules

 Pick one up today in the adafruit shop!

 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/
#include <bluefruit.h>
#include <MIDI.h>

BLEDis bledis;
BLEMidi blemidi;

// Create a new instance of the Arduino MIDI Library,
// and attach BluefruitLE MIDI as the transport.
MIDI_CREATE_BLE_INSTANCE(blemidi);

// Variable that holds the current position in the sequence.
int position = 0;

// Store example melody as an array of note values
byte note_sequence[] = {
  74,78,81,86,90,93,98,102,57,61,66,69,73,78,81,85,88,92,97,100,97,92,88,85,81,78,
  74,69,66,62,57,62,66,69,74,78,81,86,90,93,97,102,97,93,90,85,81,78,73,68,64,61,
  56,61,64,68,74,78,81,86,90,93,98,102
};

// Variable to hold the last time we sent a note
unsigned long previousSend = 0;

void setup()
{

  Serial.begin(115200);
  Serial.println("Adafruit Bluefruit52 MIDI over Bluetooth LE Example");

  Bluefruit.begin();
  Bluefruit.setName("Bluefruit52 MIDI");

  // Setup the on board blue LED to be enabled on CONNECT
  Bluefruit.autoConnLed(true);

  // Configure and Start Device Information Service
  bledis.setManufacturer("Adafruit Industries");
  bledis.setModel("Bluefruit Feather52");
  bledis.begin();

  // Initialize MIDI, and listen to all MIDI channels
  // This will also call blemidi service's begin()
  MIDI.begin(MIDI_CHANNEL_OMNI);
  blemidi.autoMIDIread(&MIDI);

  // Attach the handleNoteOn function to the MIDI Library. It will
  // be called whenever the Bluefruit receives MIDI Note On messages.
  MIDI.setHandleNoteOn(handleNoteOn);

  // Do the same for MIDI Note Off messages.
  MIDI.setHandleNoteOff(handleNoteOff);

  // Set General Discoverable Mode flag
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);

  // Advertise TX Power
  Bluefruit.Advertising.addTxPower();

  // Advertise BLE MIDI Service
  Bluefruit.Advertising.addService(blemidi);

  // Advertise device name in the Scan Response
  Bluefruit.ScanResponse.addName();

  // Start Advertising
  Bluefruit.Advertising.start();

}

void handleNoteOn(byte channel, byte pitch, byte velocity)
{
  // Log when a note is pressed.
  Serial.printf("Note on: channel = %d, pitch = %d, velocity - %d", channel, pitch, velocity);
  Serial.println();
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
  // Log when a note is released.
  Serial.printf("Note off: channel = %d, pitch = %d, velocity - %d", channel, pitch, velocity);
  Serial.println();
}

void loop()
{

  // Don't continue if we aren't connected.
  if (! Bluefruit.connected()) {
    return;
  }

  // Don't continue if the connected device isn't ready to receive messages.
  if (! blemidi.notifyEnabled()) {
    return;
  }

  // read any new MIDI messages
  //MIDI.read();

  // Store the current time
  unsigned long now = millis();

  // Check if enough time has passed since last send
  if (now - previousSend < 286) {
    return;
  }

  // Setup variables for the current and previous
  // positions in the note sequence.
  int current = position;
  int previous  = position - 1;

  // If we currently are at position 0, set the
  // previous position to the last note in the sequence.
  if (previous < 0) {
    previous = sizeof(note_sequence) - 1;
  }

  // Send Note On for current position at full velocity (127) on channel 1.
  MIDI.sendNoteOn(note_sequence[current], 127, 1);

  // Send Note Off for previous note.
  MIDI.sendNoteOff(note_sequence[previous], 0, 1);

  // Increment position
  position++;

  // If we are at the end of the sequence, start over.
  if (position >= sizeof(note_sequence)) {
    position = 0;
  }

  // Log the send time
  previousSend = now;

}