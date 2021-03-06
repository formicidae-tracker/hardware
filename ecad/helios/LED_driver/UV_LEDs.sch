EESchema Schematic File Version 4
LIBS:LED_driver-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 9 9
Title "Helios - Module - UV Illumination"
Date "12/10/2018"
Rev "C"
Comp "Université de Lausanne"
Comment1 "Author: Alexandre Tuleu"
Comment2 "Licensed under the CERN OHL v1.2"
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L LED_driver-rescue:R R?
U 1 1 5C217C9B
P 4650 3650
AR Path="/5C217C9B" Ref="R?"  Part="1" 
AR Path="/5C214647/5C217C9B" Ref="R34"  Part="1" 
F 0 "R34" V 4730 3650 50  0000 C CNN
F 1 "82" V 4650 3650 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4580 3650 50  0001 C CNN
F 3 "" H 4650 3650 50  0001 C CNN
F 4 "RMCF0603FT82R0" H 4650 3650 60  0001 C CNN "Part Number"
	1    4650 3650
	0    1    1    0   
$EndComp
$Comp
L LED_driver-rescue:R R?
U 1 1 5C217CA3
P 4650 4050
AR Path="/5C217CA3" Ref="R?"  Part="1" 
AR Path="/5C214647/5C217CA3" Ref="R39"  Part="1" 
F 0 "R39" V 4730 4050 50  0000 C CNN
F 1 "82" V 4650 4050 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4580 4050 50  0001 C CNN
F 3 "" H 4650 4050 50  0001 C CNN
F 4 "RMCF0603FT82R0" H 4650 4050 60  0001 C CNN "Part Number"
	1    4650 4050
	0    1    1    0   
$EndComp
$Comp
L LED_driver-rescue:Q_NMOS_GSD Q?
U 1 1 5C217CAB
P 6250 4250
AR Path="/5C217CAB" Ref="Q?"  Part="1" 
AR Path="/5C214647/5C217CAB" Ref="Q3"  Part="1" 
F 0 "Q3" H 6450 4300 50  0000 L CNN
F 1 "BSS138L" H 6450 4200 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 6450 4350 50  0001 C CNN
F 3 "" H 6250 4250 50  0001 C CNN
F 4 "BSS138L" H 6250 4250 60  0001 C CNN "Part Number"
	1    6250 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 3650 5750 3650
Wire Wire Line
	5750 3650 5750 3900
Wire Wire Line
	5750 4050 5250 4050
Wire Wire Line
	5750 3900 6350 3900
Wire Wire Line
	6350 3900 6350 4050
Connection ~ 5750 3900
$Comp
L LED_driver-rescue:GND #PWR?
U 1 1 5C217CB8
P 6350 4650
AR Path="/5C217CB8" Ref="#PWR?"  Part="1" 
AR Path="/5C214647/5C217CB8" Ref="#PWR021"  Part="1" 
F 0 "#PWR021" H 6350 4400 50  0001 C CNN
F 1 "GND" H 6350 4500 50  0000 C CNN
F 2 "" H 6350 4650 50  0001 C CNN
F 3 "" H 6350 4650 50  0001 C CNN
	1    6350 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	6350 4650 6350 4450
Wire Wire Line
	4800 3650 5150 3650
Wire Wire Line
	4800 4050 5150 4050
Wire Wire Line
	4350 4050 4500 4050
Wire Wire Line
	4350 3650 4500 3650
Connection ~ 4350 3650
$Comp
L LED_driver-rescue:+5V #PWR?
U 1 1 5C217CC4
P 4350 3350
AR Path="/5C217CC4" Ref="#PWR?"  Part="1" 
AR Path="/5C214647/5C217CC4" Ref="#PWR019"  Part="1" 
F 0 "#PWR019" H 4350 3200 50  0001 C CNN
F 1 "+5V" H 4350 3490 50  0000 C CNN
F 2 "" H 4350 3350 50  0001 C CNN
F 3 "" H 4350 3350 50  0001 C CNN
	1    4350 3350
	1    0    0    -1  
$EndComp
$Comp
L LED_driver-rescue:R R?
U 1 1 5C217CCB
P 5850 4450
AR Path="/5C217CCB" Ref="R?"  Part="1" 
AR Path="/5C214647/5C217CCB" Ref="R40"  Part="1" 
F 0 "R40" V 5930 4450 50  0000 C CNN
F 1 "100k" V 5850 4450 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 5780 4450 50  0001 C CNN
F 3 "" H 5850 4450 50  0001 C CNN
F 4 "RMCF0603FT100K" H 5850 4450 60  0001 C CNN "Part Number"
	1    5850 4450
	1    0    0    -1  
$EndComp
Wire Wire Line
	5850 4300 5850 4250
$Comp
L LED_driver-rescue:GND #PWR?
U 1 1 5C217CD4
P 5850 4650
AR Path="/5C217CD4" Ref="#PWR?"  Part="1" 
AR Path="/5C214647/5C217CD4" Ref="#PWR020"  Part="1" 
F 0 "#PWR020" H 5850 4400 50  0001 C CNN
F 1 "GND" H 5850 4500 50  0000 C CNN
F 2 "" H 5850 4650 50  0001 C CNN
F 3 "" H 5850 4650 50  0001 C CNN
	1    5850 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5850 4650 5850 4600
$Comp
L LED_driver-rescue:LED D?
U 1 1 5C217CDC
P 5700 3050
AR Path="/5C217CDC" Ref="D?"  Part="1" 
AR Path="/5C214647/5C217CDC" Ref="D14"  Part="1" 
F 0 "D14" H 5700 3150 50  0000 C CNN
F 1 "UV" H 5700 2950 50  0000 C CNN
F 2 "" H 5700 3050 50  0001 C CNN
F 3 "" H 5700 3050 50  0001 C CNN
F 4 "VAOL-5GUV8T4" H 5700 3050 60  0001 C CNN "Part Number"
	1    5700 3050
	-1   0    0    1   
$EndComp
Wire Wire Line
	4350 3350 4350 3650
$Comp
L LED_driver-rescue:Conn_01x02 J?
U 1 1 5C217CE5
P 5150 3450
AR Path="/5C217CE5" Ref="J?"  Part="1" 
AR Path="/5C214647/5C217CE5" Ref="J5"  Part="1" 
F 0 "J5" H 5150 3550 50  0000 C CNN
F 1 "LED HDR" V 5250 3400 50  0000 C CNN
F 2 "FORT:640454-2" H 5150 3450 50  0001 C CNN
F 3 "" H 5150 3450 50  0001 C CNN
F 4 "640454-2" H 5150 3450 60  0001 C CNN "Part Number"
	1    5150 3450
	0    -1   -1   0   
$EndComp
$Comp
L LED_driver-rescue:Conn_01x02 J?
U 1 1 5C217CED
P 5150 3850
AR Path="/5C217CED" Ref="J?"  Part="1" 
AR Path="/5C214647/5C217CED" Ref="J6"  Part="1" 
F 0 "J6" H 5150 3950 50  0000 C CNN
F 1 "LED HDR" V 5250 3800 50  0000 C CNN
F 2 "FORT:640454-2" H 5150 3850 50  0001 C CNN
F 3 "" H 5150 3850 50  0001 C CNN
F 4 "640454-2" H 5150 3850 60  0001 C CNN "Part Number"
	1    5150 3850
	0    -1   -1   0   
$EndComp
$Comp
L LED_driver-rescue:Conn_01x02 J?
U 1 1 5C217CF5
P 5650 2600
AR Path="/5C217CF5" Ref="J?"  Part="1" 
AR Path="/5C214647/5C217CF5" Ref="J7"  Part="1" 
F 0 "J7" H 5650 2700 50  0000 C CNN
F 1 "LED PLUG" V 5750 2550 50  0000 C CNN
F 2 "" H 5650 2600 50  0001 C CNN
F 3 "" H 5650 2600 50  0001 C CNN
F 4 "1375820-2" H 5650 2600 60  0001 C CNN "Part Number"
	1    5650 2600
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5650 2800 5650 2900
Wire Wire Line
	5650 2900 5450 2900
Wire Wire Line
	5450 2900 5450 3050
Wire Wire Line
	5450 3050 5550 3050
Wire Wire Line
	5750 2800 5750 2900
Wire Wire Line
	5750 2900 5950 2900
Wire Wire Line
	5950 2900 5950 3050
Wire Wire Line
	5950 3050 5850 3050
$Comp
L LED_driver-rescue:LED D?
U 1 1 5C217D05
P 6450 3050
AR Path="/5C217D05" Ref="D?"  Part="1" 
AR Path="/5C214647/5C217D05" Ref="D15"  Part="1" 
F 0 "D15" H 6450 3150 50  0000 C CNN
F 1 "UV" H 6450 2950 50  0000 C CNN
F 2 "" H 6450 3050 50  0001 C CNN
F 3 "" H 6450 3050 50  0001 C CNN
F 4 "VAOL-5GUV8T4" H 6450 3050 60  0001 C CNN "Part Number"
	1    6450 3050
	-1   0    0    1   
$EndComp
$Comp
L LED_driver-rescue:Conn_01x02 J?
U 1 1 5C217D0D
P 6400 2600
AR Path="/5C217D0D" Ref="J?"  Part="1" 
AR Path="/5C214647/5C217D0D" Ref="J8"  Part="1" 
F 0 "J8" H 6400 2700 50  0000 C CNN
F 1 "LED PLUG" V 6500 2550 50  0000 C CNN
F 2 "" H 6400 2600 50  0001 C CNN
F 3 "" H 6400 2600 50  0001 C CNN
F 4 "1375820-2" H 6400 2600 60  0001 C CNN "Part Number"
	1    6400 2600
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6400 2800 6400 2900
Wire Wire Line
	6400 2900 6200 2900
Wire Wire Line
	6200 2900 6200 3050
Wire Wire Line
	6200 3050 6300 3050
Wire Wire Line
	6500 2800 6500 2900
Wire Wire Line
	6500 2900 6700 2900
Wire Wire Line
	6700 2900 6700 3050
Wire Wire Line
	6700 3050 6600 3050
Wire Wire Line
	5750 3900 5750 4050
Wire Wire Line
	4350 3650 4350 4050
Wire Wire Line
	5850 4250 6050 4250
Text HLabel 4150 4250 0    60   Input ~ 0
UV_ON
Wire Wire Line
	4150 4250 5850 4250
Connection ~ 5850 4250
Text Notes 550  7750 0    60   ~ 0
© 2018 Université de Lausanne\n\nThis documentation describes Open Hardware and is licensed under the\nCERN OHL v. 1.2.\n\nYou may redistribute and modify this documentation under the terms of\nthe CERN OHL v.1.2. (http://ohwr.org/cernohl). This documentation is\ndistributed WITHOUT ANY EXPRESS OR IMPLIED WARRANTY,\nINCLUDING OF MERCHANTABILITY, SATISFACTORY QUALITY AND\nFITNESS FOR A PARTICULAR PURPOSE. Please see the CERN OHL v.1.2\nfor applicable conditions
$EndSCHEMATC
