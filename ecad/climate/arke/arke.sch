EESchema Schematic File Version 4
LIBS:arke-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 5
Title "Arke - RS232 <-> CAN Isolated Interface"
Date "06/11/2018"
Rev "B"
Comp "Université de Lausanne"
Comment1 "Author: Alexandre Tuleu"
Comment2 "Licensed under the CERN OHL v1.2"
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 3450 3950 1050 500 
U 5BA10795
F0 "RS232 Isolator" 60
F1 "rs232_isolator.sch" 60
F2 "ISO_RX" I L 3450 4100 60 
F3 "ISO_TX" O L 3450 4200 60 
F4 "RX" O R 4500 4150 60 
F5 "TX" I R 4500 4250 60 
F6 "GND_ISO" B L 3450 4300 60 
$EndSheet
$Sheet
S 4950 4000 1350 400 
U 5BA1079B
F0 "UC" 60
F1 "UC.sch" 60
F2 "CAN_TX" O R 6300 4150 60 
F3 "CAN_RX" I R 6300 4250 60 
F4 "UART_TX" O L 4950 4250 60 
F5 "UART_RX" I L 4950 4150 60 
$EndSheet
$Sheet
S 6850 4000 1050 400 
U 5BA1079E
F0 "CAN Transceiver" 60
F1 "../../common/CAN_transceiver.sch" 60
F2 "CAN+" B R 7900 4150 60 
F3 "CAN-" B R 7900 4250 60 
F4 "TXD" I L 6850 4150 60 
F5 "RXD" O L 6850 4250 60 
$EndSheet
$Sheet
S 6400 3150 1650 350 
U 5BA1080F
F0 "12V 5V Buck Converter" 60
F1 "../../common/12V5VDCDC.sch" 60
F2 "VIN" I L 6400 3300 60 
F3 "5V" O R 8050 3300 60 
$EndSheet
Wire Wire Line
	6300 4150 6850 4150
Wire Wire Line
	6300 4250 6850 4250
Wire Wire Line
	4500 4150 4950 4150
Wire Wire Line
	4500 4250 4950 4250
Wire Wire Line
	2050 4250 2800 4250
Wire Wire Line
	2800 4200 3450 4200
Wire Wire Line
	2050 4050 2800 4050
Wire Wire Line
	2800 4100 3450 4100
NoConn ~ 2050 3750
Wire Wire Line
	2050 4150 2250 4150
Wire Wire Line
	2250 4150 2250 3950
Wire Wire Line
	2250 3950 2050 3950
Wire Wire Line
	2050 3850 2350 3850
Wire Wire Line
	2350 3850 2350 4350
Wire Wire Line
	2350 4350 2050 4350
Wire Wire Line
	2350 4450 2050 4450
Connection ~ 2350 4350
Text Notes 3050 4850 2    60   ~ 0
DCE Connection with Loopback handshaking
Wire Wire Line
	2800 4050 2800 4100
Wire Wire Line
	2800 4250 2800 4200
$Comp
L power:+5V #PWR01
U 1 1 5BA2BA69
P 8400 3250
F 0 "#PWR01" H 8400 3100 50  0001 C CNN
F 1 "+5V" H 8400 3390 50  0000 C CNN
F 2 "" H 8400 3250 50  0001 C CNN
F 3 "" H 8400 3250 50  0001 C CNN
	1    8400 3250
	-1   0    0    -1  
$EndComp
Wire Wire Line
	8400 3250 8400 3300
Wire Wire Line
	8400 3300 8050 3300
$Comp
L power:+12V #PWR02
U 1 1 5BA2BB13
P 5950 3200
F 0 "#PWR02" H 5950 3050 50  0001 C CNN
F 1 "+12V" H 5950 3340 50  0000 C CNN
F 2 "" H 5950 3200 50  0001 C CNN
F 3 "" H 5950 3200 50  0001 C CNN
	1    5950 3200
	-1   0    0    -1  
$EndComp
Wire Wire Line
	5950 3200 5950 3300
Wire Wire Line
	5950 3300 6400 3300
$Comp
L Connector_Generic:Conn_01x02 J1
U 1 1 5BA2BBBF
P 9900 3250
F 0 "J1" H 9900 3350 50  0000 C CNN
F 1 "5566-02A" H 9900 3050 50  0000 C CNN
F 2 "FORT:Molex_Mini-Fit_Jr_5566-02A_2x01_P4.20mm_Vertical" H 9900 3250 50  0001 C CNN
F 3 "" H 9900 3250 50  0001 C CNN
F 4 "39-28-1023" H 9900 3250 60  0001 C CNN "Part Number"
	1    9900 3250
	1    0    0    -1  
$EndComp
$Comp
L power:+12V #PWR03
U 1 1 5BA2C21A
P 9550 3100
F 0 "#PWR03" H 9550 2950 50  0001 C CNN
F 1 "+12V" H 9550 3240 50  0000 C CNN
F 2 "" H 9550 3100 50  0001 C CNN
F 3 "" H 9550 3100 50  0001 C CNN
	1    9550 3100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR04
U 1 1 5BA2C362
P 9550 3500
F 0 "#PWR04" H 9550 3250 50  0001 C CNN
F 1 "GND" H 9550 3350 50  0000 C CNN
F 2 "" H 9550 3500 50  0001 C CNN
F 3 "" H 9550 3500 50  0001 C CNN
	1    9550 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	9550 3500 9550 3350
Wire Wire Line
	9550 3350 9700 3350
Wire Wire Line
	9550 3100 9550 3250
Wire Wire Line
	9550 3250 9700 3250
Wire Wire Line
	7900 4150 9500 4150
Wire Wire Line
	9500 4150 9500 4550
Wire Wire Line
	9500 4550 9700 4550
Wire Wire Line
	7900 4250 9600 4250
Wire Wire Line
	9600 4250 9600 4650
Wire Wire Line
	9600 4650 9700 4650
Connection ~ 9500 4150
Connection ~ 9600 4250
Wire Wire Line
	3450 4300 3100 4300
Wire Wire Line
	3100 4300 3100 3650
Wire Wire Line
	3100 3650 2050 3650
$Comp
L Connector_Generic:Conn_01x02 J3
U 1 1 5BA38B3F
P 9900 4150
F 0 "J3" H 9900 4250 50  0000 C CNN
F 1 "MCV 1,5/ 2-G-3,5" H 9900 3950 50  0000 C CNN
F 2 "Connector_Phoenix_MC:PhoenixContact_MCV_1,5_2-G-3.5_1x02_P3.50mm_Vertical" H 9900 4150 50  0001 C CNN
F 3 "" H 9900 4150 50  0001 C CNN
F 4 "MCV 1,5/ 2-G-3,5" H 9900 4150 60  0001 C CNN "Part Number"
	1    9900 4150
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J5
U 1 1 5BA38BE4
P 9900 4550
F 0 "J5" H 9900 4650 50  0000 C CNN
F 1 "MCV 1,5/ 2-G-3,5" H 9900 4350 50  0000 C CNN
F 2 "Connector_Phoenix_MC:PhoenixContact_MCV_1,5_2-G-3.5_1x02_P3.50mm_Vertical" H 9900 4550 50  0001 C CNN
F 3 "" H 9900 4550 50  0001 C CNN
F 4 "MCV 1,5/ 2-G-3,5" H 9900 4550 60  0001 C CNN "Part Number"
	1    9900 4550
	1    0    0    -1  
$EndComp
$Comp
L Connector:DB9_Female J2
U 1 1 5BA3A8D5
P 1750 4050
F 0 "J2" H 1750 4600 50  0000 C CNN
F 1 "DB9_Female" H 1750 3475 50  0000 C CNN
F 2 "FORT:LD09S13A4GX00LF" H 1750 4050 50  0001 C CNN
F 3 "" H 1750 4050 50  0001 C CNN
F 4 "LD09S13A4GX00LF" H 1750 4050 60  0001 C CNN "Part Number"
	1    1750 4050
	-1   0    0    1   
$EndComp
Wire Wire Line
	2350 4350 2350 4450
Wire Wire Line
	9500 4150 9700 4150
Wire Wire Line
	9600 4250 9700 4250
Text Notes 550  7750 0    50   ~ 0
© 2018 Université de Lausanne\n\nThis documentation describes Open Hardware and is licensed under the\nCERN OHL v. 1.2.\n\nYou may redistribute and modify this documentation under the terms of\nthe CERN OHL v.1.2. (http://ohwr.org/cernohl). This documentation is\ndistributed WITHOUT ANY EXPRESS OR IMPLIED WARRANTY,\nINCLUDING OF MERCHANTABILITY, SATISFACTORY QUALITY AND\nFITNESS FOR A PARTICULAR PURPOSE. Please see the CERN OHL v.1.2\nfor applicable conditions
$EndSCHEMATC
