/* File:   semaforoInteligente.c
 * Author: Eros Machado & Gustavo Sperandio
 * Turma: 412
 *  Semáforo Inteligente
 */
#define F_CPU 16000000UL  // Define a frequência de clock do microcontrolador como 16 MHz
#include <avr/io.h>        // Biblioteca para manipulação dos registradores de I/O
#include <avr/interrupt.h> // definição dos macros para a interrupção 
#include <util/delay.h>    // Biblioteca para usar funções de atraso (_delay_ms())

ISR(PCINT1_vect);  // protótipo da função
// definição nos números no display:
unsigned char display[10] = {0xE7, 0x84, 0xD3, 0xD6, 0xB4, 0x76, 0x77, 0xC4, 0xF7, 0xF6}; 

//definiçãO de variavel global para simular 4 horas (conforme solução 1 do manual):
unsigned int timer = 30; // cada unidade aqui representa um tempo de uns 400ms

    
    int main() { //  main() do código
        
    DDRB   = 0b11111111; //Configura todos os pinos do PORTB (PB0 a PB7) como saída
    PORTB  = 0b00000000; //Apaga todos os LEDs ligados ao PORTB (inicializa PORTB com 0)
    DDRC   = 0b00001011; // Define PC0 e PC1 como saídas (exemplo de LEDs) e PC2 como entrada //
    PORTC  = 0b00001100; //habilita pull-ups apenas nos botões
    DDRD   = 0b11111111; // Configura todos os pinos do PORTD (PD0 a PD7) como saída
    PORTD  = 0b00000000; // Apaga todos os LEDs ligados ao PORTD (inicializa PORTD com 0)
    PCICR  = 0b00000010;   // Habilita interrupções de mudança de estado 
    PCMSK1 = 0b00001100;  // Habilita interrupções especificamente nos pinos PC2 e PC3
    sei();    
        
    PORTB = 0b00010010;    // Configura os semáforos de Via A e Via B para estado inicial
    PORTC = 0b00000001;   // semaforo vm_ped acesso
    _delay_ms(5000);       // delay de 5 seg.

    PORTB = 0b00001001;    // Semáforo da Via A fica verde, Via B permanece vermelho, e pedestres são parados
    PORTC = 0b00000010;   // só semaforo vd_ped acesso
    _delay_ms(10000);      // Aguardar 10000 ms (10 segundos)

    while (1) {  // Loop infinito o controle dos semáforos        
         PORTC = 0b00000001;   // inicialização para piscar vm_ped
        for (int i = 0; i < 5; i++) {  // Loop para piscar o semáforo de pedestres
            PORTC ^= (1 << PC0);  // Alterna o estado do semáforo dos pedestres para piscante
            _delay_ms(200);       // Aguardar 100 ms entre os piscas
        }
        PORTC = 0b00000001;   //  vm_ped acesso
        
        PORTB = 0b00001100; // Via A fica verde, Via B fica vermelho, pedestres parados
        PORTC = 0b00000001;   // SÓ semaforo vm_ped acesso
        
        for (int i = 9; i > 0; i--) { //laço para rodar os numerais de 9 a 1
            PORTC |= 0b00001000; //liga o display da unidade      
            PORTD = display[i];//Envia o valor da unidade para os segmentos
            _delay_ms(1000); // delay de 1s
        }

        PORTB = 0b00001010; // Semáforo da Via A fica amarelo, Via B ainda vermelho
        _delay_ms(1000); // delay de 1 seg.
        PORTD = display[0]; // manda o valor 9 para o display
        _delay_ms(300); // deixei esse delay só para dar para ver que o 0 aparece 
        PORTC &= 0b11110111; //Apaga o display da unidade  
        
        PORTB = 0b00001001; // Ambos os semáforos de carros ficam vermelhos e os pedestres podem atravessar
        PORTC = 0b00000010;   // só semaforo vd_ped acesso
        _delay_ms(9000);    // Aguardar 9000 ms (9 segundos) para travessia dos pedestres
        
        PORTC = 0b00000001;   // inicialização para piscar vm_ped
        for (int i = 0; i < 5; i++) {  // Loop para piscar o semáforo de pedestres
            PORTC ^= (1 << PC0);  // Alterna o estado do semáforo dos pedestres para piscante
            _delay_ms(200);       // Aguardar 100 ms entre os piscas
        }
        PORTC = 0b00000001;   //  vm_ped acesso

        PORTB = 0b00100001; // semaforo via b abre (B-vd acesso)
        
        for (int i = 9; i > 0; i--) { //laço para rodar os numerais de 9 a 1
            PORTC |= 0b00001000; //liga o display da unidade      
            PORTD = display[i];//Envia o valor da unidade para os segmentos
            _delay_ms(1000); // delay de 1s
        }
        
        PORTB = 0b00010001; // semaforo via b amarelo
        _delay_ms(1000);
        PORTD = display[0]; // manda o valor 9 para o display
        _delay_ms(300); // deixei esse delay só para dar para ver que o 0 aparece 
        PORTC &= 0b11110111; //Apaga o display da unidade  

        PORTB = 0b00001001; //semaforo via B fecha
        PORTC = 0b00000010;   //  vd_ped acesso
        _delay_ms(9000);
        }
    }
//Interrupção: 
ISR(PCINT1_vect) {
        if (!(PINC & (1 << 2))) { // Verifica se PC2 está pressionado
       for(int i = 0; i < timer; i++){
     //   while(!(PINC & (1 << 2))){
            PORTC = 0b00000000;   //  vm_ped e vd_ped desligados
            PORTB = 0b00010010; // Acende PB1 e PB4
            _delay_ms(200);
            PORTB = 0b00000000; // Apaga PB1 e PB4
            _delay_ms(200);
        }
   }
}




















//teste:
/* File:   semaforoInteligente.c
 * Author: Eros Machado & Gustavo Sperandio
 * Turma: 412
 * Atividade 05 - Semáforo Inteligente
 */
//Acionando LED com as interrupções de mudança de estado: 
#define F_CPU 16000000UL //frequência de trabalho 
#include <avr/io.h> //definições do componente especificado 
#include <avr/interrupt.h> //define algumas macros para as interrupções 
#include <util/delay.h> //biblioteca para o uso das rotinas de delay
#include <avr/wdt.h> // biblioteca para o Watchdog Timer

//Protótipo da função de interrupção, obrigatório para sua utilização 
ISR(PCINT1_vect);

// definição nos números no display:
unsigned char display[10] = {0xE7, 0x84, 0xD3, 0xD6, 0xB4, 0x76, 0x77, 0xC4, 0xF7, 0xF6}; 

    
    int main() { //  main() do código
    wdt_disable(); // Desabilita o Watchdog Timer no início do programa   
    DDRB   = 0b11111111; //Configura todos os pinos do PORTB (PB0 a PB7) como saída
    PORTB  = 0b00000000; //Apaga todos os LEDs ligados ao PORTB (inicializa PORTB com 0)
    DDRC   = 0b00010011; // Define PC0 e PC1 como saídas (exemplo de LEDs) e PC2 como entrada
    PORTC  = 0b00011100; //habilita pull-ups apenas nos botões
    DDRD   = 0b11111111; // Configura todos os pinos do PORTD (PD0 a PD7) como saída
    PORTD  = 0b00000000; // Apaga todos os LEDs ligados ao PORTD (inicializa PORTD com 0)
    PCICR  = 0b00000010;   // Habilita interrupções de mudança de estado 
    PCMSK1 = 0b00000100;  // Habilita interrupções especificamente nos pinos
    sei();    
        
    PORTB = 0b00010010;    // Configura os semáforos de Via A e Via B para estado inicial
    PORTC = 0b00000001;   // semaforo vm_ped acesso
    _delay_ms(5000);       // delay de 5 seg.

    PORTB = 0b00001001;    // Semáforo da Via A fica verde, Via B permanece vermelho, e pedestres são parados
    PORTC = 0b00000010;   // só semaforo vd_ped acesso
    _delay_ms(10000);      // Aguardar 10000 ms (10 segundos)

    while (1) {  // Loop infinito o controle dos semáforos        
         PORTC = 0b00000001;   // inicialização para piscar vm_ped
        for (int i = 0; i < 5; i++) {  // Loop para piscar o semáforo de pedestres
            PORTC ^= (1 << PC0);  // Alterna o estado do semáforo dos pedestres para piscante
            _delay_ms(200);       // Aguardar 100 ms entre os piscas
        }
        PORTC = 0b00000001;   //  vm_ped acesso
        
        PORTB = 0b00001100; // Via A fica verde, Via B fica vermelho, pedestres parados
        PORTC = 0b00000001;   // SÓ semaforo vm_ped acesso
        
        for (int i = 9; i > 0; i--) { //laço para rodar os numerais de 9 a 1
            PORTC |= 0b00010000; //liga o display da unidade      
            PORTD = display[i];//Envia o valor da unidade para os segmentos
            _delay_ms(1000); // delay de 1s
        }

        PORTB = 0b00001010; // Semáforo da Via A fica amarelo, Via B ainda vermelho
        _delay_ms(1000); // delay de 1 seg.
        PORTD = display[0]; // manda o valor 9 para o display
        _delay_ms(300); // deixei esse delay só para dar para ver que o 0 aparece 
        PORTC &= 0b11101111; //Apaga o display da unidade  
        
        PORTB = 0b00001001; // Ambos os semáforos de carros ficam vermelhos e os pedestres podem atravessar
        PORTC = 0b00000010;   // só semaforo vd_ped acesso
        _delay_ms(9000);    // Aguardar 9000 ms (9 segundos) para travessia dos pedestres
        
        PORTC = 0b00000001;   // inicialização para piscar vm_ped
        for (int i = 0; i < 5; i++) {  // Loop para piscar o semáforo de pedestres
            PORTC ^= (1 << PC0);  // Alterna o estado do semáforo dos pedestres para piscante
            _delay_ms(200);       // Aguardar 100 ms entre os piscas
        }
        PORTC = 0b00000001;   //  vm_ped acesso

        PORTB = 0b00100001; // semaforo via b abre (B-vd acesso)
        
        for (int i = 9; i > 0; i--) { //laço para rodar os numerais de 9 a 1
            PORTC |= 0b00010000; //liga o display da unidade      
            PORTD = display[i];//Envia o valor da unidade para os segmentos
            _delay_ms(1000); // delay de 1s
        }
        
        PORTB = 0b00010001; // semaforo via b amarelo
        _delay_ms(1000);
        PORTD = display[0]; // manda o valor 9 para o display
        _delay_ms(300); // deixei esse delay só para dar para ver que o 0 aparece 
        PORTC &= 0b11101111; //Apaga o display da unidade  

        PORTB = 0b00001001; //semaforo via B fecha
        PORTC = 0b00000010;   //  vd_ped acesso
        _delay_ms(9000);
        }
    }
    
    
    void wait(void); // Declaração da função wait()

// Rotina de interrupção
ISR(PCINT1_vect) {
        
        wait();
        
        // Reinicializa usando o WDT
        wdt_enable(WDTO_15MS); // Habilita WDT com timeout de 15ms
    
}
// Definição da função wait()
void wait(void) {
    while ((PINC & (1 << PC2))) { 
            PORTC = 0b00000000;   // Desliga semáforos de pedestres
            PORTB = 0b00010010;   // Acende PB1 e PB4
            _delay_ms(200);
            PORTB = 0b00000000;   // Apaga PB1 e PB4
            _delay_ms(200);
    }
     _delay_ms(20); // Pequeno atraso para debounce
}

    
