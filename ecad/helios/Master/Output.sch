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
LIBS:master-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 8 9
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
L Conn_01x02 J11
U 1 1 5AC39A82
P 1000 1400
AR Path="/5AC39A79/5AC39A82" Ref="J11"  Part="1" 
AR Path="/5AC440F9/5AC39A82" Ref="J5"  Part="1" 
AR Path="/5AC443F1/5AC39A82" Ref="J7"  Part="1" 
AR Path="/5AC443F5/5AC39A82" Ref="J9"  Part="1" 
F 0 "J11" H 1000 1500 50  0000 C CNN
F 1 "MCV 1,5/ 2-G-3,5" H 1000 1200 50  0000 C CNN
F 2 "Connectors_Phoenix:PhoenixContact_MCV-G_02x3.50mm_Vertical" H 1000 1400 50  0001 C CNN
F 3 "" H 1000 1400 50  0001 C CNN
F 4 "1843606" H 1000 1400 60  0001 C CNN "Part Number"
	1    1000 1400
	-1   0    0    1   
$EndComp
$Comp
L Q_PMOS_GDS Q7
U 1 1 5AC3A227
P 2400 1400
AR Path="/5AC39A79/5AC3A227" Ref="Q7"  Part="1" 
AR Path="/5AC440F9/5AC3A227" Ref="Q1"  Part="1" 
AR Path="/5AC443F1/5AC3A227" Ref="Q3"  Part="1" 
AR Path="/5AC443F5/5AC3A227" Ref="Q5"  Part="1" 
F 0 "Q7" H 2600 1450 50  0000 L CNN
F 1 "IRF9Z34NPbF" H 2600 1350 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-220_Horizontal" H 2600 1500 50  0001 C CNN
F 3 "" H 2400 1400 50  0001 C CNN
F 4 "IRF9Z34NPbF" H 2400 1400 60  0001 C CNN "Part Number"
	1    2400 1400
	0    -1   -1   0   
$EndComp
$Comp
L D_Zener D10
U 1 1 5AC3A38C
P 3050 1550
AR Path="/5AC39A79/5AC3A38C" Ref="D10"  Part="1" 
AR Path="/5AC440F9/5AC3A38C" Ref="D1"  Part="1" 
AR Path="/5AC443F1/5AC3A38C" Ref="D4"  Part="1" 
AR Path="/5AC443F5/5AC3A38C" Ref="D7"  Part="1" 
F 0 "D10" H 3050 1650 50  0000 C CNN
F 1 "10V" H 3050 1450 50  0000 C CNN
F 2 "FORT:D_SOT-23_KAN" H 3050 1550 50  0001 C CNN
F 3 "" H 3050 1550 50  0001 C CNN
F 4 "LM4040DIM3X-10/NOPB" H 3050 1550 60  0001 C CNN "Part Number"
	1    3050 1550
	0    1    1    0   
$EndComp
$Comp
L R R18
U 1 1 5AC3A4DF
P 2400 2000
AR Path="/5AC39A79/5AC3A4DF" Ref="R18"  Part="1" 
AR Path="/5AC440F9/5AC3A4DF" Ref="R3"  Part="1" 
AR Path="/5AC443F1/5AC3A4DF" Ref="R8"  Part="1" 
AR Path="/5AC443F5/5AC3A4DF" Ref="R13"  Part="1" 
F 0 "R18" V 2480 2000 50  0000 C CNN
F 1 "100K" V 2400 2000 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2330 2000 50  0001 C CNN
F 3 "" H 2400 2000 50  0001 C CNN
F 4 "RMCF0603FT100K" H 2400 2000 60  0001 C CNN "Part Number"
	1    2400 2000
	1    0    0    -1  
$EndComp
Text Label 1450 1300 0    60   ~ 0
VSUPPLY
Text Label 2750 1300 0    60   ~ 0
V_PROTECTED
Text Label 1650 2550 0    60   ~ 0
GNDSUPPLY
Text Label 7050 1300 0    60   ~ 0
V36
$Comp
L ISO7320FC U20
U 1 1 5AC3E36A
P 2900 4100
AR Path="/5AC39A79/5AC3E36A" Ref="U20"  Part="1" 
AR Path="/5AC440F9/5AC3E36A" Ref="U5"  Part="1" 
AR Path="/5AC443F1/5AC3E36A" Ref="U10"  Part="1" 
AR Path="/5AC443F5/5AC3E36A" Ref="U15"  Part="1" 
F 0 "U20" H 2900 4525 50  0000 C CNN
F 1 "ISO7320FC" H 2900 4450 50  0000 C CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 2900 3750 50  0001 C CIN
F 3 "" H 2900 4100 50  0001 C CNN
F 4 "ISO7320FCDR" H 2900 4100 60  0001 C CNN "Part Number"
	1    2900 4100
	1    0    0    -1  
$EndComp
$Comp
L C C38
U 1 1 5AC3EBB4
P 3550 3800
AR Path="/5AC39A79/5AC3EBB4" Ref="C38"  Part="1" 
AR Path="/5AC440F9/5AC3EBB4" Ref="C11"  Part="1" 
AR Path="/5AC443F1/5AC3EBB4" Ref="C20"  Part="1" 
AR Path="/5AC443F5/5AC3EBB4" Ref="C29"  Part="1" 
F 0 "C38" H 3575 3900 50  0000 L CNN
F 1 "100nF" H 3575 3700 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 3588 3650 50  0001 C CNN
F 3 "" H 3550 3800 50  0001 C CNN
F 4 "06035C104KAT2A" H 3550 3800 60  0001 C CNN "Part Number"
	1    3550 3800
	1    0    0    -1  
$EndComp
$Comp
L C C37
U 1 1 5AC3EDD3
P 1750 3750
AR Path="/5AC39A79/5AC3EDD3" Ref="C37"  Part="1" 
AR Path="/5AC440F9/5AC3EDD3" Ref="C10"  Part="1" 
AR Path="/5AC443F1/5AC3EDD3" Ref="C19"  Part="1" 
AR Path="/5AC443F5/5AC3EDD3" Ref="C28"  Part="1" 
F 0 "C37" H 1775 3850 50  0000 L CNN
F 1 "100nF" H 1775 3650 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 1788 3600 50  0001 C CNN
F 3 "" H 1750 3750 50  0001 C CNN
F 4 "06035C104KAT2A" H 1750 3750 60  0001 C CNN "Part Number"
	1    1750 3750
	1    0    0    -1  
$EndComp
$Comp
L UA9638 U21
U 1 1 5AC3EEA4
P 5750 4100
AR Path="/5AC39A79/5AC3EEA4" Ref="U21"  Part="1" 
AR Path="/5AC440F9/5AC3EEA4" Ref="U6"  Part="1" 
AR Path="/5AC443F1/5AC3EEA4" Ref="U11"  Part="1" 
AR Path="/5AC443F5/5AC3EEA4" Ref="U16"  Part="1" 
F 0 "U21" H 5600 4200 50  0000 C CNN
F 1 "UA9638" H 5650 4100 50  0000 C CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 5750 4100 50  0001 C CNN
F 3 "" H 5750 4100 50  0001 C CNN
F 4 "UA9638CDR" H 5750 4100 60  0001 C CNN "Part Number"
	1    5750 4100
	1    0    0    -1  
$EndComp
$Comp
L UA9638 U21
U 2 1 5AC3EF24
P 5750 5250
AR Path="/5AC39A79/5AC3EF24" Ref="U21"  Part="2" 
AR Path="/5AC440F9/5AC3EF24" Ref="U6"  Part="2" 
AR Path="/5AC443F1/5AC3EF24" Ref="U11"  Part="2" 
AR Path="/5AC443F5/5AC3EF24" Ref="U16"  Part="2" 
F 0 "U21" H 5600 5350 50  0000 C CNN
F 1 "UA9638" H 5700 5250 50  0000 C CNN
F 2 "" H 5750 5250 50  0001 C CNN
F 3 "" H 5750 5250 50  0001 C CNN
	2    5750 5250
	1    0    0    -1  
$EndComp
$Comp
L Conn_02x04_Top_Bottom J12
U 1 1 5AC3FC48
P 9550 4200
AR Path="/5AC39A79/5AC3FC48" Ref="J12"  Part="1" 
AR Path="/5AC440F9/5AC3FC48" Ref="J6"  Part="1" 
AR Path="/5AC443F1/5AC3FC48" Ref="J8"  Part="1" 
AR Path="/5AC443F5/5AC3FC48" Ref="J10"  Part="1" 
F 0 "J12" H 9600 4400 50  0000 C CNN
F 1 "MiniFit JR 5556 08A2" H 9600 3900 50  0000 C CNN
F 2 "FORT:Molex_MiniFit-JR-5556-08A_2x04x4.20mm_Straight" H 9550 4200 50  0001 C CNN
F 3 "" H 9550 4200 50  0001 C CNN
F 4 "39-28-1083" H 9550 4200 60  0001 C CNN "Part Number"
	1    9550 4200
	1    0    0    -1  
$EndComp
Text Label 9000 4100 0    60   ~ 0
V36
Text Label 9000 4200 0    60   ~ 0
V6
Text Label 9000 4300 0    60   ~ 0
IR_ON+
Text Label 9000 4400 0    60   ~ 0
IR_ON-
Text Label 10200 4100 0    60   ~ 0
GNDSUPPLY
Text Label 10200 4200 0    60   ~ 0
GNDSUPPLY
Text Label 10200 4300 0    60   ~ 0
DATA_+
Text Label 10200 4400 0    60   ~ 0
DATA_-
$Comp
L +5V #PWR029
U 1 1 5AC41007
P 1750 3450
AR Path="/5AC39A79/5AC41007" Ref="#PWR029"  Part="1" 
AR Path="/5AC440F9/5AC41007" Ref="#PWR023"  Part="1" 
AR Path="/5AC443F1/5AC41007" Ref="#PWR025"  Part="1" 
AR Path="/5AC443F5/5AC41007" Ref="#PWR027"  Part="1" 
F 0 "#PWR029" H 1750 3300 50  0001 C CNN
F 1 "+5V" H 1750 3590 50  0000 C CNN
F 2 "" H 1750 3450 50  0001 C CNN
F 3 "" H 1750 3450 50  0001 C CNN
	1    1750 3450
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR030
U 1 1 5AC41049
P 1750 4050
AR Path="/5AC39A79/5AC41049" Ref="#PWR030"  Part="1" 
AR Path="/5AC440F9/5AC41049" Ref="#PWR024"  Part="1" 
AR Path="/5AC443F1/5AC41049" Ref="#PWR026"  Part="1" 
AR Path="/5AC443F5/5AC41049" Ref="#PWR028"  Part="1" 
F 0 "#PWR030" H 1750 3800 50  0001 C CNN
F 1 "GND" H 1750 3900 50  0000 C CNN
F 2 "" H 1750 4050 50  0001 C CNN
F 3 "" H 1750 4050 50  0001 C CNN
	1    1750 4050
	1    0    0    -1  
$EndComp
Text HLabel 2400 4200 0    60   Input ~ 0
DATA
Text HLabel 2400 4300 0    60   Input ~ 0
TRIGGER
Text Label 3700 3550 0    60   ~ 0
V5
Text Label 3700 4000 0    60   ~ 0
GNDSUPPLY
Text Label 3650 4200 0    60   ~ 0
DATA_ISO
Text Label 3650 4300 0    60   ~ 0
TRIGGER_ISO
$Comp
L C C36
U 1 1 5AC420D7
P 6100 3350
AR Path="/5AC39A79/5AC420D7" Ref="C36"  Part="1" 
AR Path="/5AC440F9/5AC420D7" Ref="C9"  Part="1" 
AR Path="/5AC443F1/5AC420D7" Ref="C18"  Part="1" 
AR Path="/5AC443F5/5AC420D7" Ref="C27"  Part="1" 
F 0 "C36" H 6125 3450 50  0000 L CNN
F 1 "100nF" H 6125 3250 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 6138 3200 50  0001 C CNN
F 3 "" H 6100 3350 50  0001 C CNN
F 4 "06035C104KAT2A" H 6100 3350 60  0001 C CNN "Part Number"
	1    6100 3350
	1    0    0    -1  
$EndComp
Text Label 6350 3150 0    60   ~ 0
V5
Text Label 6200 3650 0    60   ~ 0
GNDSUPPLY
Text Label 7200 4750 0    60   ~ 0
GNDSUPPLY
Text Label 5700 4800 0    60   ~ 0
V5
Text Label 7400 5900 0    60   ~ 0
GNDSUPPLY
Text Label 7300 3900 0    60   ~ 0
DATA_+
Text Label 7300 4300 0    60   ~ 0
DATA_-
Text Label 7200 5450 0    60   ~ 0
IR_ON-
Text Label 7200 5050 0    60   ~ 0
IR_ON+
Text HLabel 1450 1400 2    60   BiDi ~ 0
PWRGND
Text Notes 5800 950  0    60   ~ 0
3.4A Limitation
$Comp
L R R19
U 1 1 5AC4AA01
P 5350 2250
AR Path="/5AC39A79/5AC4AA01" Ref="R19"  Part="1" 
AR Path="/5AC440F9/5AC4AA01" Ref="R4"  Part="1" 
AR Path="/5AC443F1/5AC4AA01" Ref="R9"  Part="1" 
AR Path="/5AC443F5/5AC4AA01" Ref="R14"  Part="1" 
F 0 "R19" V 5430 2250 50  0000 C CNN
F 1 "715" V 5350 2250 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 5280 2250 50  0001 C CNN
F 3 "" H 5350 2250 50  0001 C CNN
F 4 "RMCF0603FT715R" H 5350 2250 60  0001 C CNN "Part Number"
	1    5350 2250
	1    0    0    -1  
$EndComp
Text Notes 5350 1050 0    60   ~ 0
Require minimum 300mm2 copper area
$Comp
L LED D11
U 1 1 5AC4C198
P 3400 2200
AR Path="/5AC39A79/5AC4C198" Ref="D11"  Part="1" 
AR Path="/5AC440F9/5AC4C198" Ref="D2"  Part="1" 
AR Path="/5AC443F1/5AC4C198" Ref="D5"  Part="1" 
AR Path="/5AC443F5/5AC4C198" Ref="D8"  Part="1" 
F 0 "D11" V 3300 2350 50  0000 C CNN
F 1 "RED" V 3200 2350 50  0000 C CNN
F 2 "LEDs:LED_D3.0mm" H 3400 2200 50  0001 C CNN
F 3 "" H 3400 2200 50  0001 C CNN
F 4 "151031SS04000" H 3400 2200 60  0001 C CNN "Part Number"
	1    3400 2200
	0    -1   -1   0   
$EndComp
$Comp
L R R17
U 1 1 5AC4C306
P 3400 1800
AR Path="/5AC39A79/5AC4C306" Ref="R17"  Part="1" 
AR Path="/5AC440F9/5AC4C306" Ref="R2"  Part="1" 
AR Path="/5AC443F1/5AC4C306" Ref="R7"  Part="1" 
AR Path="/5AC443F5/5AC4C306" Ref="R12"  Part="1" 
F 0 "R17" V 3480 1800 50  0000 C CNN
F 1 "300" V 3300 1800 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3330 1800 50  0001 C CNN
F 3 "" H 3400 1800 50  0001 C CNN
F 4 "RMCF0603FT300R" H 3400 1800 60  0001 C CNN "Part Number"
	1    3400 1800
	1    0    0    -1  
$EndComp
$Comp
L Q_PMOS_GSD Q8
U 1 1 5AC4BBFF
P 3950 1950
AR Path="/5AC39A79/5AC4BBFF" Ref="Q8"  Part="1" 
AR Path="/5AC440F9/5AC4BBFF" Ref="Q2"  Part="1" 
AR Path="/5AC443F1/5AC4BBFF" Ref="Q4"  Part="1" 
AR Path="/5AC443F5/5AC4BBFF" Ref="Q6"  Part="1" 
F 0 "Q8" H 3850 2200 50  0000 L CNN
F 1 "BSS84P" H 3650 2100 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 4150 2050 50  0001 C CNN
F 3 "" H 3950 1950 50  0001 C CNN
F 4 "BSS84P" H 3950 1950 60  0001 C CNN "Part Number"
	1    3950 1950
	-1   0    0    1   
$EndComp
$Sheet
S 7200 2300 1800 400 
U 5AC4F2F7
F0 "6V_Regulator" 60
F1 "6V_regulator.sch" 60
F2 "VIN" I L 7200 2400 60 
F3 "PWRGND" B L 7200 2550 60 
F4 "V5" O R 9000 2500 60 
F5 "V6" O R 9000 2400 60 
$EndSheet
Text Label 9250 2400 0    60   ~ 0
V6
Text Label 9250 2500 0    60   ~ 0
V5
Wire Wire Line
	1200 1300 2200 1300
Wire Wire Line
	1200 1400 1450 1400
Wire Wire Line
	2600 1300 5650 1300
Wire Wire Line
	3050 1300 3050 1400
Connection ~ 3050 1300
Wire Wire Line
	3050 1700 3050 1750
Wire Wire Line
	3050 1750 2400 1750
Wire Wire Line
	2400 1600 2400 1850
Connection ~ 2400 1750
Wire Wire Line
	1300 2550 7200 2550
Wire Wire Line
	6050 2550 6050 2350
Wire Wire Line
	6150 2550 6150 2350
Connection ~ 6050 2550
Wire Wire Line
	2400 2150 2400 2550
Connection ~ 2400 2550
Wire Wire Line
	4750 4200 4750 4100
Wire Wire Line
	4750 4100 5150 4100
Wire Wire Line
	4800 4300 4800 5250
Wire Wire Line
	4800 5250 5150 5250
Wire Wire Line
	9000 4100 9350 4100
Wire Wire Line
	10200 4400 9850 4400
Wire Wire Line
	9850 4300 10200 4300
Wire Wire Line
	9850 4200 10200 4200
Wire Wire Line
	9850 4100 10200 4100
Wire Wire Line
	9000 4200 9350 4200
Wire Wire Line
	9000 4300 9350 4300
Wire Wire Line
	9000 4400 9350 4400
Wire Wire Line
	1750 4000 2500 4000
Wire Wire Line
	1750 3900 1750 4050
Connection ~ 1750 4000
Wire Wire Line
	1750 3450 1750 3600
Wire Wire Line
	2400 4300 2500 4300
Wire Wire Line
	2400 4200 2500 4200
Wire Wire Line
	1750 3550 2150 3550
Wire Wire Line
	2150 3550 2150 3900
Wire Wire Line
	2150 3900 2500 3900
Connection ~ 1750 3550
Wire Wire Line
	3300 4000 3700 4000
Wire Wire Line
	3550 4000 3550 3950
Wire Wire Line
	3300 3900 3400 3900
Wire Wire Line
	3400 3900 3400 3550
Wire Wire Line
	3400 3550 3700 3550
Wire Wire Line
	3550 3550 3550 3650
Connection ~ 3550 3550
Connection ~ 3550 4000
Wire Wire Line
	3300 4200 4750 4200
Wire Wire Line
	4800 4300 3300 4300
Wire Wire Line
	6100 3200 6100 3150
Wire Wire Line
	5650 3150 6350 3150
Wire Wire Line
	5650 3150 5650 3700
Connection ~ 6100 3150
Wire Wire Line
	6100 3650 6100 3500
Wire Wire Line
	5650 4500 5650 4600
Wire Wire Line
	5650 4600 6400 4600
Wire Wire Line
	5700 4800 5650 4800
Wire Wire Line
	5650 4800 5650 4850
Wire Wire Line
	5650 5650 5650 5750
Wire Wire Line
	5650 5750 6300 5750
Wire Wire Line
	6350 3900 7300 3900
Wire Wire Line
	6350 4300 7300 4300
Wire Wire Line
	6350 5050 7200 5050
Wire Wire Line
	6350 5450 7200 5450
Wire Wire Line
	5350 2100 5350 1900
Wire Wire Line
	5350 1900 5650 1900
Wire Wire Line
	5350 2400 5350 2550
Connection ~ 5350 2550
Wire Wire Line
	3850 1700 5650 1700
Wire Wire Line
	5250 1600 5650 1600
Connection ~ 5250 1700
Wire Wire Line
	3400 2050 3400 1950
Wire Wire Line
	4750 1450 5650 1450
Wire Wire Line
	3400 1650 3400 1550
Wire Wire Line
	3850 2300 3850 2150
Connection ~ 6150 2550
Wire Wire Line
	9000 2500 9250 2500
Wire Wire Line
	9000 2400 9250 2400
Wire Wire Line
	6100 3650 6200 3650
Wire Wire Line
	3400 2350 3400 2550
Connection ~ 3400 2550
Wire Wire Line
	3650 2300 3850 2300
Wire Wire Line
	3400 1550 3650 1550
Wire Wire Line
	3650 1550 3650 2300
Text Notes 2700 2700 0    60   ~ 0
LED is OFF except in rare fault conditions
Wire Wire Line
	6550 1300 7050 1300
Text Label 6500 2400 0    60   ~ 0
V_PROTECTED
Wire Wire Line
	6500 2400 7200 2400
$Comp
L TPS1H100AQPWPRQ1 U19
U 1 1 5AC6500E
P 6100 1650
AR Path="/5AC39A79/5AC6500E" Ref="U19"  Part="1" 
AR Path="/5AC440F9/5AC6500E" Ref="U4"  Part="1" 
AR Path="/5AC443F1/5AC6500E" Ref="U9"  Part="1" 
AR Path="/5AC443F5/5AC6500E" Ref="U14"  Part="1" 
F 0 "U19" H 5900 1100 60  0000 C CNN
F 1 "TPS1H100AQPWPRQ1" H 6100 2150 60  0000 C CNN
F 2 "FORT:HTSSOP-14-1EP_4.4x6.5mm_Pitch0.65mm_ThermalPad" H 6100 2250 60  0001 C CNN
F 3 "" H 6050 1750 60  0001 C CNN
F 4 "TPS1H100AQPWPRQ1" H 6100 2350 60  0001 C CNN "Part Number"
	1    6100 1650
	1    0    0    -1  
$EndComp
Text Notes 9050 4650 0    60   ~ 0
Helios Light Connector
Text Notes 750  1100 0    60   ~ 0
Alim 36V 4A
Wire Wire Line
	4150 1950 4750 1950
Wire Wire Line
	4750 1950 4750 1450
Wire Wire Line
	5250 1700 5250 1600
Wire Wire Line
	3850 1700 3850 1750
Wire Wire Line
	4100 1700 4100 1600
Connection ~ 4100 1700
Text Label 4100 1600 0    60   ~ 0
V5
Text Label 4800 1450 0    60   ~ 0
~FAIL
Wire Wire Line
	1300 1400 1300 2550
Connection ~ 1300 1400
Wire Wire Line
	6400 4600 6400 4750
Wire Wire Line
	6400 4750 7200 4750
Wire Wire Line
	6300 5750 6300 5900
Wire Wire Line
	6300 5900 7400 5900
Wire Wire Line
	6700 5500 6700 5450
Wire Wire Line
	7000 5800 7000 5900
Connection ~ 7000 5900
Connection ~ 6700 5900
Wire Wire Line
	6700 5800 6700 5900
Wire Wire Line
	7000 5500 7000 5050
Connection ~ 7000 5050
Connection ~ 6700 5450
Connection ~ 7000 4750
Wire Wire Line
	7000 4700 7000 4750
Wire Wire Line
	6700 4700 6700 4750
Connection ~ 6700 4750
Wire Wire Line
	7000 4400 7000 3900
Connection ~ 7000 3900
Connection ~ 6700 4300
Wire Wire Line
	6700 4400 6700 4300
$Comp
L D_TVS D26
U 1 1 5ACB66B6
P 6700 4550
AR Path="/5AC39A79/5ACB66B6" Ref="D26"  Part="1" 
AR Path="/5AC440F9/5ACB66B6" Ref="D14"  Part="1" 
AR Path="/5AC443F1/5ACB66B6" Ref="D18"  Part="1" 
AR Path="/5AC443F5/5ACB66B6" Ref="D22"  Part="1" 
F 0 "D26" H 6700 4650 50  0000 C CNN
F 1 "5.5V" H 6700 4450 50  0000 C CNN
F 2 "FORT:D_0201" H 6700 4550 50  0001 C CNN
F 3 "" H 6700 4550 50  0001 C CNN
F 4 "ESD230B1W0201E6327XTSA1" H 6700 4550 60  0001 C CNN "Part Number"
	1    6700 4550
	0    1    1    0   
$EndComp
$Comp
L D_TVS D27
U 1 1 5ACB8F07
P 7000 4550
AR Path="/5AC39A79/5ACB8F07" Ref="D27"  Part="1" 
AR Path="/5AC440F9/5ACB8F07" Ref="D15"  Part="1" 
AR Path="/5AC443F1/5ACB8F07" Ref="D19"  Part="1" 
AR Path="/5AC443F5/5ACB8F07" Ref="D23"  Part="1" 
F 0 "D27" H 7000 4650 50  0000 C CNN
F 1 "5.5V" H 7000 4450 50  0000 C CNN
F 2 "FORT:D_0201" H 7000 4550 50  0001 C CNN
F 3 "" H 7000 4550 50  0001 C CNN
F 4 "ESD230B1W0201E6327XTSA1" H 7000 4550 60  0001 C CNN "Part Number"
	1    7000 4550
	0    1    1    0   
$EndComp
$Comp
L D_TVS D28
U 1 1 5ACB8F6F
P 6700 5650
AR Path="/5AC39A79/5ACB8F6F" Ref="D28"  Part="1" 
AR Path="/5AC440F9/5ACB8F6F" Ref="D16"  Part="1" 
AR Path="/5AC443F1/5ACB8F6F" Ref="D20"  Part="1" 
AR Path="/5AC443F5/5ACB8F6F" Ref="D24"  Part="1" 
F 0 "D28" H 6700 5750 50  0000 C CNN
F 1 "5.5V" H 6700 5550 50  0000 C CNN
F 2 "FORT:D_0201" H 6700 5650 50  0001 C CNN
F 3 "" H 6700 5650 50  0001 C CNN
F 4 "ESD230B1W0201E6327XTSA1" H 6700 5650 60  0001 C CNN "Part Number"
	1    6700 5650
	0    1    1    0   
$EndComp
$Comp
L D_TVS D17
U 1 1 5ACB9059
P 7000 5650
AR Path="/5AC440F9/5ACB9059" Ref="D17"  Part="1" 
AR Path="/5AC443F1/5ACB9059" Ref="D21"  Part="1" 
AR Path="/5AC443F5/5ACB9059" Ref="D25"  Part="1" 
AR Path="/5AC39A79/5ACB9059" Ref="D29"  Part="1" 
F 0 "D29" H 7000 5750 50  0000 C CNN
F 1 "5.5V" H 7000 5550 50  0000 C CNN
F 2 "FORT:D_0201" H 7000 5650 50  0001 C CNN
F 3 "" H 7000 5650 50  0001 C CNN
F 4 "ESD230B1W0201E6327XTSA1" H 7000 5650 60  0001 C CNN "Part Number"
	1    7000 5650
	0    1    1    0   
$EndComp
$EndSCHEMATC