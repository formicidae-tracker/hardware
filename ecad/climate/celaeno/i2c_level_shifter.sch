EESchema Schematic File Version 4
LIBS:celaeno-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 6 8
Title "I2C 5V / 3V3 Level Shifter"
Date "06/11/2018"
Rev "A"
Comp "Université de Lausanne"
Comment1 "Author: Alexandre Tuleu"
Comment2 "Licensed under the CERN OHL v1.2"
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Transistor_FET:BSS138 Q3
U 1 1 5BB396C4
P 5800 3750
F 0 "Q3" V 6050 3750 50  0000 C CNN
F 1 "BSS138" V 6141 3750 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 6000 3675 50  0001 L CIN
F 3 "https://www.fairchildsemi.com/datasheets/BS/BSS138.pdf" H 5800 3750 50  0001 L CNN
F 4 "BSS138" H 5800 3750 50  0001 C CNN "Part Number"
	1    5800 3750
	0    -1   1    0   
$EndComp
$Comp
L power:+5V #PWR07
U 1 1 5BB397D2
P 5100 2200
F 0 "#PWR07" H 5100 2050 50  0001 C CNN
F 1 "+5V" H 5115 2373 50  0000 C CNN
F 2 "" H 5100 2200 50  0001 C CNN
F 3 "" H 5100 2200 50  0001 C CNN
	1    5100 2200
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR09
U 1 1 5BB3983E
P 7100 2200
F 0 "#PWR09" H 7100 2050 50  0001 C CNN
F 1 "+3V3" H 7115 2373 50  0000 C CNN
F 2 "" H 7100 2200 50  0001 C CNN
F 3 "" H 7100 2200 50  0001 C CNN
	1    7100 2200
	1    0    0    -1  
$EndComp
$Comp
L Transistor_FET:BSS138 Q2
U 1 1 5BB39855
P 6300 3400
F 0 "Q2" V 6550 3400 50  0000 C CNN
F 1 "BSS138" V 6641 3400 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 6500 3325 50  0001 L CIN
F 3 "https://www.fairchildsemi.com/datasheets/BS/BSS138.pdf" H 6300 3400 50  0001 L CNN
F 4 "BSS138" H 6300 3400 50  0001 C CNN "Part Number"
	1    6300 3400
	0    -1   1    0   
$EndComp
$Comp
L Device:R R3
U 1 1 5BB399F4
P 4800 2500
F 0 "R3" H 4870 2546 50  0000 L CNN
F 1 "10k" H 4870 2455 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4730 2500 50  0001 C CNN
F 3 "~" H 4800 2500 50  0001 C CNN
F 4 "GPR060310K" H 4800 2500 50  0001 C CNN "Part Number"
	1    4800 2500
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 5BB39A93
P 5100 2500
F 0 "R1" H 5170 2546 50  0000 L CNN
F 1 "10k" H 5170 2455 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 5030 2500 50  0001 C CNN
F 3 "~" H 5100 2500 50  0001 C CNN
F 4 "GPR060310K" H 5100 2500 50  0001 C CNN "Part Number"
	1    5100 2500
	1    0    0    -1  
$EndComp
$Comp
L Device:R R11
U 1 1 5BB39C33
P 7550 2500
F 0 "R11" H 7620 2546 50  0000 L CNN
F 1 "10k" H 7620 2455 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 7480 2500 50  0001 C CNN
F 3 "~" H 7550 2500 50  0001 C CNN
F 4 "GPR060310K" H 7550 2500 50  0001 C CNN "Part Number"
	1    7550 2500
	1    0    0    -1  
$EndComp
$Comp
L Device:R R12
U 1 1 5BB39C3B
P 7800 2500
F 0 "R12" H 7870 2546 50  0000 L CNN
F 1 "10k" H 7870 2455 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 7730 2500 50  0001 C CNN
F 3 "~" H 7800 2500 50  0001 C CNN
F 4 "GPR060310K" H 7800 2500 50  0001 C CNN "Part Number"
	1    7800 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 3550 5800 2300
Wire Wire Line
	5800 2300 6300 2300
Wire Wire Line
	7100 2300 7100 2200
Wire Wire Line
	6300 3200 6300 2300
Connection ~ 6300 2300
Connection ~ 7100 2300
Wire Wire Line
	7550 2650 7550 3500
Wire Wire Line
	7550 3500 6500 3500
Wire Wire Line
	7800 2650 7800 3850
Wire Wire Line
	7800 3850 6000 3850
Wire Wire Line
	5100 2650 5100 3500
Wire Wire Line
	5100 3500 6100 3500
Wire Wire Line
	5600 3850 4800 3850
Wire Wire Line
	4800 3850 4800 2650
Wire Wire Line
	4800 2350 4800 2250
Wire Wire Line
	4800 2250 5100 2250
Wire Wire Line
	5100 2250 5100 2200
Wire Wire Line
	5100 2250 5100 2350
Connection ~ 5100 2250
Text HLabel 4600 3500 0    50   BiDi ~ 0
SDA_5V
Text HLabel 4600 3850 0    50   BiDi ~ 0
SCL_5V
Wire Wire Line
	4600 3500 5100 3500
Connection ~ 5100 3500
Wire Wire Line
	4600 3850 4800 3850
Connection ~ 4800 3850
Text HLabel 8050 3500 2    50   BiDi ~ 0
SDA_3V3
Text HLabel 8050 3850 2    50   BiDi ~ 0
SCL_3V3
Wire Wire Line
	7550 3500 8050 3500
Connection ~ 7550 3500
Wire Wire Line
	7800 3850 8050 3850
Connection ~ 7800 3850
Wire Wire Line
	7550 2300 7550 2350
Wire Wire Line
	7550 2300 7800 2300
Wire Wire Line
	7800 2300 7800 2350
Connection ~ 7550 2300
Text Notes 550  7750 0    50   ~ 0
© 2018 Université de Lausanne\n\nThis documentation describes Open Hardware and is licensed under the\nCERN OHL v. 1.2.\n\nYou may redistribute and modify this documentation under the terms of\nthe CERN OHL v.1.2. (http://ohwr.org/cernohl). This documentation is\ndistributed WITHOUT ANY EXPRESS OR IMPLIED WARRANTY,\nINCLUDING OF MERCHANTABILITY, SATISFACTORY QUALITY AND\nFITNESS FOR A PARTICULAR PURPOSE. Please see the CERN OHL v.1.2\nfor applicable conditions
Wire Wire Line
	5100 2250 5350 2250
Wire Wire Line
	5350 2250 5350 2350
$Comp
L Device:R R2
U 1 1 5BB39AC5
P 5350 2500
F 0 "R2" H 5420 2546 50  0000 L CNN
F 1 "10k" H 5420 2455 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 5280 2500 50  0001 C CNN
F 3 "~" H 5350 2500 50  0001 C CNN
F 4 "GPR060310K" H 5350 2500 50  0001 C CNN "Part Number"
	1    5350 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	7250 2300 7550 2300
Wire Wire Line
	7100 2300 7250 2300
Connection ~ 7250 2300
Wire Wire Line
	7250 2300 7250 2350
$Comp
L Device:R R10
U 1 1 5BB39C2B
P 7250 2500
F 0 "R10" H 7320 2546 50  0000 L CNN
F 1 "10k" H 7320 2455 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 7180 2500 50  0001 C CNN
F 3 "~" H 7250 2500 50  0001 C CNN
F 4 "GPR060310K" H 7250 2500 50  0001 C CNN "Part Number"
	1    7250 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	7250 2650 7250 3150
Text HLabel 8050 3150 2    50   BiDi ~ 0
~INT_3V3
Wire Wire Line
	8050 3150 7250 3150
Connection ~ 7250 3150
Wire Wire Line
	7250 3150 7000 3150
Wire Wire Line
	6800 2300 7100 2300
Wire Wire Line
	6300 2300 6800 2300
Connection ~ 6800 2300
Wire Wire Line
	6800 2850 6800 2300
$Comp
L Transistor_FET:BSS138 Q1
U 1 1 5BB3988D
P 6800 3050
F 0 "Q1" V 7050 3050 50  0000 C CNN
F 1 "BSS138" V 7141 3050 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 7000 2975 50  0001 L CIN
F 3 "https://www.fairchildsemi.com/datasheets/BS/BSS138.pdf" H 6800 3050 50  0001 L CNN
F 4 "BSS138" H 6800 3050 50  0001 C CNN "Part Number"
	1    6800 3050
	0    -1   1    0   
$EndComp
Wire Wire Line
	6600 3150 5350 3150
Wire Wire Line
	5350 3150 5350 2650
Connection ~ 5350 3150
Wire Wire Line
	4600 3150 5350 3150
Text HLabel 4600 3150 0    50   BiDi ~ 0
~INT_5V
$EndSCHEMATC
