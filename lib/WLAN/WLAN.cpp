/* MIT License
 *
 * Copyright (c) 2020 Andreas Merkle <web@blue-andi.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
/**************************************************************************************************
* File: WLAN.cpp
* @brief: Implementation of WLAN.h.
* @author: Gabryel Reyes <gabryelrdiaz@gmail.com>
**************************************************************************************************/
/* INCLUDES **************************************************************************************/
#include "WLAN.h"
#include <EEPROM.h>

/* CONSTANTS *************************************************************************************/

static const unsigned long WIFI_TIMEOUT_MS = 5000; /**< Timeout for WiFi connection. */
static const uint8_t CREDENTIALS_MAX_LENGTH = 50;  /**< Maximum length for saved Credentials. */
static const uint8_t NVM_SSID_ADDRESS = 0;         /**< Address of saved SSID in EEPROM. */
static const uint8_t NVM_PASSWORD_ADDRESS = 50;    /**< Address of saved Password in EEPROM. */

/* MACROS ****************************************************************************************/

/* TYPES *****************************************************************************************/

/* PROTOTYPES ************************************************************************************/

/* VARIABLES *************************************************************************************/

static bool staAvailable = false; /**< True if system is connected in STA mode to a Network. */

/* PUBLIC METHODES *******************************************************************************/

/**************************************************************************************************/

/**
*   @brief Default Constructor.
*/
WIFI::WIFI() : AP_SSID("RacingLapTimer"), AP_PASSWORD("let me in"), STA_SSID(""), STA_PASSWORD("")
{
}

/**************************************************************************************************/

/**
*   @brief Default Destructor.
*/
WIFI::~WIFI()
{
}

/**************************************************************************************************/

/**
*   @brief Initialize WIFI Module.
*   @return Success.
*/
bool WIFI::begin()
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
        }
        else
        {
            Serial.println("Network not in range or Unvalid Credentials.");
            Serial.println("Starting AP...");

            WiFi.mode(WIFI_AP);
            WiFi.softAP(AP_SSID, AP_PASSWORD);
            localIP = WiFi.softAPIP();
            success = true;
        }
    }
    else
    {
        Serial.println("No stored STA Credentials!");
        Serial.println("Starting AP");

        WiFi.mode(WIFI_AP);
        WiFi.softAP(AP_SSID, AP_PASSWORD);
        localIP = WiFi.softAPIP();

        success = true;
    }

    Serial.println(getIPAddress());

    return success;
}

/**************************************************************************************************/

/**
*   @brief Executes WIFI Connection Check.
*   @return Success.
*/
bool WIFI::cycle()
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
 * @brief Saves new STA Credentials.
 * 
 * @param ssid SSID of host network to connect to.
 * @param password Password of host network to connect to.
 * @return Success.
 */
bool WIFI::saveCredentials(const String &ssid, const String &password)
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
*   @brief Returns the IP Address of the System.
*   @return local IP.
*/
const IPAddress &WIFI::getIPAddress(void)
{
    return localIP;
}

/* PROTECTED METHODES ****************************************************************************/

/* PRIVATE METHODES ******************************************************************************/

/**************************************************************************************************/

/**
*   @brief Connect to Wireless Access Point.
*   @return Success.
*/
bool WIFI::connectStation()
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
*   @brief Imports Credentials from EEPROM.
*   @return Success.
*/
bool WIFI::importCredentials()
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
*  @brief Deletes stored Credentials.
*/
void WIFI::clearEEPROM()
{
    for (int i = 0; i < 512; i++)
    {
        EEPROM.write(i, 0);
    }
}

/* EXTERNAL FUNCTIONS ****************************************************************************/

/* INTERNAL FUNCTIONS ****************************************************************************/