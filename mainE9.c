/*
 * Interrupção Externa + PWM + Serial: Desenvolva um código em que o 
 * motor de corrente contínua é acionado com sinal PWM e a variação 
 * de velocidade é feita através de interrupções externas:
 * a interrupção externa 0 aumenta a velocidade de 10 em 10%, 
 * até o limite de 90% do valor máximo;
 * a interrupção externa 2 diminui a velocidade de 10 em 10% 
 * até o limite de 10% do valor máximo. Uma mensagem com o percentual
 * da velocidade é enviada via serial.
 */

/* 
 * File:   mainE9.c
 * Author: Luiz Felix
 *
 * Created on 15 de Dezembro de 2020, 17:33
 */

#define _XTAL_FREQ 4000000
#include <stdio.h>
#include <xc.h>
#include <string.h>
#pragma config FOSC = HS // Oscilador externo
#pragma config WDT = OFF // Watchdog Timer desligado
#pragma config MCLRE = OFF // Master Clear desabilitado
#pragma config PBADEN = OFF     // AD da porta B desligado

int perc= 90; //percentual da velocidade
char text[30]; //string de texto para enviar para a serial

void inicializa_RS232() {
    RCSTA = 0X90; // Habilita porta serial, recepção de 8 bits em modo continuo, assíncrono.
    int valor;
    TXSTA = 0X24; // modo assíncrono, transmissão 8 bits.
    valor = (int) (((_XTAL_FREQ / 9600) - 16) / 16); // valor para gerar o baud rate
    SPBRG = 25; // esse registrador, carregado com o "valor" calculado, define o baud rate
}

void escreve(char valor) {
    TXIF = 0; // limpa flag que sinaliza envio completo.
    TXREG = valor; // Envia caractere desejado à porta serial
    while (TXIF == 0); // espera caractere ser enviado
}

void imprime(char frase[]) {
    int indice = 0; // índice da cadeia de caracteres
    int tamanho = 0;
    tamanho = strlen(frase); // tamanho total da cadeia a ser impressa
    while (indice < tamanho) { // verifica se todos foram impressos
        escreve(frase[indice]); // Chama rotina que escreve o caractere
        indice++; // incrementa índice
    }
}

void setupInt() { // Função de configuração das interrupções
    GIE = 1; // Habilita interrupção global
    PEIE = 1; // Habilita interrupção de periféricos
    ADIE = 1; // Habilita interrupção do ADC
    
    INT0IE = 1; // Habilita Interrupção da INT0
    INT0IF = 0; // Zera a Flag de interrupção da INT0
    INTEDG0 = 1; // Interrupção 0 por borda crescente.
    
    INT2IE = 1; // Habilita Interrupção da INT0
    INT2IF = 0; // Zera a Flag de interrupção da INT0
    INTEDG2 = 1; // Interrupção 2 por borda crescente.
}

void interrupt interrupcao() { // Função de atendimento de interrupção

    if (INT0IF) { // Caso a flag da INT0 esteja habilitada
        if (CCPR2L < 180){
            CCPR2L += 20;
            perc += 10;
        }
        INT0IF = 0; // Limpa a Flag que aciona a INT0
    }

    if (INT2IF) { // Caso a flag da INT0 esteja habilitada
        if (CCPR2L > 20){
            CCPR2L -= 20;
            perc -= 10;
        }
        INT2IF = 0; // Limpa a Flag que aciona a INT2
    }
    sprintf(text, "Velocidade %d por cento \r", perc); //escreve a mensagem com a porcentagem da velocidade
    imprime(text); //imprime mensagem na serial
}

void setupTmr2() {
    TMR2 = 0; // Começa a contar de 0
    PR2 = 199; // Período PWM = (PR2 + 1)*Pré-escala = 200 us * pré-escala
}


void setupPWM (void) {
    setupTmr2();                    // Configura timer 2  
    CCP2CON = 0b00001100;     // Modo PWM ativo (Bits 3 e 2 ligados))
    CCPR2L = 180;                   // 90%
    TMR2IF = 0;                     // Limpa flag do TMR2
    TMR2ON = 1;                     // Dispara o timer
    TRISCbits.TRISC1 = 0;           // Habilita RC1 como saída  
    sprintf(text, "Velocidade %d por cento \r", perc); //escreve a mensagem com a porcentagem da velocidade
    imprime(text); //imprime mensagem na serial
}

void main() {
    TRISC = 0b10000000;// config. RC7 como ENTRADA (serial). O restante é SAÍDA
    setupInt(); 
    inicializa_RS232();
    setupPWM();
    LATC=0b00010010; //Motor CC sentido horário  
    
    while (1) { // Inicia loop infinito
    }
}