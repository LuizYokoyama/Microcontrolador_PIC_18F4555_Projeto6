# Microcontrolador_PIC_18F4555_Projeto6
Uso de Interrupção Externa + PWM + Serial no microcontrolador PIC 18F4555

Atividade desenvolvida na disciplina Microprocessadores e Microcontroladores da minha graduação em Engenharia de Computação.

Neste programa, o motor de corrente contínua é acionado com sinal PWM e a variação de velocidade é feita através de interrupções externas: a interrupção externa 0 aumenta a velocidade de 10 em 10%, até o limite de 90% do valor máximo; a interrupção externa 2 diminui a velocidade de 10 em 10% até o limite de 10% do valor máximo. Uma mensagem com o percentual da velocidade deve ser enviada via serial

Para compilar o código, recomenda-se o uso da IDE MPLab-X https://www.microchip.com/en-us/development-tools-tools-and-software/mplab-x-ide

