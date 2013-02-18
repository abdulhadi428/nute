EESchema Schematic File Version 2  date 2/18/2013 10:31:40 PM
LIBS:Connectors_kl
LIBS:pcb_details
LIBS:power
LIBS:Power_kl
LIBS:RF ICs
LIBS:st_kl
LIBS:Tittar_kl
LIBS:Transistors_kl
LIBS:Sensors
EELAYER 25  0
EELAYER END
$Descr A4 11700 8267
encoding utf-8
Sheet 1 1
Title ""
Date "18 feb 2013"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	4650 3650 4650 3750
Wire Wire Line
	4650 3550 4850 3550
Wire Wire Line
	4650 3350 5350 3350
Wire Wire Line
	6750 3350 6900 3350
Wire Wire Line
	6750 3500 7350 3500
Wire Wire Line
	7350 3500 7350 3800
Wire Wire Line
	7100 4200 7100 4350
Connection ~ 7100 3650
Wire Wire Line
	7100 3800 7100 3650
Wire Wire Line
	6750 3750 6900 3750
Wire Wire Line
	5750 4350 5750 4200
Wire Wire Line
	5850 4200 5850 4250
Wire Wire Line
	5850 4250 5650 4250
Wire Wire Line
	5650 4250 5650 4200
Connection ~ 5750 4250
Wire Wire Line
	6850 3800 6850 3750
Connection ~ 6850 3750
Wire Wire Line
	6750 3650 7150 3650
Wire Wire Line
	6850 4200 6850 4350
Wire Wire Line
	7350 4200 7350 4350
Wire Wire Line
	5350 3600 4850 3600
Wire Wire Line
	4850 3600 4850 3550
Wire Wire Line
	4650 3450 5350 3450
Wire Wire Line
	4700 3250 4650 3250
$Comp
L +3.3V #PWR2
U 1 1 51227351
P 4700 3250
F 0 "#PWR2" H 4700 3210 30  0001 C CNN
F 1 "+3.3V" H 4780 3280 30  0000 C CNN
	1    4700 3250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR1
U 1 1 51227349
P 4650 3750
F 0 "#PWR1" H 4740 3730 30  0001 C CNN
F 1 "GND" H 4650 3670 30  0001 C CNN
	1    4650 3750
	1    0    0    -1  
$EndComp
$Comp
L CONN_5 XL1
U 1 1 51227313
P 4450 3450
F 0 "XL1" V 4400 3450 50  0000 C CNN
F 1 "CONN_5" V 4500 3450 50  0000 C CNN
	1    4450 3450
	-1   0    0    -1  
$EndComp
Text Label 4900 3350 0    60   ~ 0
I2C_SCL
Text Label 4900 3450 0    60   ~ 0
I2C_SDA
$Comp
L MMA8452Q DD1
U 1 1 4F2EAAC5
P 6050 3700
F 0 "DD1" H 6400 4200 60  0000 C CNN
F 1 "MMA8452Q" H 5800 4200 60  0000 C CNN
	1    6050 3700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR3
U 1 1 4F2EAAC4
P 5750 4350
F 0 "#PWR3" H 5840 4330 30  0001 C CNN
F 1 "GND" H 5750 4270 30  0001 C CNN
	1    5750 4350
	1    0    0    -1  
$EndComp
Text Label 4900 3600 0    60   ~ 0
ACC_INT1
$Comp
L C C1
U 1 1 4F2EAAC3
P 6850 4000
F 0 "C1" H 6900 4100 50  0000 L CNN
F 1 "10u" H 6900 3900 50  0000 L CNN
	1    6850 4000
	1    0    0    -1  
$EndComp
$Comp
L C C2
U 1 1 4F2EAAC2
P 7100 4000
F 0 "C2" H 7150 4100 50  0000 L CNN
F 1 "0.1u" H 7150 3900 50  0000 L CNN
	1    7100 4000
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 4F2EAAC1
P 7350 4000
F 0 "C3" H 7400 4100 50  0000 L CNN
F 1 "0.1u" H 7400 3900 50  0000 L CNN
	1    7350 4000
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR6
U 1 1 4F2EAAC0
P 6900 3750
F 0 "#PWR6" H 6900 3710 30  0001 C CNN
F 1 "+3.3V" H 6980 3780 30  0000 C CNN
	1    6900 3750
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR8
U 1 1 4F2EAABF
P 7150 3650
F 0 "#PWR8" H 7150 3610 30  0001 C CNN
F 1 "+3.3V" H 7230 3680 30  0000 C CNN
	1    7150 3650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR4
U 1 1 4F2EAABE
P 6850 4350
F 0 "#PWR4" H 6940 4330 30  0001 C CNN
F 1 "GND" H 6850 4270 30  0001 C CNN
	1    6850 4350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR7
U 1 1 4F2EAABD
P 7100 4350
F 0 "#PWR7" H 7190 4330 30  0001 C CNN
F 1 "GND" H 7100 4270 30  0001 C CNN
	1    7100 4350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR9
U 1 1 4F2EAABC
P 7350 4350
F 0 "#PWR9" H 7440 4330 30  0001 C CNN
F 1 "GND" H 7350 4270 30  0001 C CNN
	1    7350 4350
	1    0    0    -1  
$EndComp
Text Notes 5750 2950 0    60   ~ 0
Accelerometer\nI2C addr = 0x1C
NoConn ~ 5350 3700
$Comp
L GND #PWR5
U 1 1 4F2EAAB2
P 6900 3350
F 0 "#PWR5" H 6990 3330 30  0001 C CNN
F 1 "GND" H 6900 3270 30  0001 C CNN
	1    6900 3350
	0    1    1    0   
$EndComp
$EndSCHEMATC
