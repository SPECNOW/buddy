EESchema Schematic File Version 2
LIBS:power
LIBS:device
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
EELAYER 25 0
EELAYER END
$Descr USLedger 17000 11000
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
L CONN_02X10 P1
U 1 1 59CC7536
P 2150 1400
F 0 "P1" H 2150 1950 50  0000 C CNN
F 1 "CONN_02X10" V 2150 1400 50  0000 C CNN
F 2 "" H 2150 200 50  0000 C CNN
F 3 "" H 2150 200 50  0000 C CNN
	1    2150 1400
	1    0    0    -1  
$EndComp
$Comp
L CONN_02X10 P2
U 1 1 59CC75BF
P 2150 2700
F 0 "P2" H 2150 3250 50  0000 C CNN
F 1 "CONN_02X10" V 2150 2700 50  0000 C CNN
F 2 "" H 2150 1500 50  0000 C CNN
F 3 "" H 2150 1500 50  0000 C CNN
	1    2150 2700
	1    0    0    -1  
$EndComp
$Comp
L CONN_02X10 P3
U 1 1 59CC7A66
P 2150 3950
F 0 "P3" H 2150 4500 50  0000 C CNN
F 1 "CONN_02X10" V 2150 3950 50  0000 C CNN
F 2 "" H 2150 2750 50  0000 C CNN
F 3 "" H 2150 2750 50  0000 C CNN
	1    2150 3950
	1    0    0    -1  
$EndComp
$Comp
L CONN_02X10 P4
U 1 1 59CC7AD3
P 2150 5200
F 0 "P4" H 2150 5750 50  0000 C CNN
F 1 "CONN_02X10" V 2150 5200 50  0000 C CNN
F 2 "" H 2150 4000 50  0000 C CNN
F 3 "" H 2150 4000 50  0000 C CNN
	1    2150 5200
	1    0    0    -1  
$EndComp
$Comp
L LM7812 U1
U 1 1 59CC7D28
P 8150 1900
F 0 "U1" H 8300 1704 50  0000 C CNN
F 1 "LM7812" H 8150 2100 50  0000 C CNN
F 2 "" H 8150 1900 50  0000 C CNN
F 3 "" H 8150 1900 50  0000 C CNN
	1    8150 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	4250 1000 4250 1350
Wire Wire Line
	4950 1000 4950 1350
Wire Wire Line
	5550 1000 5550 1350
$Comp
L +5V #PWR?
U 1 1 59CC816B
P 4250 1000
F 0 "#PWR?" H 4250 850 50  0001 C CNN
F 1 "+5V" H 4250 1140 50  0000 C CNN
F 2 "" H 4250 1000 50  0000 C CNN
F 3 "" H 4250 1000 50  0000 C CNN
	1    4250 1000
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR?
U 1 1 59CC82A7
P 4950 1000
F 0 "#PWR?" H 4950 850 50  0001 C CNN
F 1 "+3.3V" H 4950 1140 50  0000 C CNN
F 2 "" H 4950 1000 50  0000 C CNN
F 3 "" H 4950 1000 50  0000 C CNN
	1    4950 1000
	1    0    0    -1  
$EndComp
$Comp
L +12V #PWR?
U 1 1 59CC82DB
P 5550 1000
F 0 "#PWR?" H 5550 850 50  0001 C CNN
F 1 "+12V" H 5550 1140 50  0000 C CNN
F 2 "" H 5550 1000 50  0000 C CNN
F 3 "" H 5550 1000 50  0000 C CNN
	1    5550 1000
	1    0    0    -1  
$EndComp
$Comp
L LED D?
U 1 1 59CC847B
P 4250 1500
F 0 "D?" H 4250 1600 50  0000 C CNN
F 1 "LED" H 4250 1400 50  0000 C CNN
F 2 "" H 4250 1500 50  0000 C CNN
F 3 "" H 4250 1500 50  0000 C CNN
	1    4250 1500
	0    -1   -1   0   
$EndComp
$Comp
L LED D?
U 1 1 59CC8594
P 4950 1500
F 0 "D?" H 4950 1600 50  0000 C CNN
F 1 "LED" H 4950 1400 50  0000 C CNN
F 2 "" H 4950 1500 50  0000 C CNN
F 3 "" H 4950 1500 50  0000 C CNN
	1    4950 1500
	0    -1   -1   0   
$EndComp
$Comp
L LED D?
U 1 1 59CC8654
P 5550 1500
F 0 "D?" H 5550 1600 50  0000 C CNN
F 1 "LED" H 5550 1400 50  0000 C CNN
F 2 "" H 5550 1500 50  0000 C CNN
F 3 "" H 5550 1500 50  0000 C CNN
	1    5550 1500
	0    -1   -1   0   
$EndComp
$Comp
L R R?
U 1 1 59CC8850
P 4250 1900
F 0 "R?" V 4330 1900 50  0000 C CNN
F 1 "R" V 4250 1900 50  0000 C CNN
F 2 "" V 4180 1900 50  0000 C CNN
F 3 "" H 4250 1900 50  0000 C CNN
	1    4250 1900
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 59CC88BB
P 4950 1900
F 0 "R?" V 5030 1900 50  0000 C CNN
F 1 "R" V 4950 1900 50  0000 C CNN
F 2 "" V 4880 1900 50  0000 C CNN
F 3 "" H 4950 1900 50  0000 C CNN
	1    4950 1900
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 59CC8909
P 5550 1900
F 0 "R?" V 5630 1900 50  0000 C CNN
F 1 "R" V 5550 1900 50  0000 C CNN
F 2 "" V 5480 1900 50  0000 C CNN
F 3 "" H 5550 1900 50  0000 C CNN
	1    5550 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	4250 1650 4250 1750
Wire Wire Line
	4950 1750 4950 1650
Wire Wire Line
	5550 1750 5550 1650
Wire Wire Line
	4950 2050 4950 2200
$Comp
L GND #PWR?
U 1 1 59CC89B4
P 4950 2200
F 0 "#PWR?" H 4950 1950 50  0001 C CNN
F 1 "GND" H 4950 2050 50  0000 C CNN
F 2 "" H 4950 2200 50  0000 C CNN
F 3 "" H 4950 2200 50  0000 C CNN
	1    4950 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4250 2050 4250 2200
$Comp
L GND #PWR?
U 1 1 59CC8A58
P 4250 2200
F 0 "#PWR?" H 4250 1950 50  0001 C CNN
F 1 "GND" H 4250 2050 50  0000 C CNN
F 2 "" H 4250 2200 50  0000 C CNN
F 3 "" H 4250 2200 50  0000 C CNN
	1    4250 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5550 2050 5550 2200
$Comp
L GND #PWR?
U 1 1 59CC8A8D
P 5550 2200
F 0 "#PWR?" H 5550 1950 50  0001 C CNN
F 1 "GND" H 5550 2050 50  0000 C CNN
F 2 "" H 5550 2200 50  0000 C CNN
F 3 "" H 5550 2200 50  0000 C CNN
	1    5550 2200
	1    0    0    -1  
$EndComp
$Comp
L 2N7002 Q?
U 1 1 59CC8E28
P 3750 3750
F 0 "Q?" H 3950 3825 50  0000 L CNN
F 1 "2N7002" H 3950 3750 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 3950 3675 50  0001 L CIN
F 3 "" H 3750 3750 50  0000 L CNN
	1    3750 3750
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 59CC9069
P 4100 3700
F 0 "R?" V 4180 3700 50  0000 C CNN
F 1 "4.7KR" V 4100 3700 50  0000 C CNN
F 2 "" V 4030 3700 50  0000 C CNN
F 3 "" H 4100 3700 50  0000 C CNN
	1    4100 3700
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 59CC9131
P 3400 3700
F 0 "R?" V 3480 3700 50  0000 C CNN
F 1 "4.7KR" V 3400 3700 50  0000 C CNN
F 2 "" V 3330 3700 50  0000 C CNN
F 3 "" H 3400 3700 50  0000 C CNN
	1    3400 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 3550 3700 3550
Wire Wire Line
	3700 3550 3700 3450
Wire Wire Line
	4100 3550 4100 3450
Wire Wire Line
	3950 3850 4100 3850
Wire Wire Line
	4100 3850 4300 3850
Wire Wire Line
	3200 3850 3400 3850
Wire Wire Line
	3400 3850 3550 3850
Wire Wire Line
	3450 3850 3450 3800
Connection ~ 3400 3850
Connection ~ 4100 3850
$Comp
L +5V #PWR?
U 1 1 59CC958E
P 4100 3450
F 0 "#PWR?" H 4100 3300 50  0001 C CNN
F 1 "+5V" H 4100 3590 50  0000 C CNN
F 2 "" H 4100 3450 50  0000 C CNN
F 3 "" H 4100 3450 50  0000 C CNN
	1    4100 3450
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR?
U 1 1 59CC95C3
P 3700 3450
F 0 "#PWR?" H 3700 3300 50  0001 C CNN
F 1 "+3.3V" H 3700 3590 50  0000 C CNN
F 2 "" H 3700 3450 50  0000 C CNN
F 3 "" H 3700 3450 50  0000 C CNN
	1    3700 3450
	1    0    0    -1  
$EndComp
$Comp
L +12V #PWR?
U 1 1 59E82D7B
P 9100 1850
F 0 "#PWR?" H 9100 1700 50  0001 C CNN
F 1 "+12V" H 9100 1990 50  0000 C CNN
F 2 "" H 9100 1850 50  0000 C CNN
F 3 "" H 9100 1850 50  0000 C CNN
	1    9100 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	8550 1850 8700 1850
Wire Wire Line
	8700 1850 8900 1850
Wire Wire Line
	8900 1850 9100 1850
$Comp
L GND #PWR?
U 1 1 59E835C0
P 8150 2250
F 0 "#PWR?" H 8150 2000 50  0001 C CNN
F 1 "GND" H 8150 2100 50  0000 C CNN
F 2 "" H 8150 2250 50  0000 C CNN
F 3 "" H 8150 2250 50  0000 C CNN
	1    8150 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	8150 2150 8150 2200
Wire Wire Line
	8150 2200 8150 2250
Wire Wire Line
	7750 1850 7500 1850
Wire Wire Line
	7500 1850 7250 1850
Wire Wire Line
	7250 1850 7150 1850
$Comp
L CP1_Small C?
U 1 1 59E83B19
P 7250 2000
F 0 "C?" H 7260 2070 50  0000 L CNN
F 1 "CP1_Small" H 7260 1920 50  0000 L CNN
F 2 "" H 7250 2000 50  0000 C CNN
F 3 "" H 7250 2000 50  0000 C CNN
	1    7250 2000
	1    0    0    -1  
$EndComp
$Comp
L CP1_Small C?
U 1 1 59E83BB8
P 8900 2000
F 0 "C?" H 8910 2070 50  0000 L CNN
F 1 "CP1_Small" H 8910 1920 50  0000 L CNN
F 2 "" H 8900 2000 50  0000 C CNN
F 3 "" H 8900 2000 50  0000 C CNN
	1    8900 2000
	1    0    0    -1  
$EndComp
$Comp
L C_Small C?
U 1 1 59E83CC8
P 7500 2000
F 0 "C?" H 7510 2070 50  0000 L CNN
F 1 "C_Small" H 7510 1920 50  0000 L CNN
F 2 "" H 7500 2000 50  0000 C CNN
F 3 "" H 7500 2000 50  0000 C CNN
	1    7500 2000
	1    0    0    -1  
$EndComp
$Comp
L C_Small C?
U 1 1 59E83D27
P 8700 2000
F 0 "C?" H 8710 2070 50  0000 L CNN
F 1 "C_Small" H 8710 1920 50  0000 L CNN
F 2 "" H 8700 2000 50  0000 C CNN
F 3 "" H 8700 2000 50  0000 C CNN
	1    8700 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	7500 2100 7500 2200
Wire Wire Line
	7250 2200 7500 2200
Wire Wire Line
	7500 2200 8150 2200
Wire Wire Line
	8150 2200 8700 2200
Wire Wire Line
	8700 2200 8700 2200
Connection ~ 8150 2200
Wire Wire Line
	7250 2100 7250 2200
Connection ~ 7500 2200
Wire Wire Line
	7500 1900 7500 1850
Connection ~ 7500 1850
Wire Wire Line
	7250 1900 7250 1850
Connection ~ 7250 1850
Wire Wire Line
	8700 1900 8700 1850
Connection ~ 8700 1850
Wire Wire Line
	8900 1850 8900 1900
Connection ~ 8900 1850
Wire Wire Line
	8700 2200 8700 2100
Wire Wire Line
	8900 2100 8900 2200
Wire Wire Line
	8900 2200 8700 2200
Connection ~ 8700 2200
$EndSCHEMATC
