COMPONENT_SOC := esp8266

# Required if compiling with ENABLE_GDB=1, you must connect debug terminal to alternative serial pins
GDB_UART_SWAP=1

ARDUINO_LIBRARIES = \
	ToneGenerator \
	SignalGenerator
