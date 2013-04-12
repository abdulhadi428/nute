EESchema Schematic File Version 2  date 11/04/2013 13:11:15
LIBS:Connectors_kl
LIBS:pcb_details
LIBS:power
LIBS:Tittar_kl
LIBS:memory
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date "11 apr 2013"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
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
	3000 2450 3300 2450
Wire Wire Line
	3150 2550 3000 2550
Wire Wire Line
	3000 2650 3300 2650
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
Text Label 5600 2600 0    60   ~ 0
3
Text Label 5600 2500 0    60   ~ 0
2
Text Label 5600 2400 0    60   ~ 0
1
$Comp
L FUSE0R F3
U 1 1 50571311
P 5200 2750
F 0 "F3" V 5130 2750 50  0000 C CNN
F 1 "FUSE0R" V 5200 2750 50  0000 C CNN
F 2 "" H 5200 2750 60  0001 C CNN
F 3 "" H 5200 2750 60  0001 C CNN
	1    5200 2750
	0    1    1    0   
$EndComp
$Comp
L FUSE0R F2
U 1 1 5057130E
P 5200 2500
F 0 "F2" V 5130 2500 50  0000 C CNN
F 1 "FUSE0R" V 5200 2500 50  0000 C CNN
F 2 "" H 5200 2500 60  0001 C CNN
F 3 "" H 5200 2500 60  0001 C CNN
	1    5200 2500
	0    1    1    0   
$EndComp
$Comp
L FUSE0R F1
U 1 1 505712FC
P 5200 2250
F 0 "F1" V 5130 2250 50  0000 C CNN
F 1 "FUSE0R" V 5200 2250 50  0000 C CNN
F 2 "" H 5200 2250 60  0001 C CNN
F 3 "" H 5200 2250 60  0001 C CNN
	1    5200 2250
	0    1    1    0   
$EndComp
Text Notes 4850 1700 0    60   ~ 0
I2C Addr = 1010 (A2)(A1)(A0) in case of 1 or 2 kBit,\nI2C Addr = 1010 (A2)(A1) in case of 4 kBit.\nA2, A1, A0 are read as 0 when left floating
$Comp
L EEPROM_I2C DD1
U 1 1 504343E1
P 6200 2400
F 0 "DD1" H 5850 2750 60  0000 C CNN
F 1 "M24C04" H 6300 2750 60  0000 C CNN
F 2 "SO8E" H 6200 2000 60  0000 C CNN
F 3 "" H 6200 2400 60  0001 C CNN
	1    6200 2400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR01
U 1 1 504343DF
P 6600 3050
F 0 "#PWR01" H 6690 3030 30  0001 C CNN
F 1 "GND" H 6600 2970 30  0001 C CNN
F 2 "" H 6600 3050 60  0001 C CNN
F 3 "" H 6600 3050 60  0001 C CNN
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
F 3 "" H 6850 2750 60  0001 C CNN
	1    6850 2750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 504343DD
P 6850 3050
F 0 "#PWR02" H 6940 3030 30  0001 C CNN
F 1 "GND" H 6850 2970 30  0001 C CNN
F 2 "" H 6850 3050 60  0001 C CNN
F 3 "" H 6850 3050 60  0001 C CNN
	1    6850 3050
	1    0    0    -1  
$EndComp
Text Label 6700 2300 0    60   ~ 0
I2C_SCL
Text Label 6700 2200 0    60   ~ 0
I2C_SDA
$Comp
L GND #PWR03
U 1 1 504343DB
P 5700 2200
F 0 "#PWR03" H 5790 2180 30  0001 C CNN
F 1 "GND" H 5700 2120 30  0001 C CNN
F 2 "" H 5700 2200 60  0001 C CNN
F 3 "" H 5700 2200 60  0001 C CNN
	1    5700 2200
	0    -1   -1   0   
$EndComp
Text Label 3300 2450 0    60   ~ 0
I2C_SCL
Text Label 3300 2650 0    60   ~ 0
I2C_SDA
$Comp
L GND #PWR04
U 1 1 4F7C750B
P 3150 2550
F 0 "#PWR04" H 3240 2530 30  0001 C CNN
F 1 "GND" H 3150 2470 30  0001 C CNN
F 2 "" H 3150 2550 60  0001 C CNN
F 3 "" H 3150 2550 60  0001 C CNN
	1    3150 2550
	0    -1   -1   0   
$EndComp
$Comp
L CONN_4 XL1
U 1 1 51665C75
P 2800 2500
F 0 "XL1" V 2750 2500 40  0000 C CNN
F 1 "CONN_4" V 2850 2500 40  0000 C CNN
F 2 "" H 2800 2500 60  0000 C CNN
F 3 "" H 2800 2500 60  0000 C CNN
	1    2800 2500
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR05
U 1 1 51665C93
P 3150 2350
F 0 "#PWR05" H 3150 2310 30  0001 C CNN
F 1 "+3.3V" H 3230 2380 30  0000 C CNN
F 2 "" H 3150 2350 60  0000 C CNN
F 3 "" H 3150 2350 60  0000 C CNN
	1    3150 2350
	1    0    0    -1  
$EndComp
Wire Wire Line
	3150 2350 3000 2350
$Comp
L +3.3V #PWR06
U 1 1 51665FA4
P 4600 2500
F 0 "#PWR06" H 4600 2460 30  0001 C CNN
F 1 "+3.3V" H 4680 2530 30  0000 C CNN
F 2 "" H 4600 2500 60  0000 C CNN
F 3 "" H 4600 2500 60  0000 C CNN
	1    4600 2500
	-1   0    0    -1  
$EndComp
$Comp
L +3.3V #PWR07
U 1 1 51665FB3
P 6950 2500
F 0 "#PWR07" H 6950 2460 30  0001 C CNN
F 1 "+3.3V" H 7030 2530 30  0000 C CNN
F 2 "" H 6950 2500 60  0000 C CNN
F 3 "" H 6950 2500 60  0000 C CNN
	1    6950 2500
	1    0    0    -1  
$EndComp
$Comp
L LED D1
U 1 1 51666FE5
P 4150 3350
F 0 "D1" H 4250 3490 50  0000 C CNN
F 1 "LED" H 4070 3490 50  0000 C CNN
F 2 "" H 4150 3350 60  0000 C CNN
F 3 "" H 4150 3350 60  0000 C CNN
	1    4150 3350
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 51666FF4
P 4750 3350
F 0 "R1" V 4830 3200 50  0000 C CNN
F 1 "1k" V 4750 3350 50  0000 C CNN
F 2 "RES_0603" V 4830 3450 28  0000 C CNN
F 3 "" H 4750 3350 60  0000 C CNN
	1    4750 3350
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5000 3350 5100 3350
$Comp
L GND #PWR08
U 1 1 51667002
P 5100 3350
F 0 "#PWR08" H 5190 3330 30  0001 C CNN
F 1 "GND" H 5100 3270 30  0001 C CNN
F 2 "" H 5100 3350 60  0001 C CNN
F 3 "" H 5100 3350 60  0001 C CNN
	1    5100 3350
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4500 3350 4350 3350
$Comp
L +3.3V #PWR09
U 1 1 51667022
P 3800 3350
F 0 "#PWR09" H 3800 3310 30  0001 C CNN
F 1 "+3.3V" H 3880 3380 30  0000 C CNN
F 2 "" H 3800 3350 60  0000 C CNN
F 3 "" H 3800 3350 60  0000 C CNN
	1    3800 3350
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3800 3350 3950 3350
$EndSCHEMATC
