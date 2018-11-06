EESchema Schematic File Version 4
LIBS:zeus-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 9 10
Title "12V Low Side Sitch w/ free-whiling diode"
Date "06/11/2018"
Rev "A"
Comp "Université de Lausanne"
Comment1 "Author: Alexandre Tuleu"
Comment2 "Licensed under the CERN OHL v1.2"
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Device:Q_NMOS_GSD Q1
U 1 1 5BBA266C
P 5600 4250
AR Path="/5BBA23F4/5BBA266C" Ref="Q1"  Part="1" 
AR Path="/5BB2B93A/5BBA266C" Ref="Q2"  Part="1" 
F 0 "Q1" H 5800 4300 50  0000 L CNN
F 1 "PMV45EN" H 5800 4200 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 5800 4350 50  0001 C CNN
F 3 "" H 5600 4250 50  0001 C CNN
F 4 "PMV45EN" H 5600 4250 60  0001 C CNN "Part Number"
	1    5600 4250
	1    0    0    -1  
$EndComp
$Comp
L Device:D D1
U 1 1 5BBA2674
P 6500 3650
AR Path="/5BBA23F4/5BBA2674" Ref="D1"  Part="1" 
AR Path="/5BB2B93A/5BBA2674" Ref="D2"  Part="1" 
F 0 "D1" H 6500 3750 50  0000 C CNN
F 1 "1N4148WS" H 6500 3550 50  0000 C CNN
F 2 "Diode_SMD:D_SOD-323F" H 6500 3650 50  0001 C CNN
F 3 "" H 6500 3650 50  0001 C CNN
F 4 "1N4148WS" H 6500 3650 60  0001 C CNN "Part Number"
	1    6500 3650
	0    1    1    0   
$EndComp
$Comp
L power:+12V #PWR0109
U 1 1 5BBA267B
P 6500 3000
AR Path="/5BBA23F4/5BBA267B" Ref="#PWR0109"  Part="1" 
AR Path="/5BB2B93A/5BBA267B" Ref="#PWR0111"  Part="1" 
F 0 "#PWR0109" H 6500 2850 50  0001 C CNN
F 1 "+12V" H 6500 3140 50  0000 C CNN
F 2 "" H 6500 3000 50  0001 C CNN
F 3 "" H 6500 3000 50  0001 C CNN
	1    6500 3000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0110
U 1 1 5BBA2691
P 5700 4750
AR Path="/5BBA23F4/5BBA2691" Ref="#PWR0110"  Part="1" 
AR Path="/5BB2B93A/5BBA2691" Ref="#PWR0112"  Part="1" 
F 0 "#PWR0110" H 5700 4500 50  0001 C CNN
F 1 "GND" H 5700 4600 50  0000 C CNN
F 2 "" H 5700 4750 50  0001 C CNN
F 3 "" H 5700 4750 50  0001 C CNN
	1    5700 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 3800 6500 4000
Wire Wire Line
	5700 4000 6500 4000
Wire Wire Line
	5700 4000 5700 4050
Connection ~ 6500 4000
Text Label 5850 4000 0    50   ~ 0
SW_RTRN
Wire Wire Line
	6500 3000 6500 3400
Text HLabel 4950 4250 0    50   Input ~ 0
CMD
Wire Wire Line
	4950 4250 5250 4250
$Comp
L Device:R R?
U 1 1 5BBA6C7D
P 5250 4500
AR Path="/5BAB6A7C/5BBA6C7D" Ref="R?"  Part="1" 
AR Path="/5BBA23F4/5BBA6C7D" Ref="R20"  Part="1" 
AR Path="/5BB2B93A/5BBA6C7D" Ref="R21"  Part="1" 
F 0 "R20" V 5330 4500 50  0000 C CNN
F 1 "47k5" V 5250 4500 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 5180 4500 50  0001 C CNN
F 3 "" H 5250 4500 50  0001 C CNN
F 4 "GPR060347k5" H 5250 4500 60  0001 C CNN "Part Number"
	1    5250 4500
	-1   0    0    1   
$EndComp
Wire Wire Line
	5250 4350 5250 4250
Connection ~ 5250 4250
Wire Wire Line
	5250 4250 5400 4250
Wire Wire Line
	5250 4650 5250 4700
Wire Wire Line
	5250 4700 5700 4700
Wire Wire Line
	5700 4450 5700 4700
Wire Wire Line
	5700 4700 5700 4750
Connection ~ 5700 4700
Text HLabel 7250 3400 2    50   Output ~ 0
12V_SW+
Text HLabel 7200 4000 2    50   Output ~ 0
12V_SW-
Wire Wire Line
	6500 3400 7250 3400
Connection ~ 6500 3400
Wire Wire Line
	6500 3400 6500 3500
Wire Wire Line
	6500 4000 7200 4000
Text Notes 550  7750 0    50   ~ 0
© 2018 Université de Lausanne\n\nThis documentation describes Open Hardware and is licensed under the\nCERN OHL v. 1.2.\n\nYou may redistribute and modify this documentation under the terms of\nthe CERN OHL v.1.2. (http://ohwr.org/cernohl). This documentation is\ndistributed WITHOUT ANY EXPRESS OR IMPLIED WARRANTY,\nINCLUDING OF MERCHANTABILITY, SATISFACTORY QUALITY AND\nFITNESS FOR A PARTICULAR PURPOSE. Please see the CERN OHL v.1.2\nfor applicable conditions
$EndSCHEMATC
