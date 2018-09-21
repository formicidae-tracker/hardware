EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:switches
LIBS:relays
LIBS:motors
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:fort
LIBS:kerykeion-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 5
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCP2561-E/SN U3
U 1 1 5BA28540
P 5750 3800
F 0 "U3" H 5350 4150 50  0000 L CNN
F 1 "MCP2561-E/SN" H 5850 4150 50  0000 L CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 5750 3300 50  0001 C CIN
F 3 "" H 5750 3800 50  0001 C CNN
	1    5750 3800
	1    0    0    -1  
$EndComp
NoConn ~ 6250 3800
Text HLabel 6550 3700 2    60   BiDi ~ 0
CAN+
Text HLabel 6550 3900 2    60   BiDi ~ 0
CAN-
Wire Wire Line
	6250 3700 6550 3700
Wire Wire Line
	6250 3900 6550 3900
Text HLabel 5000 3600 0    60   Input ~ 0
TXD
Text HLabel 5000 3700 0    60   Output ~ 0
RXD
Wire Wire Line
	5000 3600 5250 3600
Wire Wire Line
	5000 3700 5250 3700
Wire Wire Line
	5250 4000 5050 4000
Wire Wire Line
	5050 4000 5050 4400
Wire Wire Line
	5050 4400 5750 4400
Wire Wire Line
	5750 4200 5750 4600
$Comp
L GND #PWR022
U 1 1 5BA285D4
P 5750 4600
F 0 "#PWR022" H 5750 4350 50  0001 C CNN
F 1 "GND" H 5750 4450 50  0000 C CNN
F 2 "" H 5750 4600 50  0001 C CNN
F 3 "" H 5750 4600 50  0001 C CNN
	1    5750 4600
	1    0    0    -1  
$EndComp
Connection ~ 5750 4400
$Comp
L C C12
U 1 1 5BA2863D
P 5450 2900
F 0 "C12" H 5475 3000 50  0000 L CNN
F 1 "100n" H 5475 2800 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5488 2750 50  0001 C CNN
F 3 "" H 5450 2900 50  0001 C CNN
F 4 "CC0603KRX7R8BB104" H 5450 2900 60  0001 C CNN "Part Number"
	1    5450 2900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR023
U 1 1 5BA28644
P 5450 3150
F 0 "#PWR023" H 5450 2900 50  0001 C CNN
F 1 "GND" H 5450 3000 50  0000 C CNN
F 2 "" H 5450 3150 50  0001 C CNN
F 3 "" H 5450 3150 50  0001 C CNN
	1    5450 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 3150 5450 3050
Wire Wire Line
	5450 2750 5450 2650
Wire Wire Line
	5450 2650 5750 2650
Wire Wire Line
	5750 2500 5750 3400
$Comp
L +5V #PWR024
U 1 1 5BA2868E
P 5750 2500
F 0 "#PWR024" H 5750 2350 50  0001 C CNN
F 1 "+5V" H 5750 2640 50  0000 C CNN
F 2 "" H 5750 2500 50  0001 C CNN
F 3 "" H 5750 2500 50  0001 C CNN
	1    5750 2500
	1    0    0    -1  
$EndComp
Connection ~ 5750 2650
$EndSCHEMATC
