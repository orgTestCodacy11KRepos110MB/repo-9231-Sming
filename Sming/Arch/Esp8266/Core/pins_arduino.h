/****
 * Sming Framework Project - Open Source framework for high efficiency native ESP8266 development.
 * Created 2015 by Skurydin Alexey
 * http://github.com/SmingHub/Sming
 * All files of the Sming Core are provided under the LGPL v3 license.
 *
 * pins_arduino.h
 *
 ****/

// File name selected for compatibility

#pragma once

#include "esp8266_peri.h"

constexpr uint8_t A0{17}; // Single ESP8266EX analog input pin (TOUT) 10 bit, 0..1v

#define NOT_A_PIN 0
#define NOT_A_PORT 0
#define NOT_ON_TIMER 0

#define GPIO_REG_TYPE uint32_t

// We use maximum compatibility to standard Arduino logic.

#define digitalPinToPort(pin) (0)
#define digitalPinToBitMask(pin) (1UL << (pin))
#define digitalPinToTimer(pin) (NOT_ON_TIMER)
#define portOutputRegister(port) ((volatile uint32_t*)&GPO)
#define portInputRegister(port) ((volatile uint32_t*)&GPI)
#define portModeRegister(port) ((volatile uint32_t*)&GPE)
