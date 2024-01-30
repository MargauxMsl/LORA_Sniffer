// PROJET SIR - SNIFFER LORAWAN
// Noé Caringi - Soukaina Khalil - Margaux Massol - Hillel Partouche
// Dans le cadre du cursus d'ingénieur en télécommunications au sein de l'INSA Lyon

/* EMETTEUR */

#include <SPI.h>
#include <RH_RF95.h>

#define RFM95_CS 4
#define RFM95_RST 2
#define RFM95_INT 3

// Frequence
#define RF95_FREQ 868.1

#define address_transmitter 12
#define address_receiver 235


// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() 
{
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  while (!Serial);
  Serial.begin(9600);
  delay(100);

  Serial.println("Arduino LoRa TX Test!");

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
  
    // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
    /* Other standarts modes are : 

        Bw125Cr45Sf128 	: Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. Default medium range.

        Bw500Cr45Sf128 	: Bw = 500 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. Fast+short range.
        
        Bw31_25Cr48Sf512 : Bw = 31.25 kHz, Cr = 4/8, Sf = 512chips/symbol, CRC on. Slow+long range.
        
        Bw125Cr48Sf4096 : Bw = 125 kHz, Cr = 4/8, Sf = 4096chips/symbol, low data rate, CRC on. Slow+long range.

        Bw125Cr45Sf2048 	: Bw = 125 kHz, Cr = 4/5, Sf = 2048chips/symbol, CRC on. Slow+long range.
      
    */
   rf95.setModemConfig('Bw31_25Cr48Sf512');

   // Addresses
   rf95.setThisAddress(address_transmitter);
   rf95.setHeaderFrom(address_transmitter);
   rf95.setHeaderTo(address_receiver);


  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);
}

int16_t packetnum = 0;  // packet counter, we increment per xmission

void loop()
{
  Serial.println("Sending to rf95_server");
  // Send a message to rf95_server
  
  char radiopacket[20] = "Hello World #      ";
  itoa(packetnum++, radiopacket+13, 10);
  Serial.print("Sending "); Serial.println(radiopacket);
  radiopacket[19] = 0;
  
  Serial.println("Sending..."); delay(10);
  rf95.send((uint8_t *)radiopacket, 20);

  Serial.println("Waiting for packet to complete..."); delay(10);
  rf95.waitPacketSent();
  // Now wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

//  Serial.println("Waiting for reply..."); delay(10);
//  if (rf95.waitAvailableTimeout(1000))
//  { 
//    // Should be a reply message for us now   
//    if (rf95.recv(buf, &len))
//   {
//      Serial.print("Got reply: ");
//      Serial.println((char*)buf);
//      Serial.print("RSSI: ");
//      Serial.println(rf95.lastRssi(), DEC);    
//    }
//    else
//    {
//      Serial.println("Receive failed");
//    }
//  }
//  else
//  {
//    Serial.println("No reply, is there a listener around?");
//  }
  delay(500);
}