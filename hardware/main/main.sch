EESchema Schematic File Version 4
LIBS:main-cache
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 8
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 3050 2400 1200 1050
U 5C9C4D6B
F0 "CHARGER" 50
F1 "Charger.sch" 50
F2 "Charge" O R 4250 2950 50 
F3 "RL_ON" O R 4250 3050 50 
F4 "RL_OFF" O R 4250 3150 50 
F5 "GND" O R 4250 2600 50 
F6 "+18V_IN" I R 4250 2500 50 
$EndSheet
$Sheet
S 6000 2400 1250 1050
U 5CA4DC1F
F0 "CONTROLLER" 50
F1 "controller.sch" 50
F2 "RL_OFF" I L 6000 3150 50 
F3 "RL_ON" I L 6000 3050 50 
F4 "Charge" I L 6000 2950 50 
F5 "GND" I L 6000 2600 50 
F6 "+18V_OUT" I L 6000 2500 50 
$EndSheet
$Comp
L Connector:Conn_01x07_Male J101
U 1 1 5CA76C06
P 4700 3050
F 0 "J101" H 5000 3500 50  0000 R CNN
F 1 "Conn_01x07_Male" H 5000 3400 50  0000 R CNN
F 2 "" H 4700 3050 50  0001 C CNN
F 3 "~" H 4700 3050 50  0001 C CNN
	1    4700 3050
	-1   0    0    1   
$EndComp
Wire Wire Line
	4250 3150 4350 3150
Wire Wire Line
	4350 2950 4350 2850
Wire Wire Line
	4350 2850 4500 2850
Wire Wire Line
	4250 3050 4500 3050
Wire Wire Line
	4350 3150 4350 3250
Wire Wire Line
	4350 3250 4500 3250
Wire Wire Line
	4500 2950 4400 2950
Wire Wire Line
	4400 2950 4400 3150
Wire Wire Line
	4400 3150 4500 3150
Wire Wire Line
	4400 3150 4400 3350
Wire Wire Line
	4400 3350 4500 3350
Connection ~ 4400 3150
Text Notes 4400 5000 0    50   ~ 0
TO-DO LIST:\n  [x] Separar a abstração das duas placas\n\n  [x] Criar conexão entre as duas placas\n\n  [x] Nomear componentes;\n\n  [x] Passar alimentação para o Charger;\n\n  [x] Criar fonte 12V no  Charger\n\n  [_] Adicionar snubbers para os relés (olhar na simulação);\n\n  [_] Passar o condicionamento do sinal para o Charger;\n
Wire Wire Line
	4350 2950 4250 2950
Wire Wire Line
	4250 2600 4400 2600
Wire Wire Line
	4400 2600 4400 2950
Connection ~ 4400 2950
Wire Wire Line
	4250 2500 4450 2500
Wire Wire Line
	4450 2500 4450 2750
Wire Wire Line
	4450 2750 4500 2750
$Comp
L Connector:Conn_01x07_Male J102
U 1 1 5CAB450A
P 5550 3050
F 0 "J102" H 5850 3500 50  0000 R CNN
F 1 "Conn_01x07_Male" H 5850 3400 50  0000 R CNN
F 2 "" H 5550 3050 50  0001 C CNN
F 3 "~" H 5550 3050 50  0001 C CNN
	1    5550 3050
	1    0    0    1   
$EndComp
Wire Wire Line
	6000 3150 5900 3150
Wire Wire Line
	5900 2950 5900 2850
Wire Wire Line
	5900 2850 5750 2850
Wire Wire Line
	6000 3050 5750 3050
Wire Wire Line
	5900 3150 5900 3250
Wire Wire Line
	5900 3250 5750 3250
Wire Wire Line
	5750 2950 5850 2950
Wire Wire Line
	5850 2950 5850 3150
Wire Wire Line
	5850 3150 5750 3150
Wire Wire Line
	5850 3150 5850 3350
Wire Wire Line
	5850 3350 5750 3350
Connection ~ 5850 3150
Wire Wire Line
	5900 2950 6000 2950
Wire Wire Line
	6000 2600 5850 2600
Wire Wire Line
	5850 2600 5850 2950
Connection ~ 5850 2950
Wire Wire Line
	6000 2500 5800 2500
Wire Wire Line
	5800 2500 5800 2750
Wire Wire Line
	5800 2750 5750 2750
$EndSCHEMATC