# LEDTwitterWall
Software for 10 ft LED matrix sign that runs Arduino Yun pulling from twitter mentions
<img src="https://lh3.googleusercontent.com/sPXBURgTyYzmYGj37gKUWpdt0eCrPupy-eKigALwa2i8_pHxnOYYga3gMB7aoT4TSUD-Unlmiz7i2u09DFgaeUOpraiHgjLutmg5K8KPP_u95SfsE6S022tjmaIaDCbeRXOq4Jgml3AQNELsWEQsNejKxfSHzuL6VUPo4p2sbgig1NWzZRljOWOK6LwSmJAw-q_FAzbwS3bMLm5yIGtnaUvRAoYrHrSs0OVIe4NO7GvS2awuupTvaC6vLT7QaB9tdaaoqXbXKJBRSJdNBoDm8lip8rnV6benBATEZX86jBCd7kIOv2RxpHnuNSNndkyHsaDaGs8UNt-nHEpCq3XAMoIJqDUesikxtTvXAP_wPxt7KIgGe0b8fVF9UePvOGHXNnTdW9tuPtYZD6lNKcYOWEakHjZma6BNHWzUdESn0KXXkD_nZRcTBFJgG0uRGEXV6OBnoiDpmwO8l8VG4LHo0AhGgJvNpJlFEFnFSrG5FP5WWKYzK0wlfb3Qj5qg1nauHYwVOrxx_ZH_oSrNF7wjcQQLazSRH2NtS_6uxQqpNvVvz6kRcus_RsA-CcUsnhCwyy7c=w1293-h727-no" />

The board on the wall now is an Arduino Yun.

 * Twitter Integration uses: https://temboo.com/library/Library/Twitter/Timelines/LatestMention/
 * LED board is based on: https://learn.adafruit.com/guggenhat-bluetooth-le-wearable-neopixel-marquee-scrolling-led-hat-display/code

250 API calls allowed per month for free. 730 hours in a month >> 1 call every 3 hours (10800000 ms)

## Clone instructions
 * git clone 
 * create LEDTwitterWall/TwitterMentionsWall/TembooAccount.h with your creds
 * open LEDTwitterWall/TwitterMentionsWall/TwitterMentionsWall.ino sketch in Arduino IDE
 * select Tools->Board-> Arduino Yun
 * select Tools->Port->Arduino Yun WiFi (the-yun-ip)
 * Compile and upload
 * You should be good to go!

## The sketches here...
###  TwitterMentionsWall 
The main project, bringing it all together - seems to be an issue with getting NeoPixel_matrix to play with Temboo, OR the Console. maybe interrupts? https://learn.adafruit.com/neopixels-and-servos/overview
###  Console_Test
The YUN DOES NOT SUPPORT SERIAL OVER WiFi, so you have to use the Bridge with Console.println instead to get output. This is an example to do just that. Start the sketch, then on a computer connected to the same network as the YUN, run ssh root@10.1.10.33 'telnet localhost 6571' where 10.1.10.33 is the ip or hostname of the YUN. Console messages will be sent here.
###  TwitterTest
USB only test of the Twitter/Temboo pulling code
###  TwitterTestConsole
WiFi varient of the above, switching out Serial debug messages for Console - same ssh command as above to get it to run.
###  WallTest
Tests just the LED Matrix with a static message.
###  MailboxTest
Tests Yun REST API, used to confirm Bridge is functional(Web API->Bridge->Ardruino->Bridge->Console). Requires console connection (use ssh command from Console_Test).
###  FastLEDTest
Due to the inability of NeoPixel and the bridge to play well together, trying out a different LED driver library. This is a basic test that scrolls through each light.
###  FastLEDWithTwitterTest
Extension of FastLEDTest adding in the Temboo code.

## Configuring the Arduino YUN
Sometimes you need to reset the wifi on the YUN, here are the steps:
 * Press and hold the WLAN RST button for 30 seconds
 * Power cycle the Arduino
 * Connect to the wireless network the arduino sets up (it'll be Arduino-xxxxxx where xxxxx is the MAC address of wifi interface of the YUN)
 * Open a browser and point it to http://192.168.240.1
 * Go into the advanced page and set the WAN (eth1) to be a static IP
 * Apply the settings
 * Goto the basic configure page and setup the WiFi to your local WiFi, once you click apply and the settings are changed, you should be able to connect to it via WiFi, if you can't, then connect an eth cable to it and use the static IP. 
 
## Depend Libraries
 * Adafruit-GFX-Library-master.zip - https://github.com/adafruit/Adafruit-GFX-Library
 * Adafruit_NeoMatrix-master.zip - https://github.com/adafruit/Adafruit_NeoMatrix
 * Adafruit_NeoPixel-master.zip - https://github.com/adafruit/Adafruit_NeoPixel
 * FastLED(NeoPixel alternate) - https://github.com/FastLED/FastLED
 * Temboo - https://github.com/temboo/Temboo
 
 
## Some useful references:
 * http://kiwitobes.com/2013/09/26/twitter-lights-and-memory-limits-with-arduino-yun/
 * https://temboo.com/library/Library/Twitter/Timelines/LatestMention/
 * https://www.arduino.cc/en/Tutorial/ConsoleRead
 * https://www.arduino.cc/en/Guide/ArduinoYun
 * http://stackoverflow.com/questions/20681318/arduino-yun-serial-communication-not-working
 * 
 
## The Hardware
 * The Yun with everything connected (green data PIN-6 wire goes to LED Matrix)<br>
  <img src="https://lh3.googleusercontent.com/xxXtkMrQ8tzvCtRSsG3Omn84105uK3sL9xo3fgN5u8Oz_bZYrppc9qaHA4Aa47CX-OhLJFCwSFtF4M-xlCTodeRAk3clUO6XEg-XXGDRgt6Y5CYHFPpc3-s-rS6oRTJGfPoGcwaKz9JmGwWog_izl-1LW2H3NZseTm-zQ3RzeBcTy6CLyoSsY1BVv3V7sdX-NV-igSmirGr41EKg2usaA8jbBvuf4HmwjPpRlXggpf9RbT9LzL5X-_U3TGp7bHu0FpGRTiWJbzA7wQf2kTNcUPGD9MU4G__uzgEqN-XPcFhw-osAsX95cdsbNFHEsvEj788Z9hdFYdnWuOOMFfsjINBMVARyEGFbBy-U4hve2XAQNiukqVzP-aWb15WtEXK4T79qFLVW3qMhiyfP4vjd0XPWDKInee_rqEOBCeLqlRLeU2u-y3gJHrVh2UjJXgRACcpfN4mUfD5aT06KiNoIdlQbotWglTVcYEvt6fZSyY0ry6BL-X2DpXLEgPnbhNVk2HkSTdTmzJPV8FDCBaP5d57qboL7_vb7i0McPc_u7OY5M-1fwJKu52ZNaqB5I6Pgt4dO=w560-h315-no" />

 * The matrix (power and ground are all wire paralell, data line is daisy-chained(serial) from one strip to the next)<br>
  <img src="https://lh3.googleusercontent.com/aOsoi7LWAdVqkck_Q739dLrkP-06gvilVjNQPuyFej-VTYhVY_I1ZWCTh8q22BgC8_LYARrB_eHqWCUW_tVHdx8kufARdVb4uR9OmeCK1nNP3Twfe45WXt0C2grxeoSl-m4ToFazdnCX6dibEbfdMTRxggeXlNFlcTVmKm9PFw7D0L4n0r_bGTIpFTd5BCyCZ-ZhUL-kk_osO69wqzte_C5aH04nOA7fc_6VRiX_AyBG-323DyIYZqvv73S7oZX41_AtSAI9YP_RaDhrOyzdZLTAf5a4_8bCTeb_J8IF3Xfx08DmRQ4suWSHU2pCIdYRRkAlO5QehEQ-yK7nL5cyUdaWIMt22-gg0vnf_cgmtJj4e6-CGctdIJEQFJb95olatWdx7Pu19wj1FvCwNZJj1g9INrlcWShbpCYE1Whc1_OKdgCtSI9ZmDGD3q8_cCtxoatWe5R3PvlgeYi4OGFcwsFATTObcb2eGjcdX3VA9ixzncKylkLRHbKj6DiBfKvPq9STJZJWvhe5eaFzsTWz-lL1o6hsENLsA4tDeTOyK99ez1P-Z0Z-cmCXKWw4SvXqN105=w1293-h727-no" />

 * The matrix - other end(power and ground are all common, data line is daisy-chained)<br>
  <img src="https://lh3.googleusercontent.com/7ZT59A0DtVwcv56F5uagYH40OY5RauR_jpPq3is9ELGPERG3KiFWo0aHvbyRh7DCtZR95TUvA1jwdWoOTMGSH2FBUC3l8vQVDbHiHyh5YbzEfFoBqC1PULbk0aONe8KaqgceDp9Mw1xDu49GnCdOzT0KJMSdyz7zjYz9-0RZSwfcBZ7Ly637Msd1aCYmWuH7eeSxF5Rfo_VgLF9BzmBB3sisyZabNpVacUvzeHHq8rnV-_TedB5ZNH7NbJRPXGSQa1ikYS8Ge5e8zikkpvS6OlqY2xGz2z_WjPpZ8HMI_oVoobf0ax41pVrnSki0lpQDvGmbdk1x80z-jHroaIYCQHKw2TFVOv5ZUP_7qImCdnp_9a8pvjqt36s6Z__C18BK4LgLw5SH6Me-yFLIgQgDl_wfVMXnCqUBpnIc5sZPlzRcNUkJZLDyOFwBcnJZIanrJIS6VtCkYII5acYM7OD99BiHy4XTA4lrYw2_KwyQsmz1bBpLDPxXTFvOsUmcVf3d2JwtD-X4dystZiugL1I_OYgBrwci6ZIUMm-poJvgw-NVU6-qVmdfa1u2QfBL0fs094R3=w409-h727-no" />

 * Basic 5v power supply to run it (probably can't manage whole thing lit up at once, haven't tried as text doesn't use much)<br>
  <img src="https://lh3.googleusercontent.com/RXBzUPLCjFBiKNH0nk9enD4AYyz5EThrFT9m3ar29PP4HtA-FNGWtw0PduONjInyJd6tCyXreqDiCAarQ5PtSsS9vgRQGPxVMBLIdnyvfvKQFs8K76YxpBmIp5KWqulluxOK588gZ-KAnX8RTqVs1DrAo0VYCC_w3MEOwme0S8xOOD-JNvYdmwYl0iSUGLXWcXodpPQgAhDK2NDGBsjxmJuMCrE6fIbN1Kb6TvEMILEem7OR-XLbLJOeQFfFUcL2gbVnB3L7oyWN2TWcevhZI7MX-XZoOvFN_soR_m-Ct2ANM2X2RBh5aPjLQMpngdKD1gWa4cf2xrmbZoSp0c-ErAprGTXU1JhPwpC1IK1BQTC2UZuB6FM7Eog8VGeT--uA3LXTszd3xe6RdS9pworOX5AJvjxfiaUsXcERqbUzlrgEAWGAs0vQtMmI7P7ITBjbxi9J3DUr6IZPtGGIXy0AeNtfBOCiKyUvMtj9jhOv_9GZXUHj6wBrB1S8UhKC6dRreed6LYGKZuCX86COpbmkagKOp60vo6Juko5vudEqqfL4u0aIbdlfYAA0Vzjs-Bf6XpNe=w409-h727-no" />
