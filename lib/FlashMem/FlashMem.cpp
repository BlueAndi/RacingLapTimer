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
 * @brief  Abstraction of EEPROM for the ESP8266
 * @author Gabryel Reyes <gabryelrdiaz@gmail.com>
 */

/******************************************************************************
 * Includes
 *****************************************************************************/
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

/******************************************************************************
 * Public Methods
 *****************************************************************************/

bool Flash::begin()
{
    bool isSuccess = true;
    EEPROM.begin(EEPROM_SIZE);
    return isSuccess;
}

bool Flash::importCredentials(String &ssid, String &password)
{
    bool isSuccess = false;

    if (areCredentialsStored())
    {
        fetchString(NVM_SSID_ADDRESS, NVM_SSID_MAX_LENGTH, ssid);
        fetchString(NVM_PASSWORD_ADDRESS, NVM_PASSWORD_MAX_LENGTH, password);
        isSuccess = true;
    }

    return isSuccess;
}

bool Flash::saveCredentials(const String &ssid, const String &password)
{
    bool isSuccess = false;

    Flash::clearEEPROM();

    if ((!ssid.isEmpty()) && (ssid.length() <= CREDENTIALS_MAX_LENGTH))
    {
        /* Save SSID */
        for (uint8_t memoryPosition = 0; memoryPosition <= ssid.length(); memoryPosition++)
        {
            EEPROM.write(NVM_SSID_ADDRESS + memoryPosition, ssid[memoryPosition]);
            Serial.print(ssid[memoryPosition]);
        }
        Serial.println();

        if ((!password.isEmpty()) && (password.length() <= CREDENTIALS_MAX_LENGTH))
        {
            /* Save Password */
            for (uint8_t memoryPosition = 0; memoryPosition <= password.length(); memoryPosition++)
            {
                EEPROM.write(NVM_PASSWORD_ADDRESS + memoryPosition, password[memoryPosition]);
                Serial.print(password[memoryPosition]);
            }
            Serial.println();
        }

        /* Write 0x55 and 0xA5 to Metadata Header are Credentials are Stored) */
        EEPROM.write(NVM_METADATA_ADDRESS, 0x55);
        EEPROM.write(NVM_METADATA_ADDRESS + 1, 0xA5);

        if (EEPROM.commit())
        {
            Serial.println("EEPROM successfully committed");
            isSuccess = true;
        }
        else
        {
            Serial.println("ERROR! EEPROM commit failed");
        }
    }

    return isSuccess;
}

void Flash::clearEEPROM()
{
    EEPROM.write(NVM_METADATA_ADDRESS, 0x00);
    EEPROM.write(NVM_METADATA_ADDRESS + 1, 0x00);
}

bool Flash::areCredentialsStored()
{
    bool areCredentialsStored = false;
    if ((0x55 == EEPROM.read(NVM_METADATA_ADDRESS)) &&
        (0xA5 == EEPROM.read(NVM_METADATA_ADDRESS + 1)))
    {
        areCredentialsStored = true;
    }
    return areCredentialsStored;
}

void Flash::fetchString(const uint8_t address, const uint8_t maxLength, String &output)
{
    char temp = 0x00;

    for (uint8_t memoryPosition = 0; memoryPosition <= maxLength; memoryPosition++)
    {
        temp = EEPROM.read(address + memoryPosition);

        if (0x00 == temp)
        {
            break;
        }
        else
        {
            output += temp;
        }
    }
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