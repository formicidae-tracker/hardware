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
LIBS:Evaluation Boards-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 4
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
L LM2842Y U1
U 1 1 5B362C2A
P 4500 3200
F 0 "U1" H 4300 2800 60  0000 C CNN
F 1 "LM2842Y" H 4500 3550 60  0000 C CNN
F 2 "TO_SOT_Packages_SMD:SOT-23-6" H 4200 2700 60  0001 C CNN
F 3 "" H 4500 3200 60  0001 C CNN
	1    4500 3200
	1    0    0    -1  
$EndComp
$Comp
L C C2
U 1 1 5B362C31
P 3000 3300
F 0 "C2" H 3025 3400 50  0000 L CNN
F 1 "1uF, 50V" H 3025 3200 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 3038 3150 50  0001 C CNN
F 3 "" H 3000 3300 50  0001 C CNN
	1    3000 3300
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 5B362C38
P 6950 3600
F 0 "C3" H 6975 3700 50  0000 L CNN
F 1 "47uF" H 6975 3500 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 6988 3450 50  0001 C CNN
F 3 "" H 6950 3600 50  0001 C CNN
	1    6950 3600
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 5B362C3F
P 5500 2900
F 0 "C1" H 5525 3000 50  0000 L CNN
F 1 "470nF, 50V" H 5525 2800 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 5538 2750 50  0001 C CNN
F 3 "" H 5500 2900 50  0001 C CNN
	1    5500 2900
	1    0    0    -1  
$EndComp
$Comp
L +12V #PWR1
U 1 1 5B362C46
P 2250 2500
F 0 "#PWR1" H 2250 2350 50  0001 C CNN
F 1 "+12V" H 2250 2640 50  0000 C CNN
F 2 "" H 2250 2500 50  0001 C CNN
F 3 "" H 2250 2500 50  0001 C CNN
	1    2250 2500
	1    0    0    -1  
$EndComp
$Comp
L D_Schottky D1
U 1 1 5B362C4C
P 5850 3600
F 0 "D1" H 5850 3700 50  0000 C CNN
F 1 "D_Schottky" H 5850 3500 50  0000 C CNN
F 2 "Diodes_SMD:D_SOD-123" H 5850 3600 50  0001 C CNN
F 3 "" H 5850 3600 50  0001 C CNN
	1    5850 3600
	0    1    1    0   
$EndComp
$Comp
L R R1
U 1 1 5B362C53
P 6500 3400
F 0 "R1" V 6580 3400 50  0000 C CNN
F 1 "6.81K" V 6500 3400 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 6430 3400 50  0001 C CNN
F 3 "" H 6500 3400 50  0001 C CNN
	1    6500 3400
	1    0    0    -1  
$EndComp
$Comp
L R R2
U 1 1 5B362C5A
P 6500 3800
F 0 "R2" V 6580 3800 50  0000 C CNN
F 1 "976" V 6500 3800 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 6430 3800 50  0001 C CNN
F 3 "" H 6500 3800 50  0001 C CNN
	1    6500 3800
	1    0    0    -1  
$EndComp
$Comp
L L L1
U 1 1 5B362C61
P 6150 3200
F 0 "L1" V 6100 3200 50  0000 C CNN
F 1 "100mH" V 6225 3200 50  0000 C CNN
F 2 "FORT:SRN1060-XXXM" H 6150 3200 50  0001 C CNN
F 3 "" H 6150 3200 50  0001 C CNN
	1    6150 3200
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4950 3200 6000 3200
Wire Wire Line
	6300 3200 6950 3200
Wire Wire Line
	6500 3200 6500 3250
Wire Wire Line
	6950 3200 6950 3450
Connection ~ 6500 3200
Wire Wire Line
	6500 3550 6500 3650
Wire Wire Line
	6500 3950 6500 4000
Wire Wire Line
	3000 4000 6950 4000
Wire Wire Line
	6950 4000 6950 3750
Wire Wire Line
	5850 4000 5850 3750
Connection ~ 6500 4000
Wire Wire Line
	5850 3450 5850 3200
Connection ~ 5850 3200
Wire Wire Line
	5500 3050 5500 3200
Connection ~ 5500 3200
Wire Wire Line
	4950 3000 5100 3000
Wire Wire Line
	5100 3000 5100 2600
Wire Wire Line
	5100 2600 5500 2600
Wire Wire Line
	5500 2600 5500 2750
Wire Wire Line
	4950 3400 5450 3400
Wire Wire Line
	5450 3400 5450 3900
Wire Wire Line
	5450 3900 6150 3900
Wire Wire Line
	6150 3900 6150 3600
Wire Wire Line
	6150 3600 6500 3600
Connection ~ 6500 3600
Wire Wire Line
	3850 4000 3850 3400
Wire Wire Line
	3850 3400 4050 3400
Connection ~ 5850 4000
Wire Wire Line
	3000 3450 3000 4000
Connection ~ 3850 4000
Wire Wire Line
	3000 3150 3000 3000
Wire Wire Line
	2250 3000 4050 3000
Wire Wire Line
	2250 2500 2250 3000
Connection ~ 3000 3000
Wire Wire Line
	4050 3200 3650 3200
Wire Wire Line
	3650 3200 3650 3000
Connection ~ 3650 3000
$Comp
L Conn_01x03 J2
U 1 1 5B362C8D
P 8800 3200
F 0 "J2" H 8800 3400 50  0000 C CNN
F 1 "Conn_01x03" H 8800 3000 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x03_Pitch2.54mm" H 8800 3200 50  0001 C CNN
F 3 "" H 8800 3200 50  0001 C CNN
	1    8800 3200
	1    0    0    -1  
$EndComp
$Comp
L Conn_01x03 J1
U 1 1 5B362C94
P 7800 3200
F 0 "J1" H 7800 3400 50  0000 C CNN
F 1 "Conn_01x03" H 7800 3000 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x03_Pitch2.54mm" H 7800 3200 50  0001 C CNN
F 3 "" H 7800 3200 50  0001 C CNN
	1    7800 3200
	-1   0    0    1   
$EndComp
Text Label 5250 2600 0    60   ~ 0
CB
Text Label 5100 3200 0    60   ~ 0
SW
Text Label 5100 3400 0    60   ~ 0
FB
$Comp
L GND #PWR4
U 1 1 5B362C9E
P 4850 4250
F 0 "#PWR4" H 4850 4000 50  0001 C CNN
F 1 "GND" H 4850 4100 50  0000 C CNN
F 2 "" H 4850 4250 50  0001 C CNN
F 3 "" H 4850 4250 50  0001 C CNN
	1    4850 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	4850 4250 4850 4000
Connection ~ 4850 4000
$Comp
L GND #PWR3
U 1 1 5B362CA6
P 8100 3400
F 0 "#PWR3" H 8100 3150 50  0001 C CNN
F 1 "GND" H 8100 3250 50  0000 C CNN
F 2 "" H 8100 3400 50  0001 C CNN
F 3 "" H 8100 3400 50  0001 C CNN
	1    8100 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	8000 3300 8600 3300
Wire Wire Line
	8100 3300 8100 3400
$Comp
L +12V #PWR2
U 1 1 5B362CAE
P 8100 2950
F 0 "#PWR2" H 8100 2800 50  0001 C CNN
F 1 "+12V" H 8100 3090 50  0000 C CNN
F 2 "" H 8100 2950 50  0001 C CNN
F 3 "" H 8100 2950 50  0001 C CNN
	1    8100 2950
	1    0    0    -1  
$EndComp
Text Label 6750 3200 0    60   ~ 0
6v5
Text Label 8600 2900 0    60   ~ 0
6v5
Wire Wire Line
	8600 2900 8500 2900
Wire Wire Line
	8500 2900 8500 3100
Wire Wire Line
	8500 3100 8600 3100
Wire Wire Line
	8000 3100 8100 3100
Wire Wire Line
	8100 3100 8100 2950
Connection ~ 8100 3300
Text Label 8500 2700 0    60   ~ 0
SW
Wire Wire Line
	8550 2700 8300 2700
Wire Wire Line
	8300 2700 8300 3200
Wire Wire Line
	8300 3200 8600 3200
$EndSCHEMATC