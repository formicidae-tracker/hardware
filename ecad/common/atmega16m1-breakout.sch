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
LIBS:atmega16m1-breakout-cache
EELAYER 25 0
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
L ATMEGA16M1-AU U1
U 1 1 5B852B27
P 2850 2850
F 0 "U1" H 1900 4680 50  0000 L BNN
F 1 "ATMEGA16M1-AU" H 3350 1450 50  0000 L BNN
F 2 "Housings_QFP:TQFP-32_7x7mm_Pitch0.8mm" H 2850 2850 50  0001 C CIN
F 3 "" H 2850 2850 50  0001 C CNN
	1    2850 2850
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 5B852CDF
P 850 1350
F 0 "C1" H 875 1450 50  0000 L CNN
F 1 "100nF" H 875 1250 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 888 1200 50  0001 C CNN
F 3 "" H 850 1350 50  0001 C CNN
F 4 "06035C104KAT2A" H 850 1350 60  0001 C CNN "Part Number"
	1    850  1350
	1    0    0    -1  
$EndComp
$Comp
L C C2
U 1 1 5B852D11
P 1300 1950
F 0 "C2" H 1325 2050 50  0000 L CNN
F 1 "100nF" H 1325 1850 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 1338 1800 50  0001 C CNN
F 3 "" H 1300 1950 50  0001 C CNN
F 4 "06035C104KAT2A" H 1300 1950 60  0001 C CNN "Part Number"
	1    1300 1950
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 5B852D80
P 1600 2050
F 0 "C3" H 1625 2150 50  0000 L CNN
F 1 "100nF" H 1625 1950 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 1638 1900 50  0001 C CNN
F 3 "" H 1600 2050 50  0001 C CNN
F 4 "06035C104KAT2A" H 1600 2050 60  0001 C CNN "Part Number"
	1    1600 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	1300 950  1300 1800
Wire Wire Line
	1300 1450 1750 1450
$Comp
L +5V #PWR01
U 1 1 5B852E50
P 850 850
F 0 "#PWR01" H 850 700 50  0001 C CNN
F 1 "+5V" H 850 990 50  0000 C CNN
F 2 "" H 850 850 50  0001 C CNN
F 3 "" H 850 850 50  0001 C CNN
	1    850  850 
	1    0    0    -1  
$EndComp
Wire Wire Line
	850  850  850  1200
Wire Wire Line
	1750 1150 1300 1150
Wire Wire Line
	1300 950  850  950 
Connection ~ 850  950 
$Comp
L GND #PWR02
U 1 1 5B852E84
P 850 1650
F 0 "#PWR02" H 850 1400 50  0001 C CNN
F 1 "GND" H 850 1500 50  0000 C CNN
F 2 "" H 850 1650 50  0001 C CNN
F 3 "" H 850 1650 50  0001 C CNN
	1    850  1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	850  1650 850  1500
$Comp
L GNDA #PWR03
U 1 1 5B852EBB
P 1300 2250
F 0 "#PWR03" H 1300 2000 50  0001 C CNN
F 1 "GNDA" H 1300 2100 50  0000 C CNN
F 2 "" H 1300 2250 50  0001 C CNN
F 3 "" H 1300 2250 50  0001 C CNN
	1    1300 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	1300 2250 1300 2100
Connection ~ 1300 1450
Connection ~ 1300 1150
Wire Wire Line
	1600 1450 1600 1900
Wire Wire Line
	1600 1850 1750 1850
Connection ~ 1600 1450
Connection ~ 1600 1850
Wire Wire Line
	1300 2150 1450 2150
Wire Wire Line
	1450 2150 1450 2250
Wire Wire Line
	1450 2250 1600 2250
Wire Wire Line
	1600 2250 1600 2200
Connection ~ 1300 2150
$Comp
L GND #PWR04
U 1 1 5B852F69
P 1550 4200
F 0 "#PWR04" H 1550 3950 50  0001 C CNN
F 1 "GND" H 1550 4050 50  0000 C CNN
F 2 "" H 1550 4200 50  0001 C CNN
F 3 "" H 1550 4200 50  0001 C CNN
	1    1550 4200
	1    0    0    -1  
$EndComp
$Comp
L GNDA #PWR05
U 1 1 5B852F94
P 1550 3750
F 0 "#PWR05" H 1550 3500 50  0001 C CNN
F 1 "GNDA" H 1550 3600 50  0000 C CNN
F 2 "" H 1550 3750 50  0001 C CNN
F 3 "" H 1550 3750 50  0001 C CNN
	1    1550 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	1550 3750 1550 3650
Wire Wire Line
	1550 3650 1750 3650
Wire Wire Line
	1550 4200 1550 4050
Wire Wire Line
	1550 4050 1750 4050
$Comp
L Crystal_GND24 Y1
U 1 1 5B852FF5
P 5300 4000
F 0 "Y1" H 5425 4200 50  0000 L CNN
F 1 "16MHz" H 5425 4125 50  0000 L CNN
F 2 "Crystals:Crystal_SMD_SeikoEpson_FA238-4pin_3.2x2.5mm_HandSoldering" H 5300 4000 50  0001 C CNN
F 3 "" H 5300 4000 50  0001 C CNN
F 4 "FA-238 16.0000MB-C3" H 5300 4000 60  0001 C CNN "Part Number"
	1    5300 4000
	0    -1   -1   0   
$EndComp
$Comp
L C C4
U 1 1 5B85334A
P 5550 3650
F 0 "C4" H 5575 3750 50  0000 L CNN
F 1 "18pF" H 5575 3550 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5588 3500 50  0001 C CNN
F 3 "" H 5550 3650 50  0001 C CNN
F 4 "06031A180KAT2A" H 5550 3650 60  0001 C CNN "Part Number"
	1    5550 3650
	0    1    1    0   
$EndComp
$Comp
L C C5
U 1 1 5B85341A
P 5550 4400
F 0 "C5" H 5575 4500 50  0000 L CNN
F 1 "18pF" H 5575 4300 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5588 4250 50  0001 C CNN
F 3 "" H 5550 4400 50  0001 C CNN
F 4 "06031A180KAT2A" H 5550 4400 60  0001 C CNN "Part Number"
	1    5550 4400
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5300 4150 5300 4400
Wire Wire Line
	5300 4400 5400 4400
Wire Wire Line
	5300 3850 5300 3650
Wire Wire Line
	5300 3650 5400 3650
Wire Wire Line
	5700 3650 5800 3650
Wire Wire Line
	5800 3650 5800 4400
Wire Wire Line
	5800 4400 5700 4400
Wire Wire Line
	5500 4000 6050 4000
Connection ~ 5800 4000
$Comp
L GND #PWR06
U 1 1 5B853598
P 6050 4000
F 0 "#PWR06" H 6050 3750 50  0001 C CNN
F 1 "GND" H 6050 3850 50  0000 C CNN
F 2 "" H 6050 4000 50  0001 C CNN
F 3 "" H 6050 4000 50  0001 C CNN
	1    6050 4000
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4550 3750 5300 3750
Connection ~ 5300 3750
Wire Wire Line
	4550 4300 5300 4300
Connection ~ 5300 4300
$Comp
L GND #PWR07
U 1 1 5B8539B2
P 4800 4000
F 0 "#PWR07" H 4800 3750 50  0001 C CNN
F 1 "GND" H 4800 3850 50  0000 C CNN
F 2 "" H 4800 4000 50  0001 C CNN
F 3 "" H 4800 4000 50  0001 C CNN
	1    4800 4000
	0    1    1    0   
$EndComp
Wire Wire Line
	4800 4000 5100 4000
Wire Wire Line
	4550 3750 4550 3950
Wire Wire Line
	4550 3950 3950 3950
Wire Wire Line
	3950 4050 4550 4050
Wire Wire Line
	4550 4050 4550 4300
Text Label 4150 3950 0    60   ~ 0
XTAL+
Text Label 4150 4050 0    60   ~ 0
XTAL-
$Comp
L NCP1117-5.0_SOT223 U2
U 1 1 5B853B4E
P 7000 1000
F 0 "U2" H 6850 1125 50  0000 C CNN
F 1 "NCP1117-5.0_SOT223" H 7000 1125 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-223-3Lead_TabPin2" H 7000 1200 50  0001 C CNN
F 3 "" H 7100 750 50  0001 C CNN
	1    7000 1000
	1    0    0    -1  
$EndComp
$Comp
L C C6
U 1 1 5B853CF5
P 6400 1250
F 0 "C6" H 6425 1350 50  0000 L CNN
F 1 "100nF" H 6425 1150 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 6438 1100 50  0001 C CNN
F 3 "" H 6400 1250 50  0001 C CNN
F 4 "06035C104KAT2A" H 6400 1250 60  0001 C CNN "Part Number"
	1    6400 1250
	1    0    0    -1  
$EndComp
$Comp
L C C8
U 1 1 5B853DED
P 7850 1300
F 0 "C8" H 7875 1400 50  0000 L CNN
F 1 "22uF" H 7875 1200 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 7888 1150 50  0001 C CNN
F 3 "" H 7850 1300 50  0001 C CNN
	1    7850 1300
	1    0    0    -1  
$EndComp
$Comp
L C C7
U 1 1 5B853E2B
P 7500 1300
F 0 "C7" H 7525 1400 50  0000 L CNN
F 1 "100nF" H 7525 1200 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 7538 1150 50  0001 C CNN
F 3 "" H 7500 1300 50  0001 C CNN
F 4 "06035C104KAT2A" H 7500 1300 60  0001 C CNN "Part Number"
	1    7500 1300
	1    0    0    -1  
$EndComp
$Comp
L +BATT #PWR08
U 1 1 5B853FF4
P 5600 850
F 0 "#PWR08" H 5600 700 50  0001 C CNN
F 1 "+BATT" H 5600 990 50  0000 C CNN
F 2 "" H 5600 850 50  0001 C CNN
F 3 "" H 5600 850 50  0001 C CNN
	1    5600 850 
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 850  5600 1000
Wire Wire Line
	5600 1000 6700 1000
Wire Wire Line
	6400 1100 6400 1000
Connection ~ 6400 1000
Wire Wire Line
	7300 1000 8300 1000
Wire Wire Line
	7500 1000 7500 1150
Wire Wire Line
	7850 1000 7850 1150
Connection ~ 7500 1000
Wire Wire Line
	8300 1000 8300 800 
Connection ~ 7850 1000
$Comp
L +5V #PWR09
U 1 1 5B854218
P 8300 800
F 0 "#PWR09" H 8300 650 50  0001 C CNN
F 1 "+5V" H 8300 940 50  0000 C CNN
F 2 "" H 8300 800 50  0001 C CNN
F 3 "" H 8300 800 50  0001 C CNN
	1    8300 800 
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR010
U 1 1 5B854242
P 6400 1550
F 0 "#PWR010" H 6400 1300 50  0001 C CNN
F 1 "GND" H 6400 1400 50  0000 C CNN
F 2 "" H 6400 1550 50  0001 C CNN
F 3 "" H 6400 1550 50  0001 C CNN
	1    6400 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 1400 6400 1550
Wire Wire Line
	6400 1450 7000 1450
Wire Wire Line
	7000 1300 7000 1500
Connection ~ 6400 1450
Wire Wire Line
	7000 1500 7850 1500
Wire Wire Line
	7500 1500 7500 1450
Connection ~ 7000 1450
Wire Wire Line
	7850 1500 7850 1450
Connection ~ 7500 1500
Wire Wire Line
	1650 3650 1650 4050
Connection ~ 1650 4050
Connection ~ 1650 3650
$Comp
L +BATT #PWR011
U 1 1 5B85479A
P 10050 4100
F 0 "#PWR011" H 10050 3950 50  0001 C CNN
F 1 "+BATT" H 10050 4240 50  0000 C CNN
F 2 "" H 10050 4100 50  0001 C CNN
F 3 "" H 10050 4100 50  0001 C CNN
	1    10050 4100
	-1   0    0    -1  
$EndComp
$Comp
L +5V #PWR012
U 1 1 5B854806
P 9600 3850
F 0 "#PWR012" H 9600 3700 50  0001 C CNN
F 1 "+5V" H 9600 3990 50  0000 C CNN
F 2 "" H 9600 3850 50  0001 C CNN
F 3 "" H 9600 3850 50  0001 C CNN
	1    9600 3850
	-1   0    0    -1  
$EndComp
Wire Wire Line
	10050 4150 10050 4100
Wire Wire Line
	9550 4150 10050 4150
Wire Wire Line
	9600 3850 9600 3950
Wire Wire Line
	9600 3950 9550 3950
$Comp
L GND #PWR013
U 1 1 5B85498D
P 9700 4050
F 0 "#PWR013" H 9700 3800 50  0001 C CNN
F 1 "GND" H 9700 3900 50  0000 C CNN
F 2 "" H 9700 4050 50  0001 C CNN
F 3 "" H 9700 4050 50  0001 C CNN
	1    9700 4050
	0    -1   1    0   
$EndComp
Wire Wire Line
	9700 4050 9550 4050
Wire Wire Line
	3950 1150 4250 1150
Wire Wire Line
	3950 1250 4250 1250
Wire Wire Line
	3950 1350 4250 1350
Wire Wire Line
	3950 1450 4250 1450
Wire Wire Line
	3950 1550 4250 1550
Wire Wire Line
	3950 1650 4250 1650
Wire Wire Line
	3950 1750 4250 1750
Wire Wire Line
	3950 1850 4250 1850
Wire Wire Line
	3950 2050 4250 2050
Wire Wire Line
	3950 2150 4250 2150
Wire Wire Line
	2800 6300 3100 6300
Wire Wire Line
	2800 6500 3100 6500
Wire Wire Line
	3950 2450 4250 2450
Wire Wire Line
	3950 2550 4250 2550
Wire Wire Line
	3950 2650 4250 2650
Wire Wire Line
	3950 2750 4250 2750
Wire Wire Line
	3950 2950 4250 2950
Wire Wire Line
	3950 3050 4250 3050
Wire Wire Line
	3950 3150 4250 3150
Wire Wire Line
	3950 3250 4250 3250
Wire Wire Line
	3950 3350 4250 3350
Wire Wire Line
	3950 3450 4250 3450
Wire Wire Line
	3950 3550 4250 3550
Wire Wire Line
	3950 3650 4250 3650
Wire Wire Line
	3950 3850 4250 3850
Text Label 4250 1150 0    60   ~ 0
8
Text Label 4250 1250 0    60   ~ 0
9
Text Label 4250 3350 0    60   ~ 0
12
Text Label 4250 3450 0    60   ~ 0
13
Text Label 4250 3550 0    60   ~ 0
14
Text Label 4250 3650 0    60   ~ 0
15
Text Label 4250 1350 0    60   ~ 0
16
Text Label 4250 2450 0    60   ~ 0
17
Text Label 4250 2550 0    60   ~ 0
18
Text Label 4250 2650 0    60   ~ 0
22
Text Label 4250 1450 0    60   ~ 0
23
Text Label 4250 1550 0    60   ~ 0
24
Text Label 4250 2750 0    60   ~ 0
25
Text Label 4250 1650 0    60   ~ 0
26
Text Label 4250 1750 0    60   ~ 0
27
Text Label 4250 1850 0    60   ~ 0
28
Text Label 4250 2950 0    60   ~ 0
29
Text Label 4250 2050 0    60   ~ 0
30
Text Label 4250 3850 0    60   ~ 0
31
Text Label 4250 3050 0    60   ~ 0
32
Text Label 4250 3150 0    60   ~ 0
1
Text Label 4250 3250 0    60   ~ 0
2
Text Label 4250 2150 0    60   ~ 0
3
Text Label 3100 6300 0    60   ~ 0
CAN+
Text Label 3100 6500 0    60   ~ 0
CAN-
Wire Wire Line
	7800 5350 8100 5350
Wire Wire Line
	8150 5800 8150 5500
Wire Wire Line
	8650 5800 8650 5500
Wire Wire Line
	8950 5000 8650 5000
Wire Wire Line
	8950 4900 8650 4900
Wire Wire Line
	8650 4200 8650 4500
Wire Wire Line
	8550 4200 8550 4500
Wire Wire Line
	8450 4200 8450 4500
Wire Wire Line
	8250 4200 8250 4500
Wire Wire Line
	7800 5050 8100 5050
Wire Wire Line
	7800 5150 8100 5150
Wire Wire Line
	7800 5250 8100 5250
Wire Wire Line
	8950 5300 8650 5300
Wire Wire Line
	8950 5200 8650 5200
Wire Wire Line
	8950 5100 8650 5100
Wire Wire Line
	8750 4200 8750 4500
Wire Wire Line
	8350 4200 8350 4500
Wire Wire Line
	8050 4200 8050 4500
Wire Wire Line
	7800 4850 8100 4850
Wire Wire Line
	7800 4950 8100 4950
Wire Wire Line
	8250 5800 8250 5500
Wire Wire Line
	8350 5800 8350 5500
Wire Wire Line
	8450 5800 8450 5500
Wire Wire Line
	8550 5800 8550 5500
Text Label 8100 5350 0    60   ~ 0
8
Text Label 8150 5500 1    60   ~ 0
9
Text Label 8250 5500 1    60   ~ 0
12
Text Label 8350 5500 1    60   ~ 0
13
Text Label 8450 5500 1    60   ~ 0
14
Text Label 8550 5500 1    60   ~ 0
15
Text Label 8650 5500 1    60   ~ 0
16
Text Label 8650 5300 2    60   ~ 0
17
Text Label 8650 5200 2    60   ~ 0
18
Text Label 8650 5100 2    60   ~ 0
22
Text Label 8650 5000 2    60   ~ 0
23
Text Label 8650 4900 2    60   ~ 0
24
Text Label 8750 4500 3    60   ~ 0
25
Text Label 8650 4500 3    60   ~ 0
26
Text Label 8550 4500 3    60   ~ 0
27
Text Label 8450 4500 3    60   ~ 0
28
Text Label 8350 4500 3    60   ~ 0
29
Text Label 8250 4500 3    60   ~ 0
30
Text Label 8050 4500 3    60   ~ 0
32
Text Label 8100 4850 0    60   ~ 0
1
Text Label 8100 4950 0    60   ~ 0
2
Text Label 8100 5050 0    60   ~ 0
3
Text Label 8100 5250 0    60   ~ 0
CAN+
Text Label 8100 5150 0    60   ~ 0
CAN-
Text Label 8150 4500 3    60   ~ 0
31
Wire Wire Line
	8150 4200 8150 4500
$Comp
L Conn_02x03_Odd_Even J3
U 1 1 5B855ACC
P 8450 2550
F 0 "J3" H 8500 2750 50  0000 C CNN
F 1 "Conn_02x03_Odd_Even" H 8500 2350 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_2x03_Pitch2.54mm" H 8450 2550 50  0001 C CNN
F 3 "" H 8450 2550 50  0001 C CNN
	1    8450 2550
	1    0    0    -1  
$EndComp
Text Label 7350 3200 0    60   ~ 0
ICSP_MISO
Text Label 7350 3300 0    60   ~ 0
ICSP_SCK
Text Label 7350 3400 0    60   ~ 0
~ICSP_RESET
Text Label 9000 2550 0    60   ~ 0
ICSP_MOSI
$Comp
L GND #PWR014
U 1 1 5B855C2C
P 9150 2750
F 0 "#PWR014" H 9150 2500 50  0001 C CNN
F 1 "GND" H 9150 2600 50  0000 C CNN
F 2 "" H 9150 2750 50  0001 C CNN
F 3 "" H 9150 2750 50  0001 C CNN
	1    9150 2750
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR015
U 1 1 5B855C5C
P 9150 2250
F 0 "#PWR015" H 9150 2100 50  0001 C CNN
F 1 "+5V" H 9150 2390 50  0000 C CNN
F 2 "" H 9150 2250 50  0001 C CNN
F 3 "" H 9150 2250 50  0001 C CNN
	1    9150 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	8750 2450 9150 2450
Wire Wire Line
	9150 2450 9150 2250
Wire Wire Line
	8750 2550 9000 2550
Wire Wire Line
	8750 2650 9150 2650
Wire Wire Line
	9150 2650 9150 2750
Wire Wire Line
	7900 3200 7350 3200
Wire Wire Line
	7900 3300 7350 3300
$Comp
L D_Schottky D1
U 1 1 5B855F6F
P 6800 2500
F 0 "D1" H 6800 2600 50  0000 C CNN
F 1 "D_Schottky" H 6800 2400 50  0000 C CNN
F 2 "Diodes_SMD:D_SOT-23_ANK" H 6800 2500 50  0001 C CNN
F 3 "" H 6800 2500 50  0001 C CNN
F 4 "BAT54" H 6800 2500 60  0001 C CNN "Part Number"
	1    6800 2500
	0    1    1    0   
$EndComp
$Comp
L SW_Push SW1
U 1 1 5B855FC1
P 6100 2750
F 0 "SW1" H 6150 2850 50  0000 L CNN
F 1 "SW_Push" H 6100 2690 50  0000 C CNN
F 2 "Buttons_Switches_THT:SW_PUSH_6mm" H 6100 2950 50  0001 C CNN
F 3 "" H 6100 2950 50  0001 C CNN
F 4 "1825910-6" H 6100 2750 60  0001 C CNN "Part Number"
	1    6100 2750
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 5B85604C
P 7050 2500
F 0 "R1" V 7130 2500 50  0000 C CNN
F 1 "100K" V 7050 2500 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 6980 2500 50  0001 C CNN
F 3 "" H 7050 2500 50  0001 C CNN
F 4 "RMCF0603FT10K0" H 7050 2500 60  0001 C CNN "Part Number"
	1    7050 2500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR016
U 1 1 5B8561C9
P 5800 2900
F 0 "#PWR016" H 5800 2650 50  0001 C CNN
F 1 "GND" H 5800 2750 50  0000 C CNN
F 2 "" H 5800 2900 50  0001 C CNN
F 3 "" H 5800 2900 50  0001 C CNN
	1    5800 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 2900 5800 2750
Wire Wire Line
	5800 2750 5900 2750
Wire Wire Line
	6300 2750 7450 2750
Wire Wire Line
	6800 2750 6800 2650
Wire Wire Line
	7050 2750 7050 2650
Connection ~ 6800 2750
Wire Wire Line
	7450 2750 7450 2650
Connection ~ 7050 2750
$Comp
L +5V #PWR017
U 1 1 5B85645E
P 6950 2150
F 0 "#PWR017" H 6950 2000 50  0001 C CNN
F 1 "+5V" H 6950 2290 50  0000 C CNN
F 2 "" H 6950 2150 50  0001 C CNN
F 3 "" H 6950 2150 50  0001 C CNN
	1    6950 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	6800 2150 7050 2150
Wire Wire Line
	6800 2150 6800 2350
Wire Wire Line
	7050 2150 7050 2350
Connection ~ 6950 2150
Text Label 7700 2450 0    60   ~ 0
ICSP_MISO
Text Label 7700 2550 0    60   ~ 0
ICSP_SCK
Text Label 7700 2650 0    60   ~ 0
~ICSP_RESET
Wire Wire Line
	8250 2450 7700 2450
Wire Wire Line
	8250 2550 7700 2550
Wire Wire Line
	7450 2650 8250 2650
Text Label 7350 3500 0    60   ~ 0
ICSP_MOSI
Wire Wire Line
	7350 3400 7900 3400
Wire Wire Line
	7350 3500 7900 3500
Text Label 7900 3200 0    60   ~ 0
1
Text Label 7900 3500 0    60   ~ 0
2
Text Label 7900 3300 0    60   ~ 0
12
Text Label 7900 3400 0    60   ~ 0
31
Wire Wire Line
	3950 2250 4250 2250
Wire Wire Line
	4250 2350 3950 2350
Text Label 4250 2250 0    60   ~ 0
TXCAN
Text Label 4250 2350 0    60   ~ 0
RXCAN
$Comp
L MCP2561-E/SN U3
U 1 1 5B87A3D6
P 2300 6400
F 0 "U3" H 1900 6750 50  0000 L CNN
F 1 "MCP2561-E/SN" H 2400 6750 50  0000 L CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 2300 5900 50  0001 C CIN
F 3 "" H 2300 6400 50  0001 C CNN
F 4 "MCP2561-E/SN" H 2300 6400 60  0001 C CNN "Part Number"
	1    2300 6400
	1    0    0    -1  
$EndComp
NoConn ~ 2800 6400
Wire Wire Line
	1800 6600 1550 6600
Wire Wire Line
	1550 6600 1550 7000
Wire Wire Line
	1550 7000 2300 7000
Wire Wire Line
	2300 6800 2300 7200
$Comp
L GND #PWR018
U 1 1 5B87A4DF
P 2300 7200
F 0 "#PWR018" H 2300 6950 50  0001 C CNN
F 1 "GND" H 2300 7050 50  0000 C CNN
F 2 "" H 2300 7200 50  0001 C CNN
F 3 "" H 2300 7200 50  0001 C CNN
	1    2300 7200
	1    0    0    -1  
$EndComp
Connection ~ 2300 7000
Text Label 1500 6200 2    60   ~ 0
TXCAN
Text Label 1500 6300 2    60   ~ 0
RXCAN
Wire Wire Line
	1500 6200 1800 6200
Wire Wire Line
	1500 6300 1800 6300
$Comp
L C C9
U 1 1 5B87A770
P 1950 5550
F 0 "C9" H 1975 5650 50  0000 L CNN
F 1 "100nF" H 1975 5450 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 1988 5400 50  0001 C CNN
F 3 "" H 1950 5550 50  0001 C CNN
F 4 "06035C104KAT2A" H 1950 5550 60  0001 C CNN "Part Number"
	1    1950 5550
	1    0    0    -1  
$EndComp
Wire Wire Line
	1950 5150 1950 5400
Wire Wire Line
	1950 5300 2300 5300
Wire Wire Line
	2300 5300 2300 6000
$Comp
L GND #PWR019
U 1 1 5B87A93E
P 1950 5800
F 0 "#PWR019" H 1950 5550 50  0001 C CNN
F 1 "GND" H 1950 5650 50  0000 C CNN
F 2 "" H 1950 5800 50  0001 C CNN
F 3 "" H 1950 5800 50  0001 C CNN
	1    1950 5800
	1    0    0    -1  
$EndComp
Wire Wire Line
	1950 5800 1950 5700
$Comp
L +5V #PWR020
U 1 1 5B87ABCF
P 1950 5150
F 0 "#PWR020" H 1950 5000 50  0001 C CNN
F 1 "+5V" H 1950 5290 50  0000 C CNN
F 2 "" H 1950 5150 50  0001 C CNN
F 3 "" H 1950 5150 50  0001 C CNN
	1    1950 5150
	1    0    0    -1  
$EndComp
Connection ~ 1950 5300
$Comp
L Conn_01x08 J1
U 1 1 5B87ADAC
P 8350 4000
F 0 "J1" H 8350 4400 50  0000 C CNN
F 1 "Conn_01x08" H 8350 3500 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x08_Pitch2.54mm" H 8350 4000 50  0001 C CNN
F 3 "" H 8350 4000 50  0001 C CNN
	1    8350 4000
	0    -1   -1   0   
$EndComp
$Comp
L Conn_01x03 J2
U 1 1 5B87AE13
P 9350 4050
F 0 "J2" H 9350 4250 50  0000 C CNN
F 1 "Conn_01x03" H 9350 3850 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x03_Pitch2.54mm" H 9350 4050 50  0001 C CNN
F 3 "" H 9350 4050 50  0001 C CNN
	1    9350 4050
	-1   0    0    -1  
$EndComp
$Comp
L Conn_01x06 J4
U 1 1 5B87AE6C
P 7600 5050
F 0 "J4" H 7600 5350 50  0000 C CNN
F 1 "Conn_01x06" H 7600 4650 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x06_Pitch2.54mm" H 7600 5050 50  0001 C CNN
F 3 "" H 7600 5050 50  0001 C CNN
	1    7600 5050
	-1   0    0    -1  
$EndComp
$Comp
L Conn_01x06 J6
U 1 1 5B87AED8
P 8450 6000
F 0 "J6" H 8450 6300 50  0000 C CNN
F 1 "Conn_01x06" H 8450 5600 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x06_Pitch2.54mm" H 8450 6000 50  0001 C CNN
F 3 "" H 8450 6000 50  0001 C CNN
	1    8450 6000
	0    1    1    0   
$EndComp
$Comp
L Conn_01x05 J5
U 1 1 5B87B104
P 9150 5100
F 0 "J5" H 9150 5400 50  0000 C CNN
F 1 "Conn_01x05" H 9150 4800 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x05_Pitch2.54mm" H 9150 5100 50  0001 C CNN
F 3 "" H 9150 5100 50  0001 C CNN
	1    9150 5100
	1    0    0    -1  
$EndComp
$EndSCHEMATC
