EESchema Schematic File Version 2  date 17/01/2012 21:13:41
LIBS:Transistors_kl
LIBS:adc_kl
LIBS:Atmel_kl
LIBS:Audio_kl
LIBS:Connectors_kl
LIBS:display_kl
LIBS:Interface_kl
LIBS:memory
LIBS:modules
LIBS:OpAmp_kl
LIBS:pcb_details
LIBS:power
LIBS:Power_kl
LIBS:RF ICs
LIBS:Sensors
LIBS:SmartCard_kl
LIBS:st_kl
LIBS:standard_logic
LIBS:Tittar_kl
LIBS:flashlight-cache
EELAYER 25  0
EELAYER END
$Descr A3 16535 11700
encoding utf-8
Sheet 1 1
Title ""
Date "17 jan 2012"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	7500 3100 7200 3100
Wire Wire Line
	7850 4450 7850 3950
Wire Wire Line
	7100 6800 7100 7150
Wire Wire Line
	11550 5100 11650 5100
Wire Wire Line
	10950 5100 10950 4800
Wire Wire Line
	6950 1950 6950 1800
Wire Wire Line
	6950 1800 6850 1800
Wire Wire Line
	7650 3750 7850 3750
Connection ~ 7150 1700
Wire Wire Line
	7150 1950 7150 1700
Wire Wire Line
	10300 4550 10700 4550
Connection ~ 10300 3200
Wire Wire Line
	8350 3200 7850 3200
Connection ~ 4100 6800
Wire Wire Line
	4100 6800 4100 5950
Wire Wire Line
	4100 5950 5900 5950
Wire Wire Line
	7950 3000 8350 3000
Wire Wire Line
	10600 3200 10050 3200
Wire Wire Line
	10600 3000 10050 3000
Wire Wire Line
	6850 1700 7300 1700
Wire Wire Line
	6850 1500 7300 1500
Wire Wire Line
	6850 1300 7300 1300
Wire Wire Line
	4850 7300 3950 7300
Wire Wire Line
	6200 7050 6200 6800
Wire Wire Line
	7550 6850 7550 6700
Connection ~ 7300 6800
Wire Wire Line
	7300 6800 7300 6850
Wire Wire Line
	7550 7250 7550 7350
Wire Wire Line
	7300 7250 7300 7350
Connection ~ 7100 7050
Wire Wire Line
	7100 7150 7050 7150
Connection ~ 6200 6800
Wire Wire Line
	6200 7250 6200 7350
Connection ~ 5500 6800
Wire Wire Line
	5250 7250 5250 7350
Wire Wire Line
	5500 6800 5500 6850
Connection ~ 4750 6800
Wire Wire Line
	4750 6800 4750 6750
Connection ~ 4750 7300
Connection ~ 5250 6800
Wire Wire Line
	5500 7250 5500 7350
Wire Wire Line
	6400 6800 6100 6800
Wire Wire Line
	7100 7050 7050 7050
Connection ~ 7100 6800
Wire Wire Line
	6800 6800 7650 6800
Connection ~ 7550 6800
Wire Wire Line
	5250 6850 5250 6650
Wire Wire Line
	5700 6800 3950 6800
Wire Wire Line
	4750 7250 4750 7300
Wire Wire Line
	6850 1400 7300 1400
Wire Wire Line
	6850 1600 7300 1600
Wire Wire Line
	10050 3100 10600 3100
Wire Wire Line
	10050 3300 10600 3300
Wire Wire Line
	7200 3500 7200 3600
Wire Wire Line
	6400 5950 7400 5950
Wire Wire Line
	7400 5950 7400 6800
Connection ~ 7400 6800
Wire Wire Line
	8200 3450 8200 3300
Wire Wire Line
	7150 2600 7150 2350
Wire Wire Line
	7850 3950 7650 3950
Wire Wire Line
	8200 3300 8350 3300
Wire Wire Line
	10300 3200 10300 5100
Wire Wire Line
	10300 5100 10350 5100
Connection ~ 10300 4550
Wire Wire Line
	10850 5100 11050 5100
Connection ~ 10950 5100
Wire Wire Line
	10950 4400 10950 4200
Wire Wire Line
	7850 3200 7850 3750
Wire Wire Line
	8350 3100 8000 3100
$Comp
L R R4
U 1 1 4F12787B
P 7750 3100
F 0 "R4" V 7830 3100 50  0000 C CNN
F 1 "R" V 7750 3100 50  0000 C CNN
F 2 "SMD" V 7880 3050 60  0001 C CNN
	1    7750 3100
	0    1    1    0   
$EndComp
$Comp
L VCC #PWR01
U 1 1 4F12777C
P 10950 4200
F 0 "#PWR01" H 10950 4250 30  0001 C CNN
F 1 "VCC" H 11020 4230 30  0000 C CNN
	1    10950 4200
	0    -1   -1   0   
$EndComp
$Comp
L VCC #PWR02
U 1 1 4F127760
P 7300 1400
F 0 "#PWR02" H 7300 1450 30  0001 C CNN
F 1 "VCC" H 7370 1430 30  0000 C CNN
	1    7300 1400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 4F127746
P 6950 1950
F 0 "#PWR03" H 7040 1930 30  0001 C CNN
F 1 "GND" H 6950 1870 30  0001 C CNN
	1    6950 1950
	1    0    0    -1  
$EndComp
$Comp
L R R3
U 1 1 4F127713
P 11300 5100
F 0 "R3" V 11380 5100 50  0000 C CNN
F 1 "R" V 11300 5100 50  0000 C CNN
F 2 "SMD" V 11430 5050 60  0001 C CNN
	1    11300 5100
	0    1    1    0   
$EndComp
$Comp
L IRLML6302 Q1
U 1 1 4F1276A5
P 10900 4600
F 0 "Q1" H 11100 4650 60  0000 C CNN
F 1 "IRLML6302" H 11300 4500 60  0000 C CNN
	1    10900 4600
	1    0    0    -1  
$EndComp
$Comp
L CON1 XL6
U 1 1 4EE12487
P 11800 5100
F 0 "XL6" H 11800 5250 60  0000 C CNN
F 1 "CON1" H 11800 4950 60  0000 C CNN
	1    11800 5100
	-1   0    0    1   
$EndComp
$Comp
L CON1 XL5
U 1 1 4EE1247E
P 3800 7300
F 0 "XL5" H 3800 7450 60  0000 C CNN
F 1 "CON1" H 3800 7150 60  0000 C CNN
	1    3800 7300
	1    0    0    -1  
$EndComp
$Comp
L CON1 XL4
U 1 1 4EE12478
P 3800 6800
F 0 "XL4" H 3800 6950 60  0000 C CNN
F 1 "CON1" H 3800 6650 60  0000 C CNN
	1    3800 6800
	1    0    0    -1  
$EndComp
$Comp
L CONN_2 XL2
U 1 1 4EE11F77
P 7450 3850
F 0 "XL2" V 7400 3850 40  0000 C CNN
F 1 "CONN_2" V 7500 3850 40  0000 C CNN
	1    7450 3850
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR04
U 1 1 4EE0D6CF
P 7150 2600
F 0 "#PWR04" H 7240 2580 30  0001 C CNN
F 1 "GND" H 7150 2520 30  0001 C CNN
	1    7150 2600
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 4EE0D66A
P 7150 2150
F 0 "C3" H 7200 2250 50  0000 L CNN
F 1 "0.1u" H 7100 2050 50  0000 L CNN
	1    7150 2150
	-1   0    0    1   
$EndComp
Text Notes 11250 4300 0    60   ~ 0
OUTPUT
Text Notes 10350 5550 0    60   ~ 0
The resistor R2 can be used for \ndirect output-to-controller connection\n\nR3 is a current-limit resistor
Text Notes 5900 3250 0    60   ~ 0
This LED isn't realy used,\nso we should just\nkeep some place for it.
Text Notes 5150 3900 0    60   ~ 0
The jolt sensor is presented as a 2-pin plug.
Text Notes 4900 5700 0    60   ~ 0
This resistor can be used for direct power supply connection\nin case the DC-DC convertor is not necessery.
Text Notes 3800 7700 0    60   ~ 0
DC-DC converter with friends. the whole schema is taken from the aliensTransmitter project
$Comp
L R R2
U 1 1 4EE0D314
P 10600 5100
F 0 "R2" V 10680 5100 50  0000 C CNN
F 1 "R" V 10600 5100 50  0000 C CNN
F 2 "SMD" V 10730 5050 60  0001 C CNN
	1    10600 5100
	0    1    1    0   
$EndComp
$Comp
L GND #PWR05
U 1 1 4EE0D24D
P 8200 3450
F 0 "#PWR05" H 8290 3430 30  0001 C CNN
F 1 "GND" H 8200 3370 30  0001 C CNN
	1    8200 3450
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 4EE0D189
P 6150 5950
F 0 "R1" V 6230 5950 50  0000 C CNN
F 1 "1M" V 6150 5950 50  0000 C CNN
	1    6150 5950
	0    1    -1   0   
$EndComp
$Comp
L GND #PWR06
U 1 1 4EE0D179
P 7200 3600
F 0 "#PWR06" H 7290 3580 30  0001 C CNN
F 1 "GND" H 7200 3520 30  0001 C CNN
	1    7200 3600
	1    0    0    -1  
$EndComp
$Comp
L LED D2
U 1 1 4EE0D154
P 7200 3300
F 0 "D2" H 7300 3440 50  0000 C CNN
F 1 "LED" H 7120 3440 50  0000 C CNN
F 2 "SMD" H 7350 3490 60  0001 C CNN
	1    7200 3300
	0    1    1    0   
$EndComp
$Comp
L GND #PWR07
U 1 1 4EE0D136
P 7850 4450
F 0 "#PWR07" H 7940 4430 30  0001 C CNN
F 1 "GND" H 7850 4370 30  0001 C CNN
	1    7850 4450
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR08
U 1 1 4EE0D11D
P 10600 3000
F 0 "#PWR08" H 10600 3050 30  0001 C CNN
F 1 "VCC" H 10670 3030 30  0000 C CNN
	1    10600 3000
	1    0    0    -1  
$EndComp
Text Label 10600 3200 0    60   ~ 0
MISO
Text Label 10600 3100 0    60   ~ 0
SCK
Text Label 10600 3300 0    60   ~ 0
MOSI
Text Label 7950 3000 0    60   ~ 0
RESET
Text Label 7300 1700 0    60   ~ 0
RESET
Text Label 7300 1600 0    60   ~ 0
MOSI
Text Label 7300 1500 0    60   ~ 0
SCK
Text Label 7300 1300 0    60   ~ 0
MISO
$Comp
L ATMEL_ISP XL1
U 1 1 4EE0D048
P 6500 1550
F 0 "XL1" H 6350 1950 60  0000 C CNN
F 1 "ATMEL_ISP" V 6200 1550 50  0000 C CNN
F 2 "DIP 3x2" H 6290 2010 60  0001 C CNN
	1    6500 1550
	1    0    0    -1  
$EndComp
$Comp
L ATTINY45 DD1
U 1 1 4EE0CFCD
P 9200 3200
F 0 "DD1" H 8750 3550 60  0000 C CNN
F 1 "ATTINY45" H 9500 3550 60  0000 C CNN
F 2 "SO8-208" H 8690 3610 60  0001 C CNN
	1    9200 3200
	1    0    0    -1  
$EndComp
Text Label 6150 6800 0    60   ~ 0
Lout
$Comp
L +BATT #PWR09
U 1 1 4EBAE80F
P 5250 6650
F 0 "#PWR09" H 5250 6610 30  0001 C CNN
F 1 "+BATT" H 5330 6680 30  0000 C CNN
	1    5250 6650
	0    -1   -1   0   
$EndComp
$Comp
L PWR_FLAG #FLG010
U 1 1 4EBAE74E
P 7550 6700
F 0 "#FLG010" H 7550 6970 30  0001 C CNN
F 1 "PWR_FLAG" H 7550 6930 30  0000 C CNN
	1    7550 6700
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR011
U 1 1 4EBAE740
P 7650 6800
F 0 "#PWR011" H 7650 6850 30  0001 C CNN
F 1 "VCC" H 7720 6830 30  0000 C CNN
	1    7650 6800
	1    0    0    -1  
$EndComp
$Comp
L C C4
U 1 1 4EBAE71B
P 7300 7050
F 0 "C4" H 7350 7150 50  0000 L CNN
F 1 "0.1u" H 7250 6950 50  0000 L CNN
	1    7300 7050
	1    0    0    -1  
$EndComp
$Comp
L C C5
U 1 1 4EBAE71A
P 7550 7050
F 0 "C5" H 7600 7150 50  0000 L CNN
F 1 "10u" H 7550 6950 50  0000 L CNN
	1    7550 7050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR012
U 1 1 4EBAE719
P 7300 7350
F 0 "#PWR012" H 7390 7330 30  0001 C CNN
F 1 "GND" H 7300 7270 30  0001 C CNN
	1    7300 7350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR013
U 1 1 4EBAE718
P 7550 7350
F 0 "#PWR013" H 7640 7330 30  0001 C CNN
F 1 "GND" H 7550 7270 30  0001 C CNN
	1    7550 7350
	1    0    0    -1  
$EndComp
$Comp
L D_SHOTTKY D1
U 1 1 4EBAE6D4
P 6600 6800
F 0 "D1" H 6450 6900 40  0000 C CNN
F 1 "D_SHOTTKY" H 6700 6900 40  0000 C CNN
	1    6600 6800
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR014
U 1 1 4EBAE6AB
P 6200 7350
F 0 "#PWR014" H 6290 7330 30  0001 C CNN
F 1 "GND" H 6200 7270 30  0001 C CNN
	1    6200 7350
	1    0    0    -1  
$EndComp
$Comp
L L L1
U 1 1 4EBAE696
P 5900 6800
F 0 "L1" V 5810 6710 40  0000 C CNN
F 1 "22uH" V 5800 6900 40  0000 C CNN
	1    5900 6800
	0    1    1    0   
$EndComp
$Comp
L GND #PWR015
U 1 1 4EBAE66E
P 5500 7350
F 0 "#PWR015" H 5590 7330 30  0001 C CNN
F 1 "GND" H 5500 7270 30  0001 C CNN
	1    5500 7350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR016
U 1 1 4EBAE66C
P 5250 7350
F 0 "#PWR016" H 5340 7330 30  0001 C CNN
F 1 "GND" H 5250 7270 30  0001 C CNN
	1    5250 7350
	1    0    0    -1  
$EndComp
$Comp
L C C2
U 1 1 4EBAE654
P 5500 7050
F 0 "C2" H 5550 7150 50  0000 L CNN
F 1 "10u" H 5500 6950 50  0000 L CNN
	1    5500 7050
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 4EBAE64E
P 5250 7050
F 0 "C1" H 5300 7150 50  0000 L CNN
F 1 "0.1u" H 5200 6950 50  0000 L CNN
	1    5250 7050
	1    0    0    -1  
$EndComp
$Comp
L NCP1400 DA1
U 1 1 4EBAE61B
P 6650 7150
F 0 "DA1" H 6450 7400 60  0000 C CNN
F 1 "NCP1400" H 6850 7400 60  0000 C CNN
	1    6650 7150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR017
U 1 1 4EBAC2C0
P 4850 7300
F 0 "#PWR017" H 4940 7280 30  0001 C CNN
F 1 "GND" H 4850 7220 30  0001 C CNN
	1    4850 7300
	0    1    1    0   
$EndComp
Text Notes 4150 7150 0    60   ~ 0
-BATTERY
$Comp
L PWR_FLAG #FLG018
U 1 1 4EBAC2AE
P 4750 7250
F 0 "#FLG018" H 4750 7520 30  0001 C CNN
F 1 "PWR_FLAG" H 4750 7480 30  0000 C CNN
	1    4750 7250
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG019
U 1 1 4E57A5C9
P 4750 6750
F 0 "#FLG019" H 4750 7020 30  0001 C CNN
F 1 "PWR_FLAG" H 4750 6980 30  0000 C CNN
	1    4750 6750
	1    0    0    -1  
$EndComp
Text Notes 4150 6650 0    60   ~ 0
+BATTERY
$EndSCHEMATC
