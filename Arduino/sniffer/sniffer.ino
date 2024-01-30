// PROJET SIR - SNIFFER LORAWAN
// Noé Caringi - Soukaina Khalil - Margaux Massol - Hillel Partouche
// Dans le cadre du cursus d'ingénieur en télécommunications au sein de l'INSA Lyon

/* SNIFFER */

enum OutputMode { verbose, delimited };


OutputMode mode = verbose;
#define DELIMITER_CHAR '~'


#include <SPI.h>
#include <RH_RF95.h>

#define RH_FLAGS_ACK 0x80

/* PINS CS RST INT */
#define RFM95_CS 4
#define RFM95_RST 2
#define RFM95_INT 3


// Choix de la fréquence 
#define RF95_FREQ 868.1

RH_RF95 rf95(RFM95_CS, RFM95_INT);

// Blink LED on packet receipt (Adafruit M0 here)
#define LED 13
 

int16_t rxCount = 0;                                        // packet counter
uint8_t rxBuffer[RH_RF95_MAX_MESSAGE_LEN];                  // receive buffer
uint8_t rxRecvLen;                                          // number of bytes actually received
char printBuffer[512] = "\0";                               // to send output to the PC
char formatString[] = {"%d~%d~%d~%d~%d~%d~%2x~%s~%d~%s"};
char legendString[] = "Rx Count~Rx@millis~LastRSSI~FromAddr~ToAddr~MsgId~HdrFlags~isAck~PacketLen~PacketContents";

void setup() 
{
    pinMode(LED, OUTPUT);     
    pinMode(RFM95_RST, OUTPUT);
    digitalWrite(RFM95_RST, HIGH);

    while (!Serial);
    Serial.begin(9600);
    delay(100);

    Serial.print("Feather LoRa Network Probe [Mode=");
    Serial.print(mode == verbose ? "verbose" : "delimited");
    Serial.println("]");
    
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
    rf95.setModemConfig('Bw125Cr45Sf2048');


    // Addresse 
    rf95.setThisAddress(101);

    // need to setPromiscuous(true) to receive all frames
    rf95.setPromiscuous(true);

    // set delimiter
    if (mode == delimited) {
        if (DELIMITER_CHAR != '~') {
            for (int i = 0; i < sizeof(formatString); i++)
            {
                if (formatString[i] == '~')
                    formatString[i] = DELIMITER_CHAR;
            }
            for (int i = 0; i < sizeof(legendString); i++)
            {
                if (legendString[i] == '~')
                    legendString[i] = DELIMITER_CHAR;
            }
        }
        Serial.println(legendString);
    }}


void loop()
{
    // wait for a LoRa packet
    rxRecvLen = sizeof(rxBuffer);               // RadioHead expects max buffer, will update to received bytes
    digitalWrite(LED, LOW);
    

    if (rf95.recv(rxBuffer, &rxRecvLen)) {
        char isAck[4] = {""};
        if (rf95.headerFlags() & RH_FLAGS_ACK)
            memcpy(isAck, "Ack\0", 3);
        rxBuffer[rxRecvLen] = '\0';

        // Print complete header information in the following order : 
        // Rx count - Rx@millis - Last RSSI - To Addr - MsgId - Hdr Flags - isAck - Packet Len - Packet Contents
        snprintf(printBuffer, sizeof(printBuffer), "Packet Details:\n");
        Serial.print(printBuffer);
        
        snprintf(printBuffer, sizeof(printBuffer), " %d\n", rxCount++);
        Serial.print(printBuffer);

        snprintf(printBuffer, sizeof(printBuffer), "%d\n", millis());
        Serial.print(printBuffer);

        snprintf(printBuffer, sizeof(printBuffer), "%d\n", rf95.lastRssi());
        Serial.print(printBuffer);

        snprintf(printBuffer, sizeof(printBuffer), "%d\n", rf95.headerFrom());
        Serial.print(printBuffer);

        snprintf(printBuffer, sizeof(printBuffer), "%d\n", rf95.headerTo());
        Serial.print(printBuffer);

        snprintf(printBuffer, sizeof(printBuffer), "%d\n", rf95.headerId());
        Serial.print(printBuffer);

        snprintf(printBuffer, sizeof(printBuffer), "%2x\n", rf95.headerFlags());
        Serial.print(printBuffer);

        snprintf(printBuffer, sizeof(printBuffer), "%s\n", isAck);
        Serial.print(printBuffer);

        snprintf(printBuffer, sizeof(printBuffer), "%d\n", rxRecvLen);
        Serial.print(printBuffer);

        snprintf(printBuffer, sizeof(printBuffer), "%s\n", rxBuffer);
        Serial.print(printBuffer);

        Serial.println(); // Add a newline for better readability
    }
}
