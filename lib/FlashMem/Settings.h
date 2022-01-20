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
 * @brief  Settings stored in persistent memory
 * @author Andreas Merkle <web@blue-andi.de>
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "FlashMem.h"

/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and Classes
 *****************************************************************************/

/** Settings which are stored in persistent memory. */
class Settings
{
public:

    /**
     * Get the settings instance.
     * 
     * @return Settings instance
     */
    static Settings& getInstance()
    {
        static Settings instance; /* idiom */

        return instance;
    }

    /**
     * Initialize settings and get access to the persistent memory.
     * 
     * @return If successful, it will return true otherwise false.
     */
    bool begin();

    /**
     * Get wifi SSID.
     * 
     * @param[out] ssid WiFi SSID
     */
    void getWiFiSSD(String& ssid);

    /**
     * Set wifi SSID.
     * 
     * @param[in] ssid WiFi SSID
     */
    void setWiFiSSID(const String& ssid);

    /**
     * Get wifi passphrase.
     * 
     * @param[out] passphrase WiFi passphrase
     */
    void getWiFiPassphrase(String& passphrase);

    /**
     * Set wifi passphrase.
     * 
     * @param[in] passphrase WiFi passphrase
     */
    void setWiFiPassphrase(const String& passphrase);

    /**
     * Get number of groups.
     * 
     * @param[out] numberOfGroups   Number of groups
     */
    void getNumberOfGroups(uint8_t& numberOfGroups);

    /**
     * Set number of groups.
     * 
     * @param[in] numberOfGroups    Number of groups
     */
    void setNumberOfGroups(uint8_t numberOfGroups);

    /**
     * Get name of specific group.
     * 
     * @param[in] idx   Group index
     * @param[out] name Group name
     */
    void getGroupName(uint8_t idx, String& name);

    /**
     * Set name of specific group.
     * 
     * @param[in] idx   Group index
     * @param[in] name Group name
     */
    void setGroupName(uint8_t idx, const String& name);

private:

    /**
     * Constructs the settings.
     */
    Settings()
    {
    }

    /**
     * Destroys the settings.
     */
    ~Settings()
    {
    }

};

/******************************************************************************
 * Functions
 *****************************************************************************/

#endif /* SETTINGS_H_ */