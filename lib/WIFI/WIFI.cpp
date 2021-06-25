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

extern "C"
{
#include "user_interface.h"
#include "wpa2_enterprise.h"
}

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

WIFI::WIFI() : m_isStaAvailable(false),
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

    if (Flash::getCredentials(m_staSSID, m_staPassword))
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
            Serial.println("Network not in range or Unvalid Credentials.");
            Serial.println("Starting AP...");

            WiFi.mode(WIFI_AP);
            WiFi.softAP(m_apSSID, m_apPassword);
            m_localIP = WiFi.softAPIP();
        }

        isSuccess = true;
    }
    else
    {
        Serial.println("No stored STA Credentials!");
        Serial.println("Starting AP");

        WiFi.mode(WIFI_AP);
        WiFi.softAP(m_apSSID, m_apPassword);
        m_localIP = WiFi.softAPIP();

        isSuccess = true;
    }

    Serial.println(getIPAddress());

    return isSuccess;
}

bool WIFI::beginWPA()
{
    bool isSuccess = false;

    // SSID to connect to
    static const char *ssid = "eduroam";
    // Username for authentification
    static const char *username = "username";
    // Identity for authentification
    static const char *identity = "username@hs-ulm.de";
    // Password for authentication
    static const char *password = "password";

    WiFi.mode(WIFI_STA);
    wifi_set_opmode(STATION_MODE);
    wifi_station_dhcpc_stop();

    //Static IP address configuration
    IPAddress staticIP(141, 59, 135, 184); //ESP static ip
    IPAddress gateway(141, 59, 128, 254);  //IP Address of your WiFi Router (Gateway)
    IPAddress subnet(255, 255, 240, 0);    //Subnet mask
    IPAddress dns1(141, 59, 85, 20);       //DNS1
    IPAddress dns2(141, 59, 85, 21);       //DNS2
    WiFi.config(staticIP, gateway, subnet, dns1, dns2);

    struct station_config wifi_config;

    memset(&wifi_config, 0, sizeof(wifi_config));
    strcpy((char *)wifi_config.ssid, ssid);
    strcpy((char *)wifi_config.password, password);

    wifi_station_set_config(&wifi_config);

    wifi_station_set_wpa2_enterprise_auth(1);

    // Clean up to be sure no old data is still inside
    wifi_station_clear_cert_key();
    wifi_station_clear_enterprise_ca_cert();
    wifi_station_clear_enterprise_identity();
    wifi_station_clear_enterprise_username();
    wifi_station_clear_enterprise_password();
    wifi_station_clear_enterprise_new_password();

    wifi_station_set_enterprise_identity((uint8 *)identity, strlen(identity));
    wifi_station_set_enterprise_username((uint8 *)username, strlen(username));
    wifi_station_set_enterprise_password((uint8 *)password, strlen((char *)password));

    wifi_station_connect();

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(".");
    }

    m_localIP = WiFi.localIP();
    m_isStaAvailable = true;
    isSuccess = true;

    return isSuccess;
}

bool WIFI::runCycle()
{
    bool isSuccess = true;
/*
    if (m_isStaAvailable)
    {
        if (WL_CONNECTED != WiFi.status())
        {
            isSuccess = connectStation();
        }
    }
    */
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

    Serial.println("Connecting to \"" + m_staSSID + "\"...");
    while ((WiFi.status() != WL_CONNECTED) && ((millis() - startAttempTime) < WIFI_TIMEOUT_MS))
    {
        delay(100);
    }

    if (WL_CONNECTED == WiFi.status())
    {
        Serial.println("Connected Succesfully.");
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
