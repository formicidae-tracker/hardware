EESchema Schematic File Version 4
LIBS:celeano-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 11 8
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
L Relay:FINDER-32.21-x000 K?
U 1 1 5BB41828
P 5850 3450
AR Path="/5BB4177A/5BB41828" Ref="K?"  Part="1" 
AR Path="/5BB53554/5BB41828" Ref="K?"  Part="1" 
AR Path="/5BB4D604/5BB41828" Ref="K1"  Part="1" 
F 0 "K1" H 6280 3496 50  0000 L CNN
F 1 "FINDER-32.21-x000" H 6280 3405 50  0000 L CNN
F 2 "Relay_THT:Relay_SPST_Finder_32.21-x300" H 7120 3420 50  0001 C CNN
F 3 "https://gfinder.findernet.com/assets/Series/355/S32EN.pdf" H 5850 3450 50  0001 C CNN
F 4 "32.21.7.005.4000" H 5850 3450 50  0001 C CNN "Part Number"
	1    5850 3450
	1    0    0    -1  
$EndComp
$Comp
L Device:D D?
U 1 1 5BB41B1C
P 5200 3400
AR Path="/5BBA23F4/5BB41B1C" Ref="D?"  Part="1" 
AR Path="/5BB4177A/5BB41B1C" Ref="D?"  Part="1" 
AR Path="/5BB53554/5BB41B1C" Ref="D?"  Part="1" 
AR Path="/5BB4D604/5BB41B1C" Ref="D2"  Part="1" 
F 0 "D2" H 5200 3500 50  0000 C CNN
F 1 "MCL4148" H 5200 3300 50  0000 C CNN
F 2 "Diode_SMD:D_MicroMELF" H 5200 3400 50  0001 C CNN
F 3 "" H 5200 3400 50  0001 C CNN
F 4 "MCL4148-TR" H 5200 3400 60  0001 C CNN "Part Number"
	1    5200 3400
	0    1    1    0   
$EndComp
$Comp
L Transistor_FET:BSS138 Q?
U 1 1 5BB44FFE
P 5550 4200
AR Path="/5BB4177A/5BB44FFE" Ref="Q?"  Part="1" 
AR Path="/5BB53554/5BB44FFE" Ref="Q?"  Part="1" 
AR Path="/5BB4D604/5BB44FFE" Ref="Q4"  Part="1" 
F 0 "Q4" H 5755 4246 50  0000 L CNN
F 1 "BSS138" H 5755 4155 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 5750 4125 50  0001 L CIN
F 3 "https://www.fairchildsemi.com/datasheets/BS/BSS138.pdf" H 5550 4200 50  0001 L CNN
F 4 "BSS138" H 5550 4200 50  0001 C CNN "Part Number"
	1    5550 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5650 4000 5650 3900
Wire Wire Line
	5200 3550 5200 3900
Wire Wire Line
	5200 3900 5650 3900
Connection ~ 5650 3900
Wire Wire Line
	5650 3900 5650 3750
$Comp
L power:+5V #PWR?
U 1 1 5BB45131
P 5650 2750
AR Path="/5BB4177A/5BB45131" Ref="#PWR?"  Part="1" 
AR Path="/5BB53554/5BB45131" Ref="#PWR?"  Part="1" 
AR Path="/5BB4D604/5BB45131" Ref="#PWR0101"  Part="1" 
F 0 "#PWR0101" H 5650 2600 50  0001 C CNN
F 1 "+5V" H 5665 2923 50  0000 C CNN
F 2 "" H 5650 2750 50  0001 C CNN
F 3 "" H 5650 2750 50  0001 C CNN
	1    5650 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 3250 5200 3000
Wire Wire Line
	5200 3000 5650 3000
Connection ~ 5650 3000
Wire Wire Line
	5650 3000 5650 3150
Wire Wire Line
	5650 2750 5650 3000
$Comp
L Device:R R?
U 1 1 5BB45339
P 5200 4550
AR Path="/5BAB6A7C/5BB45339" Ref="R?"  Part="1" 
AR Path="/5BBA23F4/5BB45339" Ref="R?"  Part="1" 
AR Path="/5BB4177A/5BB45339" Ref="R?"  Part="1" 
AR Path="/5BB53554/5BB45339" Ref="R?"  Part="1" 
AR Path="/5BB4D604/5BB45339" Ref="R13"  Part="1" 
F 0 "R13" V 5280 4550 50  0000 C CNN
F 1 "47k5" V 5200 4550 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 5130 4550 50  0001 C CNN
F 3 "" H 5200 4550 50  0001 C CNN
F 4 "GPR060347k5" H 5200 4550 60  0001 C CNN "Part Number"
	1    5200 4550
	-1   0    0    1   
$EndComp
Wire Wire Line
	5200 4400 5200 4200
Wire Wire Line
	5200 4200 5350 4200
Wire Wire Line
	5200 4700 5200 4800
Wire Wire Line
	5200 4800 5650 4800
Wire Wire Line
	5650 4800 5650 4400
$Comp
L power:GND #PWR?
U 1 1 5BB45528
P 5650 4950
AR Path="/5BB4177A/5BB45528" Ref="#PWR?"  Part="1" 
AR Path="/5BB53554/5BB45528" Ref="#PWR?"  Part="1" 
AR Path="/5BB4D604/5BB45528" Ref="#PWR0102"  Part="1" 
F 0 "#PWR0102" H 5650 4700 50  0001 C CNN
F 1 "GND" H 5655 4777 50  0000 C CNN
F 2 "" H 5650 4950 50  0001 C CNN
F 3 "" H 5650 4950 50  0001 C CNN
	1    5650 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	5650 4800 5650 4950
Connection ~ 5650 4800
Connection ~ 5200 4200
NoConn ~ 5950 3150
Text HLabel 4400 4200 0    50   Input ~ 0
CMD
Wire Wire Line
	4400 4200 5200 4200
Text HLabel 6500 2950 2    50   BiDi ~ 0
A
Text HLabel 6500 3900 2    50   BiDi ~ 0
B
Wire Wire Line
	6500 3900 6050 3900
Wire Wire Line
	6050 3900 6050 3750
Wire Wire Line
	6150 3150 6150 2950
Wire Wire Line
	6150 2950 6500 2950
$EndSCHEMATC
