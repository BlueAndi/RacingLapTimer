/**************************************************************************************************
* File: WLAN.cpp
* @brief: Implementation of WLAN.h
* @author: Gabryel Reyes <gabryelrdiaz@gmail.com>
**************************************************************************************************/
/* INCLUDES **************************************************************************************/
#include "WLAN.h"
#include <EEPROM.h>

/* CONSTANTS *************************************************************************************/

static const unsigned long WIFI_TIMEOUT_MS = 5000; /**< Timeout for WiFi connection */
static const uint8_t CREDENTIALS_MAX_LENGTH = 50;  /**< Maximum length for saved Credentials */
static const uint8_t NVM_SSID_ADDRESS = 0;         /**< Address of saved SSID in EEPROM */
static const uint8_t NVM_PASSWORD_ADDRESS = 50;    /**< Address of saved Password in EEPROM */

/* MACROS ****************************************************************************************/

/* TYPES *****************************************************************************************/

/* PROTOTYPES ************************************************************************************/

/* VARIABLES *************************************************************************************/

static bool staAvailable = false;

/* PUBLIC METHODES *******************************************************************************/

/**************************************************************************************************/

/**
*   Default Constructor
*/
WLAN::WLAN(/* args */)
{
}

/**************************************************************************************************/

/**
*   Default Destructor
*/
WLAN::~WLAN()
{
}

/**************************************************************************************************/

/**
*   Initialize WLAN Module
*/
bool WLAN::begin()
{
    bool success = false;

    EEPROM.begin(512);

    if (importCredentials())
    {
        WiFi.mode(WIFI_STA);
        WiFi.begin(STA_SSID, STA_PASSWORD);

        if (connectStation())
        {
            success = true;
            localIP = WiFi.localIP();
            staAvailable = true;
            Serial.println(localIP);
        }
        else
        {
            Serial.println("Network not in range or Unvalid Credentials.");
            Serial.println("Starting AP...");

            WiFi.mode(WIFI_AP);
            WiFi.softAP(AP_SSID, AP_PASSWORD);

            Serial.println(WiFi.softAPIP());
            success = true;
        }
    }
    else
    {
        Serial.println("No stored STA Credentials!");
        Serial.println("Starting AP");

        WiFi.mode(WIFI_AP);
        WiFi.softAP(AP_SSID, AP_PASSWORD);

        Serial.println(WiFi.softAPIP());

        success = true;
    }

    return success;
}

/**************************************************************************************************/

/**
*   Executes WLAN Connection Check
*/
bool WLAN::cycle()
{
    bool success = true;

    if (staAvailable)
    {
        if (WL_CONNECTED != WiFi.status())
        {
            success = connectStation();
        }
    }
    return success;
}

/**************************************************************************************************/

/**
*   Saves new STA Credentials
*/
bool WLAN::saveCredentials(const String &ssid, const String &password)
{
    bool success = false;

    clearEEPROM();

    uint8_t i = 1;
    EEPROM.write(NVM_SSID_ADDRESS, ';');
    for (i = 1; i <= ssid.length(); i++)
    {
        EEPROM.write(NVM_SSID_ADDRESS + i, ssid[i - 1]);
    }
    EEPROM.write(NVM_SSID_ADDRESS + i, ';');

    EEPROM.write(NVM_PASSWORD_ADDRESS, ';');
    for (i = 1; i <= password.length(); i++)
    {
        EEPROM.write(NVM_PASSWORD_ADDRESS + i, password[i - 1]);
    }
    EEPROM.write(NVM_PASSWORD_ADDRESS + i, ';');

    if (EEPROM.commit())
    {
        Serial.println("EEPROM successfully committed");
    }
    else
    {
        Serial.println("ERROR! EEPROM commit failed");
    }

    if (true)
    {
        Serial.println("Credentials Received");
        success = true;
    }
    return success;
}

/**************************************************************************************************/

/**
*   Returns the IP Address of the System
*/
const IPAddress &WLAN::getIPAddress(void)
{
    return localIP;
}

/* PROTECTED METHODES ****************************************************************************/

/* PRIVATE METHODES ******************************************************************************/

/**************************************************************************************************/

/**
*   Connect to Wireless Access Point
*/

bool WLAN::connectStation()
{
    bool success = false;

    unsigned long startAttempTime = millis();

    Serial.println("Connecting to \"" + STA_SSID + "\"...");
    while ((WiFi.status() != WL_CONNECTED) && ((millis() - startAttempTime) < WIFI_TIMEOUT_MS))
    {
        delay(100);
    }

    if (WL_CONNECTED == WiFi.status())
    {
        Serial.println("Connected Succesfully.");
        success = true;
        localIP = WiFi.localIP();
    }

    return success;
}

/**************************************************************************************************/

/**
*   Imports Credentials from EEPROM
*/
bool WLAN::importCredentials()
{
    bool success = false;

    if (';' == char(EEPROM.read(NVM_SSID_ADDRESS)))
    {
        char temp;

        for (uint8_t i = 1; i < CREDENTIALS_MAX_LENGTH; i++)
        {
            temp = EEPROM.read(NVM_SSID_ADDRESS + i);
            if (';' != temp)
            {
                STA_SSID += temp;
            }
            else
            {
                break;
            }
        }

        if (';' == char(EEPROM.read(NVM_PASSWORD_ADDRESS)))
        {
            for (uint8_t i = 1; i < CREDENTIALS_MAX_LENGTH; i++)
            {
                temp = EEPROM.read(NVM_PASSWORD_ADDRESS + i);
                if (';' != temp)
                {
                    STA_PASSWORD += temp;
                }
                else
                {
                    break;
                }
            }
        }

        success = true;
    }

    return success;
}

/**************************************************************************************************/

/**
*  Deletes stored Credentials
*/
void WLAN::clearEEPROM()
{
    for (int i = 0; i < 512; i++)
    {
        EEPROM.write(i, 0);
    }
}

/* EXTERNAL FUNCTIONS ****************************************************************************/

/* INTERNAL FUNCTIONS ****************************************************************************/