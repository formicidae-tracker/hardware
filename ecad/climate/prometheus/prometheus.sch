EESchema Schematic File Version 4
LIBS:prometheus-cache
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
L Connector:Barrel_Jack J1
U 1 1 5BFFABF8
P 4050 3900
F 0 "J1" H 4105 4225 50  0000 C CNN
F 1 "2.5mm I.D. 5.5mm O.D." H 4105 4134 50  0000 C CNN
F 2 "FORT:BarrelJack_CUI_PJ-037B_Horizontal" H 4100 3860 50  0001 C CNN
F 3 "~" H 4100 3860 50  0001 C CNN
F 4 "PJ037-B" H 4050 3900 50  0001 C CNN "Part Number"
	1    4050 3900
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J2
U 1 1 5BFFAD92
P 6600 3400
F 0 "J2" H 6680 3392 50  0000 L CNN
F 1 "5566-02A" H 6680 3301 50  0000 L CNN
F 2 "Connector_Molex:Molex_Mini-Fit_Jr_5566-02A_2x01_P4.20mm_Vertical" H 6600 3400 50  0001 C CNN
F 3 "~" H 6600 3400 50  0001 C CNN
F 4 "39-28-1023" H 6600 3400 50  0001 C CNN "Part Number"
	1    6600 3400
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J3
U 1 1 5C004389
P 6600 3650
F 0 "J3" H 6680 3642 50  0000 L CNN
F 1 "5566-02A" H 6680 3551 50  0000 L CNN
F 2 "Connector_Molex:Molex_Mini-Fit_Jr_5566-02A_2x01_P4.20mm_Vertical" H 6600 3650 50  0001 C CNN
F 3 "~" H 6600 3650 50  0001 C CNN
F 4 "39-28-1023" H 6600 3650 50  0001 C CNN "Part Number"
	1    6600 3650
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J4
U 1 1 5C0043A7
P 6600 3900
F 0 "J4" H 6680 3892 50  0000 L CNN
F 1 "5566-02A" H 6680 3801 50  0000 L CNN
F 2 "Connector_Molex:Molex_Mini-Fit_Jr_5566-02A_2x01_P4.20mm_Vertical" H 6600 3900 50  0001 C CNN
F 3 "~" H 6600 3900 50  0001 C CNN
F 4 "39-28-1023" H 6600 3900 50  0001 C CNN "Part Number"
	1    6600 3900
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J5
U 1 1 5C0043C9
P 6600 4150
F 0 "J5" H 6680 4142 50  0000 L CNN
F 1 "5566-02A" H 6680 4051 50  0000 L CNN
F 2 "Connector_Molex:Molex_Mini-Fit_Jr_5566-02A_2x01_P4.20mm_Vertical" H 6600 4150 50  0001 C CNN
F 3 "~" H 6600 4150 50  0001 C CNN
F 4 "39-28-1023" H 6600 4150 50  0001 C CNN "Part Number"
	1    6600 4150
	1    0    0    -1  
$EndComp
$Comp
L power:+12V #PWR03
U 1 1 5C00475B
P 4700 3450
F 0 "#PWR03" H 4700 3300 50  0001 C CNN
F 1 "+12V" H 4715 3623 50  0000 C CNN
F 2 "" H 4700 3450 50  0001 C CNN
F 3 "" H 4700 3450 50  0001 C CNN
	1    4700 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	4350 3800 4700 3800
Wire Wire Line
	4700 3800 4700 3450
$Comp
L power:GND #PWR04
U 1 1 5C0047D2
P 4700 4350
F 0 "#PWR04" H 4700 4100 50  0001 C CNN
F 1 "GND" H 4705 4177 50  0000 C CNN
F 2 "" H 4700 4350 50  0001 C CNN
F 3 "" H 4700 4350 50  0001 C CNN
	1    4700 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	4350 4000 4700 4000
Wire Wire Line
	4700 4000 4700 4350
$Comp
L power:+12V #PWR01
U 1 1 5C0047FB
P 6200 3050
F 0 "#PWR01" H 6200 2900 50  0001 C CNN
F 1 "+12V" H 6215 3223 50  0000 C CNN
F 2 "" H 6200 3050 50  0001 C CNN
F 3 "" H 6200 3050 50  0001 C CNN
	1    6200 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	6200 3050 6200 3400
Wire Wire Line
	6200 3400 6400 3400
Wire Wire Line
	6200 3400 6200 3650
Wire Wire Line
	6200 3650 6400 3650
Connection ~ 6200 3400
Wire Wire Line
	6200 3650 6200 3900
Wire Wire Line
	6200 3900 6400 3900
Connection ~ 6200 3650
Wire Wire Line
	6200 4400 6400 4400
$Comp
L power:GND #PWR06
U 1 1 5C00493A
P 6300 4800
F 0 "#PWR06" H 6300 4550 50  0001 C CNN
F 1 "GND" H 6305 4627 50  0000 C CNN
F 2 "" H 6300 4800 50  0001 C CNN
F 3 "" H 6300 4800 50  0001 C CNN
	1    6300 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	6300 4800 6300 4500
Wire Wire Line
	6300 4500 6400 4500
Wire Wire Line
	6300 4500 6300 4250
Wire Wire Line
	6300 4000 6400 4000
Connection ~ 6300 4500
Wire Wire Line
	6300 4000 6300 3750
Wire Wire Line
	6300 3750 6400 3750
Wire Wire Line
	6300 3750 6300 3500
Wire Wire Line
	6300 3500 6400 3500
Connection ~ 6300 3750
$Comp
L Device:R R1
U 1 1 5C004EA8
P 5450 3650
F 0 "R1" H 5520 3696 50  0000 L CNN
F 1 "3k3" H 5520 3605 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 5380 3650 50  0001 C CNN
F 3 "~" H 5450 3650 50  0001 C CNN
F 4 "GPR06033K30" H 5450 3650 50  0001 C CNN "Part Number"
	1    5450 3650
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D1
U 1 1 5C005002
P 5450 4150
F 0 "D1" V 5488 4032 50  0000 R CNN
F 1 "GREEN" V 5397 4032 50  0000 R CNN
F 2 "LED_SMD:LED_0603_1608Metric" H 5450 4150 50  0001 C CNN
F 3 "~" H 5450 4150 50  0001 C CNN
F 4 "150060VS75000" H 5450 4150 50  0001 C CNN "Part Number"
	1    5450 4150
	0    -1   -1   0   
$EndComp
$Comp
L power:+12V #PWR02
U 1 1 5C0055E5
P 5450 3350
F 0 "#PWR02" H 5450 3200 50  0001 C CNN
F 1 "+12V" H 5465 3523 50  0000 C CNN
F 2 "" H 5450 3350 50  0001 C CNN
F 3 "" H 5450 3350 50  0001 C CNN
	1    5450 3350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR05
U 1 1 5C0055FE
P 5450 4450
F 0 "#PWR05" H 5450 4200 50  0001 C CNN
F 1 "GND" H 5455 4277 50  0000 C CNN
F 2 "" H 5450 4450 50  0001 C CNN
F 3 "" H 5450 4450 50  0001 C CNN
	1    5450 4450
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 4450 5450 4300
Wire Wire Line
	5450 4000 5450 3800
Wire Wire Line
	5450 3500 5450 3350
$Comp
L Mechanical:MountingHole MH1
U 1 1 5C005BA4
P 7700 3550
F 0 "MH1" H 7800 3596 50  0000 L CNN
F 1 "MountingHole" H 7800 3505 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965" H 7700 3550 50  0001 C CNN
F 3 "~" H 7700 3550 50  0001 C CNN
	1    7700 3550
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole MH2
U 1 1 5C005CD8
P 7700 3750
F 0 "MH2" H 7800 3796 50  0000 L CNN
F 1 "MountingHole" H 7800 3705 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965" H 7700 3750 50  0001 C CNN
F 3 "~" H 7700 3750 50  0001 C CNN
	1    7700 3750
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole MH3
U 1 1 5C005D0A
P 7700 3950
F 0 "MH3" H 7800 3996 50  0000 L CNN
F 1 "MountingHole" H 7800 3905 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965" H 7700 3950 50  0001 C CNN
F 3 "~" H 7700 3950 50  0001 C CNN
	1    7700 3950
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole MH4
U 1 1 5C005D3E
P 7700 4150
F 0 "MH4" H 7800 4196 50  0000 L CNN
F 1 "MountingHole" H 7800 4105 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965" H 7700 4150 50  0001 C CNN
F 3 "~" H 7700 4150 50  0001 C CNN
	1    7700 4150
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J6
U 1 1 5C019117
P 6600 4400
F 0 "J6" H 6680 4392 50  0000 L CNN
F 1 "5566-02A" H 6680 4301 50  0000 L CNN
F 2 "Connector_Molex:Molex_Mini-Fit_Jr_5566-02A_2x01_P4.20mm_Vertical" H 6600 4400 50  0001 C CNN
F 3 "~" H 6600 4400 50  0001 C CNN
F 4 "39-28-1023" H 6600 4400 50  0001 C CNN "Part Number"
	1    6600 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6300 4250 6400 4250
Wire Wire Line
	6300 4250 6300 4000
Connection ~ 6300 4250
Connection ~ 6300 4000
Wire Wire Line
	6200 3900 6200 4150
Connection ~ 6200 3900
Wire Wire Line
	6200 4150 6400 4150
Connection ~ 6200 4150
Wire Wire Line
	6200 4150 6200 4400
$EndSCHEMATC