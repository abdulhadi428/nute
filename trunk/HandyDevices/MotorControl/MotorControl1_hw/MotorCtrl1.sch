EESchema Schematic File Version 2  date 4/12/2013 6:02:38 PM
LIBS:Connectors_kl
LIBS:pcb_details
LIBS:power
LIBS:Power_kl
LIBS:st_kl
LIBS:Tittar_kl
LIBS:MotorCtrl1-cache
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date "12 apr 2013"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L L6470 U1
U 1 1 5165951A
P 5450 3250
F 0 "U1" H 5950 4200 60  0000 C CNN
F 1 "L6470" H 4950 4200 60  0000 C CNN
F 2 "~" H 5450 3250 60  0000 C CNN
F 3 "~" H 5450 3250 60  0000 C CNN
	1    5450 3250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR01
U 1 1 51659529
P 5500 4500
F 0 "#PWR01" H 5590 4480 30  0001 C CNN
F 1 "GND" H 5500 4420 30  0001 C CNN
F 2 "" H 5500 4500 60  0000 C CNN
F 3 "" H 5500 4500 60  0000 C CNN
	1    5500 4500
	1    0    0    -1  
$EndComp
$Comp
L TESTPOINT TP1
U 1 1 51659554
P 4350 3950
F 0 "TP1" H 4350 4100 60  0000 C CNN
F 1 "TESTPOINT" H 4325 4175 60  0001 C CNN
F 2 "" H 4350 3950 60  0000 C CNN
F 3 "" H 4350 3950 60  0000 C CNN
	1    4350 3950
	0    -1   -1   0   
$EndComp
NoConn ~ 4600 3850
$Comp
L CONN_5 XL3
U 1 1 5165956D
P 3600 3100
F 0 "XL3" V 3550 3100 50  0000 C CNN
F 1 "CONN_5" V 3650 3100 50  0000 C CNN
F 2 "" H 3600 3100 60  0000 C CNN
F 3 "" H 3600 3100 60  0000 C CNN
	1    3600 3100
	-1   0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 516595BD
P 3800 3400
F 0 "#PWR02" H 3890 3380 30  0001 C CNN
F 1 "GND" H 3800 3320 30  0001 C CNN
F 2 "" H 3800 3400 60  0000 C CNN
F 3 "" H 3800 3400 60  0000 C CNN
	1    3800 3400
	1    0    0    -1  
$EndComp
$Comp
L CONN_5 XL2
U 1 1 516595E1
P 3600 2450
F 0 "XL2" V 3550 2450 50  0000 C CNN
F 1 "CONN_5" V 3650 2450 50  0000 C CNN
F 2 "" H 3600 2450 60  0000 C CNN
F 3 "" H 3600 2450 60  0000 C CNN
	1    3600 2450
	-1   0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 516595E7
P 3800 2750
F 0 "#PWR03" H 3890 2730 30  0001 C CNN
F 1 "GND" H 3800 2670 30  0001 C CNN
F 2 "" H 3800 2750 60  0000 C CNN
F 3 "" H 3800 2750 60  0000 C CNN
	1    3800 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	5300 4300 5300 4400
Wire Wire Line
	5300 4400 5700 4400
Wire Wire Line
	5700 4400 5700 4300
Wire Wire Line
	5400 4300 5400 4400
Connection ~ 5400 4400
Wire Wire Line
	5500 4300 5500 4500
Connection ~ 5500 4400
Wire Wire Line
	5600 4300 5600 4400
Connection ~ 5600 4400
Wire Wire Line
	4350 3950 4600 3950
Wire Wire Line
	3800 2900 4100 2900
Wire Wire Line
	4100 2900 4100 3100
Wire Wire Line
	4100 3100 4600 3100
Wire Wire Line
	3800 3000 4600 3000
Wire Wire Line
	3800 3100 4000 3100
Wire Wire Line
	4000 3100 4000 3200
Wire Wire Line
	4000 3200 4600 3200
Wire Wire Line
	4600 2900 4200 2900
Wire Wire Line
	4200 2900 4200 3300
Wire Wire Line
	4200 3300 3900 3300
Wire Wire Line
	3900 3300 3900 3200
Wire Wire Line
	3900 3200 3800 3200
Wire Wire Line
	3800 3400 3800 3300
Wire Wire Line
	3800 2750 3800 2650
Wire Wire Line
	4600 2550 4300 2550
Wire Wire Line
	4300 2550 4300 2250
Wire Wire Line
	4300 2250 3800 2250
Wire Wire Line
	4600 2450 4400 2450
Wire Wire Line
	4400 2450 4400 2350
Wire Wire Line
	4400 2350 3800 2350
Wire Wire Line
	4600 2650 4200 2650
Wire Wire Line
	4200 2650 4200 2450
Wire Wire Line
	4200 2450 3800 2450
Wire Wire Line
	4600 2750 4100 2750
Wire Wire Line
	4100 2750 4100 2550
Wire Wire Line
	4100 2550 3800 2550
$Comp
L CONN_2 XL4
U 1 1 51659684
P 6900 4050
F 0 "XL4" V 6850 4050 40  0000 C CNN
F 1 "CONN_2" V 6950 4050 40  0000 C CNN
F 2 "" H 6900 4050 60  0000 C CNN
F 3 "" H 6900 4050 60  0000 C CNN
	1    6900 4050
	1    0    0    -1  
$EndComp
$Comp
L CONN_2 XL5
U 1 1 51659693
P 6900 4450
F 0 "XL5" V 6850 4450 40  0000 C CNN
F 1 "CONN_2" V 6950 4450 40  0000 C CNN
F 2 "" H 6900 4450 60  0000 C CNN
F 3 "" H 6900 4450 60  0000 C CNN
	1    6900 4450
	1    0    0    -1  
$EndComp
$Comp
L R R2
U 1 1 5165974F
P 7500 3700
F 0 "R2" V 7580 3550 50  0000 C CNN
F 1 "2k7" V 7500 3700 50  0000 C CNN
F 2 "RES_0603" V 7580 3800 28  0000 C CNN
F 3 "" H 7500 3700 60  0000 C CNN
	1    7500 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	6700 3950 6600 3950
Wire Wire Line
	6600 3950 6600 3650
Wire Wire Line
	6600 3650 6300 3650
Wire Wire Line
	6300 3750 6500 3750
Wire Wire Line
	6500 3750 6500 4150
Wire Wire Line
	6500 4150 6700 4150
Wire Wire Line
	6300 3850 6400 3850
Wire Wire Line
	6400 3850 6400 4350
Wire Wire Line
	6400 4350 6700 4350
Wire Wire Line
	6300 3950 6300 4550
Wire Wire Line
	6300 4550 6700 4550
$Comp
L R R1
U 1 1 51659DC2
P 7500 3100
F 0 "R1" V 7580 2950 50  0000 C CNN
F 1 "62k" V 7500 3100 50  0000 C CNN
F 2 "RES_0603" V 7580 3200 28  0000 C CNN
F 3 "" H 7500 3100 60  0000 C CNN
	1    7500 3100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR04
U 1 1 51659DCA
P 7500 4050
F 0 "#PWR04" H 7590 4030 30  0001 C CNN
F 1 "GND" H 7500 3970 30  0001 C CNN
F 2 "" H 7500 4050 60  0000 C CNN
F 3 "" H 7500 4050 60  0000 C CNN
	1    7500 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	6300 3400 7500 3400
Wire Wire Line
	7500 3350 7500 3450
Connection ~ 7500 3400
Wire Wire Line
	7500 3950 7500 4050
Wire Wire Line
	6300 2800 8800 2800
Wire Wire Line
	6400 2800 6400 3200
Wire Wire Line
	6400 3200 6300 3200
Wire Wire Line
	6300 3100 6400 3100
Connection ~ 6400 3100
Wire Wire Line
	6300 2900 6400 2900
Connection ~ 6400 2900
Connection ~ 6400 2800
$Comp
L C C5
U 1 1 51659F83
P 8000 3100
F 0 "C5" H 7900 3000 50  0000 L CNN
F 1 "220n" H 7900 3200 50  0000 L CNN
F 2 "CAP_0603" V 8100 2950 28  0000 C BNN
F 3 "" H 8000 3100 60  0000 C CNN
	1    8000 3100
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 51659FA1
P 7000 1700
F 0 "C3" V 6900 1550 50  0000 L CNN
F 1 "220n" V 6900 1800 50  0000 L CNN
F 2 "CAP_0603" V 7100 1550 28  0000 C BNN
F 3 "" H 7000 1700 60  0000 C CNN
	1    7000 1700
	0    -1   -1   0   
$EndComp
$Comp
L C C4
U 1 1 51659FB0
P 7000 2450
F 0 "C4" H 6900 2350 50  0000 L CNN
F 1 "10n" H 6900 2550 50  0000 L CNN
F 2 "CAP_0603" V 7100 2300 28  0000 C BNN
F 3 "" H 7000 2450 60  0000 C CNN
	1    7000 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	7000 2650 6300 2650
$Comp
L DIODE D1
U 1 1 5165A010
P 6750 2050
F 0 "D1" H 6750 2150 40  0000 C CNN
F 1 "1N4148" H 6750 1950 40  0000 C CNN
F 2 "" H 6750 2050 60  0000 C CNN
F 3 "" H 6750 2050 60  0000 C CNN
	1    6750 2050
	-1   0    0    1   
$EndComp
$Comp
L DIODE D2
U 1 1 5165A01F
P 7250 2050
F 0 "D2" H 7250 2150 40  0000 C CNN
F 1 "1N4148" H 7250 1950 40  0000 C CNN
F 2 "" H 7250 2050 60  0000 C CNN
F 3 "" H 7250 2050 60  0000 C CNN
	1    7250 2050
	-1   0    0    1   
$EndComp
Wire Wire Line
	7000 2250 7000 2050
Wire Wire Line
	6950 2050 7050 2050
Connection ~ 7000 2050
Wire Wire Line
	6300 2450 6500 2450
Wire Wire Line
	6500 2450 6500 1700
Wire Wire Line
	6500 1700 6800 1700
Wire Wire Line
	6550 2050 6500 2050
Connection ~ 6500 2050
Wire Wire Line
	7200 1700 7500 1700
Wire Wire Line
	7500 1700 7500 2850
Wire Wire Line
	7500 2050 7450 2050
Connection ~ 7500 2050
Connection ~ 7500 2800
Wire Wire Line
	8000 2800 8000 2900
Wire Wire Line
	8350 2800 8350 2900
Connection ~ 8000 2800
$Comp
L GND #PWR05
U 1 1 5165A3F4
P 8000 3450
F 0 "#PWR05" H 8090 3430 30  0001 C CNN
F 1 "GND" H 8000 3370 30  0001 C CNN
F 2 "" H 8000 3450 60  0000 C CNN
F 3 "" H 8000 3450 60  0000 C CNN
	1    8000 3450
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR06
U 1 1 5165A403
P 8350 3450
F 0 "#PWR06" H 8440 3430 30  0001 C CNN
F 1 "GND" H 8350 3370 30  0001 C CNN
F 2 "" H 8350 3450 60  0000 C CNN
F 3 "" H 8350 3450 60  0000 C CNN
	1    8350 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	8000 3300 8000 3450
Wire Wire Line
	8350 3300 8350 3450
Text Label 7750 2800 0    60   ~ 0
Vs
$Comp
L CP C6
U 1 1 5165A4AD
P 8350 3100
F 0 "C6" H 8400 3200 50  0000 L CNN
F 1 "100u" H 8400 3000 50  0000 L CNN
F 2 "" H 8350 3100 60  0000 C CNN
F 3 "" H 8350 3100 60  0000 C CNN
	1    8350 3100
	1    0    0    -1  
$EndComp
NoConn ~ 4600 3650
Wire Wire Line
	4600 3400 4500 3400
Wire Wire Line
	4500 3400 4500 3500
Wire Wire Line
	2700 3500 4600 3500
Connection ~ 4500 3500
$Comp
L C C2
U 1 1 5165A56F
P 3800 3850
F 0 "C2" H 3700 3750 50  0000 L CNN
F 1 "10u" H 3700 3950 50  0000 L CNN
F 2 "CAP_0603" V 3900 3700 28  0000 C BNN
F 3 "" H 3800 3850 60  0000 C CNN
	1    3800 3850
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 5165A57E
P 3450 3850
F 0 "C1" H 3350 3750 50  0000 L CNN
F 1 "0.1u" H 3350 3950 50  0000 L CNN
F 2 "CAP_0603" V 3550 3700 28  0000 C BNN
F 3 "" H 3450 3850 60  0000 C CNN
	1    3450 3850
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR07
U 1 1 5165A595
P 3450 4200
F 0 "#PWR07" H 3540 4180 30  0001 C CNN
F 1 "GND" H 3450 4120 30  0001 C CNN
F 2 "" H 3450 4200 60  0000 C CNN
F 3 "" H 3450 4200 60  0000 C CNN
	1    3450 4200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR08
U 1 1 5165A59B
P 3800 4200
F 0 "#PWR08" H 3890 4180 30  0001 C CNN
F 1 "GND" H 3800 4120 30  0001 C CNN
F 2 "" H 3800 4200 60  0000 C CNN
F 3 "" H 3800 4200 60  0000 C CNN
	1    3800 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	3450 4050 3450 4200
Wire Wire Line
	3800 4050 3800 4200
Wire Wire Line
	3800 3650 3800 3500
Connection ~ 3800 3500
Wire Wire Line
	3450 3650 3450 3500
Text Label 3950 3500 0    60   ~ 0
Vdd
$Comp
L CONN_2 XL1
U 1 1 5165A651
P 2500 3600
F 0 "XL1" V 2450 3600 40  0000 C CNN
F 1 "CONN_2" V 2550 3600 40  0000 C CNN
F 2 "" H 2500 3600 60  0000 C CNN
F 3 "" H 2500 3600 60  0000 C CNN
	1    2500 3600
	-1   0    0    -1  
$EndComp
Connection ~ 3450 3500
$Comp
L GND #PWR09
U 1 1 5165A6B6
P 2700 3850
F 0 "#PWR09" H 2790 3830 30  0001 C CNN
F 1 "GND" H 2700 3770 30  0001 C CNN
F 2 "" H 2700 3850 60  0000 C CNN
F 3 "" H 2700 3850 60  0000 C CNN
	1    2700 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	2700 3700 2700 3850
$Comp
L HOLE_METALLED HOLE1
U 1 1 5165A6BF
P 5450 1900
F 0 "HOLE1" H 4950 1850 60  0000 C CNN
F 1 "HOLE_METALLED" H 4900 1950 60  0000 C CNN
F 2 "" H 5450 1900 60  0000 C CNN
F 3 "" H 5450 1900 60  0000 C CNN
	1    5450 1900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR010
U 1 1 5165A6CE
P 5900 1900
F 0 "#PWR010" H 5990 1880 30  0001 C CNN
F 1 "GND" H 5900 1820 30  0001 C CNN
F 2 "" H 5900 1900 60  0000 C CNN
F 3 "" H 5900 1900 60  0000 C CNN
	1    5900 1900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5900 1900 5750 1900
$Comp
L HOLE_METALLED HOLE2
U 1 1 5165A735
P 5500 4900
F 0 "HOLE2" H 5000 4850 60  0000 C CNN
F 1 "HOLE_METALLED" H 4950 4950 60  0000 C CNN
F 2 "" H 5500 4900 60  0000 C CNN
F 3 "" H 5500 4900 60  0000 C CNN
	1    5500 4900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR011
U 1 1 5165A73B
P 5950 4900
F 0 "#PWR011" H 6040 4880 30  0001 C CNN
F 1 "GND" H 5950 4820 30  0001 C CNN
F 2 "" H 5950 4900 60  0000 C CNN
F 3 "" H 5950 4900 60  0000 C CNN
	1    5950 4900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5950 4900 5800 4900
$Comp
L CONN_2 XL6
U 1 1 5165A742
P 9000 2900
F 0 "XL6" V 8950 2900 40  0000 C CNN
F 1 "CONN_2" V 9050 2900 40  0000 C CNN
F 2 "" H 9000 2900 60  0000 C CNN
F 3 "" H 9000 2900 60  0000 C CNN
	1    9000 2900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR012
U 1 1 5165A748
P 8800 3150
F 0 "#PWR012" H 8890 3130 30  0001 C CNN
F 1 "GND" H 8800 3070 30  0001 C CNN
F 2 "" H 8800 3150 60  0000 C CNN
F 3 "" H 8800 3150 60  0000 C CNN
	1    8800 3150
	-1   0    0    -1  
$EndComp
Wire Wire Line
	8800 3000 8800 3150
Connection ~ 8350 2800
Text Label 6350 3650 0    60   ~ 0
OUT1A
Text Label 6350 3750 0    60   ~ 0
OUT2A
Text Label 6350 3850 0    60   ~ 0
OUT1B
Text Label 6350 4550 0    60   ~ 0
OUT2B
$EndSCHEMATC
