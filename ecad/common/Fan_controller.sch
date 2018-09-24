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
LIBS:Oscillators
LIBS:nxp
LIBS:zeus-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 7 7
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
L EMC2302-1-AIZL-TR U6
U 1 1 5BA5AD00
P 6000 3500
F 0 "U6" H 5600 4050 60  0000 C CNN
F 1 "EMC2302-1-AIZL-TR" H 6500 2950 60  0000 C CNN
F 2 "Housings_SSOP:MSOP-10_3x3mm_Pitch0.5mm" H 6100 3500 60  0001 C CNN
F 3 "" H 6100 3500 60  0001 C CNN
F 4 "EMC2302-1-AIZL-TR" H 6100 3500 60  0001 C CNN "Part Number"
	1    6000 3500
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR037
U 1 1 5BA5AD44
P 6000 2200
F 0 "#PWR037" H 6000 2050 50  0001 C CNN
F 1 "+3.3V" H 6000 2340 50  0000 C CNN
F 2 "" H 6000 2200 50  0001 C CNN
F 3 "" H 6000 2200 50  0001 C CNN
	1    6000 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 4200 6000 4950
Wire Wire Line
	6000 2200 6000 2800
$Comp
L C C5
U 1 1 5BA5AE71
P 5700 2450
F 0 "C5" H 5725 2550 50  0000 L CNN
F 1 "100nF" H 5725 2350 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5738 2300 50  0001 C CNN
F 3 "" H 5700 2450 50  0001 C CNN
F 4 "06035C104KAT2A" H 5700 2450 60  0001 C CNN "Part Number"
	1    5700 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	5700 2300 5700 2250
Wire Wire Line
	5700 2250 6000 2250
Connection ~ 6000 2250
$Comp
L GND #PWR038
U 1 1 5BA5AEA9
P 5700 2700
F 0 "#PWR038" H 5700 2450 50  0001 C CNN
F 1 "GND" H 5700 2550 50  0000 C CNN
F 2 "" H 5700 2700 50  0001 C CNN
F 3 "" H 5700 2700 50  0001 C CNN
	1    5700 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	5700 2700 5700 2600
$Comp
L ASE-xxxMHz X1
U 1 1 5BA5AF21
P 4750 4300
F 0 "X1" H 4550 4550 50  0000 L CNN
F 1 "32.768kHz" H 4800 4050 50  0000 L CNN
F 2 "Oscillators:Oscillator_SMD_Abracon_ASE-4pin_3.2x2.5mm" H 5450 3950 50  0001 C CNN
F 3 "" H 4650 4300 50  0001 C CNN
F 4 "ASEK-32.768KHZ-LRT" H 4750 4300 60  0001 C CNN "Part Number"
	1    4750 4300
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR039
U 1 1 5BA5B077
P 4750 3850
F 0 "#PWR039" H 4750 3700 50  0001 C CNN
F 1 "+3.3V" H 4750 3990 50  0000 C CNN
F 2 "" H 4750 3850 50  0001 C CNN
F 3 "" H 4750 3850 50  0001 C CNN
	1    4750 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 3850 4750 4000
Wire Wire Line
	4750 3900 4300 3900
Wire Wire Line
	4300 3900 4300 4300
Wire Wire Line
	4300 4300 4450 4300
Connection ~ 4750 3900
$Comp
L GND #PWR040
U 1 1 5BA5B0F5
P 6000 4950
F 0 "#PWR040" H 6000 4700 50  0001 C CNN
F 1 "GND" H 6000 4800 50  0000 C CNN
F 2 "" H 6000 4950 50  0001 C CNN
F 3 "" H 6000 4950 50  0001 C CNN
	1    6000 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 4600 4750 4750
Wire Wire Line
	4750 4750 6000 4750
Connection ~ 6000 4750
Wire Wire Line
	5200 4300 5050 4300
Wire Wire Line
	5200 3750 5200 4300
Wire Wire Line
	5200 3750 5350 3750
Text HLabel 6900 3300 2    60   Output ~ 0
PWM1
Text HLabel 6900 3600 2    60   Output ~ 0
PWM2
Text HLabel 6900 3400 2    60   Input ~ 0
TACH1
Text HLabel 6900 3700 2    60   Input ~ 0
TACH2
Wire Wire Line
	6650 3300 6900 3300
Wire Wire Line
	6650 3400 6900 3400
Wire Wire Line
	6650 3600 6900 3600
Wire Wire Line
	6650 3700 6900 3700
Text HLabel 5150 3250 0    60   Output ~ 0
~ALERT
Text HLabel 5150 3450 0    60   BiDi ~ 0
SDA
Text HLabel 5150 3550 0    60   Input ~ 0
SCL
Wire Wire Line
	5150 3550 5350 3550
Wire Wire Line
	5150 3450 5350 3450
Wire Wire Line
	5150 3250 5350 3250
Text Label 5200 4300 0    60   ~ 0
32K768_CLK
$EndSCHEMATC
