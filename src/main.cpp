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
#define CMD_PLAY 0x03
#define CMD_STOP
#define CMD_PAUSE
#define CMD_GET_PLAYBACK 0x45


HardwareSerial MP3Serial(2);
word checksum;


void MP3_cmd(byte command, byte data1, byte data2);

void setup() {

  // Init port serie
  MP3Serial.begin(9600,SERIAL_8N1,RX,TX);

  //Init module MPO3
  MP3_cmd(0x0C, 0, 0);
  byte volume = 0x09;
  MP3_cmd(0x06, 0, volume);
  
  // Play song
  MP3_cmd(CMD_PLAY,0,1);



}

void loop() {
}


// --------------------------------------
void MP3_cmd(byte command, byte data1, byte data2){
    checksum = -( VersionByte + Command_length + CMD_PLAY + ACK + data1 + data2);
    byte data[] = {StartByte,VersionByte,Command_length,CMD_PLAY, ACK, data1, data2, highByte(checksum), lowByte(checksum), EndByte};
    
    for (byte i=0; i<10; i++){
        MP3Serial.write(data[i]);
    }
}