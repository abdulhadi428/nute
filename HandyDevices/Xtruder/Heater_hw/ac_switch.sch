EESchema Schematic File Version 2  date 01/08/2012 17:41:45
LIBS:Connectors_kl
LIBS:pcb_details
LIBS:power
LIBS:Power_kl
LIBS:Tittar_kl
LIBS:Heater-cache
EELAYER 25  0
EELAYER END
$Descr A4 11700 8267
encoding utf-8
Sheet 1 1
Title "noname.sch"
Date "1 aug 2012"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Notes 7800 5050 0    60   ~ 0
Indication
Wire Wire Line
	7450 4800 7450 4900
Wire Wire Line
	7650 5900 7650 5950
Wire Wire Line
	7650 5950 7200 5950
Wire Wire Line
	7200 5950 7200 5900
Wire Wire Line
	7650 5500 7650 5450
Wire Wire Line
	7650 5450 7200 5450
Wire Wire Line
	7200 5450 7200 5500
Wire Wire Line
	7850 3100 7850 3250
Wire Wire Line
	7850 2900 6800 2900
Connection ~ 4450 3950
Wire Wire Line
	4450 3900 4450 3950
Connection ~ 4450 2900
Wire Wire Line
	4450 2950 4450 2900
Wire Wire Line
	5000 2900 5100 2900
Connection ~ 6450 3400
Wire Wire Line
	6500 3400 6150 3400
Wire Wire Line
	6150 3400 6150 3100
Connection ~ 6850 2900
Connection ~ 6850 4050
Wire Wire Line
	6850 3550 6850 4050
Wire Wire Line
	6450 4000 6450 4050
Connection ~ 7200 2900
Wire Wire Line
	7200 2900 7200 3000
Wire Wire Line
	7200 3500 7200 3550
Wire Wire Line
	7850 4250 7850 4400
Wire Wire Line
	7200 4050 7200 3950
Connection ~ 7200 4050
Wire Wire Line
	6450 3400 6450 3500
Wire Wire Line
	6850 2900 6850 3050
Wire Wire Line
	6150 2900 6300 2900
Wire Wire Line
	4400 2900 4500 2900
Wire Wire Line
	5100 3000 4950 3000
Wire Wire Line
	4450 3450 4450 3500
Wire Wire Line
	4150 3950 4950 3950
Wire Wire Line
	4950 3950 4950 3000
Wire Wire Line
	6450 4050 7850 4050
Wire Wire Line
	7450 4300 7450 4050
Connection ~ 7450 4050
Wire Wire Line
	7450 5400 7450 5450
Connection ~ 7450 5450
Wire Wire Line
	7450 6150 7450 5950
Connection ~ 7450 5950
$Comp
L R R?
U 1 1 50192824
P 7450 5150
F 0 "R?" V 7530 5150 50  0000 C CNN
F 1 "51k" V 7450 5150 50  0000 C CNN
F 2 "RES_1206" V 7380 5150 28  0000 C CNN
	1    7450 5150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 501926A9
P 7450 6150
F 0 "#PWR?" H 7540 6130 30  0001 C CNN
F 1 "GND" H 7450 6070 30  0001 C CNN
	1    7450 6150
	1    0    0    -1  
$EndComp
$Comp
L LED D?
U 1 1 5019269D
P 7650 5700
F 0 "D?" H 7750 5840 50  0000 C CNN
F 1 "LED" H 7570 5840 50  0000 C CNN
	1    7650 5700
	0    1    1    0   
$EndComp
$Comp
L DIODE D?
U 1 1 50192691
P 7200 5700
F 0 "D?" H 7200 5800 40  0000 C CNN
F 1 "1N4004" H 7200 5600 40  0000 C CNN
F 2 "DO-41" H 7200 5900 60  0000 C CNN
	1    7200 5700
	0    -1   -1   0   
$EndComp
$Comp
L R R?
U 1 1 5019264E
P 7450 4550
F 0 "R?" V 7530 4550 50  0000 C CNN
F 1 "51k" V 7450 4550 50  0000 C CNN
F 2 "RES_1206" V 7380 4550 28  0000 C CNN
	1    7450 4550
	1    0    0    -1  
$EndComp
Text Notes 8200 4200 0    60   ~ 0
220V Output
Text Notes 8200 3000 0    60   ~ 0
220V Input
Text Label 7350 2900 0    60   ~ 0
In_220
$Comp
L GND #PWR?
U 1 1 50192371
P 7850 3250
F 0 "#PWR?" H 7940 3230 30  0001 C CNN
F 1 "GND" H 7850 3170 30  0001 C CNN
	1    7850 3250
	1    0    0    -1  
$EndComp
$Comp
L CONN_2 XL?
U 1 1 5019234C
P 8050 3000
F 0 "XL?" V 8000 3000 40  0000 C CNN
F 1 "CONN_2" V 8100 3000 40  0000 C CNN
	1    8050 3000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5019233B
P 7850 4400
F 0 "#PWR?" H 7940 4380 30  0001 C CNN
F 1 "GND" H 7850 4320 30  0001 C CNN
	1    7850 4400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 50192329
P 2400 3150
F 0 "#PWR?" H 2490 3130 30  0001 C CNN
F 1 "GND" H 2400 3070 30  0001 C CNN
	1    2400 3150
	1    0    0    -1  
$EndComp
$Comp
L CONN_2 XL?
U 1 1 50192318
P 2200 2900
F 0 "XL?" V 2150 2900 40  0000 C CNN
F 1 "CONN_2" V 2250 2900 40  0000 C CNN
	1    2200 2900
	-1   0    0    -1  
$EndComp
$Comp
L CONN_2 XL?
U 1 1 501922C4
P 8050 4150
F 0 "XL?" V 8000 4150 40  0000 C CNN
F 1 "CONN_2" V 8100 4150 40  0000 C CNN
	1    8050 4150
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 501922C2
P 7200 3250
F 0 "R?" V 7280 3250 50  0000 C CNN
F 1 "39R" V 7200 3250 50  0000 C CNN
F 2 "RES_1206" V 7130 3250 28  0000 C CNN
	1    7200 3250
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 501922C1
P 7200 3750
F 0 "C?" H 7100 3650 50  0000 L CNN
F 1 "0.01u" H 7250 3650 50  0000 L CNN
F 2 "CAP_1206" V 7100 3750 28  0000 C BNN
F 4 "300V" H 7350 3850 60  0000 C CNN "Umax"
	1    7200 3750
	1    0    0    -1  
$EndComp
$Comp
L TRIAC U?
U 1 1 501922C0
P 6850 3300
F 0 "U?" H 6600 3350 60  0000 C CNN
F 1 "BTB600" H 6600 3450 60  0000 C CNN
	1    6850 3300
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 501922BF
P 6450 3750
F 0 "R?" V 6530 3750 50  0000 C CNN
F 1 "200k" V 6450 3750 50  0000 C CNN
F 2 "RES_1206" V 6380 3750 28  0000 C CNN
	1    6450 3750
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 501922BE
P 6550 2900
F 0 "R?" V 6630 2900 50  0000 C CNN
F 1 "200R" V 6550 2900 50  0000 C CNN
F 2 "RES_1206" V 6480 2900 28  0000 C CNN
	1    6550 2900
	0    -1   -1   0   
$EndComp
$Comp
L OPTOCOUPLER_TR U?
U 1 1 501922BD
P 5600 3000
F 0 "U?" H 6000 3250 60  0000 C CNN
F 1 "MOC3061M" H 5500 3250 60  0000 C CNN
	1    5600 3000
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 501922BC
P 4750 2900
F 0 "R?" V 4830 2900 50  0000 C CNN
F 1 "150R" V 4750 2900 50  0000 C CNN
F 2 "RES_0603" V 4680 2900 28  0000 C CNN
	1    4750 2900
	0    -1   -1   0   
$EndComp
$Comp
L +3.3V #PWR?
U 1 1 501922BB
P 4400 2900
F 0 "#PWR?" H 4400 2860 30  0001 C CNN
F 1 "+3.3V" H 4480 2930 30  0000 C CNN
	1    4400 2900
	-1   0    0    1   
$EndComp
Text Label 4150 3950 0    60   ~ 0
Out1
$Comp
L LED D?
U 1 1 501922BA
P 4450 3700
F 0 "D?" H 4550 3840 50  0000 C CNN
F 1 "LED" H 4370 3840 50  0000 C CNN
	1    4450 3700
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 501922B9
P 4450 3200
F 0 "R?" V 4530 3200 50  0000 C CNN
F 1 "510R" V 4450 3200 50  0000 C CNN
F 2 "RES_0603" V 4380 3200 28  0000 C CNN
	1    4450 3200
	1    0    0    -1  
$EndComp
Text Label 6650 4050 0    60   ~ 0
Out_220
$EndSCHEMATC
