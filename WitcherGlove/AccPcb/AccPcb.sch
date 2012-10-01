EESchema Schematic File Version 2  date 30/09/2012 15:57:54
LIBS:Connectors_kl
LIBS:pcb_details
LIBS:power
LIBS:Sensors
LIBS:Tittar_kl
LIBS:AccPcb-cache
EELAYER 25  0
EELAYER END
$Descr A4 11700 8267
encoding utf-8
Sheet 1 1
Title ""
Date "30 sep 2012"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L CONN_1 XL7
U 1 1 506810C5
P 2850 4900
F 0 "XL7" V 2800 4910 40  0000 C CNN
F 1 "CONN_1" V 2860 4920 40  0000 C CNN
	1    2850 4900
	-1   0    0    -1  
$EndComp
$Comp
L CONN_1 XL6
U 1 1 506810C0
P 2850 4550
F 0 "XL6" V 2800 4560 40  0000 C CNN
F 1 "CONN_1" V 2860 4570 40  0000 C CNN
	1    2850 4550
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3050 2700 3600 2700
Wire Wire Line
	3600 2700 3600 3050
Wire Wire Line
	3600 3050 4150 3050
Wire Wire Line
	4150 3250 3500 3250
Wire Wire Line
	3500 3250 3500 3400
Wire Wire Line
	3500 3400 3050 3400
Wire Wire Line
	4150 3350 3600 3350
Wire Wire Line
	3700 4100 3700 3500
Wire Wire Line
	3700 4100 3050 4100
Wire Wire Line
	3050 4550 3150 4550
Connection ~ 5850 3500
Wire Wire Line
	5950 3500 5450 3500
Connection ~ 5600 3500
Wire Wire Line
	5600 3700 5600 3500
Wire Wire Line
	5850 4100 5850 4250
Wire Wire Line
	5600 4100 5600 4250
Connection ~ 4550 4150
Wire Wire Line
	4850 4100 4850 4150
Wire Wire Line
	4850 4150 4450 4150
Wire Wire Line
	4450 4100 4450 4250
Wire Wire Line
	4550 4150 4550 4100
Connection ~ 4450 4150
Wire Wire Line
	5850 3500 5850 3700
Wire Wire Line
	5450 3400 5500 3400
Wire Wire Line
	5500 3400 5500 3500
Connection ~ 5500 3500
Wire Wire Line
	3700 3500 4150 3500
Wire Wire Line
	3050 4900 3100 4900
Wire Wire Line
	3100 4900 3100 5050
Wire Wire Line
	3050 3750 3600 3750
Wire Wire Line
	3600 3750 3600 3350
Wire Wire Line
	3050 3050 3500 3050
Wire Wire Line
	3500 3050 3500 3150
Wire Wire Line
	3500 3150 4150 3150
$Comp
L +3.3V #PWR01
U 1 1 50680944
P 3150 4550
F 0 "#PWR01" H 3150 4510 30  0001 C CNN
F 1 "+3.3V" H 3230 4580 30  0000 C CNN
	1    3150 4550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 5068093C
P 3100 5050
F 0 "#PWR02" H 3190 5030 30  0001 C CNN
F 1 "GND" H 3100 4970 30  0001 C CNN
	1    3100 5050
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 XL5
U 1 1 5068092B
P 2850 4100
F 0 "XL5" V 2800 4110 40  0000 C CNN
F 1 "CONN_1" V 2860 4120 40  0000 C CNN
	1    2850 4100
	-1   0    0    -1  
$EndComp
$Comp
L CONN_1 XL4
U 1 1 50680924
P 2850 3750
F 0 "XL4" V 2800 3760 40  0000 C CNN
F 1 "CONN_1" V 2860 3770 40  0000 C CNN
	1    2850 3750
	-1   0    0    -1  
$EndComp
$Comp
L CONN_1 XL3
U 1 1 5068091C
P 2850 3400
F 0 "XL3" V 2800 3410 40  0000 C CNN
F 1 "CONN_1" V 2860 3420 40  0000 C CNN
	1    2850 3400
	-1   0    0    -1  
$EndComp
$Comp
L CONN_1 XL2
U 1 1 50680918
P 2850 3050
F 0 "XL2" V 2800 3060 40  0000 C CNN
F 1 "CONN_1" V 2860 3070 40  0000 C CNN
	1    2850 3050
	-1   0    0    -1  
$EndComp
$Comp
L CONN_1 XL1
U 1 1 50680913
P 2850 2700
F 0 "XL1" V 2800 2710 40  0000 C CNN
F 1 "CONN_1" V 2860 2720 40  0000 C CNN
	1    2850 2700
	-1   0    0    -1  
$EndComp
Text Label 3750 3350 0    60   ~ 0
CS
Text Label 3750 3250 0    60   ~ 0
SDO
Text Notes 4500 2500 0    60   ~ 0
Accelerometer\nI2C Addr =\n001100(A0)
$Comp
L LIS3 DD1
U 1 1 5067E549
P 4800 3400
F 0 "DD1" H 4450 3900 60  0000 C CNN
F 1 "LIS3" H 5100 3900 60  0000 C CNN
	1    4800 3400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR04
U 1 1 5067E548
P 4450 4250
F 0 "#PWR04" H 4540 4230 30  0001 C CNN
F 1 "GND" H 4450 4170 30  0001 C CNN
	1    4450 4250
	1    0    0    -1  
$EndComp
NoConn ~ 5450 3050
NoConn ~ 5450 3150
NoConn ~ 5450 3250
$Comp
L C C1
U 1 1 5067E547
P 5600 3900
F 0 "C1" H 5500 3800 50  0000 L CNN
F 1 "0.1u" H 5500 4000 50  0000 L CNN
F 2 "CAP_0603" V 5700 3750 28  0000 C BNN
	1    5600 3900
	1    0    0    -1  
$EndComp
$Comp
L C C2
U 1 1 5067E546
P 5850 3900
F 0 "C2" H 5750 3800 50  0000 L CNN
F 1 "10u" H 5750 4000 50  0000 L CNN
F 2 "CAP_0603" V 5950 3750 28  0000 C BNN
	1    5850 3900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR05
U 1 1 5067E545
P 5600 4250
F 0 "#PWR05" H 5690 4230 30  0001 C CNN
F 1 "GND" H 5600 4170 30  0001 C CNN
	1    5600 4250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR06
U 1 1 5067E544
P 5850 4250
F 0 "#PWR06" H 5940 4230 30  0001 C CNN
F 1 "GND" H 5850 4170 30  0001 C CNN
	1    5850 4250
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR07
U 1 1 5067E543
P 5950 3500
F 0 "#PWR07" H 5950 3460 30  0001 C CNN
F 1 "+3.3V" H 6030 3530 30  0000 C CNN
	1    5950 3500
	1    0    0    -1  
$EndComp
NoConn ~ 4150 3600
Text Label 3700 3500 0    60   ~ 0
ACC_IRQ
Text Label 3750 3050 0    60   ~ 0
I2C_SCL
Text Label 3750 3150 0    60   ~ 0
I2C_SDA
$EndSCHEMATC
