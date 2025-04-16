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
 * @brief  Settings stored in persistent memory
 * @author Andreas Merkle <web@blue-andi.de>
 */

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "Settings.h"
#include "FlashMem.h"

/******************************************************************************
 * Compiler Switches
 *****************************************************************************/

/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and classes
 *****************************************************************************/

/******************************************************************************
 * Prototypes
 *****************************************************************************/

/******************************************************************************
 * Local Variables
 *****************************************************************************/

/** Address of Meta Data header in EEPROM. */
static const uint16_t NVM_METADATA_ADDRESS = 0;

/** Meta Data maximal length. */
static const uint8_t NVM_METADATA_MAX_LENGTH = 2;

/** Metadata for Valid Stored Credetials */
static const String NVM_METADATA_VALID = "UZ";

/** Address of saved SSID in EEPROM. */
static const uint16_t NVM_SSID_ADDRESS = NVM_METADATA_ADDRESS + NVM_METADATA_MAX_LENGTH;

/** SSID maximal length */
static const uint8_t NVM_SSID_MAX_LENGTH = 32;

/** Address of saved Password in EEPROM. */
static const uint16_t NVM_PASSWORD_ADDRESS = NVM_SSID_ADDRESS + NVM_SSID_MAX_LENGTH;

/** WPA-PSK Password laximal length */
static const uint8_t NVM_PASSWORD_MAX_LENGTH = 63;

/** Address of saved Groups in EEPROM. */
static const uint16_t NVM_GROUPS_ADDRESS = NVM_PASSWORD_ADDRESS + NVM_PASSWORD_MAX_LENGTH;

/** Length of saved Groups in EEPROM. */
static const uint8_t NVM_GROUPS_LENGTH = 1;

/** Address of the saved group names in EEPROM. */
static const uint16_t NVM_GROUP_NAMES_ADDRESS = NVM_GROUPS_ADDRESS + NVM_GROUPS_LENGTH;

/** Max. number of groups. */
static const uint8_t NVM_MAX_GROUPS = 10;

/** Max. length of group name, including string termination. */
static const uint8_t NVM_MAX_GROUP_NAME_SIZE = 20;

/** Length of saved group names in EEPROM. */
static const uint8_t NVM_GROUP_NAMES_LENGTH = NVM_MAX_GROUPS * NVM_MAX_GROUP_NAME_SIZE;

/******************************************************************************
 * Public Methods
 *****************************************************************************/

bool Settings::begin()
{
    bool isSuccess = FlashMem::begin();

    if (true == isSuccess)
    {
        String metaData;

        FlashMem::getString(NVM_METADATA_ADDRESS, NVM_METADATA_MAX_LENGTH, metaData);

        /* Is persistent memory invalid? */
        if (0 == metaData.equals(NVM_METADATA_VALID))
        {
            uint8_t idx = 0;

            /* Restore factory settings. */
            (void)FlashMem::setString(NVM_METADATA_ADDRESS, NVM_METADATA_MAX_LENGTH, NVM_METADATA_VALID);
            setWiFiSSID("");
            setWiFiPassphrase("");
            setNumberOfGroups(3);
            
            for(idx = 0; idx < NVM_MAX_GROUPS; ++idx)
            {
                setGroupName(idx, "");
            }
        }
    }

    return isSuccess;
}

void Settings::getWiFiSSD(String& ssid)
{
    FlashMem::getString(NVM_SSID_ADDRESS, NVM_SSID_MAX_LENGTH, ssid);
}

void Settings::setWiFiSSID(const String& ssid)
{
    (void)FlashMem::setString(NVM_SSID_ADDRESS, NVM_SSID_MAX_LENGTH, ssid);
}

void Settings::getWiFiPassphrase(String& passphrase)
{
    FlashMem::getString(NVM_PASSWORD_ADDRESS, NVM_PASSWORD_MAX_LENGTH, passphrase);
}

void Settings::setWiFiPassphrase(const String& passphrase)
{
    (void)FlashMem::setString(NVM_PASSWORD_ADDRESS, NVM_PASSWORD_MAX_LENGTH, passphrase);
}

void Settings::getNumberOfGroups(uint8_t& numberOfGroups)
{
    FlashMem::getUInt8(NVM_GROUPS_ADDRESS, numberOfGroups);
}

void Settings::setNumberOfGroups(uint8_t numberOfGroups)
{
    (void)FlashMem::setUInt8(NVM_GROUPS_ADDRESS, numberOfGroups);
}

void Settings::getGroupName(uint8_t idx, String& name)
{
    FlashMem::getString(NVM_GROUP_NAMES_ADDRESS + idx * NVM_MAX_GROUP_NAME_SIZE, NVM_MAX_GROUP_NAME_SIZE, name);
}

void Settings::setGroupName(uint8_t idx, const String& name)
{
    (void)FlashMem::setString(NVM_GROUP_NAMES_ADDRESS + idx * NVM_MAX_GROUP_NAME_SIZE, NVM_MAX_GROUP_NAME_SIZE, name);
}

/******************************************************************************
 * Protected Methods
 *****************************************************************************/

/******************************************************************************
 * Private Methods
 *****************************************************************************/

/******************************************************************************
 * External Functions
 *****************************************************************************/

/******************************************************************************
 * Local Functions
 *****************************************************************************/