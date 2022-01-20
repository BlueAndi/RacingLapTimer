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

bool Flash::getCredentials(String &ssid, String &password)
{
    bool isSuccess = false;

    if (areCredentialsStored())
    {
        getString(NVM_SSID_ADDRESS, NVM_SSID_MAX_LENGTH, ssid);
        getString(NVM_PASSWORD_ADDRESS, NVM_PASSWORD_MAX_LENGTH, password);
        isSuccess = true;
    }

    return isSuccess;
}

bool Flash::setCredentials(const String &ssid, const String &password)
{
    bool isSuccess = false;

    /* Save SSID */
    if (setString(NVM_SSID_ADDRESS, NVM_SSID_MAX_LENGTH, ssid))
    {
        /* Save Password */
        if (setString(NVM_PASSWORD_ADDRESS, NVM_PASSWORD_MAX_LENGTH, password))
        {
            isSuccess = true;
        }
    }

    return setHeader(isSuccess);
}

void Flash::clearEEPROM()
{
    EEPROM.write(NVM_METADATA_ADDRESS, 0x00);
}

bool Flash::areCredentialsStored()
{
    bool areCredentialsStored = false;
    String storedHeader = "";

    getString(NVM_METADATA_ADDRESS, NVM_METADATA_MAX_LENGTH, storedHeader);

    if (NVM_METADATA_VALID.equals(storedHeader))
    {
        areCredentialsStored = true;
    }

    return areCredentialsStored;
}

bool Flash::setHeader(bool areCredentialsStored)
{
    bool isSuccess = false;

    if (areCredentialsStored)
    {
        setString(NVM_METADATA_ADDRESS, NVM_METADATA_MAX_LENGTH, NVM_METADATA_VALID);
    }
    else
    {
        clearEEPROM();
    }

    if (EEPROM.commit())
    {
        isSuccess = true;
    }

    return isSuccess;
}

void Flash::getString(const uint16_t &address, const uint8_t &maxLength, String &output)
{
    output.clear();

    for (uint8_t memoryPosition = 0; memoryPosition < maxLength; memoryPosition++)
    {
        char temp = EEPROM.read(address + memoryPosition);

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

bool Flash::setString(const uint16_t &address, const uint8_t &maxLength, const String &input)
{
    bool isSuccess = false;

    for (uint8_t memoryPosition = 0; memoryPosition < maxLength; memoryPosition++)
    {
        EEPROM.write(address + memoryPosition, input[memoryPosition]);
        if (0x00 == input[memoryPosition])
        {
            isSuccess = true;
            break;
        }
    }
    return isSuccess;
}

bool Flash::getUInt8(const uint16_t &address, uint8_t &value)
{
    bool isSuccess = false;

    for (uint8_t memoryPosition = 0; memoryPosition < NVM_GROUPS_LENGTH; memoryPosition++)
    {
        value = EEPROM.read(address + memoryPosition);
        isSuccess = true;
    }
    return isSuccess;
}

bool Flash::setUInt8(const uint16_t &address, uint8_t value)
{
    bool isSuccess = false;

    EEPROM.write(address, value);

    if (EEPROM.commit())
    {
        isSuccess = true;
    }

    return isSuccess;
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