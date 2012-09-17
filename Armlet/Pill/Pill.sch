EESchema Schematic File Version 2  date 17/09/2012 16:52:04
LIBS:Connectors_kl
LIBS:pcb_details
LIBS:power
LIBS:Tittar_kl
LIBS:memory
EELAYER 25  0
EELAYER END
$Descr A4 11700 8267
encoding utf-8
Sheet 1 1
Title ""
Date "17 sep 2012"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	5700 4000 5850 4000
Wire Wire Line
	5800 2600 5450 2600
Connection ~ 4900 2500
Wire Wire Line
	4950 2750 4900 2750
Wire Wire Line
	5450 2250 5450 2400
Wire Wire Line
	5450 2400 5800 2400
Wire Wire Line
	4600 2500 4950 2500
Wire Wire Line
	2800 2800 3100 2800
Wire Wire Line
	2800 2500 3100 2500
Wire Wire Line
	3100 2300 2800 2300
Wire Wire Line
	2800 2100 3100 2100
Wire Wire Line
	2800 1900 2950 1900
Wire Wire Line
	2950 2000 2800 2000
Wire Wire Line
	2800 2200 3100 2200
Wire Wire Line
	3100 2400 2800 2400
Wire Wire Line
	2800 2600 3100 2600
Wire Wire Line
	2800 2700 3100 2700
Wire Wire Line
	6600 2300 6700 2300
Wire Wire Line
	6700 2200 6600 2200
Wire Wire Line
	6850 2950 6850 3050
Wire Wire Line
	6600 2500 6950 2500
Wire Wire Line
	6600 2600 6600 3050
Wire Wire Line
	6850 2550 6850 2500
Connection ~ 6850 2500
Wire Wire Line
	5800 2200 5700 2200
Wire Wire Line
	5450 2500 5800 2500
Wire Wire Line
	5450 2600 5450 2750
Wire Wire Line
	4900 2750 4900 2250
Wire Wire Line
	4900 2250 4950 2250
Wire Wire Line
	4950 4000 5200 4000
Wire Wire Line
	6250 4000 6500 4000
Text Label 5600 2600 0    60   ~ 0
3
Text Label 5600 2500 0    60   ~ 0
2
Text Label 5600 2400 0    60   ~ 0
1
Text Label 4950 4000 0    60   ~ 0
PB0
$Comp
L GND #PWR01
U 1 1 50571377
P 6500 4000
F 0 "#PWR01" H 6590 3980 30  0001 C CNN
F 1 "GND" H 6500 3920 30  0001 C CNN
	1    6500 4000
	0    -1   -1   0   
$EndComp
$Comp
L R R1
U 1 1 50571363
P 5450 4000
F 0 "R1" V 5530 4000 50  0000 C CNN
F 1 "100R" V 5450 4000 50  0000 C CNN
F 2 "RES_0603" V 5380 4000 28  0000 C CNN
	1    5450 4000
	0    -1   -1   0   
$EndComp
$Comp
L LED D1
U 1 1 50571355
P 6050 4000
F 0 "D1" H 6150 4140 50  0000 C CNN
F 1 "LED" H 5970 4140 50  0000 C CNN
	1    6050 4000
	1    0    0    -1  
$EndComp
$Comp
L FUSE0R F3
U 1 1 50571311
P 5200 2750
F 0 "F3" V 5130 2750 50  0000 C CNN
F 1 "FUSE0R" V 5200 2750 50  0000 C CNN
	1    5200 2750
	0    1    1    0   
$EndComp
$Comp
L FUSE0R F2
U 1 1 5057130E
P 5200 2500
F 0 "F2" V 5130 2500 50  0000 C CNN
F 1 "FUSE0R" V 5200 2500 50  0000 C CNN
	1    5200 2500
	0    1    1    0   
$EndComp
$Comp
L FUSE0R F1
U 1 1 505712FC
P 5200 2250
F 0 "F1" V 5130 2250 50  0000 C CNN
F 1 "FUSE0R" V 5200 2250 50  0000 C CNN
	1    5200 2250
	0    1    1    0   
$EndComp
Text Notes 4850 1800 0    60   ~ 0
I2C Addr = 1010 (A2)(A1)(A0)\nA2, A1, A0 are read as 0 when left floating
Text Label 4600 2500 0    60   ~ 0
PB2
Text Label 6950 2500 0    60   ~ 0
PB2
$Comp
L CONN_10 XL1
U 1 1 4FCBBD2A
P 2600 2350
F 0 "XL1" V 2550 2350 50  0000 C CNN
F 1 "CONN_10" V 2650 2350 50  0000 C CNN
	1    2600 2350
	-1   0    0    -1  
$EndComp
$Comp
L EEPROM_I2C DD1
U 1 1 504343E1
P 6200 2400
F 0 "DD1" H 5850 2750 60  0000 C CNN
F 1 "M24C02" H 6300 2750 60  0000 C CNN
F 2 "TSSOP8" H 6200 2000 60  0000 C CNN
	1    6200 2400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 504343DF
P 6600 3050
F 0 "#PWR03" H 6690 3030 30  0001 C CNN
F 1 "GND" H 6600 2970 30  0001 C CNN
	1    6600 3050
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 504343DE
P 6850 2750
F 0 "C1" H 6750 2650 50  0000 L CNN
F 1 "0.1u" H 6750 2850 50  0000 L CNN
F 2 "CAP_0603" V 6950 2600 28  0000 C BNN
	1    6850 2750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR04
U 1 1 504343DD
P 6850 3050
F 0 "#PWR04" H 6940 3030 30  0001 C CNN
F 1 "GND" H 6850 2970 30  0001 C CNN
	1    6850 3050
	1    0    0    -1  
$EndComp
Text Label 6700 2300 0    60   ~ 0
I2C_SCL
Text Label 6700 2200 0    60   ~ 0
I2C_SDA
$Comp
L GND #PWR05
U 1 1 504343DB
P 5700 2200
F 0 "#PWR05" H 5790 2180 30  0001 C CNN
F 1 "GND" H 5700 2120 30  0001 C CNN
	1    5700 2200
	0    -1   -1   0   
$EndComp
Text Label 2950 1900 0    60   ~ 0
PB2
Text Label 3100 2500 0    60   ~ 0
I2C_SCL
Text Label 3100 2600 0    60   ~ 0
I2C_SDA
Text Label 3100 2800 0    60   ~ 0
PB1
Text Label 3100 2700 0    60   ~ 0
PB0
Text Label 3100 2300 0    60   ~ 0
TX1
Text Label 3100 2400 0    60   ~ 0
RX1
Text Label 3100 2200 0    60   ~ 0
SWDIO
Text Label 3100 2100 0    60   ~ 0
SWCLK
$Comp
L GND #PWR06
U 1 1 4F7C750B
P 2950 2000
F 0 "#PWR06" H 3040 1980 30  0001 C CNN
F 1 "GND" H 2950 1920 30  0001 C CNN
	1    2950 2000
	0    -1   -1   0   
$EndComp
$EndSCHEMATC
