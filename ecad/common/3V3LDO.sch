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
Sheet 4 7
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
L LP3985IM5-3.3/NOPB U1
U 1 1 5BA4F2C2
P 5900 3850
F 0 "U1" H 5650 3450 60  0000 C CNN
F 1 "LP3985IM5-3.3/NOPB" H 5900 4200 60  0000 C CNN
F 2 "TO_SOT_Packages_SMD:SOT-23-5" H 5850 3850 60  0001 C CNN
F 3 "" H 5850 3850 60  0001 C CNN
	1    5900 3850
	1    0    0    -1  
$EndComp
$Comp
L C C2
U 1 1 5BA4F2CA
P 6700 4100
F 0 "C2" H 6725 4200 50  0000 L CNN
F 1 "10nF" H 6725 4000 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 6738 3950 50  0001 C CNN
F 3 "" H 6700 4100 50  0001 C CNN
F 4 "06035C103KAT2A" H 6700 4100 60  0001 C CNN "Part Number"
	1    6700 4100
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 5BA4F2D2
P 7150 3900
F 0 "C3" H 7175 4000 50  0000 L CNN
F 1 "1uF" H 7175 3800 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 7188 3750 50  0001 C CNN
F 3 "" H 7150 3900 50  0001 C CNN
F 4 "0603YD105KAT2A" H 7150 3900 60  0001 C CNN "Part Number"
	1    7150 3900
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 5BA4F2DA
P 4850 3950
F 0 "C1" H 4875 4050 50  0000 L CNN
F 1 "1uF" H 4875 3850 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 4888 3800 50  0001 C CNN
F 3 "" H 4850 3950 50  0001 C CNN
F 4 "0603YD105KAT2A" H 4850 3950 60  0001 C CNN "Part Number"
	1    4850 3950
	1    0    0    -1  
$EndComp
Text HLabel 4100 3650 0    60   Input ~ 0
5V
Text HLabel 7550 3650 2    60   Output ~ 0
3V3
Wire Wire Line
	6400 3650 7550 3650
Wire Wire Line
	7150 3650 7150 3750
Wire Wire Line
	6400 3850 6700 3850
Wire Wire Line
	6700 3850 6700 3950
Wire Wire Line
	7150 4500 7150 4050
Wire Wire Line
	4850 4500 7150 4500
Wire Wire Line
	5900 4500 5900 4400
Wire Wire Line
	6700 4250 6700 4500
Connection ~ 6700 4500
Wire Wire Line
	4850 4100 4850 4500
Connection ~ 5900 4500
Wire Wire Line
	4850 3800 4850 3650
Wire Wire Line
	4100 3650 5400 3650
Wire Wire Line
	5400 3850 5250 3850
Wire Wire Line
	5250 3850 5250 3650
Connection ~ 5250 3650
Connection ~ 4850 3650
Connection ~ 7150 3650
$Comp
L GND #PWR030
U 1 1 5BA4F478
P 5600 4650
F 0 "#PWR030" H 5600 4400 50  0001 C CNN
F 1 "GND" H 5600 4500 50  0000 C CNN
F 2 "" H 5600 4650 50  0001 C CNN
F 3 "" H 5600 4650 50  0001 C CNN
	1    5600 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 4650 5600 4500
Connection ~ 5600 4500
Text Label 6700 3850 2    60   ~ 0
BYPASS
$EndSCHEMATC
