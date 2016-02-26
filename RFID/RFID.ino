// #####################################################################################################################
// ######################################### RFID ######################################################################
// ##################################################################################################################### 
/*
    This script shows you how to use the RC522 RFID-module and it's library on an Arduino UNO/Nano.
    The basic idea is:
        - Check if a card is present
        - If so, check if the cards unique ID is valid.
        - If so, turn on a LED
        - Otherwise flash the LED rapid

    Wiring
        - Pin 9     RST
        - Pin 10    SDA
        - Pin 11    MOSI
        - Pin 12    MISO
        - Pin 13    SCK
        - 3V3       3V3
        - GND       GND

    For further information, please visit my blog.
*/
// Comment this line out, if you don't want a serial communication
#define SERIAL

// Get the neccessarry libs
#include <SPI.h>
#include <MFRC522.h>

// Get the size of the array (used later)
#define SIZE(x) (sizeof(x) / sizeof((x)[0]))

// Define pins
#define LED 2
#define RFID_RST 9
#define RFID_SDA 10

// Name the receiver
MFRC522 RFID(RFID_SDA, RFID_RST);

byte* UID_Array = 0;
byte UID_Array_Size = 0;

// Enter your card-ID here
byte Access_UID_Array[2][7] = {{115, 130, 10, 1, 0, 0, 0}, {128, 60, 2, 1, 0, 0, 0}};
// ##################################################################################################################### 
// ######################################### SETUP #####################################################################
// ##################################################################################################################### 
void setup()
{
    pinMode(LED, OUTPUT);
    digitalWrite(LED, LOW);

    // Initialize the SPI BUS and the RFID-module
    SPI.begin();
    RFID.PCD_Init();

    // Only print to serial monitor if it has been defined.
    // If you don't need a serial monitor, comment out line 24.
    #ifdef SERIAL
    Serial.begin(9600);
    Serial.print("RFID receiver initialized.");
    #endif
}
// ##################################################################################################################### 
// ######################################### LOOP ######################################################################
// ##################################################################################################################### 
void loop()
{
    /*  
        If a card is recognized, then go on and check if it is valid.
        If so, then turn on the LED for 2 seconds, otherwise flash the LED rapid.
    */
    if (getUID())
    {
        if (checkUID())
        {
            digitalWrite(LED, HIGH);
            delay(2000);
            digitalWrite(LED, LOW);
        }
        else 
        {
            for (int i = 0; i < 5; i++)
            {
                digitalWrite(LED, HIGH);
                delay(100);
                digitalWrite(LED, LOW);
                delay(100);
            }
        }
    }
}
// ##################################################################################################################### 
// ######################################### GET UNIQUE ID #############################################################
// ##################################################################################################################### 
boolean getUID()
{
    // Check if a card is resent and also if it can be accessed.
    if (RFID.PICC_IsNewCardPresent() && RFID.PICC_ReadCardSerial())
    {
        #ifdef SERIAL
        Serial.print("\n\nCard detected.\n  ID: ");
        #endif
        /*
            The array size shall have the size of the unique ID (4 or 7)
            Therefore we delete array and create a new one with the size of the UID.
        */
        delete [] UID_Array;
        UID_Array_Size = RFID.uid.size;
        UID_Array = new byte [UID_Array_Size];

        // Get the UID from the card
        for (byte i = 0; i < UID_Array_Size; i++)
        {
            UID_Array[i] = RFID.uid.uidByte[i];

            #ifdef SERIAL
            Serial.print(UID_Array[i]);
            Serial.print(" ");
            #endif
        }

        return true;
    }
    // If there is no card or it can't be accessed, then return false
    else 
        return false;
}
// ##################################################################################################################### 
// ######################################### CHECK UNIQUE ID ###########################################################
// #####################################################################################################################
boolean checkUID()
{
    boolean Access = false;
    /*
        Here we check, if the cards UID is stored in the array, which we have defined at the beginning.
        Therefore we need two for-loops. The first one goes trough the stored cards: card 1, card 2, ..., card n.
        The second one goes trough the 4 or 7 blocks of the UID.
        If every block of the stored card matches with ever block from the present one, then return true.

        Attention: If only one card is found with "all-true", the checkUID-function will be terminated, and the true
        statement will be returned. All other stored cards won't be checked.
        If there is no match at all, false will be returned.
    */
    for (byte i = 0; i < SIZE(Access_UID_Array); i++)
    {
        for (byte j = 0; j < UID_Array_Size; j++)
        {
            Access = false;
            if (Access_UID_Array[i][j] != UID_Array[j])
                break;
            else 
                Access = true;
        }

        if (Access)
        {
            #ifdef SERIAL
            Serial.print("\n  Access granted.");
            #endif
            return true;
        }
    }

    #ifdef SERIAL
    Serial.print("\n  Access denied.");
    #endif
    return false;
} 
// ##################################################################################################################### 
// ######################################### END OF CODE ###############################################################
// ##################################################################################################################### 