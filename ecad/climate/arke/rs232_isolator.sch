EESchema Schematic File Version 4
LIBS:power
LIBS:74xx
LIBS:fort
LIBS:arke-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 5
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
L fort:ADM3251E U1
U 1 1 5BA26332
P 5300 3700
F 0 "U1" H 4850 4450 60  0000 C CNN
F 1 "ADM3251E" H 4900 2950 60  0000 C CNN
F 2 "Package_SO:SOIC-20W_7.5x12.8mm_P1.27mm" H 4750 4350 60  0001 C CNN
F 3 "" H 4850 4450 60  0001 C CNN
F 4 "ADM3251E" H 4950 4550 60  0001 C CNN "Part Number"
	1    5300 3700
	1    0    0    -1  
$EndComp
Text HLabel 6350 3800 2    60   Input ~ 0
ISO_RX
Text HLabel 6350 3900 2    60   Output ~ 0
ISO_TX
Wire Wire Line
	6000 3800 6350 3800
Wire Wire Line
	6000 3900 6350 3900
Text HLabel 4150 3800 0    60   Output ~ 0
RX
Text HLabel 4150 3900 0    60   Input ~ 0
TX
Wire Wire Line
	4150 3900 4600 3900
Wire Wire Line
	4150 3800 4600 3800
$Comp
L power:GND #PWR05
U 1 1 5BA263FA
P 5150 4800
F 0 "#PWR05" H 5150 4550 50  0001 C CNN
F 1 "GND" H 5150 4650 50  0000 C CNN
F 2 "" H 5150 4800 50  0001 C CNN
F 3 "" H 5150 4800 50  0001 C CNN
	1    5150 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 4800 5150 4600
$Comp
L Device:C C1
U 1 1 5BA26629
P 4400 2950
F 0 "C1" H 4425 3050 50  0000 L CNN
F 1 "100n" H 4425 2850 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4438 2800 50  0001 C CNN
F 3 "" H 4400 2950 50  0001 C CNN
F 4 "CC0603KRX7R8BB104" H 4400 2950 60  0001 C CNN "Part Number"
	1    4400 2950
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR06
U 1 1 5BA26630
P 4400 3200
F 0 "#PWR06" H 4400 2950 50  0001 C CNN
F 1 "GND" H 4400 3050 50  0000 C CNN
F 2 "" H 4400 3200 50  0001 C CNN
F 3 "" H 4400 3200 50  0001 C CNN
	1    4400 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 3200 4400 3100
$Comp
L Device:C C2
U 1 1 5BA26706
P 6400 3100
F 0 "C2" V 6450 3150 50  0000 L CNN
F 1 "100n" V 6450 2850 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6438 2950 50  0001 C CNN
F 3 "" H 6400 3100 50  0001 C CNN
F 4 "CC0603KRX7R8BB104" H 6400 3100 60  0001 C CNN "Part Number"
	1    6400 3100
	0    -1   -1   0   
$EndComp
$Comp
L Device:C C5
U 1 1 5BA268C2
P 6400 3300
F 0 "C5" V 6450 3350 50  0000 L CNN
F 1 "100n" V 6450 3050 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6438 3150 50  0001 C CNN
F 3 "" H 6400 3300 50  0001 C CNN
F 4 "CC0603KRX7R8BB104" H 6400 3300 60  0001 C CNN "Part Number"
	1    6400 3300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6000 3200 6700 3200
Wire Wire Line
	6700 3200 6700 3100
Wire Wire Line
	6700 3100 6550 3100
Wire Wire Line
	6000 3100 6250 3100
Wire Wire Line
	6000 3300 6250 3300
Wire Wire Line
	6000 3400 6700 3400
Wire Wire Line
	6700 3400 6700 3300
Wire Wire Line
	6700 3300 6550 3300
$Comp
L Device:C C3
U 1 1 5BA26B07
P 7050 3200
F 0 "C3" H 7075 3300 50  0000 L CNN
F 1 "100n" H 7075 3100 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 7088 3050 50  0001 C CNN
F 3 "" H 7050 3200 50  0001 C CNN
F 4 "CC0603KRX7R8BB104" H 7050 3200 60  0001 C CNN "Part Number"
	1    7050 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 2800 5450 2750
Wire Wire Line
	5450 2750 7050 2750
Wire Wire Line
	7050 2750 7050 3050
Text Label 6450 2750 2    60   ~ 0
V_ISO
Wire Wire Line
	6000 3500 7050 3500
Wire Wire Line
	7050 3500 7050 3350
$Comp
L Device:C C4
U 1 1 5BA26BE9
P 7400 3200
F 0 "C4" H 7425 3300 50  0000 L CNN
F 1 "100n" H 7425 3100 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 7438 3050 50  0001 C CNN
F 3 "" H 7400 3200 50  0001 C CNN
F 4 "CC0603KRX7R8BB104" H 7400 3200 60  0001 C CNN "Part Number"
	1    7400 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	7400 2750 7400 3050
Connection ~ 7050 2750
Wire Wire Line
	7400 3350 7400 4800
Wire Wire Line
	5450 4800 7050 4800
Wire Wire Line
	5450 4800 5450 4600
$Comp
L Device:C C6
U 1 1 5BA26C69
P 7050 4100
F 0 "C6" H 7075 4200 50  0000 L CNN
F 1 "100n" H 7075 4000 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 7088 3950 50  0001 C CNN
F 3 "" H 7050 4100 50  0001 C CNN
F 4 "CC0603KRX7R8BB104" H 7050 4100 60  0001 C CNN "Part Number"
	1    7050 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 3600 7050 3600
Wire Wire Line
	7050 3600 7050 3950
Wire Wire Line
	7050 4250 7050 4800
Connection ~ 7050 4800
Wire Wire Line
	4400 2800 4400 2700
Wire Wire Line
	4400 2700 5150 2700
Wire Wire Line
	5150 2550 5150 2700
$Comp
L power:+5V #PWR07
U 1 1 5BA26D6E
P 5150 2550
F 0 "#PWR07" H 5150 2400 50  0001 C CNN
F 1 "+5V" H 5150 2690 50  0000 C CNN
F 2 "" H 5150 2550 50  0001 C CNN
F 3 "" H 5150 2550 50  0001 C CNN
	1    5150 2550
	1    0    0    -1  
$EndComp
Connection ~ 5150 2700
Text HLabel 7700 4800 2    60   BiDi ~ 0
GND_ISO
Connection ~ 7400 4800
Wire Wire Line
	7050 2750 7400 2750
Wire Wire Line
	7050 4800 7400 4800
Wire Wire Line
	5150 2700 5150 2800
Wire Wire Line
	7400 4800 7700 4800
$EndSCHEMATC