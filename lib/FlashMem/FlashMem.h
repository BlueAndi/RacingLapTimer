/* MIT License
 *
 * Copyright (c) 2020-2022 Andreas Merkle <web@blue-andi.de>
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

#ifndef FLASHMEM_H_
#define FLASHMEM_H_

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <EEPROM.h>
#include <Arduino.h>

/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and Classes
 *****************************************************************************/

namespace FlashMem
{
    /**
     *  Initialization of the EEPROM module.
     * 
     *  @return If Initialization is successful, return true. Otherwise false.
     */
    bool begin();

    /**
     *  Retrieves a Null-terminated String from the EEPROM.
     *
     *  @param[in] address Address where the String is saved.
     *  @param[in] maxLength Maximum Length of the String.
     *  @param[out] output Buffer to save the String to.
     */
    void getString(const uint16_t &address, const uint8_t &maxLength, String &output);

    /**
     *  Saves a Null-terminated String in the EEPROM.
     *
     *  @param[in] address Address where the String will be saved.
     *  @param[in] maxLength Maximum Length of the String.
     *  @param[in] input String to save in EEPROM.
     *  @return If string written in EEPROM, returns true. Otherwise false.
     */
    bool setString(const uint16_t &address, const uint8_t &maxLength, const String &input);

    /**
     *  Retrieves an 8-bit Unsigned Integer from the EEPROM.
     *
     *  @param[in] address Address where the 8-bit Unsigned Integer is saved.
     *  @param[out] value Buffer to save the 8-bit Unsigned Integer to.
     *  @return If 8-bit Unsigned Integer succesfully retrieved from EEPROM, returns true. 
     *          Otherwise false.
     */
    bool getUInt8(const uint16_t &address, uint8_t &value);

    /**
     *  Saves an 8-bit Unsigned Integer in the EEPROM.
     *
     *  @param[in] address Address where the 8-bit Unsigned Integer will be saved.
     *  @param[in] value 8-bit Unsigned Integer to save in EEPROM.
     *  @return If 8-bit Unsigned Integer written in EEPROM, returns true. Otherwise false.
     */
    bool setUInt8(const uint16_t &address, uint8_t value);

};

/******************************************************************************
 * Functions
 *****************************************************************************/

#endif /* FLASHMEM_H_ */