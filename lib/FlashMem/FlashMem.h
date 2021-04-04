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
    /** Address of Meta Data header in EEPROM. */
    static const uint8_t NVM_METADATA_ADDRESS = 0;

    /** Meta Data maximal length. */
    static const uint8_t NVM_METADATA_MAX_LENGTH = 2;

    /** Metadata for Valid Stored Credetials */
    static const String NVM_METADATA_VALID = "UZ";

    /** Address of saved SSID in EEPROM. */
    static const uint8_t NVM_SSID_ADDRESS = NVM_METADATA_ADDRESS + NVM_METADATA_MAX_LENGTH;

    /** SSID maximal length */
    static const uint8_t NVM_SSID_MAX_LENGTH = 32;

    /** Address of saved Password in EEPROM. */
    static const uint8_t NVM_PASSWORD_ADDRESS = NVM_SSID_ADDRESS + NVM_SSID_MAX_LENGTH;

    /** WPA-PSK Password laximal length */
    static const uint8_t NVM_PASSWORD_MAX_LENGTH = 63;

    /** Maximum length for saved Credentials. */
    static const uint8_t CREDENTIALS_MAX_LENGTH = NVM_PASSWORD_ADDRESS + NVM_PASSWORD_MAX_LENGTH;

    /** Address of saved Groups in EEPROM. */
    static const uint8_t NVM_GROUPS_ADDRESS = CREDENTIALS_MAX_LENGTH;

    /** Length of saved Groups in EEPROM. */
    static const uint8_t NVM_INTEGER_LENGTH = 1;

    /** Size of the EEPROM destined to store credentials*/
    static const uint16_t EEPROM_SIZE = 512;

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

    /**
     *  Checks if there are Credentials stored in the EEPROM
     * 
     *  @return If Credentials are stored in EEPROM, returns True. Otherwise False.
     */
    bool areCredentialsStored();

    /**
     *  Sets the Meta Data header
     * 
     *  @param[in] areCredentialsStored If true, Credentials stored are valid 
     *  and Header is set to.
     *  @return If Header succesfully set, returns True. Otherwise False.
     */
    bool setHeader(bool areCredentialsStored);

    /**
     *  Retrieves a Null-terminated String from the EEPROM.
     *
     *  @param[in] address Address where the String is saved.
     *  @param[in] maxLength Maximum Length of the String.
     *  @param[out] output Buffer to save the String to.
     */
    void fetchString(const uint8_t &address, const uint8_t &maxLength, String &output);

    /**
     *  Saves a Null-terminated String in the EEPROM.
     *
     *  @param[in] address Address where the String will be saved.
     *  @param[in] maxLength Maximum Length of the String.
     *  @param[in] input String to save in EEPROM.
     *  @return If string written in EEPROM, returns True. Otherwise False.
     */
    bool saveString(const uint8_t &address, const uint8_t &maxLength, const String &input);

    /**
     *  Retrieves an Unsigned Integer from the EEPROM.
     *
     *  @param[in] address Address where the String is saved.
     *  @param[out] value Buffer to save the Integer to.
     *  @return If Integer succesfully retrieved from EEPROM, returns True. 
     *          Otherwise False.
     */
    bool fetchInt(const uint8_t &address, uint8_t &value);

    /**
     *  Saves an Unsigned Integer in the EEPROM.
     *
     *  @param[in] address Address where the String will be saved.
     *  @param[in] value String to save in EEPROM.
     *  @return If integer written in EEPROM, returns True. Otherwise False.
     */
    bool saveInt(const uint8_t &address, const uint8_t &value);

    /**
     *  Retrieves the Number of Groups from the EEPROM.
     * 
     *  @param[out] groups Buffer to save the Number of Groups to.
     *  @return If the value is succesfully retrieved from EEPROM, returns True. 
     *          Otherwise False.
     */
    bool importGroups(uint8_t &groups);

    /**
     *  Saves the Number of Groups in the EEPROM.
     *
     *  @param[in] groups Number of Groups to save in EEPROM.
     *  @return If the value is written in EEPROM, returns True. 
     *          Otherwise False.
     */
    bool saveGroups(const uint8_t &groups);
};

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

#endif /* FLASHMEM_H_ */