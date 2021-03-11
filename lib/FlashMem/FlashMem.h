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

namespace Flash
{
    /** Address of saved SSID in EEPROM. */
    const uint8_t NVM_SSID_ADDRESS = 0;

    /** SSID maximal length */
    const uint8_t NVM_SSID_MAX_LENGTH = 32;

    /** Address of saved Password in EEPROM. */
    const uint8_t NVM_PASSWORD_ADDRESS = NVM_SSID_ADDRESS + NVM_SSID_MAX_LENGTH;

    /** WPA-PSK Password laximal length */
    const uint8_t NVM_PASSWORD_MAX_LENGTH = 63;

    /** Maximum length for saved Credentials. */
    const uint8_t CREDENTIALS_MAX_LENGTH = NVM_PASSWORD_ADDRESS + NVM_PASSWORD_MAX_LENGTH;

    /** Size of the EEPROM destined to store credentials*/
    const uint16_t EEPROM_SIZE = 512;

    /**
     *  Initialization of the EEPROM Module
     * 
     *  @return If Initialization is successful, return True. Otherwise False.
     */
    bool begin();

    /**
     *  Imports the Station Credentials from the EEPROM.
     * 
     *  @param[out] ssid SSID of host network to connect to.
     *  @param[out] password Password of host network to connect to.
     *  @return If Credentials successfully imported, returns True. Otherwise, False.
     */
    bool importCredentials(String &ssid, String &password);

    /**
     *  Saves new STA Credentials.
     * 
     *  @param[in] ssid SSID of host network to connect to.
     *  @param[in] password Password of host network to connect to.
     *  @return If Credentials succesfully saved, return True. Otherwise, False.
     */
    bool saveCredentials(const String &ssid, const String &password);

    /**
     *  Deletes stored Data in the EEPROM.
     */
    void clearEEPROM();

}; /* Flash */

/******************************************************************************
 * Prototypes
 *****************************************************************************/

/******************************************************************************
 * Variables
 *****************************************************************************/

/******************************************************************************
 * External functions
 *****************************************************************************/

/******************************************************************************
 * Local functions
 *****************************************************************************/

#endif /*FLASHMEM_H_*/