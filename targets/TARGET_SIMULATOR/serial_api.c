/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// This is incomplete! Just forwards to stdout. Needs to be implemented!

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "serial_api.h"
#include "cmsis.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "gpio_api.h"
#include "mbed_wait_api.h"
#include "emscripten.h"

#if defined(DEVICE_SERIAL)

int stdio_uart_inited = 0;
serial_t stdio_uart;

void serial_init(serial_t *obj, PinName tx, PinName rx) {
    obj->rx_pin = rx;
    obj->tx_pin = tx;

    stdio_uart_inited = 1;
    stdio_uart = *obj;

    EM_ASM_({ return window.MbedJSHal.serial.init($0) }, obj);
}

void serial_free(serial_t *obj) {

}


void serial_baud(serial_t *obj, int baudrate) {
}

void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits) {
}

// todo: can we have more than one in the simulator?
static uart_irq_handler irq_handler;
static uint32_t irq_handler_id;

void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id) {
    irq_handler = handler;
    irq_handler_id = id;
}

void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable) {
    EM_ASM({ console.log('unhandled serial_irq_set')});
}

/******************************************************************************
 * READ/WRITE
 ******************************************************************************/
int serial_getc(serial_t *obj) {
    while (serial_readable(obj) == 0) {
        wait_ms(20);
    }

    return EM_ASM_INT({ return window.MbedJSHal.serial.read() });
}

void serial_putc(serial_t *obj, int c) {
    EM_ASM_({ window.MbedJSHal.serial.write($0) }, c);
}

int serial_readable(serial_t *obj) {
    int readable = EM_ASM_INT({ return window.MbedJSHal.serial.readable() });
    return readable;
}

int serial_writable(serial_t *obj) {
    return 1;
}

void serial_clear(serial_t *obj) {
}

void serial_pinout_tx(PinName tx) {
}

void serial_break_set(serial_t *obj) {
}

void serial_break_clear(serial_t *obj) {
}

void serial_set_flow_control(serial_t *obj, FlowControl type, PinName rxflow, PinName txflow) {
}

EMSCRIPTEN_KEEPALIVE
void invoke_serial_irq(uint32_t obj_ptr) {
    if (irq_handler) {
        irq_handler(irq_handler_id, RxIrq);
    }
}

#endif
