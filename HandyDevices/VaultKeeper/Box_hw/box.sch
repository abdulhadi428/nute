EESchema Schematic File Version 2  date 30/10/2011 11:33:07
LIBS:Connectors_kl
LIBS:Interface_kl
LIBS:pcb_details
LIBS:power
LIBS:Power_kl
LIBS:st_kl
LIBS:Tittar_kl
LIBS:box-cache
EELAYER 25  0
EELAYER END
$Descr A3 16535 11700
encoding utf-8
Sheet 1 1
Title ""
Date "30 oct 2011"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L RJ11 XL?
U 1 1 4EACFD6B
P 9750 7350
F 0 "XL?" H 9600 7650 60  0000 C CNN
F 1 "RJ11" H 9850 7650 60  0000 C CNN
	1    9750 7350
	-1   0    0    -1  
$EndComp
NoConn ~ 9400 7500
NoConn ~ 9400 7200
$Comp
L R R?
U 1 1 4EACFD6A
P 8500 7050
F 0 "R?" V 8580 7050 50  0000 C CNN
F 1 "200k" V 8500 7050 50  0000 C CNN
	1    8500 7050
	0    1    -1   0   
$EndComp
$Comp
L GND #PWR?
U 1 1 4EACFD69
P 8600 7400
F 0 "#PWR?" H 8690 7380 30  0001 C CNN
F 1 "GND" H 8600 7320 30  0001 C CNN
	1    8600 7400
	0    -1   1    0   
$EndComp
Text Label 7550 7050 0    60   ~ 0
SENSOR_PWR
$Comp
L R R?
U 1 1 4EACFD68
P 9050 7300
F 0 "R?" V 9130 7300 50  0000 C CNN
F 1 "100k" V 9050 7300 50  0000 C CNN
	1    9050 7300
	0    -1   -1   0   
$EndComp
$Comp
L R R?
U 1 1 4EACFD67
P 9050 7400
F 0 "R?" V 9130 7400 50  0000 C CNN
F 1 "100k" V 9050 7400 50  0000 C CNN
	1    9050 7400
	0    1    1    0   
$EndComp
Wire Wire Line
	8600 7400 8800 7400
Wire Wire Line
	9300 7400 9400 7400
Wire Wire Line
	7550 7050 8250 7050
Wire Wire Line
	9300 7300 9400 7300
Wire Wire Line
	8750 7050 8750 7300
Connection ~ 8750 7300
Wire Wire Line
	8000 7300 8800 7300
Wire Wire Line
	8000 8050 8800 8050
Connection ~ 8750 8050
Wire Wire Line
	8750 7800 8750 8050
Wire Wire Line
	9300 8050 9400 8050
Wire Wire Line
	7550 7800 8250 7800
Wire Wire Line
	9300 8150 9400 8150
Wire Wire Line
	8600 8150 8800 8150
$Comp
L R R?
U 1 1 4EACFD66
P 9050 8150
F 0 "R?" V 9130 8150 50  0000 C CNN
F 1 "100k" V 9050 8150 50  0000 C CNN
	1    9050 8150
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 4EACFD65
P 9050 8050
F 0 "R?" V 9130 8050 50  0000 C CNN
F 1 "100k" V 9050 8050 50  0000 C CNN
	1    9050 8050
	0    -1   -1   0   
$EndComp
Text Label 7550 7800 0    60   ~ 0
SENSOR_PWR
$Comp
L GND #PWR?
U 1 1 4EACFD64
P 8600 8150
F 0 "#PWR?" H 8690 8130 30  0001 C CNN
F 1 "GND" H 8600 8070 30  0001 C CNN
	1    8600 8150
	0    -1   1    0   
$EndComp
$Comp
L R R?
U 1 1 4EACFD63
P 8500 7800
F 0 "R?" V 8580 7800 50  0000 C CNN
F 1 "200k" V 8500 7800 50  0000 C CNN
	1    8500 7800
	0    1    -1   0   
$EndComp
NoConn ~ 9400 7950
NoConn ~ 9400 8250
$Comp
L RJ11 XL?
U 1 1 4EACFD62
P 9750 8100
F 0 "XL?" H 9600 8400 60  0000 C CNN
F 1 "RJ11" H 9850 8400 60  0000 C CNN
	1    9750 8100
	-1   0    0    -1  
$EndComp
$Comp
L RJ11 XL?
U 1 1 4EACFD05
P 9750 6600
F 0 "XL?" H 9600 6900 60  0000 C CNN
F 1 "RJ11" H 9850 6900 60  0000 C CNN
	1    9750 6600
	-1   0    0    -1  
$EndComp
NoConn ~ 9400 6750
NoConn ~ 9400 6450
$Comp
L R R?
U 1 1 4EACFD04
P 8500 6300
F 0 "R?" V 8580 6300 50  0000 C CNN
F 1 "200k" V 8500 6300 50  0000 C CNN
	1    8500 6300
	0    1    -1   0   
$EndComp
$Comp
L GND #PWR?
U 1 1 4EACFD03
P 8600 6650
F 0 "#PWR?" H 8690 6630 30  0001 C CNN
F 1 "GND" H 8600 6570 30  0001 C CNN
	1    8600 6650
	0    -1   1    0   
$EndComp
Text Label 7550 6300 0    60   ~ 0
SENSOR_PWR
$Comp
L R R?
U 1 1 4EACFD02
P 9050 6550
F 0 "R?" V 9130 6550 50  0000 C CNN
F 1 "100k" V 9050 6550 50  0000 C CNN
	1    9050 6550
	0    -1   -1   0   
$EndComp
$Comp
L R R?
U 1 1 4EACFD01
P 9050 6650
F 0 "R?" V 9130 6650 50  0000 C CNN
F 1 "100k" V 9050 6650 50  0000 C CNN
	1    9050 6650
	0    1    1    0   
$EndComp
Wire Wire Line
	8600 6650 8800 6650
Wire Wire Line
	9300 6650 9400 6650
Wire Wire Line
	7550 6300 8250 6300
Wire Wire Line
	9300 6550 9400 6550
Wire Wire Line
	8750 6300 8750 6550
Connection ~ 8750 6550
Wire Wire Line
	8000 6550 8800 6550
Wire Wire Line
	8000 5800 8800 5800
Wire Wire Line
	10500 1850 10250 1850
Wire Wire Line
	11300 1850 11350 1850
Wire Wire Line
	10250 1850 10250 1900
Wire Wire Line
	13300 4600 13300 4650
Wire Wire Line
	13850 7050 14500 7050
Wire Wire Line
	14500 7250 14350 7250
Wire Wire Line
	14500 7150 14450 7150
Wire Wire Line
	14500 7550 14450 7550
Wire Wire Line
	14500 7450 14350 7450
Wire Wire Line
	14500 6850 14350 6850
Wire Wire Line
	14500 6950 14450 6950
Wire Wire Line
	14500 7350 13850 7350
Wire Wire Line
	14500 6200 13850 6200
Wire Wire Line
	14900 2200 15000 2200
Wire Wire Line
	10200 2000 10250 2000
Wire Bus Line
	7150 2200 7150 1750
Wire Wire Line
	10200 4000 10200 4100
Wire Wire Line
	10250 3700 10200 3700
Connection ~ 9150 3700
Wire Wire Line
	9150 3700 9150 4000
Wire Wire Line
	9150 4000 9200 4000
Wire Wire Line
	9200 3700 8700 3700
Wire Wire Line
	12500 2500 13200 2500
Wire Wire Line
	14400 2200 14500 2200
Connection ~ 8750 5800
Wire Wire Line
	8750 5550 8750 5800
Wire Wire Line
	9300 5800 9400 5800
Wire Wire Line
	7250 2200 7800 2200
Wire Wire Line
	7250 2100 7800 2100
Wire Wire Line
	7250 2000 7800 2000
Wire Wire Line
	5250 2600 6250 2600
Connection ~ 9150 2300
Wire Wire Line
	9150 2400 9150 2000
Connection ~ 9150 2000
Wire Wire Line
	9150 2300 9200 2300
Wire Wire Line
	6250 2800 6250 2900
Wire Wire Line
	2450 9200 1350 9200
Wire Wire Line
	2850 3400 3000 3400
Connection ~ 4050 9200
Wire Wire Line
	4050 9300 4050 9200
Connection ~ 3800 9200
Wire Wire Line
	3800 9250 3800 9200
Wire Wire Line
	3800 10350 3800 10400
Wire Wire Line
	2750 10100 2750 10400
Wire Wire Line
	13850 2600 13850 2500
Wire Wire Line
	13850 2500 13800 2500
Connection ~ 10450 2200
Wire Wire Line
	10450 2250 10450 2200
Wire Wire Line
	10200 2300 10200 2700
Wire Wire Line
	9100 9400 9250 9400
Wire Wire Line
	7450 9750 7550 9750
Wire Wire Line
	8250 10100 8350 10100
Wire Wire Line
	8250 9400 8350 9400
Connection ~ 4650 9200
Wire Wire Line
	4650 9250 4650 9200
Wire Wire Line
	4400 9650 4400 9700
Wire Wire Line
	1400 9650 1400 9700
Wire Wire Line
	1400 9200 1400 9250
Connection ~ 1400 9200
Wire Wire Line
	4400 9200 4400 9250
Connection ~ 4400 9200
Wire Wire Line
	4650 9650 4650 9700
Wire Wire Line
	2650 2700 2650 2750
Wire Wire Line
	3000 2500 2650 2500
Wire Wire Line
	3000 2300 2650 2300
Wire Wire Line
	3000 2100 2650 2100
Wire Wire Line
	5400 2200 5250 2200
Wire Wire Line
	5400 2400 5250 2400
Wire Wire Line
	5350 3000 5250 3000
Wire Wire Line
	5250 3100 5350 3100
Wire Wire Line
	14500 5800 14450 5800
Wire Wire Line
	1950 5550 2050 5550
Wire Wire Line
	1950 5750 2100 5750
Wire Wire Line
	1950 5850 2100 5850
Wire Wire Line
	2100 5850 2100 5900
Wire Wire Line
	2050 5650 1950 5650
Connection ~ 2850 4050
Wire Wire Line
	2850 4000 2850 4050
Wire Wire Line
	1200 3550 1200 4100
Connection ~ 2950 4050
Wire Wire Line
	2950 4100 2950 4050
Wire Wire Line
	2200 4050 2250 4050
Wire Wire Line
	1650 3750 3000 3750
Wire Wire Line
	1450 4500 1450 4600
Wire Wire Line
	1950 4500 1950 4600
Connection ~ 1700 3750
Wire Wire Line
	1700 4100 1700 3750
Connection ~ 3950 4400
Wire Wire Line
	4050 4400 4050 4350
Connection ~ 3750 4400
Wire Wire Line
	3850 4400 3850 4350
Wire Wire Line
	3750 4500 3750 4350
Wire Wire Line
	3950 4400 3950 4350
Connection ~ 3850 4400
Wire Wire Line
	1450 3650 1450 4100
Connection ~ 1450 3650
Connection ~ 1950 3900
Wire Wire Line
	1700 4500 1700 4600
Wire Wire Line
	2700 4500 2700 4600
Wire Wire Line
	2950 4500 2950 4600
Wire Wire Line
	1400 3650 3000 3650
Wire Wire Line
	1900 3900 3000 3900
Wire Wire Line
	3000 4050 2650 4050
Wire Wire Line
	2700 4050 2700 4100
Connection ~ 2700 4050
Wire Wire Line
	1950 4100 1950 3900
Wire Wire Line
	1200 4500 1200 4600
Wire Wire Line
	3000 3550 1150 3550
Connection ~ 1200 3550
Wire Wire Line
	3750 4400 4200 4400
Wire Wire Line
	4200 4400 4200 4350
Connection ~ 4050 4400
Wire Wire Line
	14500 5700 14350 5700
Wire Wire Line
	5250 2300 5400 2300
Wire Wire Line
	5250 2100 5400 2100
Wire Wire Line
	2650 2000 3000 2000
Wire Wire Line
	2650 2200 3000 2200
Wire Wire Line
	2650 2400 3000 2400
Wire Wire Line
	2650 2600 3000 2600
Wire Wire Line
	5250 2500 5400 2500
Wire Wire Line
	14500 6300 14350 6300
Wire Wire Line
	14500 6400 14450 6400
Wire Wire Line
	8500 9750 8600 9750
Wire Wire Line
	7200 9400 7300 9400
Wire Wire Line
	7200 10100 7300 10100
Wire Wire Line
	9500 9750 9600 9750
Wire Wire Line
	9250 10100 9100 10100
Wire Wire Line
	10200 2200 10500 2200
Wire Wire Line
	10450 2650 10450 2700
Wire Wire Line
	8650 1900 9200 1900
Wire Wire Line
	8650 2200 9200 2200
Wire Wire Line
	7550 5550 8250 5550
Wire Wire Line
	3800 9850 3800 9750
Connection ~ 3800 9800
Wire Wire Line
	2450 9600 2450 10400
Wire Wire Line
	3550 9200 4800 9200
Wire Wire Line
	3550 9800 4050 9800
Wire Wire Line
	4050 9800 4050 9700
Wire Wire Line
	2300 3400 2450 3400
Wire Wire Line
	5250 3600 5400 3600
Wire Wire Line
	2450 9450 1900 9450
Wire Wire Line
	14500 6000 14450 6000
Wire Wire Line
	14500 6100 14350 6100
Wire Wire Line
	5950 2000 5250 2000
Wire Wire Line
	5950 1900 5250 1900
Wire Wire Line
	9150 2900 9150 2950
Wire Wire Line
	8650 2000 9200 2000
Wire Wire Line
	5250 2800 5950 2800
Wire Wire Line
	9300 5900 9400 5900
Wire Wire Line
	8600 5900 8800 5900
Wire Wire Line
	13800 2200 13900 2200
Wire Wire Line
	5250 3200 5300 3200
Wire Wire Line
	8700 3600 9200 3600
Wire Wire Line
	8700 3900 9200 3900
Wire Wire Line
	10250 3600 10200 3600
Wire Wire Line
	10350 3900 10200 3900
Wire Bus Line
	6050 2950 6050 1750
Wire Bus Line
	6050 1750 8550 1750
Wire Bus Line
	8550 1750 8550 2200
Wire Wire Line
	10250 1900 10200 1900
Wire Wire Line
	12600 2200 13200 2200
Wire Wire Line
	13850 5900 14500 5900
Wire Wire Line
	13300 3900 13300 3850
Wire Wire Line
	13500 4250 13600 4250
Wire Wire Line
	14500 4250 14600 4250
Wire Wire Line
	14300 3900 14300 3850
Wire Wire Line
	14300 4600 14300 4650
Wire Wire Line
	10250 2000 10250 2050
Wire Wire Line
	11300 2050 11350 2050
Wire Wire Line
	10250 2050 10500 2050
Text Label 13850 7350 0    60   ~ 0
RS485_B
Text Label 13850 7050 0    60   ~ 0
RS485_A
Text Label 13850 5900 0    60   ~ 0
RS485_A
Text Label 13850 6200 0    60   ~ 0
RS485_B
Text Label 12600 2200 0    60   ~ 0
RS485_A
Text Label 15000 2200 0    60   ~ 0
RS485_B
Text Label 10250 3700 0    60   ~ 0
B
Text Label 10250 3600 0    60   ~ 0
A
Text Label 10350 2050 0    60   ~ 0
B
Text Label 10350 1850 0    60   ~ 0
A
$Comp
L EMIFILTER U?
U 1 1 4EACFA52
P 10900 1950
F 0 "U?" H 10550 2200 60  0000 C CNN
F 1 "EMIFILTER" H 11100 2200 60  0000 C CNN
	1    10900 1950
	1    0    0    -1  
$EndComp
Text Notes 13400 3350 0    60   ~ 0
Shottki supress \npositive and negative\npeaks at signal fronts
$Comp
L BAXXXS D?
U 1 1 4EAC780B
P 14300 4250
F 0 "D?" H 14100 4400 40  0000 C CNN
F 1 "BAXXXS" H 14400 4400 40  0000 C CNN
	1    14300 4250
	0    -1   -1   0   
$EndComp
$Comp
L +3.3V #PWR?
U 1 1 4EAC780A
P 14300 3850
F 0 "#PWR?" H 14300 3810 30  0001 C CNN
F 1 "+3.3V" H 14380 3880 30  0000 C CNN
	1    14300 3850
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR?
U 1 1 4EAC7809
P 14300 4650
F 0 "#PWR?" H 14390 4630 30  0001 C CNN
F 1 "GND" H 14300 4570 30  0001 C CNN
	1    14300 4650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 4EAC77F4
P 13300 4650
F 0 "#PWR?" H 13390 4630 30  0001 C CNN
F 1 "GND" H 13300 4570 30  0001 C CNN
	1    13300 4650
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR?
U 1 1 4EAC77EA
P 13300 3850
F 0 "#PWR?" H 13300 3810 30  0001 C CNN
F 1 "+3.3V" H 13380 3880 30  0000 C CNN
	1    13300 3850
	0    -1   -1   0   
$EndComp
$Comp
L BAXXXS D?
U 1 1 4EAC7721
P 13300 4250
F 0 "D?" H 13100 4400 40  0000 C CNN
F 1 "BAXXXS" H 13400 4400 40  0000 C CNN
	1    13300 4250
	0    -1   -1   0   
$EndComp
Text Label 13600 4250 0    60   ~ 0
RS485_P
Text Label 14600 4250 0    60   ~ 0
RS485_M
$Comp
L RJ45 XL?
U 1 1 4EAC720A
P 14850 7200
F 0 "XL?" H 14600 7700 60  0000 C CNN
F 1 "RJ45" H 14900 7700 60  0000 C CNN
	1    14850 7200
	-1   0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 4EAC7209
P 14350 6850
F 0 "#PWR?" H 14440 6830 30  0001 C CNN
F 1 "GND" H 14350 6770 30  0001 C CNN
	1    14350 6850
	0    -1   1    0   
$EndComp
$Comp
L VCC #PWR?
U 1 1 4EAC7208
P 14450 6950
F 0 "#PWR?" H 14450 7000 30  0001 C CNN
F 1 "VCC" H 14520 6980 30  0000 C CNN
	1    14450 6950
	-1   0    0    -1  
$EndComp
$Comp
L VCC #PWR?
U 1 1 4EAC7207
P 14450 7550
F 0 "#PWR?" H 14450 7600 30  0001 C CNN
F 1 "VCC" H 14520 7580 30  0000 C CNN
	1    14450 7550
	-1   0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 4EAC7206
P 14350 7450
F 0 "#PWR?" H 14440 7430 30  0001 C CNN
F 1 "GND" H 14350 7370 30  0001 C CNN
	1    14350 7450
	0    -1   1    0   
$EndComp
$Comp
L GND #PWR?
U 1 1 4EAC7205
P 14350 7250
F 0 "#PWR?" H 14440 7230 30  0001 C CNN
F 1 "GND" H 14350 7170 30  0001 C CNN
	1    14350 7250
	0    -1   1    0   
$EndComp
$Comp
L VCC #PWR?
U 1 1 4EAC7204
P 14450 7150
F 0 "#PWR?" H 14450 7200 30  0001 C CNN
F 1 "VCC" H 14520 7180 30  0000 C CNN
	1    14450 7150
	-1   0    0    -1  
$EndComp
Text Notes 9400 3350 0    60   ~ 0
Other package
$Comp
L +3.3V #PWR?
U 1 1 4EAC70E4
P 10350 3900
F 0 "#PWR?" H 10350 3860 30  0001 C CNN
F 1 "+3.3V" H 10430 3930 30  0000 C CNN
	1    10350 3900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 4EAC70D5
P 10200 4100
F 0 "#PWR?" H 10290 4080 30  0001 C CNN
F 1 "GND" H 10200 4020 30  0001 C CNN
	1    10200 4100
	-1   0    0    -1  
$EndComp
Text Label 8700 3900 0    60   ~ 0
UART2_TX
Text Label 8700 3600 0    60   ~ 0
UART2_RX
Text Label 8700 3700 0    60   ~ 0
Tx/Rx
$Comp
L RS485HALF DA?
U 1 1 4EAC7069
P 9700 3800
F 0 "DA?" H 9400 4150 60  0000 C CNN
F 1 "RS485HALF" H 9900 4150 60  0000 C CNN
	1    9700 3800
	1    0    0    -1  
$EndComp
Text Label 5300 3200 0    60   ~ 0
IsTerminatorOn
$Comp
L RS485HALF DA?
U 1 1 4EAC6FFC
P 9700 2100
F 0 "DA?" H 9400 2450 60  0000 C CNN
F 1 "RS485HALF" H 9900 2450 60  0000 C CNN
	1    9700 2100
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 4EAC6E54
P 14700 2200
F 0 "C?" V 14600 2350 50  0000 L CNN
F 1 "1000pF" V 14850 2200 50  0000 L CNN
	1    14700 2200
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 4EAC60F1
P 9050 5900
F 0 "R?" V 9130 5900 50  0000 C CNN
F 1 "100k" V 9050 5900 50  0000 C CNN
	1    9050 5900
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 4EAC60DF
P 9050 5800
F 0 "R?" V 9130 5800 50  0000 C CNN
F 1 "100k" V 9050 5800 50  0000 C CNN
	1    9050 5800
	0    -1   -1   0   
$EndComp
Text Notes 8850 5250 0    60   ~ 0
Tips' connectors
Text Notes 7800 9150 0    60   ~ 0
ESD protection
Text Label 7300 2100 0    60   ~ 0
UART2_TX
Text Label 7300 2000 0    60   ~ 0
UART2_RX
Text Label 7300 2200 0    60   ~ 0
Tx/Rx
Entry Wire Line
	7150 2100 7250 2200
Entry Wire Line
	7150 1900 7250 2000
Entry Wire Line
	7150 2000 7250 2100
$Comp
L CONN_3 XL?
U 1 1 4EAC5F4E
P 8000 2100
F 0 "XL?" V 7950 2100 40  0000 C CNN
F 1 "CONN_3" V 8050 2100 40  0000 C CNN
	1    8000 2100
	-1   0    0    -1  
$EndComp
Entry Wire Line
	8550 1800 8650 1900
Entry Wire Line
	8550 1900 8650 2000
Entry Wire Line
	8550 2100 8650 2200
Entry Wire Line
	5950 2800 6050 2900
Entry Wire Line
	5950 2000 6050 2100
Entry Wire Line
	5950 1900 6050 2000
Text Label 8700 2000 0    60   ~ 0
Tx/Rx
Text Notes 9300 2700 0    60   ~ 0
Disable TX\nwhen start
$Comp
L GND #PWR?
U 1 1 4EAC59D5
P 9150 2950
F 0 "#PWR?" H 9240 2930 30  0001 C CNN
F 1 "GND" H 9150 2870 30  0001 C CNN
	1    9150 2950
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 4EAC59CD
P 9150 2650
F 0 "R?" V 9230 2650 50  0000 C CNN
F 1 "10k" V 9150 2650 50  0000 C CNN
	1    9150 2650
	1    0    0    -1  
$EndComp
Text Notes 6600 2700 0    60   ~ 0
Debug\nUART
$Comp
L GND #PWR?
U 1 1 4EAC4E0F
P 6250 2900
F 0 "#PWR?" H 6340 2880 30  0001 C CNN
F 1 "GND" H 6250 2820 30  0001 C CNN
	1    6250 2900
	1    0    0    -1  
$EndComp
$Comp
L CONN_2 XL?
U 1 1 4EAC4DF0
P 6450 2700
F 0 "XL?" V 6400 2700 40  0000 C CNN
F 1 "CONN_2" V 6500 2700 40  0000 C CNN
	1    6450 2700
	1    0    0    -1  
$EndComp
NoConn ~ 5250 2700
Text Label 5350 1900 0    60   ~ 0
UART2_TX
Text Label 5350 2000 0    60   ~ 0
UART2_RX
$Comp
L VCC #PWR?
U 1 1 4EAC4A83
P 14450 6000
F 0 "#PWR?" H 14450 6050 30  0001 C CNN
F 1 "VCC" H 14520 6030 30  0000 C CNN
	1    14450 6000
	-1   0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 4EAC4A82
P 14350 6100
F 0 "#PWR?" H 14440 6080 30  0001 C CNN
F 1 "GND" H 14350 6020 30  0001 C CNN
	1    14350 6100
	0    -1   1    0   
$EndComp
Text Label 1900 9450 0    60   ~ 0
PWR_ERROR
Text Label 5400 3600 0    60   ~ 0
PWR_ERROR
$Comp
L C C?
U 1 1 4EAC43C3
P 2650 3400
F 0 "C?" V 2550 3250 50  0000 L CNN
F 1 "0.1u" V 2550 3550 50  0000 L CNN
	1    2650 3400
	0    1    1    0   
$EndComp
$Comp
L GND #PWR?
U 1 1 4EAC43B6
P 2300 3400
F 0 "#PWR?" H 2390 3380 30  0001 C CNN
F 1 "GND" H 2300 3320 30  0001 C CNN
	1    2300 3400
	0    1    1    0   
$EndComp
NoConn ~ 3550 9450
NoConn ~ 3550 9350
$Comp
L C C?
U 1 1 4EAC41F0
P 4050 9500
F 0 "C?" H 4100 9600 50  0000 L CNN
F 1 "10n" H 4050 9400 50  0000 L CNN
	1    4050 9500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 4EAC41D8
P 3800 10400
F 0 "#PWR?" H 3890 10380 30  0001 C CNN
F 1 "GND" H 3800 10320 30  0001 C CNN
	1    3800 10400
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 4EAC41A0
P 3800 10100
F 0 "R?" V 3880 10100 50  0000 C CNN
F 1 "120k" V 3800 10100 50  0000 C CNN
	1    3800 10100
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 4EAC419B
P 3800 9500
F 0 "R?" V 3880 9500 50  0000 C CNN
F 1 "200k" V 3800 9500 50  0000 C CNN
	1    3800 9500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 4EABEFD4
P 2450 10400
F 0 "#PWR?" H 2540 10380 30  0001 C CNN
F 1 "GND" H 2450 10320 30  0001 C CNN
	1    2450 10400
	1    0    0    -1  
$EndComp
$Comp
L LP2951 DA?
U 1 1 4EABEFAD
P 3000 9450
F 0 "DA?" H 2700 9850 60  0000 C CNN
F 1 "LP2951" H 3200 9850 60  0000 C CNN
	1    3000 9450
	1    0    0    -1  
$EndComp
NoConn ~ 13800 2400
NoConn ~ 13800 2100
Text Notes 13300 1850 0    60   ~ 0
Terminator
Text Label 12500 2500 0    60   ~ 0
IsTerminatorOn
$Comp
L GND #PWR?
U 1 1 4EABE7CE
P 13850 2600
F 0 "#PWR?" H 13940 2580 30  0001 C CNN
F 1 "GND" H 13850 2520 30  0001 C CNN
	1    13850 2600
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 4EABE7A1
P 14150 2200
F 0 "R?" V 14230 2200 50  0000 C CNN
F 1 "120R" V 14150 2200 50  0000 C CNN
	1    14150 2200
	0    1    1    0   
$EndComp
$Comp
L SWITCH_2X SW?
U 1 1 4EABE787
P 13550 2300
F 0 "SW?" H 13300 2650 60  0000 C CNN
F 1 "SWITCH_2X" H 13650 2650 60  0000 C CNN
	1    13550 2300
	1    0    0    -1  
$EndComp
Text Label 5350 2800 0    60   ~ 0
Tx/Rx
Text Label 8700 1900 0    60   ~ 0
UART2_RX
Text Label 8700 2200 0    60   ~ 0
UART2_TX
$Comp
L +3.3V #PWR?
U 1 1 4EABDE2F
P 10500 2200
F 0 "#PWR?" H 10500 2160 30  0001 C CNN
F 1 "+3.3V" H 10580 2230 30  0000 C CNN
	1    10500 2200
	1    0    0    1   
$EndComp
$Comp
L GND #PWR?
U 1 1 4EABDE21
P 10450 2700
F 0 "#PWR?" H 10540 2680 30  0001 C CNN
F 1 "GND" H 10450 2620 30  0001 C CNN
	1    10450 2700
	-1   0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 4EABDE14
P 10450 2450
F 0 "C?" H 10500 2550 50  0000 L CNN
F 1 "0.1u" H 10500 2350 50  0000 L CNN
	1    10450 2450
	-1   0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 4EABDE0A
P 10200 2700
F 0 "#PWR?" H 10290 2680 30  0001 C CNN
F 1 "GND" H 10200 2620 30  0001 C CNN
	1    10200 2700
	-1   0    0    -1  
$EndComp
Text Label 11350 2050 0    60   ~ 0
RS485_B
Text Label 11350 1850 0    60   ~ 0
RS485_A
$Comp
L VCC #PWR?
U 1 1 4EABD3CC
P 9100 10100
F 0 "#PWR?" H 9100 10150 30  0001 C CNN
F 1 "VCC" H 9170 10130 30  0000 C CNN
	1    9100 10100
	-1   0    0    1   
$EndComp
$Comp
L VCC #PWR?
U 1 1 4EABD3BF
P 9100 9400
F 0 "#PWR?" H 9100 9450 30  0001 C CNN
F 1 "VCC" H 9170 9430 30  0000 C CNN
	1    9100 9400
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR?
U 1 1 4EABD37C
P 9600 9750
F 0 "#PWR?" H 9690 9730 30  0001 C CNN
F 1 "GND" H 9600 9670 30  0001 C CNN
	1    9600 9750
	0    1    1    0   
$EndComp
$Comp
L GND #PWR?
U 1 1 4EABD357
P 7550 9750
F 0 "#PWR?" H 7640 9730 30  0001 C CNN
F 1 "GND" H 7550 9670 30  0001 C CNN
	1    7550 9750
	0    1    1    0   
$EndComp
$Comp
L GND #PWR?
U 1 1 4EABD347
P 8600 9750
F 0 "#PWR?" H 8690 9730 30  0001 C CNN
F 1 "GND" H 8600 9670 30  0001 C CNN
	1    8600 9750
	0    1    1    0   
$EndComp
Text Label 7300 10100 0    60   ~ 0
ADC7
Text Label 7300 9400 0    60   ~ 0
ADC6
Text Label 8350 10100 0    60   ~ 0
ADC5
Text Label 8350 9400 0    60   ~ 0
ADC4
$Comp
L PESD1CAN DA?
U 1 1 4EABD303
P 9250 9750
F 0 "DA?" H 8750 9850 60  0000 C CNN
F 1 "PESD1CAN" H 8900 9950 60  0000 C CNN
	1    9250 9750
	1    0    0    -1  
$EndComp
$Comp
L PESD1CAN DA?
U 1 1 4EABD2FE
P 7200 9750
F 0 "DA?" H 6700 9850 60  0000 C CNN
F 1 "PESD1CAN" H 6850 9950 60  0000 C CNN
	1    7200 9750
	1    0    0    -1  
$EndComp
$Comp
L PESD1CAN DA?
U 1 1 4EABD2F7
P 8250 9750
F 0 "DA?" H 7750 9850 60  0000 C CNN
F 1 "PESD1CAN" H 7900 9950 60  0000 C CNN
	1    8250 9750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 4EABD258
P 14350 6300
F 0 "#PWR?" H 14440 6280 30  0001 C CNN
F 1 "GND" H 14350 6220 30  0001 C CNN
	1    14350 6300
	0    -1   1    0   
$EndComp
$Comp
L VCC #PWR?
U 1 1 4EABD257
P 14450 6400
F 0 "#PWR?" H 14450 6450 30  0001 C CNN
F 1 "VCC" H 14520 6430 30  0000 C CNN
	1    14450 6400
	-1   0    0    -1  
$EndComp
Text Label 7550 5550 0    60   ~ 0
SENSOR_PWR
Text Label 5400 2500 0    60   ~ 0
SENSOR_PWR
$Comp
L VCC #PWR?
U 1 1 4EAB1435
P 1350 9200
F 0 "#PWR?" H 1350 9250 30  0001 C CNN
F 1 "VCC" H 1420 9230 30  0000 C CNN
	1    1350 9200
	-1   0    0    1   
$EndComp
Text Notes 2350 8900 0    60   ~ 0
3.3V power regulator
$Comp
L C C?
U 1 1 4EAB139A
P 1400 9450
F 0 "C?" H 1450 9550 50  0000 L CNN
F 1 "1u" H 1450 9350 50  0000 L CNN
	1    1400 9450
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 4EAB1399
P 4400 9450
F 0 "C?" H 4450 9550 50  0000 L CNN
F 1 "1u" H 4450 9350 50  0000 L CNN
	1    4400 9450
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 4EAB1397
P 1400 9700
F 0 "#PWR?" H 1490 9680 30  0001 C CNN
F 1 "GND" H 1400 9620 30  0001 C CNN
	1    1400 9700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 4EAB1396
P 2750 10400
F 0 "#PWR?" H 2840 10380 30  0001 C CNN
F 1 "GND" H 2750 10320 30  0001 C CNN
	1    2750 10400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 4EAB1395
P 4400 9700
F 0 "#PWR?" H 4490 9680 30  0001 C CNN
F 1 "GND" H 4400 9620 30  0001 C CNN
	1    4400 9700
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 4EAB1394
P 4650 9450
F 0 "C?" H 4700 9550 50  0000 L CNN
F 1 "10u" H 4700 9350 50  0000 L CNN
	1    4650 9450
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 4EAB1393
P 4650 9700
F 0 "#PWR?" H 4740 9680 30  0001 C CNN
F 1 "GND" H 4650 9620 30  0001 C CNN
	1    4650 9700
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR?
U 1 1 4EAB1392
P 4800 9200
F 0 "#PWR?" H 4800 9160 30  0001 C CNN
F 1 "+3.3V" H 4880 9230 30  0000 C CNN
	1    4800 9200
	1    0    0    -1  
$EndComp
NoConn ~ 3000 3200
NoConn ~ 3000 3100
NoConn ~ 3000 3000
NoConn ~ 3000 2900
NoConn ~ 3000 2800
NoConn ~ 3000 2700
NoConn ~ 3000 1900
NoConn ~ 3000 1800
NoConn ~ 3000 1700
Text Notes 2050 2350 0    60   ~ 0
GPIOs
$Comp
L GND #PWR?
U 1 1 4EAB0FC7
P 2650 2750
F 0 "#PWR?" H 2740 2730 30  0001 C CNN
F 1 "GND" H 2650 2670 30  0001 C CNN
	1    2650 2750
	1    0    0    -1  
$EndComp
$Comp
L CONN_8 XL?
U 1 1 4EAB0FB9
P 2450 2350
F 0 "XL?" V 2400 2350 50  0000 C CNN
F 1 "CONN_8" V 2500 2350 50  0000 C CNN
	1    2450 2350
	-1   0    0    -1  
$EndComp
NoConn ~ 5250 1800
NoConn ~ 5250 1700
Text Label 8000 8050 0    60   ~ 0
ADC7
Text Label 8000 7300 0    60   ~ 0
ADC6
NoConn ~ 5250 3800
NoConn ~ 5250 3700
NoConn ~ 5250 3450
NoConn ~ 5250 3350
Text Label 8000 6550 0    60   ~ 0
ADC5
Text Label 5400 2200 0    60   ~ 0
ADC5
Text Label 5400 2300 0    60   ~ 0
ADC6
Text Label 5400 2100 0    60   ~ 0
ADC4
Text Label 5400 2400 0    60   ~ 0
ADC7
Text Label 8000 5800 0    60   ~ 0
ADC4
$Comp
L GND #PWR?
U 1 1 4EAB09F1
P 8600 5900
F 0 "#PWR?" H 8690 5880 30  0001 C CNN
F 1 "GND" H 8600 5820 30  0001 C CNN
	1    8600 5900
	0    -1   1    0   
$EndComp
$Comp
L R R?
U 1 1 4EAB09B2
P 8500 5550
F 0 "R?" V 8580 5550 50  0000 C CNN
F 1 "200k" V 8500 5550 50  0000 C CNN
	1    8500 5550
	0    1    -1   0   
$EndComp
NoConn ~ 9400 5700
NoConn ~ 9400 6000
$Comp
L RJ11 XL?
U 1 1 4EAB097E
P 9750 5850
F 0 "XL?" H 9600 6150 60  0000 C CNN
F 1 "RJ11" H 9850 6150 60  0000 C CNN
	1    9750 5850
	-1   0    0    -1  
$EndComp
Text Label 5350 3000 0    60   ~ 0
SWDIO
Text Label 5350 3100 0    60   ~ 0
SWCLK
Text Label 5350 2600 0    60   ~ 0
UART1_TX
$Comp
L VCC #PWR?
U 1 1 4EAB022E
P 14450 5800
F 0 "#PWR?" H 14450 5850 30  0001 C CNN
F 1 "VCC" H 14520 5830 30  0000 C CNN
	1    14450 5800
	-1   0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 4EAB0219
P 14350 5700
F 0 "#PWR?" H 14440 5680 30  0001 C CNN
F 1 "GND" H 14350 5620 30  0001 C CNN
	1    14350 5700
	0    -1   1    0   
$EndComp
$Comp
L RJ45 XL?
U 1 1 4EAB01F7
P 14850 6050
F 0 "XL?" H 14600 6550 60  0000 C CNN
F 1 "RJ45" H 14900 6550 60  0000 C CNN
	1    14850 6050
	-1   0    0    -1  
$EndComp
$Comp
L +3.3V #PWR?
U 1 1 4EAB0050
P 2100 5750
F 0 "#PWR?" H 2100 5710 30  0001 C CNN
F 1 "+3.3V" H 2180 5780 30  0000 C CNN
	1    2100 5750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 4EAB004F
P 2100 5900
F 0 "#PWR?" H 2190 5880 30  0001 C CNN
F 1 "GND" H 2100 5820 30  0001 C CNN
	1    2100 5900
	1    0    0    -1  
$EndComp
$Comp
L ST_SWD XL?
U 1 1 4EAB004E
P 1600 5700
F 0 "XL?" H 1450 6000 60  0000 C CNN
F 1 "ST_SWD" H 1800 6000 60  0000 C CNN
	1    1600 5700
	1    0    0    -1  
$EndComp
Text Label 2050 5550 0    60   ~ 0
SWCLK
Text Label 2050 5650 0    60   ~ 0
SWDIO
Text Notes 750  5750 0    60   ~ 0
Programming
$Comp
L GND #PWR?
U 1 1 4EAAFEB5
P 3750 4500
F 0 "#PWR?" H 3840 4480 30  0001 C CNN
F 1 "GND" H 3750 4420 30  0001 C CNN
	1    3750 4500
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 4EAAFEB4
P 1950 4300
F 0 "C?" H 2000 4400 50  0000 L CNN
F 1 "1u" H 2000 4200 50  0000 L CNN
	1    1950 4300
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 4EAAFEB3
P 1700 4300
F 0 "C?" H 1750 4400 50  0000 L CNN
F 1 "0.1u" H 1750 4200 50  0000 L CNN
	1    1700 4300
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 4EAAFEB2
P 1450 4300
F 0 "C?" H 1500 4400 50  0000 L CNN
F 1 "0.1u" H 1500 4200 50  0000 L CNN
	1    1450 4300
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR?
U 1 1 4EAAFEB1
P 1400 3650
F 0 "#PWR?" H 1400 3610 30  0001 C CNN
F 1 "+3.3V" H 1480 3680 30  0000 C CNN
	1    1400 3650
	-1   0    0    1   
$EndComp
$Comp
L +3.3V #PWR?
U 1 1 4EAAFEB0
P 1650 3750
F 0 "#PWR?" H 1650 3710 30  0001 C CNN
F 1 "+3.3V" H 1730 3780 30  0000 C CNN
	1    1650 3750
	-1   0    0    1   
$EndComp
$Comp
L +3.3V #PWR?
U 1 1 4EAAFEAF
P 1900 3900
F 0 "#PWR?" H 1900 3860 30  0001 C CNN
F 1 "+3.3V" H 1980 3930 30  0000 C CNN
	1    1900 3900
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR?
U 1 1 4EAAFEAE
P 1450 4600
F 0 "#PWR?" H 1540 4580 30  0001 C CNN
F 1 "GND" H 1450 4520 30  0001 C CNN
	1    1450 4600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 4EAAFEAD
P 1700 4600
F 0 "#PWR?" H 1790 4580 30  0001 C CNN
F 1 "GND" H 1700 4520 30  0001 C CNN
	1    1700 4600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 4EAAFEAC
P 1950 4600
F 0 "#PWR?" H 2040 4580 30  0001 C CNN
F 1 "GND" H 1950 4520 30  0001 C CNN
	1    1950 4600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 4EAAFEAB
P 2950 4600
F 0 "#PWR?" H 3040 4580 30  0001 C CNN
F 1 "GND" H 2950 4520 30  0001 C CNN
	1    2950 4600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 4EAAFEAA
P 2700 4600
F 0 "#PWR?" H 2790 4580 30  0001 C CNN
F 1 "GND" H 2700 4520 30  0001 C CNN
	1    2700 4600
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 4EAAFEA9
P 2700 4300
F 0 "C?" H 2750 4400 50  0000 L CNN
F 1 "0.1u" H 2750 4200 50  0000 L CNN
	1    2700 4300
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 4EAAFEA8
P 2950 4300
F 0 "C?" H 3000 4400 50  0000 L CNN
F 1 "1u" H 3000 4200 50  0000 L CNN
	1    2950 4300
	1    0    0    -1  
$EndComp
$Comp
L L L?
U 1 1 4EAAFEA7
P 2450 4050
F 0 "L?" V 2360 3960 40  0000 C CNN
F 1 "10uH" V 2350 4150 40  0000 C CNN
	1    2450 4050
	0    1    1    0   
$EndComp
$Comp
L +3.3V #PWR?
U 1 1 4EAAFEA6
P 2200 4050
F 0 "#PWR?" H 2200 4010 30  0001 C CNN
F 1 "+3.3V" H 2280 4080 30  0000 C CNN
	1    2200 4050
	-1   0    0    1   
$EndComp
$Comp
L STM32F10X-48 DD?
U 1 1 4EAAFE98
P 4000 2900
F 0 "DD?" H 3500 4250 60  0000 C CNN
F 1 "STM32F100c8-48" H 4150 4250 60  0000 C CNN
	1    4000 2900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 4EAAFE97
P 1200 4600
F 0 "#PWR?" H 1290 4580 30  0001 C CNN
F 1 "GND" H 1200 4520 30  0001 C CNN
	1    1200 4600
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR?
U 1 1 4EAAFE96
P 1150 3550
F 0 "#PWR?" H 1150 3510 30  0001 C CNN
F 1 "+3.3V" H 1230 3580 30  0000 C CNN
	1    1150 3550
	-1   0    0    1   
$EndComp
$Comp
L C C?
U 1 1 4EAAFE95
P 1200 4300
F 0 "C?" H 1250 4400 50  0000 L CNN
F 1 "0.1u" H 1250 4200 50  0000 L CNN
	1    1200 4300
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG?
U 1 1 4EAAFE85
P 2850 4000
F 0 "#FLG?" H 2850 4270 30  0001 C CNN
F 1 "PWR_FLAG" H 2850 4230 30  0000 C CNN
	1    2850 4000
	1    0    0    -1  
$EndComp
$EndSCHEMATC
