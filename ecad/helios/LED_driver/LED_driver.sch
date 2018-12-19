EESchema Schematic File Version 4
LIBS:LED_driver-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 9
Title "Helios - Module"
Date "12/10/2018"
Rev "C"
Comp "Université de Lausanne"
Comment1 "Author: Alexandre Tuleu"
Comment2 "Licensed under the CERN OHL v1.2"
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 3950 2600 1400 400 
U 5AB14C4B
F0 "Power_Management" 60
F1 "Power_Management.sch" 60
F2 "VS36" I L 3950 2700 60 
F3 "VS6" I L 3950 2800 60 
F4 "EN36" I L 3950 2900 60 
$EndSheet
$Sheet
S 4950 3500 1200 700 
U 5AB48756
F0 "Controller" 60
F1 "Controller.sch" 60
F2 "DATA_IN" I L 4950 3700 60 
F3 "FLASH_IN" I L 4950 4000 60 
F4 "VIS_PWM" O R 6150 3800 60 
F5 "IR_A_ON" O R 6150 3900 60 
F6 "EN36V" O R 6150 3600 60 
F7 "UV_ON" O R 6150 3700 60 
F8 "IR_B_ON" O R 6150 4000 60 
$EndSheet
Text Label 1550 3650 0    60   ~ 0
TRIG_+
Text Label 1550 3750 0    60   ~ 0
TRIG_-
Text Label 2800 3650 0    60   ~ 0
DATA_+
Text Label 2800 3750 0    60   ~ 0
DATA_-
Text Label 1550 3450 0    60   ~ 0
VS36
Wire Wire Line
	9900 4900 9550 4900
Wire Wire Line
	10400 5100 10850 5100
Wire Wire Line
	10850 4900 10400 4900
Text Label 9550 5100 2    60   ~ 0
IRB-
Text Label 9550 5000 2    60   ~ 0
IRB+
Text Label 10850 5100 2    60   ~ 0
IRA-
Text Label 10850 5000 2    60   ~ 0
IRA+
Text Label 10850 4900 2    60   ~ 0
VIS-
Text Label 9550 4900 2    60   ~ 0
VIS+
$Comp
L LED_driver-rescue:Conn_02x04_Top_Bottom J1
U 1 1 5AB64FA8
P 2050 3550
F 0 "J1" H 2100 3750 50  0000 C CNN
F 1 "5566-08A2" H 2100 3250 50  0000 C CNN
F 2 "FORT:Molex_MiniFit-JR-5556-08A_2x04x4.20mm_Straight" H 2050 3550 50  0001 C CNN
F 3 "" H 2050 3550 50  0001 C CNN
F 4 "39-28-1083" H 2050 3550 60  0001 C CNN "Part Number"
	1    2050 3550
	1    0    0    -1  
$EndComp
$Comp
L LED_driver-rescue:Conn_02x04_Top_Bottom J3
U 1 1 5AB6535E
P 2050 4100
F 0 "J3" H 2100 4300 50  0000 C CNN
F 1 "5566-08A2" H 2100 3800 50  0000 C CNN
F 2 "FORT:Molex_MiniFit-JR-5556-08A_2x04x4.20mm_Straight" H 2050 4100 50  0001 C CNN
F 3 "" H 2050 4100 50  0001 C CNN
F 4 "39-28-1083" H 2050 4100 60  0001 C CNN "Part Number"
	1    2050 4100
	1    0    0    -1  
$EndComp
$Comp
L LED_driver-rescue:Conn_02x03_Top_Bottom J2
U 1 1 5AB65796
P 10200 5000
F 0 "J2" H 10250 5200 50  0000 C CNN
F 1 "5566-06A2" H 10250 4800 50  0000 C CNN
F 2 "Connectors_Molex:Molex_MiniFit-JR-5556-06A_2x03x4.20mm_Straight" H 10200 5000 50  0001 C CNN
F 3 "" H 10200 5000 50  0001 C CNN
F 4 "39-28-1063" H 10200 5000 60  0001 C CNN "Part Number"
	1    10200 5000
	-1   0    0    1   
$EndComp
$Sheet
S 7450 4300 1050 950 
U 5C0F5EE2
F0 "LED Drivers" 60
F1 "LED_Drivers.sch" 60
F2 "VIS+" O R 8500 4400 60 
F3 "VIS-" I R 8500 4500 60 
F4 "IR_B+" O R 8500 5000 60 
F5 "IR_B-" I R 8500 5100 60 
F6 "IR_A+" O R 8500 4700 60 
F7 "IR_A-" I R 8500 4800 60 
F8 "VIS_ON" I L 7450 4450 60 
F9 "IR_B_ON" I L 7450 5050 60 
F10 "IR_A_ON" I L 7450 4750 60 
$EndSheet
$Sheet
S 3850 3500 950  700 
U 5C12803A
F0 "Receivers" 60
F1 "Receiver.sch" 60
F2 "DATA_+" I L 3850 3650 60 
F3 "DATA_-" I L 3850 3750 60 
F4 "TRIG_+" I L 3850 3950 60 
F5 "TRIG_-" I L 3850 4050 60 
F6 "DATA" O R 4800 3700 60 
F7 "TRIG" O R 4800 4000 60 
$EndSheet
Wire Wire Line
	4800 3700 4950 3700
Wire Wire Line
	4800 4000 4950 4000
$Comp
L LED_driver-rescue:GND #PWR01
U 1 1 5C142013
P 2450 4500
F 0 "#PWR01" H 2450 4250 50  0001 C CNN
F 1 "GND" H 2455 4327 50  0000 C CNN
F 2 "" H 2450 4500 50  0001 C CNN
F 3 "" H 2450 4500 50  0001 C CNN
	1    2450 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	2350 3550 2450 3550
Wire Wire Line
	2350 3450 2450 3450
Wire Wire Line
	2350 4000 2450 4000
Wire Wire Line
	2350 3650 2550 3650
Wire Wire Line
	2550 3650 2550 4200
Wire Wire Line
	2550 4200 2350 4200
Wire Wire Line
	2350 4100 2450 4100
Wire Wire Line
	2350 4300 2650 4300
Wire Wire Line
	2650 4300 2650 3750
Wire Wire Line
	2650 3750 2350 3750
Wire Wire Line
	3850 3650 2550 3650
Connection ~ 2450 4100
Connection ~ 2450 4000
Wire Wire Line
	2450 4100 2450 4500
Wire Wire Line
	2450 4000 2450 4100
Connection ~ 2450 3550
Wire Wire Line
	2450 3550 2450 4000
Wire Wire Line
	2450 3450 2450 3550
Connection ~ 2550 3650
Wire Wire Line
	2650 3750 3850 3750
Connection ~ 2650 3750
Wire Wire Line
	3850 4050 3350 4050
Wire Wire Line
	3850 3950 3250 3950
Text Label 1550 3550 0    60   ~ 0
VS6
Wire Wire Line
	1250 3450 1850 3450
Wire Wire Line
	1250 4000 1850 4000
Wire Wire Line
	1350 3550 1850 3550
Wire Wire Line
	1450 3650 1850 3650
Wire Wire Line
	1550 3750 1850 3750
Wire Wire Line
	1450 4200 1850 4200
Wire Wire Line
	1550 4300 1850 4300
Wire Wire Line
	3250 3950 3250 4750
Wire Wire Line
	1450 4750 3250 4750
Wire Wire Line
	3350 4050 3350 4850
Wire Wire Line
	1550 4850 3350 4850
Wire Wire Line
	1250 2700 3950 2700
Wire Wire Line
	1350 2800 3950 2800
Wire Wire Line
	6150 3600 6400 3600
Wire Wire Line
	6400 3600 6400 3250
Wire Wire Line
	6400 3250 3800 3250
Wire Wire Line
	3800 3250 3800 2900
Wire Wire Line
	3800 2900 3950 2900
Wire Wire Line
	6150 3800 7150 3800
Wire Wire Line
	7150 3800 7150 4450
Wire Wire Line
	7150 4450 7450 4450
Wire Wire Line
	6150 3900 7000 3900
Wire Wire Line
	7000 3900 7000 4750
Wire Wire Line
	7000 4750 7450 4750
Wire Wire Line
	7450 5050 6850 5050
Wire Wire Line
	6850 5050 6850 4000
Wire Wire Line
	6850 4000 6150 4000
Wire Wire Line
	8500 5000 9900 5000
Wire Wire Line
	8500 5100 9900 5100
Wire Wire Line
	8500 4400 9550 4400
Wire Wire Line
	9550 4400 9550 4900
Wire Wire Line
	10850 4500 10850 4900
Wire Wire Line
	10850 5100 10850 5350
Wire Wire Line
	10850 5350 9100 5350
Wire Wire Line
	9100 5350 9100 4800
Wire Wire Line
	9100 4800 8500 4800
Wire Wire Line
	8500 4700 9000 4700
Wire Wire Line
	9000 4700 9000 5450
Wire Wire Line
	9000 5450 10950 5450
Wire Wire Line
	10950 5450 10950 5000
Wire Wire Line
	10400 5000 10950 5000
$Sheet
S 7450 3550 1000 350 
U 5C214647
F0 "UV LEDs" 60
F1 "UV_LEDs.sch" 60
F2 "UV_ON" I L 7450 3700 60 
$EndSheet
Wire Wire Line
	6150 3700 7450 3700
Wire Wire Line
	1550 3750 1550 4300
Wire Wire Line
	1450 3650 1450 4200
Wire Wire Line
	1350 3550 1350 4100
Wire Wire Line
	1350 4100 1850 4100
Wire Wire Line
	1250 3450 1250 4000
Wire Wire Line
	1450 4750 1450 4200
Connection ~ 1450 4200
Wire Wire Line
	1550 4300 1550 4850
Connection ~ 1550 4300
Wire Wire Line
	1250 3450 1250 2700
Connection ~ 1250 3450
Wire Wire Line
	1350 2800 1350 3550
Connection ~ 1350 3550
Wire Wire Line
	8500 4500 10850 4500
Text Notes 550  7750 0    60   ~ 0
© 2018 Université de Lausanne\n\nThis documentation describes Open Hardware and is licensed under the\nCERN OHL v. 1.2.\n\nYou may redistribute and modify this documentation under the terms of\nthe CERN OHL v.1.2. (http://ohwr.org/cernohl). This documentation is\ndistributed WITHOUT ANY EXPRESS OR IMPLIED WARRANTY,\nINCLUDING OF MERCHANTABILITY, SATISFACTORY QUALITY AND\nFITNESS FOR A PARTICULAR PURPOSE. Please see the CERN OHL v.1.2\nfor applicable conditions
$EndSCHEMATC
