/**************************************************************************************************
* File: WLAN.cpp
* @brief: Implementation of WLAN.h
* @author: Gabryel Reyes <gabryelrdiaz@gmail.com>
**************************************************************************************************/
/* INCLUDES **************************************************************************************/
#include "WLAN.h"

/* CONSTANTS *************************************************************************************/

static const uint32_t WIFI_TIMEOUT_MS = 20000; /**< Timeout for WiFi connection */

/* MACROS ****************************************************************************************/

/* TYPES *****************************************************************************************/

/* PROTOTYPES ************************************************************************************/

bool connectStation(); /**< Connect to Wireless Access Point */

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

    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(AP_SSID, AP_PASSWORD);
    Serial.println(WiFi.softAPIP());

    if (!STA_SSID.isEmpty())
    {
        WiFi.begin(STA_SSID, STA_PASSWORD);

        if (connectStation())
        {
            success = true;
            localIP = WiFi.localIP();
            staAvailable = true;
            Serial.println(localIP);
        }
    }
    else
    {
        Serial.println("No stored STA Credentials!");
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

bool connectStation()
{
    bool success = false;

    uint32_t startConnect = millis();

    while ((WL_CONNECTED != WiFi.status()) && ((millis() - startConnect) < WIFI_TIMEOUT_MS))
    {
    }

    if (WL_CONNECTED == WiFi.status())
    {
        success = true;
    }

    return success;
}