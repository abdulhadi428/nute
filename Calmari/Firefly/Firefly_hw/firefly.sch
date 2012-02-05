EESchema Schematic File Version 2  date 05/02/2012 14:52:08
LIBS:Connectors_kl
LIBS:pcb_details
LIBS:power
LIBS:Power_kl
LIBS:Tittar_kl
LIBS:firefly-cache
EELAYER 25  0
EELAYER END
$Descr A4 11700 8267
encoding utf-8
Sheet 1 1
Title ""
Date "5 feb 2012"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Label 6650 3600 0    60   ~ 0
LedLow
Text Label 6300 3100 0    60   ~ 0
LOut
Text Label 4600 3100 0    60   ~ 0
In
Text Label 7150 3100 0    60   ~ 0
Out
Text Notes 5650 4150 0    60   ~ 0
1.2k =>18mA
Wire Wire Line
	4250 3100 5750 3100
Wire Wire Line
	5750 2750 5400 2750
Wire Wire Line
	5400 2750 5400 3600
Wire Wire Line
	5450 3850 5450 3750
Connection ~ 4950 3100
Wire Wire Line
	4950 3150 4950 3100
Wire Wire Line
	6450 3750 6450 3850
Connection ~ 7950 3100
Wire Wire Line
	8250 3150 8250 3100
Wire Wire Line
	8250 3100 7000 3100
Connection ~ 7600 3600
Wire Wire Line
	7600 3550 7600 3600
Connection ~ 7250 3100
Wire Wire Line
	7250 3150 7250 3100
Wire Wire Line
	7950 3100 7950 3150
Connection ~ 6500 3100
Wire Wire Line
	6600 3100 6150 3100
Wire Wire Line
	5400 3450 5450 3450
Wire Wire Line
	5400 3600 5450 3600
Connection ~ 5400 3450
Wire Wire Line
	6450 3450 6500 3450
Wire Wire Line
	7600 3150 7600 3100
Connection ~ 7600 3100
Wire Wire Line
	6450 3600 7950 3600
Wire Wire Line
	7950 3600 7950 3550
Wire Wire Line
	7250 3550 7250 3600
Connection ~ 7250 3600
Wire Wire Line
	8250 3550 8250 3850
Connection ~ 5400 3100
Wire Wire Line
	4950 3550 4950 3650
Wire Wire Line
	5450 4350 5450 4450
Wire Wire Line
	6150 2750 6500 2750
Wire Wire Line
	6500 2750 6500 3450
Connection ~ 4350 3600
Wire Wire Line
	4250 3600 4450 3600
Wire Wire Line
	4350 3050 4350 3100
Connection ~ 4350 3100
Wire Wire Line
	4250 4000 4350 4000
Wire Wire Line
	4350 4000 4350 3550
$Comp
L GND #PWR01
U 1 1 4F2E5205
P 4450 3600
F 0 "#PWR01" H 4540 3580 30  0001 C CNN
F 1 "GND" H 4450 3520 30  0001 C CNN
	1    4450 3600
	0    1    1    0   
$EndComp
$Comp
L CONN_1 XL2
U 1 1 4F2E5204
P 4050 3600
F 0 "XL2" V 4000 3610 40  0000 C CNN
F 1 "CONN_1" V 4060 3620 40  0000 C CNN
	1    4050 3600
	-1   0    0    1   
$EndComp
Text Notes 3500 3650 0    60   ~ 0
-BATTERY
$Comp
L PWR_FLAG #FLG02
U 1 1 4F2E5203
P 4350 3550
F 0 "#FLG02" H 4350 3820 30  0001 C CNN
F 1 "PWR_FLAG" H 4350 3780 30  0000 C CNN
	1    4350 3550
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG03
U 1 1 4F2E5202
P 4350 3050
F 0 "#FLG03" H 4350 3320 30  0001 C CNN
F 1 "PWR_FLAG" H 4350 3280 30  0000 C CNN
	1    4350 3050
	1    0    0    -1  
$EndComp
Text Notes 3500 3150 0    60   ~ 0
+BATTERY
$Comp
L CONN_1 XL1
U 1 1 4F2E5201
P 4050 3100
F 0 "XL1" V 4000 3110 40  0000 C CNN
F 1 "CONN_1" V 4060 3120 40  0000 C CNN
	1    4050 3100
	-1   0    0    1   
$EndComp
$Comp
L CONN_1 XL3
U 1 1 4F2E5200
P 4050 4000
F 0 "XL3" V 4000 4010 40  0000 C CNN
F 1 "CONN_1" V 4060 4020 40  0000 C CNN
	1    4050 4000
	-1   0    0    -1  
$EndComp
Text Notes 5350 2550 0    60   ~ 0
Inductors of different packages
$Comp
L L L1
U 1 1 4F2E50BA
P 5950 2750
F 0 "L1" V 5860 2660 40  0000 C CNN
F 1 "6.8uH" V 5850 2850 40  0000 C CNN
	1    5950 2750
	0    1    1    0   
$EndComp
Text Notes 7900 2800 0    60   ~ 0
ZENER for overvoltage\nprotection in case of\nopen circuit
Text Notes 7200 2800 0    60   ~ 0
LEDs of\ndifferent\npackages
$Comp
L GND #PWR04
U 1 1 4F2E504B
P 5450 4450
F 0 "#PWR04" H 5540 4430 30  0001 C CNN
F 1 "GND" H 5450 4370 30  0001 C CNN
	1    5450 4450
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 4F2E5045
P 5450 4100
F 0 "R1" V 5530 4100 50  0000 C CNN
F 1 "1.2k" V 5450 4100 50  0000 C CNN
	1    5450 4100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR05
U 1 1 4F2E5020
P 4950 3650
F 0 "#PWR05" H 5040 3630 30  0001 C CNN
F 1 "GND" H 4950 3570 30  0001 C CNN
	1    4950 3650
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 4F2E5006
P 4950 3350
F 0 "C1" H 5000 3450 50  0000 L CNN
F 1 "4.7u" H 5000 3250 50  0000 L CNN
	1    4950 3350
	1    0    0    -1  
$EndComp
$Comp
L C C2
U 1 1 4F2E4FD6
P 8250 3350
F 0 "C2" H 8300 3450 50  0000 L CNN
F 1 "1u" H 8300 3250 50  0000 L CNN
	1    8250 3350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR06
U 1 1 4F2E4FC8
P 8250 3850
F 0 "#PWR06" H 8340 3830 30  0001 C CNN
F 1 "GND" H 8250 3770 30  0001 C CNN
	1    8250 3850
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR07
U 1 1 4F2E4FC2
P 6450 3850
F 0 "#PWR07" H 6540 3830 30  0001 C CNN
F 1 "GND" H 6450 3770 30  0001 C CNN
	1    6450 3850
	1    0    0    -1  
$EndComp
$Comp
L ZENER D4
U 1 1 4F2E4FA0
P 7950 3350
F 0 "D4" H 7950 3450 40  0000 C CNN
F 1 "BZV55-C24" H 7950 3250 40  0000 C CNN
	1    7950 3350
	0    -1   -1   0   
$EndComp
$Comp
L LED D3
U 1 1 4F2E4F87
P 7600 3350
F 0 "D3" H 7700 3490 50  0000 C CNN
F 1 "LED" H 7520 3490 50  0000 C CNN
	1    7600 3350
	0    1    1    0   
$EndComp
$Comp
L LED D2
U 1 1 4F2E4F6F
P 7250 3350
F 0 "D2" H 7350 3490 50  0000 C CNN
F 1 "LED" H 7170 3490 50  0000 C CNN
	1    7250 3350
	0    1    1    0   
$EndComp
$Comp
L D_SHOTTKY D1
U 1 1 4F2E4F47
P 6800 3100
F 0 "D1" H 6800 3200 40  0000 C CNN
F 1 "BAT54WS" H 6800 3000 40  0000 C CNN
	1    6800 3100
	1    0    0    -1  
$EndComp
$Comp
L L L2
U 1 1 4F2E4F13
P 5950 3100
F 0 "L2" V 5860 3010 40  0000 C CNN
F 1 "6.8uH" V 5850 3200 40  0000 C CNN
	1    5950 3100
	0    1    1    0   
$EndComp
$Comp
L LT1932 DA1
U 1 1 4F2E4F05
P 5950 3600
F 0 "DA1" H 6050 3900 60  0000 C CNN
F 1 "LT1932" H 5650 3900 60  0000 C CNN
	1    5950 3600
	1    0    0    -1  
$EndComp
$EndSCHEMATC
