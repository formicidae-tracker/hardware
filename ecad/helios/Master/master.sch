EESchema Schematic File Version 4
LIBS:master-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 8
Title "Helios Master Module"
Date "20/11/2018"
Rev "C"
Comp "Université de Lausanne"
Comment1 "Author: Alexandre Tuleu"
Comment2 "Licensed under the CERN OHL v1.2"
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	4200 1050 4400 1050
$Comp
L power:GND #PWR01
U 1 1 5AC5785D
P 4800 1350
F 0 "#PWR01" H 4800 1100 50  0001 C CNN
F 1 "GND" H 4800 1200 50  0000 C CNN
F 2 "" H 4800 1350 50  0001 C CNN
F 3 "" H 4800 1350 50  0001 C CNN
	1    4800 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 1150 4800 1150
Wire Wire Line
	4800 1150 4800 1350
$Sheet
S 7850 2450 1200 350 
U 5AC39A79
F0 "Output" 60
F1 "Output.sch" 60
F2 "DATA" I L 7850 2550 60 
F3 "TRIGGER" I L 7850 2650 60 
$EndSheet
$Comp
L power:GND #PWR013
U 1 1 5AC5FCF9
P 3100 5500
F 0 "#PWR013" H 3100 5250 50  0001 C CNN
F 1 "GND" H 3100 5350 50  0000 C CNN
F 2 "" H 3100 5500 50  0001 C CNN
F 3 "" H 3100 5500 50  0001 C CNN
	1    3100 5500
	1    0    0    -1  
$EndComp
$Comp
L master-rescue:Conn_01x02 J2
U 1 1 5AC56DF2
P 2800 2550
F 0 "J2" H 2800 2650 50  0000 C CNN
F 1 "MCV 1,5/ 2-G-3,5" H 2800 2350 50  0000 C CNN
F 2 "Connector_Phoenix_MC:PhoenixContact_MCV_1,5_2-G-3.5_1x02_P3.50mm_Vertical" H 2800 2550 50  0001 C CNN
F 3 "" H 2800 2550 50  0001 C CNN
F 4 "1843606" H 2800 2550 60  0001 C CNN "Part Number"
	1    2800 2550
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3250 2950 3000 2950
Wire Wire Line
	3150 3050 3000 3050
$Comp
L master-rescue:Conn_01x03 J4
U 1 1 5AC5ED3E
P 2750 5100
F 0 "J4" H 2750 5300 50  0000 C CNN
F 1 "MCV 1,5/ 3-G-3,5" H 2900 4900 50  0000 C CNN
F 2 "Connector_Phoenix_MC:PhoenixContact_MCV_1,5_3-G-3.5_1x03_P3.50mm_Vertical" H 2750 5100 50  0001 C CNN
F 3 "" H 2750 5100 50  0001 C CNN
F 4 "1843619" H 2750 5100 60  0001 C CNN "Part Number"
	1    2750 5100
	-1   0    0    -1  
$EndComp
$Comp
L power:+5V #PWR017
U 1 1 5AC5EE12
P 3100 4750
F 0 "#PWR017" H 3100 4600 50  0001 C CNN
F 1 "+5V" H 3100 4890 50  0000 C CNN
F 2 "" H 3100 4750 50  0001 C CNN
F 3 "" H 3100 4750 50  0001 C CNN
	1    3100 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	2950 5000 3100 5000
Wire Wire Line
	3100 5000 3100 4750
$Comp
L power:+12V #PWR018
U 1 1 5AC5F8E5
P 4400 850
F 0 "#PWR018" H 4400 700 50  0001 C CNN
F 1 "+12V" H 4400 990 50  0000 C CNN
F 2 "" H 4400 850 50  0001 C CNN
F 3 "" H 4400 850 50  0001 C CNN
	1    4400 850 
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 850  4400 1050
$Comp
L master-rescue:Conn_01x02 J3
U 1 1 5AC66E2D
P 2800 2950
F 0 "J3" H 2800 3050 50  0000 C CNN
F 1 "MCV 1,5/ 2-G-3,5" H 2800 2750 50  0000 C CNN
F 2 "Connector_Phoenix_MC:PhoenixContact_MCV_1,5_2-G-3.5_1x02_P3.50mm_Vertical" H 2800 2950 50  0001 C CNN
F 3 "" H 2800 2950 50  0001 C CNN
F 4 "1843606" H 2800 2950 60  0001 C CNN "Part Number"
	1    2800 2950
	-1   0    0    -1  
$EndComp
Text Notes 1850 5150 0    60   ~ 0
CoaXPress Trigger
$Comp
L master-rescue:Conn_01x02 J1
U 1 1 5AC680D1
P 4000 1050
F 0 "J1" H 4000 1150 50  0000 C CNN
F 1 "5566-02A" H 4000 800 50  0000 C CNN
F 2 "Connector_Molex:Molex_Mini-Fit_Jr_5566-02A_2x01_P4.20mm_Vertical" H 4000 1050 50  0001 C CNN
F 3 "" H 4000 1050 50  0001 C CNN
F 4 "39-28-1023" H 4000 1050 60  0001 C CNN "Part Number"
	1    4000 1050
	-1   0    0    -1  
$EndComp
Text Notes 3600 850  0    60   ~ 0
Alim 7-34V\n
Text Notes 2950 2350 2    60   ~ 0
2x CAN
$Sheet
S 5100 950  1350 200 
U 5BF56E6D
F0 "12V to 5V LDO" 50
F1 "../../common/12V5VDCDC.sch" 50
F2 "VIN" I L 5100 1050 50 
F3 "5V" O R 6450 1050 50 
$EndSheet
$Sheet
S 5400 2450 1800 400 
U 5BFAC0A8
F0 "UC" 50
F1 "UC.sch" 50
F2 "TXCAN" O L 5400 2550 50 
F3 "RXCAN" I L 5400 2650 50 
F4 "DATA" O R 7200 2550 50 
F5 "INHIBIT" I L 5400 2750 50 
$EndSheet
Wire Wire Line
	3000 2650 3150 2650
Wire Wire Line
	4400 1050 5100 1050
Connection ~ 4400 1050
Wire Wire Line
	6450 1050 6750 1050
Wire Wire Line
	6750 1050 6750 850 
$Comp
L power:+5V #PWR02
U 1 1 5BF539D0
P 6750 850
F 0 "#PWR02" H 6750 700 50  0001 C CNN
F 1 "+5V" H 6765 1023 50  0000 C CNN
F 2 "" H 6750 850 50  0001 C CNN
F 3 "" H 6750 850 50  0001 C CNN
	1    6750 850 
	1    0    0    -1  
$EndComp
Wire Wire Line
	3000 2550 3250 2550
Wire Wire Line
	3150 3050 3150 2650
Connection ~ 3150 2650
Wire Wire Line
	3250 2950 3250 2550
Wire Wire Line
	3250 2550 3600 2550
Connection ~ 3250 2550
Wire Wire Line
	3150 2650 3600 2650
Wire Wire Line
	5150 2550 5400 2550
Wire Wire Line
	5150 2650 5400 2650
Wire Wire Line
	7200 2550 7850 2550
Wire Wire Line
	7600 2650 7850 2650
$Sheet
S 3600 2450 1550 300 
U 5BF9C366
F0 "CAN Transceiver" 50
F1 "../../common/CAN_transceiver.sch" 50
F2 "CAN+" B L 3600 2550 50 
F3 "CAN-" B L 3600 2650 50 
F4 "TXD" I R 5150 2550 50 
F5 "RXD" O R 5150 2650 50 
$EndSheet
Text Notes 550  7750 0    50   ~ 0
© 2018 Université de Lausanne\n\nThis documentation describes Open Hardware and is licensed under the\nCERN OHL v. 1.2.\n\nYou may redistribute and modify this documentation under the terms of\nthe CERN OHL v.1.2. (http://ohwr.org/cernohl). This documentation is\ndistributed WITHOUT ANY EXPRESS OR IMPLIED WARRANTY,\nINCLUDING OF MERCHANTABILITY, SATISFACTORY QUALITY AND\nFITNESS FOR A PARTICULAR PURPOSE. Please see the CERN OHL v.1.2\nfor applicable conditions
$Comp
L power:VCC #PWR03
U 1 1 5BF64521
P 7000 850
F 0 "#PWR03" H 7000 700 50  0001 C CNN
F 1 "VCC" H 7017 1023 50  0000 C CNN
F 2 "" H 7000 850 50  0001 C CNN
F 3 "" H 7000 850 50  0001 C CNN
	1    7000 850 
	1    0    0    -1  
$EndComp
Wire Wire Line
	6750 1050 7000 1050
Wire Wire Line
	7000 1050 7000 850 
Connection ~ 6750 1050
$Comp
L Mechanical:MountingHole MH1
U 1 1 5BFF644B
P 6300 6950
F 0 "MH1" H 6400 6996 50  0000 L CNN
F 1 "M3" H 6400 6905 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965" H 6400 6859 50  0001 L CNN
F 3 "~" H 6300 6950 50  0001 C CNN
	1    6300 6950
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole MH2
U 1 1 5BFF7966
P 6300 7150
F 0 "MH2" H 6400 7196 50  0000 L CNN
F 1 "M3" H 6400 7105 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965" H 6400 7059 50  0001 L CNN
F 3 "~" H 6300 7150 50  0001 C CNN
	1    6300 7150
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole MH3
U 1 1 5BFF798E
P 6300 7400
F 0 "MH3" H 6400 7446 50  0000 L CNN
F 1 "M3" H 6400 7355 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965" H 6400 7309 50  0001 L CNN
F 3 "~" H 6300 7400 50  0001 C CNN
	1    6300 7400
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole MH4
U 1 1 5BFF79BA
P 6300 7600
F 0 "MH4" H 6400 7646 50  0000 L CNN
F 1 "M3" H 6400 7555 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965" H 6400 7509 50  0001 L CNN
F 3 "~" H 6300 7600 50  0001 C CNN
	1    6300 7600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0107
U 1 1 5C0EC4BA
P 3150 4150
F 0 "#PWR0107" H 3150 3900 50  0001 C CNN
F 1 "GND" H 3150 4000 50  0000 C CNN
F 2 "" H 3150 4150 50  0001 C CNN
F 3 "" H 3150 4150 50  0001 C CNN
	1    3150 4150
	1    0    0    -1  
$EndComp
$Comp
L master-rescue:Conn_01x03 J8
U 1 1 5C0EC4D0
P 2800 3800
F 0 "J8" H 2800 4000 50  0000 C CNN
F 1 "MCV 1,5/ 3-G-3,5" H 2950 3600 50  0000 C CNN
F 2 "Connector_Phoenix_MC:PhoenixContact_MCV_1,5_3-G-3.5_1x03_P3.50mm_Vertical" H 2800 3800 50  0001 C CNN
F 3 "" H 2800 3800 50  0001 C CNN
F 4 "1843619" H 2800 3800 60  0001 C CNN "Part Number"
	1    2800 3800
	-1   0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0108
U 1 1 5C0EC4D7
P 3150 3450
F 0 "#PWR0108" H 3150 3300 50  0001 C CNN
F 1 "+5V" H 3150 3590 50  0000 C CNN
F 2 "" H 3150 3450 50  0001 C CNN
F 3 "" H 3150 3450 50  0001 C CNN
	1    3150 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	3000 3700 3150 3700
Wire Wire Line
	3150 3700 3150 3450
Text Notes 1900 3850 0    60   ~ 0
Inhibit
Wire Wire Line
	5400 2750 5250 2750
Wire Wire Line
	7600 2650 7600 5100
$Sheet
S 3700 5000 750  200 
U 5C100773
F0 "Filter 1" 50
F1 "filter.sch" 50
F2 "IN" I L 3700 5100 50 
F3 "OUT" O R 4450 5100 50 
$EndSheet
$Sheet
S 3700 3700 750  200 
U 5C1001FD
F0 "Filter 2" 50
F1 "filter.sch" 50
F2 "IN" I L 3700 3800 50 
F3 "OUT" O R 4450 3800 50 
$EndSheet
Wire Wire Line
	3000 3900 3150 3900
Wire Wire Line
	3150 3900 3150 4150
Wire Wire Line
	2950 5200 3100 5200
Wire Wire Line
	3100 5200 3100 5500
Wire Wire Line
	3000 3800 3700 3800
Wire Wire Line
	2950 5100 3700 5100
Wire Wire Line
	4450 3800 5250 3800
Wire Wire Line
	5250 2750 5250 3800
Wire Wire Line
	4450 5100 7600 5100
Text Label 4800 3800 0    50   ~ 0
INHIBIT
Text Label 4850 5100 0    50   ~ 0
TRIGGER
$EndSCHEMATC
