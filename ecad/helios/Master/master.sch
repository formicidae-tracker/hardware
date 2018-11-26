EESchema Schematic File Version 4
LIBS:master-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 6
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
	4200 1850 4400 1850
$Comp
L power:GND #PWR01
U 1 1 5AC5785D
P 4800 2150
F 0 "#PWR01" H 4800 1900 50  0001 C CNN
F 1 "GND" H 4800 2000 50  0000 C CNN
F 2 "" H 4800 2150 50  0001 C CNN
F 3 "" H 4800 2150 50  0001 C CNN
	1    4800 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 1950 4800 1950
Wire Wire Line
	4800 1950 4800 2150
$Sheet
S 7850 3250 1200 350 
U 5AC39A79
F0 "Output" 60
F1 "Output.sch" 60
F2 "DATA" I L 7850 3350 60 
F3 "TRIGGER" I L 7850 3450 60 
$EndSheet
$Comp
L power:+5V #PWR012
U 1 1 5AC5FB02
P 3850 4700
F 0 "#PWR012" H 3850 4550 50  0001 C CNN
F 1 "+5V" H 3850 4840 50  0000 C CNN
F 2 "" H 3850 4700 50  0001 C CNN
F 3 "" H 3850 4700 50  0001 C CNN
	1    3850 4700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR013
U 1 1 5AC5FCF9
P 3850 5500
F 0 "#PWR013" H 3850 5250 50  0001 C CNN
F 1 "GND" H 3850 5350 50  0000 C CNN
F 2 "" H 3850 5500 50  0001 C CNN
F 3 "" H 3850 5500 50  0001 C CNN
	1    3850 5500
	1    0    0    -1  
$EndComp
$Comp
L master-rescue:R R1
U 1 1 5AC5FDB3
P 3850 4900
F 0 "R1" V 3930 4900 50  0000 C CNN
F 1 "10K" V 3850 4900 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3780 4900 50  0001 C CNN
F 3 "" H 3850 4900 50  0001 C CNN
F 4 "RMCF0603FT10K0" H 3850 4900 60  0001 C CNN "Part Number"
	1    3850 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 5150 3850 5150
Wire Wire Line
	3850 5150 3850 5050
Wire Wire Line
	3850 4750 3850 4700
Connection ~ 3850 5150
Wire Wire Line
	3300 5250 3850 5250
Wire Wire Line
	3850 5250 3850 5500
$Comp
L master-rescue:Conn_01x02 J2
U 1 1 5AC56DF2
P 2800 3350
F 0 "J2" H 2800 3450 50  0000 C CNN
F 1 "MCV 1,5/ 2-G-3,5" H 2800 3150 50  0000 C CNN
F 2 "Connector_Phoenix_MC:PhoenixContact_MCV_1,5_2-G-3.5_1x02_P3.50mm_Vertical" H 2800 3350 50  0001 C CNN
F 3 "" H 2800 3350 50  0001 C CNN
F 4 "1843606" H 2800 3350 60  0001 C CNN "Part Number"
	1    2800 3350
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3250 3750 3000 3750
Wire Wire Line
	3150 3850 3000 3850
Text Notes 4000 4700 0    60   ~ 0
Pull-up required only \nif using isolated contact output
$Comp
L master-rescue:Conn_01x03 J4
U 1 1 5AC5ED3E
P 3100 5150
F 0 "J4" H 3100 5350 50  0000 C CNN
F 1 "MCV 1,5/ 3-G-3,5" H 3250 4950 50  0000 C CNN
F 2 "Connector_Phoenix_MC:PhoenixContact_MCV_1,5_3-G-3.5_1x03_P3.50mm_Vertical" H 3100 5150 50  0001 C CNN
F 3 "" H 3100 5150 50  0001 C CNN
F 4 "1843619" H 3100 5150 60  0001 C CNN "Part Number"
	1    3100 5150
	-1   0    0    -1  
$EndComp
$Comp
L power:+5V #PWR017
U 1 1 5AC5EE12
P 3450 4800
F 0 "#PWR017" H 3450 4650 50  0001 C CNN
F 1 "+5V" H 3450 4940 50  0000 C CNN
F 2 "" H 3450 4800 50  0001 C CNN
F 3 "" H 3450 4800 50  0001 C CNN
	1    3450 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 5050 3450 5050
Wire Wire Line
	3450 5050 3450 4800
$Comp
L power:+12V #PWR018
U 1 1 5AC5F8E5
P 4400 1650
F 0 "#PWR018" H 4400 1500 50  0001 C CNN
F 1 "+12V" H 4400 1790 50  0000 C CNN
F 2 "" H 4400 1650 50  0001 C CNN
F 3 "" H 4400 1650 50  0001 C CNN
	1    4400 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 1650 4400 1850
$Comp
L master-rescue:Conn_01x02 J3
U 1 1 5AC66E2D
P 2800 3750
F 0 "J3" H 2800 3850 50  0000 C CNN
F 1 "MCV 1,5/ 2-G-3,5" H 2800 3550 50  0000 C CNN
F 2 "Connector_Phoenix_MC:PhoenixContact_MCV_1,5_2-G-3.5_1x02_P3.50mm_Vertical" H 2800 3750 50  0001 C CNN
F 3 "" H 2800 3750 50  0001 C CNN
F 4 "1843606" H 2800 3750 60  0001 C CNN "Part Number"
	1    2800 3750
	-1   0    0    -1  
$EndComp
Text Notes 2200 5200 0    60   ~ 0
CoaXPress Trigger
$Comp
L master-rescue:Conn_01x02 J1
U 1 1 5AC680D1
P 4000 1850
F 0 "J1" H 4000 1950 50  0000 C CNN
F 1 "5566-02A" H 4000 1600 50  0000 C CNN
F 2 "Connector_Molex:Molex_Mini-Fit_Jr_5566-02A_2x01_P4.20mm_Vertical" H 4000 1850 50  0001 C CNN
F 3 "" H 4000 1850 50  0001 C CNN
F 4 "39-28-1023" H 4000 1850 60  0001 C CNN "Part Number"
	1    4000 1850
	-1   0    0    -1  
$EndComp
Text Notes 3600 1650 0    60   ~ 0
Alim 7-34V\n
Text Notes 2950 3150 2    60   ~ 0
2x CAN
$Sheet
S 5100 1750 1350 200 
U 5BF56E6D
F0 "12V to 5V LDO" 50
F1 "../../common/12V5VDCDC.sch" 50
F2 "VIN" I L 5100 1850 50 
F3 "5V" O R 6450 1850 50 
$EndSheet
$Sheet
S 5400 3250 1800 350 
U 5BFAC0A8
F0 "UC" 50
F1 "UC.sch" 50
F2 "TXCAN" O L 5400 3350 50 
F3 "RXCAN" I L 5400 3450 50 
F4 "DATA" O R 7200 3350 50 
$EndSheet
Wire Wire Line
	3000 3450 3150 3450
Wire Wire Line
	4400 1850 5100 1850
Connection ~ 4400 1850
Wire Wire Line
	6450 1850 6750 1850
Wire Wire Line
	6750 1850 6750 1650
$Comp
L power:+5V #PWR02
U 1 1 5BF539D0
P 6750 1650
F 0 "#PWR02" H 6750 1500 50  0001 C CNN
F 1 "+5V" H 6765 1823 50  0000 C CNN
F 2 "" H 6750 1650 50  0001 C CNN
F 3 "" H 6750 1650 50  0001 C CNN
	1    6750 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3000 3350 3250 3350
Wire Wire Line
	3150 3850 3150 3450
Connection ~ 3150 3450
Wire Wire Line
	3250 3750 3250 3350
Wire Wire Line
	3250 3350 3600 3350
Connection ~ 3250 3350
Wire Wire Line
	3150 3450 3600 3450
Wire Wire Line
	5150 3350 5400 3350
Wire Wire Line
	5150 3450 5400 3450
Wire Wire Line
	7200 3350 7850 3350
$Comp
L 74xGxx:74LVC1G17 U1
U 1 1 5BF56D21
P 5500 5150
F 0 "U1" H 5475 5417 50  0000 C CNN
F 1 "74LVC1G17" H 5475 5326 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23-5" H 5500 5150 50  0001 C CNN
F 3 "http://www.ti.com/lit/sg/scyt129e/scyt129e.pdf" H 5500 5150 50  0001 C CNN
F 4 "SN74LVC1G17DBVR" H 5500 5150 50  0001 C CNN "Part Number"
	1    5500 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	3850 5150 5200 5150
Wire Wire Line
	7600 3450 7850 3450
Wire Wire Line
	5750 5150 7600 5150
$Sheet
S 3600 3250 1550 300 
U 5BF9C366
F0 "CAN Transceiver" 50
F1 "../../common/CAN_transceiver.sch" 50
F2 "CAN+" B L 3600 3350 50 
F3 "CAN-" B L 3600 3450 50 
F4 "TXD" I R 5150 3350 50 
F5 "RXD" O R 5150 3450 50 
$EndSheet
Text Notes 550  7750 0    50   ~ 0
© 2018 Université de Lausanne\n\nThis documentation describes Open Hardware and is licensed under the\nCERN OHL v. 1.2.\n\nYou may redistribute and modify this documentation under the terms of\nthe CERN OHL v.1.2. (http://ohwr.org/cernohl). This documentation is\ndistributed WITHOUT ANY EXPRESS OR IMPLIED WARRANTY,\nINCLUDING OF MERCHANTABILITY, SATISFACTORY QUALITY AND\nFITNESS FOR A PARTICULAR PURPOSE. Please see the CERN OHL v.1.2\nfor applicable conditions
$Comp
L power:VCC #PWR03
U 1 1 5BF64521
P 7000 1650
F 0 "#PWR03" H 7000 1500 50  0001 C CNN
F 1 "VCC" H 7017 1823 50  0000 C CNN
F 2 "" H 7000 1650 50  0001 C CNN
F 3 "" H 7000 1650 50  0001 C CNN
	1    7000 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	6750 1850 7000 1850
Wire Wire Line
	7000 1850 7000 1650
Connection ~ 6750 1850
Wire Wire Line
	7600 3450 7600 5150
Text Label 4600 5150 0    50   ~ 0
TRIG_w_NOISE
$EndSCHEMATC
