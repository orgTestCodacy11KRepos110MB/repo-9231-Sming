COMPONENT_SOC := esp* host

## Local build configuration
## Parameters configured here will override default and ENV values.
## Uncomment and change examples:

## ESP_HOME sets the path where ESP tools and SDK are located.
## Windows:
# ESP_HOME = c:/Espressif

## MacOS / Linux:
#ESP_HOME = /opt/esp-open-sdk

## SMING_HOME sets the path where Sming framework is located.
## Windows:
# SMING_HOME = c:/tools/Sming/Sming 

# MacOS / Linux
# SMING_HOME = /opt/Sming/Sming

## COM port parameter is reqruied to flash firmware correctly.
## Windows: 
# COM_PORT = COM3

# MacOS / Linux:
# COM_PORT = /dev/tty.usbserial

# Com port speed
# COM_SPEED	= 115200

#
HWCONFIG = spiffs

#
ARDUINO_LIBRARIES := OneWire ArduinoJson5
