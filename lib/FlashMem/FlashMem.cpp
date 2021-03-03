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
 * @brief  Implememtation of FlashMem.h
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

    if (';' == char(EEPROM.read(NVM_SSID_ADDRESS)))
    {
        char temp;

        for (uint8_t i = 1; i < CREDENTIALS_MAX_LENGTH; i++)
        {
            temp = EEPROM.read(NVM_SSID_ADDRESS + i);
            if (';' != temp)
            {
                ssid += temp;
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
                    password += temp;
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

bool Flash::saveCredentials(const String &ssid, const String &password)
{
    bool isSuccess = false;

    Flash::clearEEPROM();

    uint8_t memoryPosition = 1;

    if ((!ssid.isEmpty()) &&
        (!password.isEmpty()) &&
        (ssid.length() < CREDENTIALS_MAX_LENGTH) &&
        (password.length() < CREDENTIALS_MAX_LENGTH))
    {
        /** Save SSID */
        EEPROM.write(NVM_SSID_ADDRESS, ';');

        for (memoryPosition = 1; memoryPosition <= ssid.length(); memoryPosition++)
        {
            EEPROM.write(NVM_SSID_ADDRESS + memoryPosition, ssid[memoryPosition - 1]);
        }

        EEPROM.write(NVM_SSID_ADDRESS + memoryPosition, ';');

        /** Save Password */
        EEPROM.write(NVM_PASSWORD_ADDRESS, ';');

        for (memoryPosition = 1; memoryPosition <= password.length(); memoryPosition++)
        {
            EEPROM.write(NVM_PASSWORD_ADDRESS + memoryPosition, password[memoryPosition - 1]);
        }

        EEPROM.write(NVM_PASSWORD_ADDRESS + memoryPosition, ';');

        if (EEPROM.commit())
        {
            Serial.println("EEPROM successfully committed");
            Serial.println("Credentials Received");
            Serial.println("SSID: " + ssid);
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
    for (int i = 0; i < EEPROM_SIZE; i++)
    {
        EEPROM.write(i, 0);
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