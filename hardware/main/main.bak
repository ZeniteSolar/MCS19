EESchema Schematic File Version 4
LIBS:main-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 9
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
S 3050 2400 1200 1450
U 5C9C4D6B
F0 "CHARGER" 50
F1 "Charger.sch" 50
F2 "Charge" O R 4250 3150 50 
F3 "RL_ON" O R 4250 3500 50 
F4 "RL_OFF" O R 4250 3400 50 
F5 "GND" O R 4250 2600 50 
F6 "+18V_IN" I R 4250 2500 50 
F7 "sig_bat+" O R 4250 2950 50 
F8 "sig_cap+" O R 4250 3050 50 
$EndSheet
$Sheet
S 6000 2400 1250 1450
U 5CA4DC1F
F0 "CONTROLLER" 50
F1 "controller.sch" 50
F2 "RL_OFF" I L 6000 3400 50 
F3 "RL_ON" I L 6000 3500 50 
F4 "Charge" I L 6000 3150 50 
F5 "GND" I L 6000 2600 50 
F6 "+18V_OUT" I L 6000 2500 50 
F7 "sig_bat+" I L 6000 2950 50 
F8 "sig_cap+" I L 6000 3050 50 
$EndSheet
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
	4400 3150 4500 3150
Text Notes 4450 5600 0    50   ~ 0
TO-DO LIST:\n  [x] Separar a abstração das duas placas\n\n  [x] Criar conexão entre as duas placas\n\n  [x] Nomear componentes;\n\n  [x] Passar alimentação para o Charger;\n\n  [x] Criar fonte 12V no  Charger\n\n  [x] Adicionar snubbers para os relés (olhar na simulação);\n\n  [x] Passar o condicionamento do sinal para o Charger;\n\n  [x] Incluir pontos de teste!!!\n\n  [x] Modificar alimentação do Analog para 12V
Wire Wire Line
	4350 2950 4250 2950
Wire Wire Line
	4250 2600 4400 2600
Wire Wire Line
	4250 2500 4450 2500
Wire Wire Line
	4450 2500 4450 2750
Wire Wire Line
	4400 3750 4500 3750
Wire Wire Line
	4400 3350 4500 3350
Wire Wire Line
	4500 3450 4350 3450
Wire Wire Line
	4350 3450 4350 3400
Wire Wire Line
	4350 3400 4250 3400
Wire Wire Line
	4250 3500 4300 3500
Wire Wire Line
	4300 3500 4300 3650
Wire Wire Line
	4300 3650 4500 3650
Wire Wire Line
	6000 3150 5900 3150
Wire Wire Line
	5850 2950 5850 3150
Wire Wire Line
	5850 3150 5850 3350
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
	5850 3350 5850 3550
Connection ~ 5850 3350
Wire Wire Line
	5850 3550 5850 3750
Connection ~ 5850 3550
Wire Wire Line
	5900 3450 5900 3400
Wire Wire Line
	5900 3400 6000 3400
Wire Wire Line
	6000 3500 5950 3500
Wire Wire Line
	5950 3500 5950 3650
$Comp
L Connector_Generic:Conn_02x11_Odd_Even J1
U 1 1 5CA976F8
P 4700 3250
F 0 "J1" H 4750 3967 50  0000 C CNN
F 1 "Conn_02x11_Odd_Even" H 4750 3876 50  0000 C CNN
F 2 "KicadZeniteSolarLibrary18:PinHeader_2x11_P2.54mm_Vertical_HandSoldering" H 4700 3250 50  0001 C CNN
F 3 "~" H 4700 3250 50  0001 C CNN
	1    4700 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 2750 4500 2750
Wire Wire Line
	5000 2750 4500 2750
Connection ~ 4500 2750
Connection ~ 4500 2850
Wire Wire Line
	4400 2950 4500 2950
Connection ~ 4500 2950
Connection ~ 4500 3050
Wire Wire Line
	5000 3150 4500 3150
Connection ~ 4500 3150
Wire Wire Line
	4350 3250 4500 3250
Connection ~ 4500 3250
Wire Wire Line
	5000 3350 4500 3350
Connection ~ 4500 3350
Connection ~ 4500 3450
Wire Wire Line
	4400 3550 4500 3550
Connection ~ 4500 3550
Connection ~ 4500 3650
Wire Wire Line
	5000 3750 4500 3750
Connection ~ 4500 3750
Wire Wire Line
	5000 3650 4500 3650
Wire Wire Line
	5000 3050 4500 3050
Wire Wire Line
	5000 3450 4500 3450
Wire Wire Line
	5850 3750 5750 3750
Wire Wire Line
	4500 3550 5000 3550
Wire Wire Line
	5800 2750 5750 2750
Wire Wire Line
	4500 2950 5000 2950
Wire Wire Line
	4500 3250 5000 3250
Wire Wire Line
	5900 3250 5750 3250
Wire Wire Line
	5000 2850 4500 2850
Connection ~ 4400 3550
Connection ~ 4400 3350
Wire Wire Line
	4400 3550 4400 3750
Wire Wire Line
	4400 3350 4400 3550
Connection ~ 4400 2950
Wire Wire Line
	4400 2600 4400 2950
Connection ~ 4400 3150
Wire Wire Line
	4400 3150 4400 3350
Wire Wire Line
	4400 2950 4400 3150
Wire Wire Line
	5900 2950 5900 2850
Wire Wire Line
	5900 3150 5900 3250
$Comp
L Connector_Generic:Conn_02x11_Odd_Even J2
U 1 1 5CAAC328
P 5450 3250
F 0 "J2" H 5500 3967 50  0000 C CNN
F 1 "Conn_02x11_Odd_Even" H 5500 3876 50  0000 C CNN
F 2 "KicadZeniteSolarLibrary18:PinHeader_2x11_P2.54mm_Vertical_HandSoldering" H 5450 3250 50  0001 C CNN
F 3 "~" H 5450 3250 50  0001 C CNN
	1    5450 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 2750 5750 2750
Connection ~ 5750 2750
Wire Wire Line
	5250 2850 5750 2850
Connection ~ 5750 2850
Wire Wire Line
	5750 2850 5900 2850
Wire Wire Line
	5250 2950 5750 2950
Connection ~ 5750 2950
Wire Wire Line
	5750 2950 5850 2950
Wire Wire Line
	5250 3050 5750 3050
Connection ~ 5750 3050
Wire Wire Line
	5750 3050 6000 3050
Wire Wire Line
	5250 3150 5750 3150
Connection ~ 5750 3150
Wire Wire Line
	5750 3150 5850 3150
Wire Wire Line
	5250 3250 5750 3250
Connection ~ 5750 3250
Wire Wire Line
	5250 3350 5750 3350
Connection ~ 5750 3350
Wire Wire Line
	5750 3350 5850 3350
Wire Wire Line
	5250 3450 5750 3450
Connection ~ 5750 3450
Wire Wire Line
	5750 3450 5900 3450
Wire Wire Line
	5250 3550 5750 3550
Connection ~ 5750 3550
Wire Wire Line
	5750 3550 5850 3550
Wire Wire Line
	5250 3650 5750 3650
Connection ~ 5750 3650
Wire Wire Line
	5750 3650 5950 3650
Wire Wire Line
	5250 3750 5750 3750
Connection ~ 5750 3750
$EndSCHEMATC
