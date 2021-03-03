/* MIT License
 *
 * Copyright (c) 2020-2021 Andreas Merkle <web@blue-andi.de>
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

/*******************************************************************************
    DESCRIPTION
*******************************************************************************/
/**
 * @brief  Implementation of WIFI.h.
 * @author Gabryel Reyes <gabryelrdiaz@gmail.com>
 */

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "WIFI.h"
#include <EEPROM.h>

/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and Classes
 *****************************************************************************/

/******************************************************************************
 * Prototypes
 *****************************************************************************/

/******************************************************************************
 * Local Variables
 *****************************************************************************/

/******************************************************************************
 * Public Methods
 *****************************************************************************/

WIFI::WIFI() : m_ApSSID("RacingLapTimer"), m_ApPassword("let me in"), m_Sta_SSID(""), m_Sta_Password("")
{
}

WIFI::~WIFI()
{
}

bool WIFI::begin()
{
    bool isSuccess = false;

    EEPROM.begin(512);

    if (importCredentials())
    {
        WiFi.mode(WIFI_STA);
        WiFi.begin(m_Sta_SSID, m_Sta_Password);

        if (connectStation())
        {
            isSuccess = true;
            m_LocalIP = WiFi.localIP();
            isStaAvailable = true;
        }
        else
        {
            Serial.println("Network not in range or Unvalid Credentials.");
            Serial.println("Starting AP...");

            WiFi.mode(WIFI_AP);
            WiFi.softAP(m_ApSSID, m_ApPassword);
            m_LocalIP = WiFi.softAPIP();
            isSuccess = true;
        }
    }
    else
    {
        Serial.println("No stored STA Credentials!");
        Serial.println("Starting AP");

        WiFi.mode(WIFI_AP);
        WiFi.softAP(m_ApSSID, m_ApPassword);
        m_LocalIP = WiFi.softAPIP();

        isSuccess = true;
    }

    Serial.println(getIPAddress());

    return isSuccess;
}

bool WIFI::runCycle()
{
    bool isSuccess = true;

    if (isStaAvailable)
    {
        if (WL_CONNECTED != WiFi.status())
        {
            isSuccess = connectStation();
        }
    }
    return isSuccess;
}

bool WIFI::saveCredentials(const String &ssid, const String &password)
{
    bool isSuccess = false;

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
        isSuccess = true;
    }
    return isSuccess;
}

const IPAddress &WIFI::getIPAddress(void)
{
    return m_LocalIP;
}

/******************************************************************************
 * Protected Methods
 *****************************************************************************/

/******************************************************************************
 * Private Methods
 *****************************************************************************/

bool WIFI::connectStation()
{
    bool isSuccess = false;

    unsigned long startAttempTime = millis();

    Serial.println("Connecting to \"" + m_Sta_SSID + "\"...");
    while ((WiFi.status() != WL_CONNECTED) && ((millis() - startAttempTime) < WIFI_TIMEOUT_MS))
    {
        delay(100);
    }

    if (WL_CONNECTED == WiFi.status())
    {
        Serial.println("Connected Succesfully.");
        isSuccess = true;
        m_LocalIP = WiFi.localIP();
    }

    return isSuccess;
}

bool WIFI::importCredentials()
{
    bool isSuccess = false;

    if (';' == char(EEPROM.read(NVM_SSID_ADDRESS)))
    {
        char temp;

        for (uint8_t i = 1; i < CREDENTIALS_MAX_LENGTH; i++)
        {
            temp = EEPROM.read(NVM_SSID_ADDRESS + i);
            if (';' != temp)
            {
                m_Sta_SSID += temp;
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
                    m_Sta_Password += temp;
                }
                else
                {
                    break;
                }
            }
        }

        isSuccess = true;
    }

    return isSuccess;
}

void WIFI::clearEEPROM()
{
    for (int i = 0; i < 512; i++)
    {
        EEPROM.write(i, 0);
    }
}

/******************************************************************************
 * External functions
 *****************************************************************************/

/******************************************************************************
 * Local functions
 *****************************************************************************/
