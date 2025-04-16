/* MIT License
 *
 * Copyright (c) 2020 - 2025 Andreas Merkle <web@blue-andi.de>
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
#include "Settings.h"

#include <Log.h>

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

WIFI::WIFI() :
    m_isStaAvailable(false),
    m_apSSID(AP_MODE_SSID_DEFAULT),
    m_apPassword(AP_MODE_PASSWORD_DEFAULT),
    m_staSSID(),
    m_staPassword(),
    m_localIP()
{
}

WIFI::~WIFI()
{
}

bool WIFI::begin()
{
    bool isSuccess = true;

    Settings::getInstance().getWiFiSSD(m_staSSID);
    Settings::getInstance().getWiFiPassphrase(m_staPassword);

    if ((0 < m_staSSID.length()) &&
        (0 < m_staPassword.length()))
    {
        WiFi.mode(WIFI_STA);
        WiFi.begin(m_staSSID, m_staPassword);

        if (connectStation())
        {
            m_localIP = WiFi.localIP();
            m_isStaAvailable = true;
        }
        else
        {
            LOG_INFO("Network not in range or invalid credentials.");
            LOG_INFO("Starting AP...");

            WiFi.mode(WIFI_AP);
            WiFi.softAP(m_apSSID, m_apPassword);
            m_localIP = WiFi.softAPIP();
        }

            isSuccess = true;
        }
    else
    {
        LOG_INFO("No stored STA Credentials!");
        LOG_INFO("Starting AP");

        WiFi.mode(WIFI_AP);
        WiFi.softAP(m_apSSID, m_apPassword);
        m_localIP = WiFi.softAPIP();

        isSuccess = true;
    }

    LOG_INFO(getIPAddress().toString());

    return isSuccess;
}

bool WIFI::runCycle()
{
    bool isSuccess = true;

    if (m_isStaAvailable)
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
    return m_localIP;
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

    LOG_INFO(String("Connecting to \"") + m_staSSID + "\"...");
    while ((WiFi.status() != WL_CONNECTED) && ((millis() - startAttempTime) < WIFI_TIMEOUT_MS))
    {
        delay(100);
    }

    if (WL_CONNECTED == WiFi.status())
    {
        LOG_INFO("Connected Succesfully.");
        isSuccess = true;
        m_localIP = WiFi.localIP();
    }

    return isSuccess;
}

/******************************************************************************
 * External functions
 *****************************************************************************/

/******************************************************************************
 * Local functions
 *****************************************************************************/
