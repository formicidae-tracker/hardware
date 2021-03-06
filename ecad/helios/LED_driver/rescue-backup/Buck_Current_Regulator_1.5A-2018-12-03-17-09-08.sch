EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:switches
LIBS:relays
LIBS:motors
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:fort
LIBS:LED_driver-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 6
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
L TPS92512 U2
U 1 1 5AA2AC23
P 6350 2550
AR Path="/5AA2AB58/5AA2AC23" Ref="U2"  Part="1" 
AR Path="/5AA6DCFC/5AA2AC23" Ref="U3"  Part="1" 
F 0 "U2" H 6100 3150 60  0000 C CNN
F 1 "TPS92512" H 6250 2000 60  0000 C CNN
F 2 "FORT:PDSO-10-1EP-3x3mm-Pitch0.5mm" H 6150 2550 60  0001 C CNN
F 3 "" H 6150 2550 60  0001 C CNN
F 4 "TPS92512DGQR" H 6350 2550 60  0001 C CNN "Part Number"
	1    6350 2550
	1    0    0    -1  
$EndComp
Text Notes 6150 4200 0    60   ~ 0
Operating Frequency : 440kHz\nLoad : 6x SFH4235 3V 1.5A, in 5ms/30ms max pulses\n\n
Text HLabel 9550 2250 2    60   Output ~ 0
LED+
Text HLabel 9550 3000 2    60   Input ~ 0
LED-
$Comp
L C C8
U 1 1 5AA7001C
P 2100 2550
AR Path="/5AA2AB58/5AA7001C" Ref="C8"  Part="1" 
AR Path="/5AA6DCFC/5AA7001C" Ref="C18"  Part="1" 
F 0 "C8" H 2125 2650 50  0000 L CNN
F 1 "4.7uF,50V" H 2125 2450 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206" H 2138 2400 50  0001 C CNN
F 3 "" H 2100 2550 50  0001 C CNN
F 4 "12065C475KAT2A" H 2100 2550 60  0001 C CNN "Part Number"
	1    2100 2550
	1    0    0    -1  
$EndComp
$Comp
L L L1
U 1 1 5AA2B956
P 8000 2250
AR Path="/5AA2AB58/5AA2B956" Ref="L1"  Part="1" 
AR Path="/5AA6DCFC/5AA2B956" Ref="L2"  Part="1" 
F 0 "L1" V 8100 2250 50  0000 C CNN
F 1 "100uH, 2.5A" V 7950 2250 50  0000 C CNN
F 2 "FORT:7447709101" H 8000 2250 50  0001 C CNN
F 3 "" H 8000 2250 50  0001 C CNN
F 4 "7447709101" H 8000 2250 60  0001 C CNN "Part Number"
	1    8000 2250
	0    -1   -1   0   
$EndComp
$Comp
L D_Schottky D2
U 1 1 5AA7001E
P 7800 2700
AR Path="/5AA2AB58/5AA7001E" Ref="D2"  Part="1" 
AR Path="/5AA6DCFC/5AA7001E" Ref="D4"  Part="1" 
F 0 "D2" H 7800 2800 50  0000 C CNN
F 1 "60V, 2A" H 7800 2600 50  0000 C CNN
F 2 "Diodes_SMD:D_SMB" H 7800 2700 50  0001 C CNN
F 3 "" H 7800 2700 50  0001 C CNN
F 4 "B260-13-F" H 7800 2700 60  0001 C CNN "Part Number"
	1    7800 2700
	0    1    1    0   
$EndComp
$Comp
L D_Schottky D1
U 1 1 5AA7001F
P 7100 1600
AR Path="/5AA2AB58/5AA7001F" Ref="D1"  Part="1" 
AR Path="/5AA6DCFC/5AA7001F" Ref="D3"  Part="1" 
F 0 "D1" H 7100 1700 50  0000 C CNN
F 1 "60V, 2A" H 7100 1500 50  0000 C CNN
F 2 "Diodes_SMD:D_SMB" H 7100 1600 50  0001 C CNN
F 3 "" H 7100 1600 50  0001 C CNN
F 4 "B260-13-F" H 7100 1600 60  0001 C CNN "Part Number"
	1    7100 1600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR023
U 1 1 5AA70020
P 8400 3750
AR Path="/5AA2AB58/5AA70020" Ref="#PWR023"  Part="1" 
AR Path="/5AA6DCFC/5AA70020" Ref="#PWR025"  Part="1" 
F 0 "#PWR023" H 8400 3500 50  0001 C CNN
F 1 "GND" H 8400 3600 50  0000 C CNN
F 2 "" H 8400 3750 50  0001 C CNN
F 3 "" H 8400 3750 50  0001 C CNN
	1    8400 3750
	1    0    0    -1  
$EndComp
$Comp
L R R9
U 1 1 5AA70021
P 7200 2600
AR Path="/5AA2AB58/5AA70021" Ref="R9"  Part="1" 
AR Path="/5AA6DCFC/5AA70021" Ref="R16"  Part="1" 
F 0 "R9" V 7280 2600 50  0000 C CNN
F 1 "1K" V 7200 2600 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 7130 2600 50  0001 C CNN
F 3 "" H 7200 2600 50  0001 C CNN
F 4 "RMCF0603FT1K00" H 7200 2600 60  0001 C CNN "Part Number"
	1    7200 2600
	0    1    1    0   
$EndComp
$Comp
L R R13
U 1 1 5AA70022
P 8400 3350
AR Path="/5AA2AB58/5AA70022" Ref="R13"  Part="1" 
AR Path="/5AA6DCFC/5AA70022" Ref="R20"  Part="1" 
F 0 "R13" V 8480 3350 50  0000 C CNN
F 1 "0.2, 1%, 0.5W" H 8700 3200 50  0000 C CNN
F 2 "Resistors_SMD:R_1206" V 8330 3350 50  0001 C CNN
F 3 "" H 8400 3350 50  0001 C CNN
F 4 "CSR1206FKR200" H 8400 3350 60  0001 C CNN "Part Number"
	1    8400 3350
	1    0    0    -1  
$EndComp
Text HLabel 1500 2050 0    60   Input ~ 0
VIN
Text HLabel 5250 2400 0    60   Input ~ 0
PDIM
$Comp
L R R8
U 1 1 5AA70023
P 5500 2250
AR Path="/5AA2AB58/5AA70023" Ref="R8"  Part="1" 
AR Path="/5AA6DCFC/5AA70023" Ref="R15"  Part="1" 
F 0 "R8" V 5580 2250 50  0000 C CNN
F 1 "10M" V 5500 2250 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 5430 2250 50  0001 C CNN
F 3 "" H 5500 2250 50  0001 C CNN
F 4 "RMCF0603FT10M0" H 5500 2250 60  0001 C CNN "Part Number"
	1    5500 2250
	0    1    1    0   
$EndComp
Wire Wire Line
	6900 2850 7350 2850
Wire Wire Line
	7350 2850 7350 3600
Wire Wire Line
	6900 2950 7350 2950
Connection ~ 7350 2950
Wire Wire Line
	6900 2050 7100 2050
Wire Wire Line
	7400 2050 7700 2050
Wire Wire Line
	7700 2050 7700 2250
Connection ~ 7700 2250
Wire Wire Line
	6900 2600 7050 2600
Wire Wire Line
	6900 2250 7850 2250
Wire Wire Line
	8150 2250 9550 2250
Wire Wire Line
	8400 1600 8400 2500
Connection ~ 8400 2250
Wire Wire Line
	8400 3200 8400 2800
Connection ~ 8400 3000
Wire Wire Line
	8400 3750 8400 3500
Wire Wire Line
	1500 2050 5800 2050
Wire Wire Line
	5550 1600 6950 1600
Wire Wire Line
	7350 3600 8400 3600
Connection ~ 8400 3600
Wire Wire Line
	7550 3000 9550 3000
Wire Wire Line
	7800 2550 7800 2250
Connection ~ 7800 2250
Wire Wire Line
	7800 2850 7800 3600
Connection ~ 7800 3600
Wire Wire Line
	7350 2600 7550 2600
Wire Wire Line
	7550 2600 7550 3000
Wire Wire Line
	7250 1600 8400 1600
Wire Wire Line
	5250 2400 5800 2400
Wire Wire Line
	5650 2250 5800 2250
Wire Wire Line
	5200 2050 5200 2250
Wire Wire Line
	5200 2250 5350 2250
Wire Wire Line
	5550 1600 5550 2050
Connection ~ 5550 2050
$Comp
L R R7
U 1 1 5AA70024
P 4450 2250
AR Path="/5AA2AB58/5AA70024" Ref="R7"  Part="1" 
AR Path="/5AA6DCFC/5AA70024" Ref="R14"  Part="1" 
F 0 "R7" V 4530 2250 50  0000 C CNN
F 1 "1.96M" V 4450 2250 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4380 2250 50  0001 C CNN
F 3 "" H 4450 2250 50  0001 C CNN
F 4 "RMCF0603FT1M96" H 4450 2250 60  0001 C CNN "Part Number"
	1    4450 2250
	1    0    0    -1  
$EndComp
$Comp
L R R10
U 1 1 5AA70025
P 4450 2750
AR Path="/5AA2AB58/5AA70025" Ref="R10"  Part="1" 
AR Path="/5AA6DCFC/5AA70025" Ref="R17"  Part="1" 
F 0 "R10" V 4530 2750 50  0000 C CNN
F 1 "66.5k" V 4450 2750 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4380 2750 50  0001 C CNN
F 3 "" H 4450 2750 50  0001 C CNN
F 4 "RMCF0603FT66K5" H 4450 2750 60  0001 C CNN "Part Number"
	1    4450 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 2700 4700 2700
Wire Wire Line
	4700 2700 4700 2500
Wire Wire Line
	4700 2500 4450 2500
Wire Wire Line
	4450 2400 4450 2600
Connection ~ 4450 2500
Wire Wire Line
	4450 2100 4450 2050
Connection ~ 5200 2050
$Comp
L R R11
U 1 1 5AA69587
P 5350 3200
AR Path="/5AA2AB58/5AA69587" Ref="R11"  Part="1" 
AR Path="/5AA6DCFC/5AA69587" Ref="R18"  Part="1" 
F 0 "R11" V 5430 3200 50  0000 C CNN
F 1 "267k" V 5350 3200 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 5280 3200 50  0001 C CNN
F 3 "" H 5350 3200 50  0001 C CNN
F 4 "RMCF0603FT267K" H 5350 3200 60  0001 C CNN "Part Number"
	1    5350 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5350 3050 5350 2950
Wire Wire Line
	5350 2950 5800 2950
$Comp
L GND #PWR024
U 1 1 5AA69658
P 5350 3500
AR Path="/5AA2AB58/5AA69658" Ref="#PWR024"  Part="1" 
AR Path="/5AA6DCFC/5AA69658" Ref="#PWR026"  Part="1" 
F 0 "#PWR024" H 5350 3250 50  0001 C CNN
F 1 "GND" H 5350 3350 50  0000 C CNN
F 2 "" H 5350 3500 50  0001 C CNN
F 3 "" H 5350 3500 50  0001 C CNN
	1    5350 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	5350 3500 5350 3350
Wire Wire Line
	4450 2900 4450 3400
Wire Wire Line
	1750 3400 5350 3400
Connection ~ 5350 3400
$Comp
L C C11
U 1 1 5AA69C44
P 3650 2550
AR Path="/5AA2AB58/5AA69C44" Ref="C11"  Part="1" 
AR Path="/5AA6DCFC/5AA69C44" Ref="C21"  Part="1" 
F 0 "C11" H 3675 2650 50  0000 L CNN
F 1 "10nF, 50V" H 3675 2450 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 3688 2400 50  0001 C CNN
F 3 "" H 3650 2550 50  0001 C CNN
F 4 "06035C103KAT4A" H 3650 2550 60  0001 C CNN "Part Number"
	1    3650 2550
	1    0    0    -1  
$EndComp
Connection ~ 4450 2050
Wire Wire Line
	2100 2400 2100 2050
Connection ~ 2100 2050
Wire Wire Line
	2650 2400 2650 2050
Connection ~ 2650 2050
Wire Wire Line
	3150 2400 3150 2050
Connection ~ 3150 2050
Wire Wire Line
	3650 2400 3650 2050
Connection ~ 3650 2050
Wire Wire Line
	3650 3400 3650 2700
Connection ~ 4450 3400
Wire Wire Line
	3150 3400 3150 2700
Connection ~ 3650 3400
Wire Wire Line
	2650 3400 2650 2700
Connection ~ 3150 3400
Wire Wire Line
	2100 3400 2100 2700
Connection ~ 2650 3400
$Comp
L C C14
U 1 1 5AA7002B
P 4850 3100
AR Path="/5AA2AB58/5AA7002B" Ref="C14"  Part="1" 
AR Path="/5AA6DCFC/5AA7002B" Ref="C24"  Part="1" 
F 0 "C14" H 4875 3200 50  0000 L CNN
F 1 "100nF" H 4875 3000 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 4888 2950 50  0001 C CNN
F 3 "" H 4850 3100 50  0001 C CNN
F 4 "06035C104KAT2A" H 4850 3100 60  0001 C CNN "Part Number"
	1    4850 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 2800 4850 2800
Wire Wire Line
	4850 2800 4850 2950
Wire Wire Line
	4850 3250 4850 3400
Connection ~ 4850 3400
$Comp
L C C9
U 1 1 5AA6A967
P 2650 2550
AR Path="/5AA2AB58/5AA6A967" Ref="C9"  Part="1" 
AR Path="/5AA6DCFC/5AA6A967" Ref="C19"  Part="1" 
F 0 "C9" H 2675 2650 50  0000 L CNN
F 1 "4.7uF,50V" H 2675 2450 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206" H 2688 2400 50  0001 C CNN
F 3 "" H 2650 2550 50  0001 C CNN
F 4 "12065C475KAT2A" H 2650 2550 60  0001 C CNN "Part Number"
	1    2650 2550
	1    0    0    -1  
$EndComp
$Comp
L C C10
U 1 1 5AA6A9D1
P 3150 2550
AR Path="/5AA2AB58/5AA6A9D1" Ref="C10"  Part="1" 
AR Path="/5AA6DCFC/5AA6A9D1" Ref="C20"  Part="1" 
F 0 "C10" H 3175 2650 50  0000 L CNN
F 1 "4.7uF,50V" H 3175 2450 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206" H 3188 2400 50  0001 C CNN
F 3 "" H 3150 2550 50  0001 C CNN
F 4 "12065C475KAT2A" H 3150 2550 60  0001 C CNN "Part Number"
	1    3150 2550
	1    0    0    -1  
$EndComp
$Comp
L C C6
U 1 1 5AA7002E
P 7250 2050
AR Path="/5AA2AB58/5AA7002E" Ref="C6"  Part="1" 
AR Path="/5AA6DCFC/5AA7002E" Ref="C16"  Part="1" 
F 0 "C6" V 7350 2150 50  0000 L CNN
F 1 "100nF" V 7100 1950 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 7288 1900 50  0001 C CNN
F 3 "" H 7250 2050 50  0001 C CNN
F 4 "06035C104KAT2A" H 7250 2050 60  0001 C CNN "Part Number"
	1    7250 2050
	0    1    1    0   
$EndComp
$Comp
L C C12
U 1 1 5AA6B735
P 8400 2650
AR Path="/5AA2AB58/5AA6B735" Ref="C12"  Part="1" 
AR Path="/5AA6DCFC/5AA6B735" Ref="C22"  Part="1" 
F 0 "C12" H 8425 2750 50  0000 L CNN
F 1 "4.7uF,50V" H 8425 2550 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206" H 8438 2500 50  0001 C CNN
F 3 "" H 8400 2650 50  0001 C CNN
F 4 "12065C475KAT2A" H 8400 2650 60  0001 C CNN "Part Number"
	1    8400 2650
	1    0    0    -1  
$EndComp
$Comp
L C C7
U 1 1 5AAA9295
P 1750 2550
AR Path="/5AA2AB58/5AAA9295" Ref="C7"  Part="1" 
AR Path="/5AA6DCFC/5AAA9295" Ref="C17"  Part="1" 
F 0 "C7" H 1775 2650 50  0000 L CNN
F 1 "4.7uF,50V" H 1775 2450 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206" H 1788 2400 50  0001 C CNN
F 3 "" H 1750 2550 50  0001 C CNN
F 4 "12065C475KAT2A" H 1750 2550 60  0001 C CNN "Part Number"
	1    1750 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	1750 2400 1750 2050
Connection ~ 1750 2050
Wire Wire Line
	1750 2700 1750 3400
Connection ~ 2100 3400
$Comp
L C C13
U 1 1 5AAA9C17
P 8950 2650
AR Path="/5AA2AB58/5AAA9C17" Ref="C13"  Part="1" 
AR Path="/5AA6DCFC/5AAA9C17" Ref="C23"  Part="1" 
F 0 "C13" H 8975 2750 50  0000 L CNN
F 1 "4.7uF,50V" H 8975 2550 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206" H 8988 2500 50  0001 C CNN
F 3 "" H 8950 2650 50  0001 C CNN
F 4 "12065C475KAT2A" H 8950 2650 60  0001 C CNN "Part Number"
	1    8950 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	8950 2800 8950 3000
Connection ~ 8950 3000
Wire Wire Line
	8950 2500 8950 2250
Connection ~ 8950 2250
Text Label 7050 2250 0    60   ~ 0
PHI
$EndSCHEMATC
