// --------------------------------------
//  Test lecteur MP3
// 
//  ESP32 -- MP3
//  GND  
//  RX 16 -- TX (3)
//  TX 17 -- RX (2)
//
// Vitesse de transmission par
// défaut : 9600bps
// Bits de données : 1
// Sortie de caisse : aucun
// Contrôle de flux : aucun
// --------------------------------------

#include <Arduino.h>

#define RX 16
#define TX 17

#define StartByte 0x7E
#define VersionByte 0x07
#define EndByte 0xEF
#define ACK 0x01
#define Command_length 0x06

#define CMD_RESET 0x0C              // need min 200ms after reset before sending cmd
#define CMD_NEXT 0x01
#define CMD_PREV 0x02
#define CMD_PLAY 0x03
#define CMD_SET_VOLUME_UP 0x04
#define CMD_SET_VOLUME_DOWN 0x05
#define CMD_SET_VOLUME 0x06
#define CMD_SET_EQ 0x07             // Réglez l'égaliseur sur 00 / 01 / 02 / 03 / 04 / 05 : Normal/Pop/Rock/Jazz/Classique/Base
#define CMD_PAUSE 0x0E

// variables définies par IA. A Vérifier
#define CMD_GET_PLAYBACK 0x45
#define CMD_GET_VOLUME 0x43

#define CMD_SET_DEVICE 0x09
#define CMD_SET_SLEEP 0x0A

#define CMD_SET_PLAYBACK 0x0D
#define CMD_SET_DAC 0x1A
#define CMD_SET_AUDIO 0x1B
#define CMD_SET_LOOP 0x19
#define CMD_SET_EOF 0x0B
#define CMD_GET_STATUS 0x42



HardwareSerial MP3Serial(2);
word checksum;


void MP3_cmd(byte command, byte data1, byte data2);

// --------------------------------------
// Setup
// --------------------------------------
void setup() {

    // Init serial port 
    MP3Serial.begin(9600,SERIAL_8N1,RX,TX);

    //Init module MPO3. A delay is necessary after
    MP3_cmd(CMD_RESET, 0, 0);
    delay(500);

    // Set volume
    byte volume = 0x0F;
    MP3_cmd(CMD_SET_VOLUME, 0, volume);

    // Play song
    MP3_cmd(CMD_PLAY,0,1);

}

// --------------------------------------
// Loop
// --------------------------------------
void loop() {
}

// --------------------------------------
// Functions
// --------------------------------------

// --------------------------------------
void MP3_cmd(byte command, byte data1, byte data2){
    // Send a command to the mp3 player

    checksum = -( VersionByte + Command_length + command + ACK + data1 + data2);
    byte data[] = {StartByte,VersionByte,Command_length,command, ACK, data1, data2, highByte(checksum), lowByte(checksum), EndByte};
    
    for (byte i=0; i<10; i++){
        MP3Serial.write(data[i]);
    }
}