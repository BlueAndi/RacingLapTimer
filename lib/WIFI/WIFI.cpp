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
 * @brief  Implementation of Wireless Capabilities.
 * @author Gabryel Reyes <gabryelrdiaz@gmail.com>
 */

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "WIFI.h"
#include "FlashMem.h"

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

WIFI::WIFI() : WIFI_TIMEOUT_MS(30000), m_ApSSID("RacingLapTimer"), m_ApPassword("let me in"), m_Sta_SSID(""), m_Sta_Password("")
{
}

WIFI::~WIFI()
{
}

bool WIFI::begin()
{
    bool isSuccess = false;

    Flash::begin();

    if (Flash::importCredentials(m_Sta_SSID, m_Sta_Password))
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

/******************************************************************************
 * External functions
 *****************************************************************************/

/******************************************************************************
 * Local functions
 *****************************************************************************/
