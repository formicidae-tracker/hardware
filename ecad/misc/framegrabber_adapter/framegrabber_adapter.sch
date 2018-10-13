EESchema Schematic File Version 4
LIBS:framegrabber_adapter-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
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
L Connector_Generic:Conn_02x13_Odd_Even J1
U 1 1 5BC1DAAA
P 5750 2900
F 0 "J1" H 5800 3717 50  0000 C CNN
F 1 "IDC 26" H 5800 3626 50  0000 C CNN
F 2 "Connector_IDC:IDC-Header_2x13_P2.54mm_Vertical" H 5750 2900 50  0001 C CNN
F 3 "~" H 5750 2900 50  0001 C CNN
	1    5750 2900
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J2
U 1 1 5BC1DBEB
P 3650 3000
F 0 "J2" H 3730 3042 50  0000 L CNN
F 1 "MC 1,5/3-G-3.5" H 3730 2951 50  0000 L CNN
F 2 "Connector_Phoenix_MC:PhoenixContact_MC_1,5_3-G-3.5_1x03_P3.50mm_Horizontal" H 3650 3000 50  0001 C CNN
F 3 "~" H 3650 3000 50  0001 C CNN
F 4 "1844223" H 3650 3000 50  0001 C CNN "Part Number"
	1    3650 3000
	-1   0    0    -1  
$EndComp
NoConn ~ 5550 2300
NoConn ~ 5550 2400
NoConn ~ 5550 2500
NoConn ~ 5550 2600
NoConn ~ 5550 2700
NoConn ~ 5550 2800
NoConn ~ 5550 2900
NoConn ~ 5550 3100
NoConn ~ 5550 3200
NoConn ~ 5550 3300
NoConn ~ 5550 3400
NoConn ~ 5550 3500
NoConn ~ 6050 3500
NoConn ~ 6050 3400
NoConn ~ 6050 3300
NoConn ~ 6050 3200
NoConn ~ 6050 3100
NoConn ~ 6050 2900
NoConn ~ 6050 2800
NoConn ~ 6050 2700
NoConn ~ 6050 2600
NoConn ~ 6050 2500
NoConn ~ 6050 2400
NoConn ~ 6050 2300
$Comp
L power:GND #PWR0101
U 1 1 5BC1E548
P 6400 3100
F 0 "#PWR0101" H 6400 2850 50  0001 C CNN
F 1 "GND" H 6405 2927 50  0000 C CNN
F 2 "" H 6400 3100 50  0001 C CNN
F 3 "" H 6400 3100 50  0001 C CNN
	1    6400 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	6050 3000 6400 3000
Wire Wire Line
	6400 3000 6400 3100
$Comp
L power:GND #PWR0102
U 1 1 5BC1E66A
P 4050 3200
F 0 "#PWR0102" H 4050 2950 50  0001 C CNN
F 1 "GND" H 4055 3027 50  0000 C CNN
F 2 "" H 4050 3200 50  0001 C CNN
F 3 "" H 4050 3200 50  0001 C CNN
	1    4050 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	3850 3100 4050 3100
Wire Wire Line
	4050 3100 4050 3200
Wire Wire Line
	3850 3000 5550 3000
Text Label 4450 3000 0    50   ~ 0
TRIGGER
NoConn ~ 3850 2900
$EndSCHEMATC
