EESchema Schematic File Version 4
LIBS:celaeno-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 5 8
Title "4 Wire PC FAN Controller"
Date "06/11/2018"
Rev "B"
Comp "Université de Lausanne"
Comment1 "Author: Alexandre Tuleu"
Comment2 "Licensed under the CERN OHL v1.2"
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L power:+3.3V #PWR017
U 1 1 5BA5AD44
P 6000 2200
AR Path="/5BA5AC9B/5BA5AD44" Ref="#PWR017"  Part="1" 
AR Path="/5BAA713E/5BA5AD44" Ref="#PWR036"  Part="1" 
AR Path="/5BA5AD44" Ref="#PWR036"  Part="1" 
F 0 "#PWR017" H 6000 2050 50  0001 C CNN
F 1 "+3.3V" H 6000 2340 50  0000 C CNN
F 2 "" H 6000 2200 50  0001 C CNN
F 3 "" H 6000 2200 50  0001 C CNN
	1    6000 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 2200 6000 2250
$Comp
L Device:C C5
U 1 1 5BA5AE71
P 5700 2450
AR Path="/5BA5AC9B/5BA5AE71" Ref="C5"  Part="1" 
AR Path="/5BAA713E/5BA5AE71" Ref="C6"  Part="1" 
F 0 "C5" H 5725 2550 50  0000 L CNN
F 1 "100nF" H 5725 2350 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 5738 2300 50  0001 C CNN
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
L power:GND #PWR018
U 1 1 5BA5AEA9
P 5700 2700
AR Path="/5BA5AC9B/5BA5AEA9" Ref="#PWR018"  Part="1" 
AR Path="/5BAA713E/5BA5AEA9" Ref="#PWR037"  Part="1" 
F 0 "#PWR018" H 5700 2450 50  0001 C CNN
F 1 "GND" H 5700 2550 50  0000 C CNN
F 2 "" H 5700 2700 50  0001 C CNN
F 3 "" H 5700 2700 50  0001 C CNN
	1    5700 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	5700 2700 5700 2600
$Comp
L power:GND #PWR020
U 1 1 5BA5B0F5
P 6000 4950
AR Path="/5BA5AC9B/5BA5B0F5" Ref="#PWR020"  Part="1" 
AR Path="/5BAA713E/5BA5B0F5" Ref="#PWR039"  Part="1" 
F 0 "#PWR020" H 6000 4700 50  0001 C CNN
F 1 "GND" H 6000 4800 50  0000 C CNN
F 2 "" H 6000 4950 50  0001 C CNN
F 3 "" H 6000 4950 50  0001 C CNN
	1    6000 4950
	1    0    0    -1  
$EndComp
Text HLabel 8150 3300 2    60   Output ~ 0
PWM1
Text HLabel 8150 3600 2    60   Output ~ 0
PWM2
Text HLabel 8150 3400 2    60   Input ~ 0
TACH1
Text HLabel 8150 3700 2    60   Input ~ 0
TACH2
Text HLabel 5150 3450 0    60   BiDi ~ 0
SDA
Text HLabel 5150 3550 0    60   Input ~ 0
SCL
Wire Wire Line
	5150 3550 5350 3550
Wire Wire Line
	5150 3450 5350 3450
Wire Wire Line
	6000 2250 6000 2800
Text Notes 550  7750 0    50   ~ 0
© 2018 Université de Lausanne\n\nThis documentation describes Open Hardware and is licensed under the\nCERN OHL v. 1.2.\n\nYou may redistribute and modify this documentation under the terms of\nthe CERN OHL v.1.2. (http://ohwr.org/cernohl). This documentation is\ndistributed WITHOUT ANY EXPRESS OR IMPLIED WARRANTY,\nINCLUDING OF MERCHANTABILITY, SATISFACTORY QUALITY AND\nFITNESS FOR A PARTICULAR PURPOSE. Please see the CERN OHL v.1.2\nfor applicable conditions
$Comp
L Device:R R?
U 1 1 5BE6233F
P 7350 3050
AR Path="/5BB3963A/5BE6233F" Ref="R?"  Part="1" 
AR Path="/5BA5AC9B/5BE6233F" Ref="R16"  Part="1" 
F 0 "R16" H 7420 3096 50  0000 L CNN
F 1 "10k" H 7420 3005 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 7280 3050 50  0001 C CNN
F 3 "~" H 7350 3050 50  0001 C CNN
F 4 "GPR060310K" H 7350 3050 50  0001 C CNN "Part Number"
	1    7350 3050
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 5BE6236B
P 7650 3050
AR Path="/5BB3963A/5BE6236B" Ref="R?"  Part="1" 
AR Path="/5BA5AC9B/5BE6236B" Ref="R17"  Part="1" 
F 0 "R17" H 7720 3096 50  0000 L CNN
F 1 "10k" H 7720 3005 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 7580 3050 50  0001 C CNN
F 3 "~" H 7650 3050 50  0001 C CNN
F 4 "GPR060310K" H 7650 3050 50  0001 C CNN "Part Number"
	1    7650 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	7350 3200 7350 3400
$Comp
L power:+3.3V #PWR0108
U 1 1 5BE6376D
P 7500 2700
AR Path="/5BA5AC9B/5BE6376D" Ref="#PWR0108"  Part="1" 
AR Path="/5BAA713E/5BE6376D" Ref="#PWR?"  Part="1" 
AR Path="/5BE6376D" Ref="#PWR?"  Part="1" 
F 0 "#PWR0108" H 7500 2550 50  0001 C CNN
F 1 "+3.3V" H 7500 2840 50  0000 C CNN
F 2 "" H 7500 2700 50  0001 C CNN
F 3 "" H 7500 2700 50  0001 C CNN
	1    7500 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	7350 2900 7350 2800
Wire Wire Line
	7350 2800 7500 2800
Wire Wire Line
	7650 2900 7650 2800
Wire Wire Line
	7500 2800 7500 2700
Connection ~ 7500 2800
Wire Wire Line
	7500 2800 7650 2800
Wire Wire Line
	7650 3200 7650 3700
Wire Wire Line
	6000 4200 6000 4950
NoConn ~ 5350 3750
Wire Wire Line
	6650 3400 7350 3400
Wire Wire Line
	6650 3700 7650 3700
Wire Wire Line
	6650 3600 8150 3600
Wire Wire Line
	6650 3300 8150 3300
Connection ~ 7350 3400
Wire Wire Line
	7350 3400 8150 3400
Connection ~ 7650 3700
Wire Wire Line
	7650 3700 8150 3700
Text HLabel 5150 3250 0    50   Output ~ 0
~ALERT
$Comp
L fort:EMC2302-1-AIZL-TR U6
U 1 1 5BA5AD00
P 6000 3500
AR Path="/5BA5AC9B/5BA5AD00" Ref="U6"  Part="1" 
AR Path="/5BAA713E/5BA5AD00" Ref="U61"  Part="1" 
F 0 "U6" H 5600 4050 60  0000 C CNN
F 1 "EMC2302-1-AIZL-TR" H 6500 2950 60  0000 C CNN
F 2 "Package_SO:MSOP-10_3x3mm_P0.5mm" H 6100 3500 60  0001 C CNN
F 3 "" H 6100 3500 60  0001 C CNN
F 4 "EMC2302-1-AIZL-TR" H 6100 3500 60  0001 C CNN "Part Number"
	1    6000 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 3250 5350 3250
$EndSCHEMATC
