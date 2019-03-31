# MCS19
**M**ódulo de **C**arregamento do **S**istema 2019  
Embarcação Guarapuvu II  
Equipe Zênite Solar  

### Sobre

Este módulo é responsável por gerenciar o carregamento do sistema com a finalidade de garantir segurança para a embarcação.

#### Requisitos e Abordagens
1. Gerenciar a conexão elétrica entre as baterias e o restante do sistema.
    - Utilizar de um relé de alta potência.
    - Ser controlado através da rede CAN da embarcação.
2. Proteger o sistema de falhas por sobre-corrente.
    - Utilizar de um fusível corretamente dimensionado.
3. Realizar o carregamento do banco de capacitores de maneira segura.
    - Utilização de um circuito de inrush.
        - Utilizar de um NTC para limitar o di/dt.
        - Utilizar de um resistor para limitar o pico de corrente
        - Utilizar de um relé para garantir a desconexão completa entre o sistema
      e o banco de baterias.
4. Facilitar o diagnóstico
    - Utilizar reportagens pela rede CAN
    - Utilizar LEDs e Buzzer
        - Sinalizar o estado dos relés
            - Relé do inrush
            - Relé principal
        - Sinalizar o estado do fusível principal
5. Garantir fixação mecânica que não flexione a PCB
    - Utilizar estrutura auxiliar de material isolante
