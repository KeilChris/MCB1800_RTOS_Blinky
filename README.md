# CMSIS-RTOS Blinky

The Blinky project is a simple Keil RTX5 based example for the NXP 'LPC1857' microcontroller mounted on the Keil 'MCB1800'
evaluation board. It is fully compliant to the Common Microcontroller Software Interface Standard (CMSIS).

## Example functionality

- System clock: 100.00 MHz.
- The LEDs are blinking as running light with speed depending on potentiometer position.
- P4_0 button is used for stop/start blinking LED.

## Available targets

The program is available in different targets:
- ULINKplus: runs from internal on-chip Flash; configured for use with ULINKplus and SWO trace
- ULINKpro:  runs from internal on-chip Flash; configured for use with ULINKpro and ETM based instruction trace

### ULINKplus notes

- Connect the 10-pin Cortex Debug connector
- Connect ULINKplus I/O 0 to P4_0 on the board
- Connect ULINKplus Power I/F using the 100 mA (2.55 Ohm) to the 3V3R (J1) and GND (J2)
   
Open a debug session, and observe the following:
 - In System Analyzer, you can observe the system clock, power consumption, and RTX thread and exception information
 - Use the tool box buttons to enable/disable power down in the idle thread (alternatively, use the P4_0 button to do the same)
 - Event Recorder

### ULINKpro notes

- Connect the 20-pin Cortex Debug + ETM connector
- Open a debug session, and observe the following:
  - Code Coverage window showing coverage information for each module
  - Execution Profiler (annotations next to the code lines)
  - Performance Analyzer

Instruction trace is very time and resource consuming therefore if you want to use instruction trace use nothing else.

