EESchema Schematic File Version 4
LIBS:LED_driver-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 9
Title "Helios - Module - LED Drivers"
Date "12/10/2018"
Rev "C"
Comp "Université de Lausanne"
Comment1 "Author: Alexandre Tuleu"
Comment2 "Licensed under the CERN OHL v1.2"
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 6750 3150 1750 500 
U 5C0F6264
F0 "Buck Current Regulator 1.5A 1" 60
F1 "Buck_Current_Regulator_1.5A.sch" 60
F2 "LED+" O R 8500 3350 60 
F3 "LED-" I R 8500 3450 60 
F4 "VIN" I L 6750 3300 60 
F5 "PDIM" I L 6750 3400 60 
F6 "SYNC" I L 6750 3500 60 
$EndSheet
$Sheet
S 6750 4450 1750 500 
U 5C0F626B
F0 "Buck Current Regulator 1.5A 2" 60
F1 "Buck_Current_Regulator_1.5A.sch" 60
F2 "LED+" O R 8500 4650 60 
F3 "LED-" I R 8500 4750 60 
F4 "VIN" I L 6750 4600 60 
F5 "PDIM" I L 6750 4700 60 
F6 "SYNC" I L 6750 4800 60 
$EndSheet
$Sheet
S 6750 1850 1750 500 
U 5C0F6272
F0 "Buck Current Regulator 1A" 60
F1 "Buck_Current_Regulator_1A.sch" 60
F2 "LED+" O R 8500 2050 60 
F3 "LED-" I R 8500 2150 60 
F4 "VIN" I L 6750 2000 60 
F5 "PDIM" I L 6750 2100 60 
F6 "SYNC" I L 6750 2200 60 
$EndSheet
$Comp
L LED_driver-rescue:R R?
U 1 1 5C0F6279
P 3500 3300
AR Path="/5C0F6279" Ref="R?"  Part="1" 
AR Path="/5C0F5EE2/5C0F6279" Ref="R3"  Part="1" 
F 0 "R3" V 3580 3300 50  0000 C CNN
F 1 "4.7, 3/4W" V 3400 3300 50  0000 C CNN
F 2 "Resistor_SMD:R_2010_5025Metric" V 3430 3300 50  0001 C CNN
F 3 "" H 3500 3300 50  0001 C CNN
F 4 "AC2010JK-074R7L" H 3500 3300 60  0001 C CNN "Part Number"
	1    3500 3300
	0    1    1    0   
$EndComp
$Comp
L LED_driver-rescue:GND #PWR?
U 1 1 5C0F6286
P 4000 5150
AR Path="/5C0F6286" Ref="#PWR?"  Part="1" 
AR Path="/5C0F5EE2/5C0F6286" Ref="#PWR07"  Part="1" 
F 0 "#PWR07" H 4000 4900 50  0001 C CNN
F 1 "GND" H 4000 5000 50  0000 C CNN
F 2 "" H 4000 5150 50  0001 C CNN
F 3 "" H 4000 5150 50  0001 C CNN
	1    4000 5150
	1    0    0    -1  
$EndComp
$Comp
L LED_driver-rescue:GND #PWR?
U 1 1 5C0F6292
P 4400 5150
AR Path="/5C0F6292" Ref="#PWR?"  Part="1" 
AR Path="/5C0F5EE2/5C0F6292" Ref="#PWR08"  Part="1" 
F 0 "#PWR08" H 4400 4900 50  0001 C CNN
F 1 "GND" H 4400 5000 50  0000 C CNN
F 2 "" H 4400 5150 50  0001 C CNN
F 3 "" H 4400 5150 50  0001 C CNN
	1    4400 5150
	1    0    0    -1  
$EndComp
Text Notes 3600 5500 0    60   ~ 0
appr. 8min bleeding
$Comp
L LED_driver-rescue:R R?
U 1 1 5C0F629A
P 4600 3600
AR Path="/5C0F629A" Ref="R?"  Part="1" 
AR Path="/5C0F5EE2/5C0F629A" Ref="R1"  Part="1" 
F 0 "R1" V 4680 3600 50  0000 C CNN
F 1 "100k" V 4600 3600 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4530 3600 50  0001 C CNN
F 3 "" H 4600 3600 50  0001 C CNN
F 4 "RMCF0603FT100K" H 4600 3600 60  0001 C CNN "Part Number"
	1    4600 3600
	1    0    0    -1  
$EndComp
$Comp
L LED_driver-rescue:R R?
U 1 1 5C0F62A2
P 4400 4900
AR Path="/5C0F62A2" Ref="R?"  Part="1" 
AR Path="/5C0F5EE2/5C0F62A2" Ref="R6"  Part="1" 
F 0 "R6" V 4480 4900 50  0000 C CNN
F 1 "100k" V 4400 4900 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4330 4900 50  0001 C CNN
F 3 "" H 4400 4900 50  0001 C CNN
F 4 "RMCF0603FT100K" H 4400 4900 60  0001 C CNN "Part Number"
	1    4400 4900
	1    0    0    -1  
$EndComp
$Comp
L LED_driver-rescue:CP C?
U 1 1 5C0F62AA
P 4000 3550
AR Path="/5C0F62AA" Ref="C?"  Part="1" 
AR Path="/5C0F5EE2/5C0F62AA" Ref="C4"  Part="1" 
F 0 "C4" H 4025 3650 50  0000 L CNN
F 1 "4.7mF" H 4025 3450 50  0000 L CNN
F 2 "Capacitors_THT:CP_Radial_D25.0mm_P10.00mm_SnapIn" H 4038 3400 50  0001 C CNN
F 3 "" H 4000 3550 50  0001 C CNN
F 4 "381LX472M050J022" H 4000 3550 60  0001 C CNN "Part Number"
	1    4000 3550
	1    0    0    -1  
$EndComp
$Comp
L LED_driver-rescue:CP C?
U 1 1 5C0F62B2
P 4000 4850
AR Path="/5C0F62B2" Ref="C?"  Part="1" 
AR Path="/5C0F5EE2/5C0F62B2" Ref="C5"  Part="1" 
F 0 "C5" H 4025 4950 50  0000 L CNN
F 1 "4.7mF" H 4025 4750 50  0000 L CNN
F 2 "Capacitors_THT:CP_Radial_D25.0mm_P10.00mm_SnapIn" H 4038 4700 50  0001 C CNN
F 3 "" H 4000 4850 50  0001 C CNN
F 4 "381LX472M050J022" H 4000 4850 60  0001 C CNN "Part Number"
	1    4000 4850
	1    0    0    -1  
$EndComp
$Comp
L LED_driver-rescue:+36V #PWR?
U 1 1 5C0F62B9
P 3200 850
AR Path="/5C0F62B9" Ref="#PWR?"  Part="1" 
AR Path="/5C0F5EE2/5C0F62B9" Ref="#PWR02"  Part="1" 
F 0 "#PWR02" H 3200 700 50  0001 C CNN
F 1 "+36V" H 3200 990 50  0000 C CNN
F 2 "" H 3200 850 50  0001 C CNN
F 3 "" H 3200 850 50  0001 C CNN
	1    3200 850 
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 3300 4000 3400
Connection ~ 4000 3300
Wire Wire Line
	4000 3700 4000 3850
Wire Wire Line
	4000 4600 4000 4700
Connection ~ 4000 4600
Wire Wire Line
	4000 5000 4000 5150
Wire Wire Line
	4400 3850 4400 3800
Wire Wire Line
	4400 5150 4400 5100
Wire Wire Line
	4400 4750 4400 4600
Connection ~ 4400 4600
Wire Wire Line
	4400 3450 4400 3300
Connection ~ 4400 3300
Wire Wire Line
	3650 3300 4000 3300
Wire Wire Line
	3650 4600 4000 4600
Wire Wire Line
	3200 4600 3350 4600
Wire Wire Line
	3350 3300 3200 3300
Wire Wire Line
	8500 2050 8850 2050
Wire Wire Line
	8500 2150 8850 2150
Wire Wire Line
	8500 3350 8850 3350
Wire Wire Line
	8500 3450 8850 3450
Wire Wire Line
	8500 4650 8850 4650
Wire Wire Line
	8500 4750 8850 4750
$Comp
L LED_driver-rescue:R R?
U 1 1 5C0F62E4
P 3500 4600
AR Path="/5C0F62E4" Ref="R?"  Part="1" 
AR Path="/5C0F5EE2/5C0F62E4" Ref="R5"  Part="1" 
F 0 "R5" V 3580 4600 50  0000 C CNN
F 1 "4.7, 3/4W" V 3400 4600 50  0000 C CNN
F 2 "Resistor_SMD:R_2010_5025Metric" V 3430 4600 50  0001 C CNN
F 3 "" H 3500 4600 50  0001 C CNN
F 4 "AC2010JK-074R7L" H 3500 4600 60  0001 C CNN "Part Number"
	1    3500 4600
	0    1    1    0   
$EndComp
$Comp
L LED_driver-rescue:R R?
U 1 1 5C0F62EC
P 6550 2300
AR Path="/5C0F62EC" Ref="R?"  Part="1" 
AR Path="/5C0F5EE2/5C0F62EC" Ref="R26"  Part="1" 
F 0 "R26" V 6630 2300 50  0000 C CNN
F 1 "100k" V 6550 2300 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 6480 2300 50  0001 C CNN
F 3 "" H 6550 2300 50  0001 C CNN
F 4 "RMCF0603FT100K" H 6550 2300 60  0001 C CNN "Part Number"
	1    6550 2300
	1    0    0    -1  
$EndComp
$Comp
L LED_driver-rescue:R R?
U 1 1 5C0F62F4
P 6500 3700
AR Path="/5C0F62F4" Ref="R?"  Part="1" 
AR Path="/5C0F5EE2/5C0F62F4" Ref="R12"  Part="1" 
F 0 "R12" V 6580 3700 50  0000 C CNN
F 1 "100k" V 6500 3700 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 6430 3700 50  0001 C CNN
F 3 "" H 6500 3700 50  0001 C CNN
F 4 "RMCF0603FT100K" H 6500 3700 60  0001 C CNN "Part Number"
	1    6500 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	6550 2150 6550 2100
Wire Wire Line
	6500 3550 6500 3400
$Comp
L LED_driver-rescue:GND #PWR?
U 1 1 5C0F62FD
P 6550 2500
AR Path="/5C0F62FD" Ref="#PWR?"  Part="1" 
AR Path="/5C0F5EE2/5C0F62FD" Ref="#PWR03"  Part="1" 
F 0 "#PWR03" H 6550 2250 50  0001 C CNN
F 1 "GND" H 6550 2350 50  0000 C CNN
F 2 "" H 6550 2500 50  0001 C CNN
F 3 "" H 6550 2500 50  0001 C CNN
	1    6550 2500
	1    0    0    -1  
$EndComp
$Comp
L LED_driver-rescue:GND #PWR?
U 1 1 5C0F6303
P 6500 3900
AR Path="/5C0F6303" Ref="#PWR?"  Part="1" 
AR Path="/5C0F5EE2/5C0F6303" Ref="#PWR06"  Part="1" 
F 0 "#PWR06" H 6500 3650 50  0001 C CNN
F 1 "GND" H 6500 3750 50  0000 C CNN
F 2 "" H 6500 3900 50  0001 C CNN
F 3 "" H 6500 3900 50  0001 C CNN
	1    6500 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 3900 6500 3850
Wire Wire Line
	6550 2500 6550 2450
Wire Wire Line
	4000 4600 4400 4600
Wire Wire Line
	4400 4600 4600 4600
Wire Wire Line
	4400 3300 4600 3300
$Comp
L LED_driver-rescue:R R?
U 1 1 5C0F6321
P 4400 3600
AR Path="/5C0F6321" Ref="R?"  Part="1" 
AR Path="/5C0F5EE2/5C0F6321" Ref="R4"  Part="1" 
F 0 "R4" V 4480 3600 50  0000 C CNN
F 1 "100k" V 4400 3600 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4330 3600 50  0001 C CNN
F 3 "" H 4400 3600 50  0001 C CNN
F 4 "RMCF0603FT100K" H 4400 3600 60  0001 C CNN "Part Number"
	1    4400 3600
	1    0    0    -1  
$EndComp
$Comp
L LED_driver-rescue:R R?
U 1 1 5C0F6329
P 4600 4900
AR Path="/5C0F6329" Ref="R?"  Part="1" 
AR Path="/5C0F5EE2/5C0F6329" Ref="R41"  Part="1" 
F 0 "R41" V 4680 4900 50  0000 C CNN
F 1 "100k" V 4600 4900 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4530 4900 50  0001 C CNN
F 3 "" H 4600 4900 50  0001 C CNN
F 4 "RMCF0603FT100K" H 4600 4900 60  0001 C CNN "Part Number"
	1    4600 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 4600 4600 4750
Connection ~ 4600 4600
Wire Wire Line
	4600 4600 4800 4600
Wire Wire Line
	4400 3800 4600 3800
Wire Wire Line
	4600 3800 4600 3750
Connection ~ 4400 3800
Wire Wire Line
	4400 3800 4400 3750
Wire Wire Line
	4600 3450 4600 3300
Connection ~ 4600 3300
Wire Wire Line
	4600 3300 4800 3300
Wire Wire Line
	4400 5100 4600 5100
Wire Wire Line
	4600 5100 4600 5050
Connection ~ 4400 5100
Wire Wire Line
	4400 5100 4400 5050
Text HLabel 8850 2050 2    60   Output ~ 0
VIS+
Text HLabel 8850 2150 2    60   Input ~ 0
VIS-
Text HLabel 8850 4650 2    60   Output ~ 0
IR_B+
Text HLabel 8850 4750 2    60   Input ~ 0
IR_B-
Text HLabel 8850 3350 2    60   Output ~ 0
IR_A+
Text HLabel 8850 3450 2    60   Input ~ 0
IR_A-
$Comp
L LED_driver-rescue:R R?
U 1 1 5C0FAC24
P 4800 3600
AR Path="/5C0FAC24" Ref="R?"  Part="1" 
AR Path="/5C0F5EE2/5C0FAC24" Ref="R2"  Part="1" 
F 0 "R2" V 4880 3600 50  0000 C CNN
F 1 "100k" V 4800 3600 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4730 3600 50  0001 C CNN
F 3 "" H 4800 3600 50  0001 C CNN
F 4 "RMCF0603FT100K" H 4800 3600 60  0001 C CNN "Part Number"
	1    4800 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 3800 4800 3800
Wire Wire Line
	4800 3800 4800 3750
Wire Wire Line
	4800 3300 4800 3450
Connection ~ 4800 3300
Wire Wire Line
	4800 3300 4950 3300
$Comp
L LED_driver-rescue:R R?
U 1 1 5C0FC0A1
P 4800 4900
AR Path="/5C0FC0A1" Ref="R?"  Part="1" 
AR Path="/5C0F5EE2/5C0FC0A1" Ref="R42"  Part="1" 
F 0 "R42" V 4880 4900 50  0000 C CNN
F 1 "100k" V 4800 4900 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4730 4900 50  0001 C CNN
F 3 "" H 4800 4900 50  0001 C CNN
F 4 "RMCF0603FT100K" H 4800 4900 60  0001 C CNN "Part Number"
	1    4800 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 5100 4800 5100
Wire Wire Line
	4800 5100 4800 5050
Connection ~ 4600 5100
Wire Wire Line
	4800 4750 4800 4600
Connection ~ 4800 4600
Wire Wire Line
	4800 4600 4950 4600
Connection ~ 4600 3800
Wire Wire Line
	4000 3300 4400 3300
$Comp
L Device:Jumper_NO_Small JP?
U 1 1 5C102EEA
P 3950 2900
AR Path="/5C102EEA" Ref="JP?"  Part="1" 
AR Path="/5C0F5EE2/5C102EEA" Ref="JP1"  Part="1" 
F 0 "JP1" H 3950 2980 50  0000 C CNN
F 1 "S1621-46R" H 3960 2840 50  0000 C CNN
F 2 "FORT:S1621-46R" H 3950 2900 50  0001 C CNN
F 3 "~" H 3950 2900 50  0001 C CNN
F 4 "S1621-46R" H 0   0   50  0001 C CNN "Part Number"
	1    3950 2900
	-1   0    0    1   
$EndComp
Wire Wire Line
	3850 2900 3200 2900
Connection ~ 3200 2900
Wire Wire Line
	3200 2900 3200 3300
Wire Wire Line
	4050 2900 4950 2900
Wire Wire Line
	4950 2900 4950 3300
Connection ~ 4950 3300
$Comp
L Device:Jumper_NO_Small JP?
U 1 1 5C1050A4
P 3950 4200
AR Path="/5C1050A4" Ref="JP?"  Part="1" 
AR Path="/5C0F5EE2/5C1050A4" Ref="JP2"  Part="1" 
F 0 "JP2" H 3950 4280 50  0000 C CNN
F 1 "S1621-46R" H 3960 4140 50  0000 C CNN
F 2 "FORT:S1621-46R" H 3950 4200 50  0001 C CNN
F 3 "~" H 3950 4200 50  0001 C CNN
F 4 "S1621-46R" H 0   0   50  0001 C CNN "Part Number"
	1    3950 4200
	-1   0    0    1   
$EndComp
Wire Wire Line
	3850 4200 3200 4200
Wire Wire Line
	4050 4200 4950 4200
Wire Wire Line
	4950 4200 4950 4600
Connection ~ 4950 4600
Wire Wire Line
	4950 4600 6750 4600
Wire Wire Line
	4950 3300 6750 3300
Wire Wire Line
	3200 4600 3200 4200
Connection ~ 3200 3300
Connection ~ 3200 4200
Wire Wire Line
	3200 4200 3200 3300
Wire Wire Line
	3200 2000 3200 2900
Wire Wire Line
	3200 850  3200 2000
Connection ~ 3200 2000
$Comp
L LED_driver-rescue:R R?
U 1 1 5C10E7BE
P 6500 5000
AR Path="/5C10E7BE" Ref="R?"  Part="1" 
AR Path="/5C0F5EE2/5C10E7BE" Ref="R19"  Part="1" 
F 0 "R19" V 6580 5000 50  0000 C CNN
F 1 "100k" V 6500 5000 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 6430 5000 50  0001 C CNN
F 3 "" H 6500 5000 50  0001 C CNN
F 4 "RMCF0603FT100K" H 6500 5000 60  0001 C CNN "Part Number"
	1    6500 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 4850 6500 4700
$Comp
L LED_driver-rescue:GND #PWR?
U 1 1 5C10E7C6
P 6500 5200
AR Path="/5C10E7C6" Ref="#PWR?"  Part="1" 
AR Path="/5C0F5EE2/5C10E7C6" Ref="#PWR09"  Part="1" 
F 0 "#PWR09" H 6500 4950 50  0001 C CNN
F 1 "GND" H 6500 5050 50  0000 C CNN
F 2 "" H 6500 5200 50  0001 C CNN
F 3 "" H 6500 5200 50  0001 C CNN
	1    6500 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 5200 6500 5150
Wire Wire Line
	6750 4700 6500 4700
Wire Wire Line
	6750 3400 6500 3400
Wire Wire Line
	6750 2100 6550 2100
Wire Wire Line
	3200 2000 6750 2000
Text HLabel 6400 2100 0    60   Input ~ 0
VIS_ON
Wire Wire Line
	6400 2100 6550 2100
Connection ~ 6550 2100
Text HLabel 6350 4700 0    60   Input ~ 0
IR_B_ON
Wire Wire Line
	6300 3400 6500 3400
Connection ~ 6500 3400
Text HLabel 6300 3400 0    60   Input ~ 0
IR_A_ON
Wire Wire Line
	6350 4700 6500 4700
Connection ~ 6500 4700
$Comp
L LED_driver-rescue:GND #PWR?
U 1 1 5C119324
P 4000 3850
AR Path="/5C119324" Ref="#PWR?"  Part="1" 
AR Path="/5C0F5EE2/5C119324" Ref="#PWR04"  Part="1" 
F 0 "#PWR04" H 4000 3600 50  0001 C CNN
F 1 "GND" H 4000 3700 50  0000 C CNN
F 2 "" H 4000 3850 50  0001 C CNN
F 3 "" H 4000 3850 50  0001 C CNN
	1    4000 3850
	1    0    0    -1  
$EndComp
$Comp
L LED_driver-rescue:GND #PWR?
U 1 1 5C119546
P 4400 3850
AR Path="/5C119546" Ref="#PWR?"  Part="1" 
AR Path="/5C0F5EE2/5C119546" Ref="#PWR05"  Part="1" 
F 0 "#PWR05" H 4400 3600 50  0001 C CNN
F 1 "GND" H 4400 3700 50  0000 C CNN
F 2 "" H 4400 3850 50  0001 C CNN
F 3 "" H 4400 3850 50  0001 C CNN
	1    4400 3850
	1    0    0    -1  
$EndComp
Text Notes 550  7750 0    60   ~ 0
© 2018 Université de Lausanne\n\nThis documentation describes Open Hardware and is licensed under the\nCERN OHL v. 1.2.\n\nYou may redistribute and modify this documentation under the terms of\nthe CERN OHL v.1.2. (http://ohwr.org/cernohl). This documentation is\ndistributed WITHOUT ANY EXPRESS OR IMPLIED WARRANTY,\nINCLUDING OF MERCHANTABILITY, SATISFACTORY QUALITY AND\nFITNESS FOR A PARTICULAR PURPOSE. Please see the CERN OHL v.1.2\nfor applicable conditions
$EndSCHEMATC
