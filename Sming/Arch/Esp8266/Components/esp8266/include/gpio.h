/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2016 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS ESP8266 only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#pragma once

#include <c_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GPIO_PIN_ADDR(i) (GPIO_PIN0_ADDRESS + i * 4)

#define GPIO_ID_IS_PIN_REGISTER(reg_id) ((reg_id >= GPIO_ID_PIN0) && (reg_id <= GPIO_ID_PIN(GPIO_PIN_COUNT - 1)))

#define GPIO_REGID_TO_PINIDX(reg_id) ((reg_id)-GPIO_ID_PIN0)

typedef enum {
	GPIO_PIN_INTR_DISABLE = 0,
	GPIO_PIN_INTR_POSEDGE = 1,
	GPIO_PIN_INTR_NEGEDGE = 2,
	GPIO_PIN_INTR_ANYEDGE = 3,
	GPIO_PIN_INTR_LOLEVEL = 4,
	GPIO_PIN_INTR_HILEVEL = 5
} GPIO_INT_TYPE;

#define GPIO_OUTPUT_SET(gpio_no, bit_value)                                                                            \
	gpio_output_set((bit_value) << gpio_no, ((~(bit_value)) & 0x01) << gpio_no, 1 << gpio_no, 0)
#define GPIO_DIS_OUTPUT(gpio_no) gpio_output_set(0, 0, 0, 1 << gpio_no)
#define GPIO_INPUT_GET(gpio_no) ((gpio_input_get() >> gpio_no) & BIT0)

/* GPIO interrupt handler, registered through gpio_intr_handler_register */
typedef void (*gpio_intr_handler_fn_t)(uint32_t intr_mask, void* arg);

/*
 * Initialize GPIO.  This includes reading the GPIO Configuration DataSet
 * to initialize "output enables" and pin configurations for each gpio pin.
 * Must be called once during startup.
 */
void gpio_init(void);

/*
 * Change GPIO pin output by setting, clearing, or disabling pins.
 * In general, it is expected that a bit will be set in at most one
 * of these masks.  If a bit is clear in all masks, the output state
 * remains unchanged.
 *
 * There is no particular ordering guaranteed; so if the order of
 * writes is significant, calling code should divide a single call
 * into multiple calls.
 */
void gpio_output_set(uint32_t set_mask, uint32_t clear_mask, uint32_t enable_mask, uint32_t disable_mask);

/*
 * Sample the value of GPIO input pins and returns a bitmask.
 */
uint32_t gpio_input_get(void);

/*
 * Set the specified GPIO register to the specified value.
 * This is a very general and powerful interface that is not
 * expected to be used during normal operation.  It is intended
 * mainly for debug, or for unusual requirements.
 */
void gpio_register_set(uint32_t reg_id, uint32_t value);

/* Get the current value of the specified GPIO register. */
uint32_t gpio_register_get(uint32_t reg_id);

/*
 * Register an application-specific interrupt handler for GPIO pin
 * interrupts.  Once the interrupt handler is called, it will not
 * be called again until after a call to gpio_intr_ack.  Any GPIO
 * interrupts that occur during the interim are masked.
 *
 * The application-specific handler is called with a mask of
 * pending GPIO interrupts.  After processing pin interrupts, the
 * application-specific handler may wish to use gpio_intr_pending
 * to check for any additional pending interrupts before it returns.
 */
void gpio_intr_handler_register(gpio_intr_handler_fn_t fn, void* arg);

/* Determine which GPIO interrupts are pending. */
uint32_t gpio_intr_pending(void);

/*
 * Acknowledge GPIO interrupts.
 * Intended to be called from the gpio_intr_handler_fn.
 */
void gpio_intr_ack(uint32_t ack_mask);

void gpio_pin_wakeup_enable(uint32_t i, GPIO_INT_TYPE intr_state);

void gpio_pin_wakeup_disable();

void gpio_pin_intr_state_set(uint32_t i, GPIO_INT_TYPE intr_state);

#ifdef __cplusplus
}
#endif
