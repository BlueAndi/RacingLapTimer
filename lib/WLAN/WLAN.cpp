/**************************************************************************************************
* File: WLAN.cpp
* @brief: Implementation of WLAN.h
* @author: Gabryel Reyes <gabryelrdiaz@gmail.com>
**************************************************************************************************/
/* INCLUDES **************************************************************************************/
#include "WLAN.h"

/* CONSTANTS *************************************************************************************/

static const unsigned long WIFI_TIMEOUT_MS = 5000; /**< Timeout for WiFi connection */

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

    /*
    * IMPORT CREDENTIALS FROM MEMORY HERE
    */

    if (STA_SSID != "")
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
bool WLAN::saveCredentials(String ssid, String password)
{
    bool success = true;

    Serial.println("Credentials Received");
    /* Code */

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

/* EXTERNAL FUNCTIONS ****************************************************************************/

/* INTERNAL FUNCTIONS ****************************************************************************/

/**************************************************************************************************/

/**
*   Connect to Wireless Access Point
*/

bool WLAN::connectStation()
{
    bool success = false;

    unsigned long startAttempTime = millis();

    Serial.println("Connecting...");
    while ((WiFi.status() != WL_CONNECTED) && ((millis() - startAttempTime) < WIFI_TIMEOUT_MS))
    {
        delay(100);
    }

    if (WL_CONNECTED == WiFi.status())
    {
        Serial.println("Connected");
        success = true;
        localIP = WiFi.localIP();
    }

    return success;
}