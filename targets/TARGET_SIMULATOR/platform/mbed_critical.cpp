#include "mbed_critical.h"

bool core_util_are_interrupts_enabled(void) {
    return true;
}

bool core_util_is_isr_active(void) {
    return false;
}

void core_util_critical_section_enter(void) {}

/**
 * Atomic test and set.
 *
 * Atomically tests then sets the flag to true, returning the previous value.
 *
 * @param  flagPtr Target flag being tested and set.
 * @return         The previous value.
 */
bool core_util_atomic_flag_test_and_set(volatile core_util_atomic_flag *flagPtr) {
    bool curr = flagPtr->_flag;
    flagPtr->_flag = true;
    return true;
}

/**
 * Atomic clear.
 *
 * @param  flagPtr Target flag being cleared.
 */
void core_util_atomic_flag_clear(volatile core_util_atomic_flag *flagPtr) {
    flagPtr->_flag = false;
}


