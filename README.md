# Racing Lap Timer
A racing lap timer, based on a laser sensor.

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](http://choosealicense.com/licenses/mit/)
[![Repo Status](https://www.repostatus.org/badges/latest/wip.svg)](https://www.repostatus.org/#wip)
[![Release](https://img.shields.io/github/release/BlueAndi/RacingLapTimer.svg)](https://github.com/BlueAndi/RacingLapTimer/releases)
[![Build Status](https://github.com/BlueAndi/RacingLapTimer/actions/workflows/main.yml/badge.svg?branch=master)](https://github.com/BlueAndi/RacingLapTimer/actions/workflows/main.yml)

![RacingLapTimer](./doc/screenshot.jpg)

- [Racing Lap Timer](#racing-lap-timer)
- [Motivation](#motivation)
- [Initial Startup](#initial-startup)
- [Electronic](#electronic)
  - [Schematic](#schematic)
- [Software](#software)
  - [Preparation](#preparation)
  - [IDE](#ide)
  - [Installation](#installation)
  - [Build Project](#build-project)
  - [Update of the device](#update-of-the-device)
    - [Update via usb](#update-via-usb)
  - [Used Libraries](#used-libraries)
  - [System Context](#system-context)
- [Issues, Ideas And Bugs](#issues-ideas-and-bugs)
- [License](#license)
  - [Contribution](#contribution)

# Motivation
For line follower competitions a racing lap timer shall be used. It doesn't only measure the lap time, it shows it nice via web interface, inclusive automatically result table generation.

# Initial Startup
If the system starts up the first time, it will spawn a wifi access point with the SSID RacingLapTimer. Connect to it, it will request a password (default: "let me in").
Configure there the wifi network SSID/password to which the system shall later connect.

After configuration, the system will try to establish a connection. If the connection is not successful, the access point will be spawned again. Otherwise the credentials will be stored to persistent memory and loaded the next time automatically.

# Electronic

* [Wemos D1 Mini (esp8266)](https://docs.platformio.org/en/latest/boards/espressif8266/d1_mini.html)
* [Waveshare laser sensor](https://www.waveshare.com/wiki/Laser_Sensor)

## Schematic

Schematic files can be found [here](./doc/electronic/RacingLapTimer). These have been developed using KiCAD 6.

### MCU
![MCU](./doc/electronic/RacingLapTimer/SCH_MCU_v1_0.png)
- Digital signal of laser sensor is connected to D1/GPIO_5 of the Wemos D1 Mini.
- Power is supplied by USB 5V directly to the Wemos D1 Mini

### Laser Power Supply
![Power Supply](./doc/electronic/RacingLapTimer/SCH_POW_v1_0.png)
- Laser module Vcc shall be 3.3V, because MCU is only 3.3V tolerant.
- THT or SMD resistor options are provided. **DO NOT PLACE ALL RESISTORS**.

### Laser Sensor
![Power Supply](./doc/electronic/RacingLapTimer/SCH_Laser_v1_0.png)
- Pin header for Waveshare Laser Sensor.

## PCB 
### v1.0
![PCBv1.0](./doc/electronic/RacingLapTimer/PCBv1_0.png)
- It is recommended to give the Laser Sensor some kind of support in order to protect the pin header from breaking. The height of the support depends on the pin headers used.

# Software

## Preparation
Install the serial driver for the Wemos D1 Mini board. Find the latest drivers here: https://www.wemos.cc/en/latest/ch340_driver.html

## IDE
The [PlatformIO IDE](https://platformio.org/platformio-ide) is used for the development. Its build on top of Microsoft Visual Studio Code.

## Installation
1. Install [VSCode](https://code.visualstudio.com/).
2. Install PlatformIO IDE according to this [HowTo](https://platformio.org/install/ide?install=vscode).
3. Close and start VSCode again.
4. Recommended is to take a look to the [quick-start guide](https://docs.platformio.org/en/latest/ide/vscode.html#quick-start).

## Build Project
1. Load workspace in VSCode.
2. Change to PlatformIO toolbar.
3. _Project Tasks -> Build_ or via hotkey ctrl-alt-b

## Update of the device

### Update via usb
1. Connect the Wemos D1 Mini board to your PC via USB.
2. Build and upload the software via _Project Tasks -> Upload_.
3. Build and upload the filesystem via _Project Tasks -> Upload File System image_.

## Used Libraries
* [Arduino](https://github.com/esp8266/Arduino) - ESP framework.
* [WifiManager](https://github.com/tzapu/WiFiManager) - ESP8266 WiFi Connection manager with fallback web configuration portal.
* [WebSockets](https://github.com/Links2004/arduinoWebSockets) - A WebSocket Server and Client for Arduino based on RFC6455.

## System Context

![SystemContextDiagram](./doc/design/SystemContextDiagram.png)

# Issues, Ideas And Bugs
If you have further ideas or you found some bugs, great! Create a [issue](https://github.com/BlueAndi/RacingLapTimer/issues) or if you are able and willing to fix it by yourself, clone the repository and create a pull request.

# License
The whole source code is published under the [MIT license](http://choosealicense.com/licenses/mit/).

## Contribution
Unless you explicitly state otherwise, any contribution intentionally submitted for inclusion in the work by you, shall be licensed as above, without any
additional terms or conditions.
