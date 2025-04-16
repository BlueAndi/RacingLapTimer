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

/** Size of the EEPROM destined to store credentials*/
static const uint16_t EEPROM_SIZE = 512;

/******************************************************************************
 * Public Methods
 *****************************************************************************/

bool FlashMem::begin()
{
    bool isSuccess = true;
    
    EEPROM.begin(EEPROM_SIZE);
    return isSuccess;
}

void FlashMem::getString(const uint16_t &address, const uint8_t &maxLength, String &output)
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

bool FlashMem::setString(const uint16_t &address, const uint8_t &maxLength, const String &input)
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

    if (true == isSuccess)
    {
        if (false == EEPROM.commit())
        {
            isSuccess = false;
        }
    }

    return isSuccess;
}

bool FlashMem::getUInt8(const uint16_t &address, uint8_t &value)
{
    value = EEPROM.read(address);

    return true;
}

bool FlashMem::setUInt8(const uint16_t &address, uint8_t value)
{
    bool isSuccess = false;

    EEPROM.write(address, value);

    if (true == EEPROM.commit())
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