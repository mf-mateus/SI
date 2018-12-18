# Inteligent Sensors Final Project
## Ambient Monitoring Coupled with a Mobile Robot

### Getting Started
* [Github commands](https://confluence.atlassian.com/bitbucketserver/basic-git-commands-776639767.html) - Basic comands
* [Report](TBD) - Google Docs Report

### Project Dev Boards
* [RaspberryPi](https://www.raspberrypi.org/) - Server Board
* [ESP8266](https://www.banggood.com/Geekcreit-Doit-NodeMcu-Lua-ESP8266-ESP-12F-WIFI-Development-Board-p-985891.html?p=FX25081395033201503Y) - IOT Sensor Processing
* [PIC24](TBD) - Extra credit???

### Project Sensor Modules
* [HTU21D](https://www.banggood.com/GY-213V-HTU21D-3_3V-I2C-Temperature-Humidity-Sensor-Module-For-Arduino-p-1184748.html?p=FX25081395033201503Y) - Humidity Sensor
	- [HTU21D  Datasheet](http://www.farnell.com/datasheets/1780639.pdf) - Datasheet

<p align="center">
	<img src="/images/si_draft1.png">
</p>

### How it Works
* ESP8266 Reads all data from sensors (Humidity/Light/Odometry-encoder)
* ESP8266 Sends data to RaspberryPi (using node Red???)
* GG EASY