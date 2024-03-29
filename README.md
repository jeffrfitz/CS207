# Welcome to my CS 207 final project!

This project focuses on the construction and general improvement of the Alarm Clock by Tittiamo.

<a href = "https://www.hackster.io/Tittiamo/alarm-clock-f61bad"> Here is the original project </a>

<a href = "https://www.hackster.io/Tittiamo/alarm-clock-f61bad"> Wayback Machine link </a>


<h3> Introduction: </h3>

<p> The goal of this project is to construct a functioning alarm clock and to add features that may be relevant for the user. </p>

These features include:

- Turning off the LCD backlight.
- Turning off the alarm while ringing.
- Preventing the device from being stuck on menu settings (if happened by accident).
- Removing the LED to simplify build, as the LCD is proven to be bright enough by itself.
- Adding EEPROM to save the alarm hours and minutes to be used even after power was lost rather than resetting to zero.

Additional challenges for me were:

- Soldering the DS1307 RTC module. It was a challenge, an easier method would have been to purchase a DS3231.
- Power sources, how to maximize portability. 
- Learning the uses of the I2C protocol and drawing the modules used in Fritzing (namely Serial Interface Board Module 1602).


# To Compile the Code you must import these libraries: 

<p>
- EEPROM.h by Arduino </br>
- Wire.h by Arduino </br>
- <a href = "https://github.com/adafruit/RTClib"> RTClib.h by adafruit </a> </br>
- <a href = "https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library"> Arduino-LiquidCrystal-I2C-Library by fdebrabander </a>
</p>
