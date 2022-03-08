/*------------------------------------------------------------------------------
 * Copyright (c) 2020 Arm Limited (or its affiliates). All
 * rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   1.Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   2.Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   3.Neither the name of Arm nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *------------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher
 *----------------------------------------------------------------------------*/

#include "main.h"
#include "Board_LED.h"                  // ::Board Support:LED
#include "Board_Buttons.h"              // ::Board Support:Buttons
#include "Board_ADC.h"                  // ::Board Support:A/D Converter

static osThreadId_t tid_thrLED;         // Thread id of thread: LED
static osThreadId_t tid_thrBUT;         // Thread id of thread: BUT
static osThreadId_t tid_thrADC;         // Thread id of thread: ADC

static volatile uint32_t delay_val = 500U;

/*------------------------------------------------------------------------------
  thrLED: blink LED
 *----------------------------------------------------------------------------*/
__NO_RETURN static void thrLED (void *arg) {
  uint32_t led_cnt = LED_GetCount();
  uint32_t led_num = 0U;
  uint32_t active_flag = 1U;

  (void)arg;

  for (;;) {
    if (osThreadFlagsWait (1U, osFlagsWaitAny, 0U) == 1U) {
      active_flag ^=1U;
    }

    if (active_flag == 1U){
      LED_On (led_num);                                    // Switch LED on
      osDelay (delay_val);                                 // Delay
      LED_Off (led_num);                                   // Switch off
      osDelay (delay_val);                                 // Delay

      led_num++;                                           // Increase LED number
      if (led_num >= led_cnt) {
        led_num = 0;                                       // Restart with first LED
      }
    }
    else {
      osDelay (delay_val);                                 // Delay
    }
  }
}

/*------------------------------------------------------------------------------
  thrBUT: check button state
 *----------------------------------------------------------------------------*/
__NO_RETURN static void thrBUT(void *arg) {
  uint32_t last = 0U;
  uint32_t state;

  (void)arg;

  for (;;) {
    state = (Buttons_GetState () & 1U);                    // Get pressed button state
    if (state != last){
      if (state == 1){
        osThreadFlagsSet (tid_thrLED, 1U);                 // Set flag to thrLED
      }
    }
    last = state;
    osDelay (100U);
  }
}

/*------------------------------------------------------------------------------
  thrADC: get potentiomenter value
 *----------------------------------------------------------------------------*/
__NO_RETURN static void thrADC(void *arg) {
//  uint32_t last = 0U;
//  uint32_t state;

  (void)arg;

  for (;;) {
    ADC_StartConversion();                                 // start ADC conversion
    osDelay(20U);                                          // wait 20ms

    if (ADC_ConversionDone() == 0) {
      delay_val = ((uint32_t)ADC_GetValue() >> 4) + 10U;   // scale delay value
    }
  }
}

/*------------------------------------------------------------------------------
 * Application main thread
 *----------------------------------------------------------------------------*/
__NO_RETURN void app_main (void *arg) {
  (void)arg;

  tid_thrLED = osThreadNew (thrLED, NULL, NULL);           // Create LED thread
  if (tid_thrLED == NULL) { /* add error handling */ }

  tid_thrBUT = osThreadNew (thrBUT, NULL, NULL);           // Create BUT thread
  if (tid_thrBUT == NULL) { /* add error handling */ }

  tid_thrADC = osThreadNew (thrADC, NULL, NULL);           // Create ADC thread
  if (tid_thrADC == NULL) { /* add error handling */ }

  osThreadExit();
}
