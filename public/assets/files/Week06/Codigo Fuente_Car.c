//Mandar datos
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

/*
BAUD 9600 OK
1 bit parada OK
No tiene paridad Ok
tamaño msj es de 8 bits
*/
void config_pwm(){
//Activar el temporizador debe decir OC1- CON EL 1, 0C0 CON EL 0
//Trabajo con el 0
TCCR2B=1<<CS20;// Activo timer0 sin prescala
//Activo el PIND6 PORQUE ESL OC0A como el invertido 
TCCR2A= (1<<COM2A0 )|(1<< COM2A1)|(1<<COM2B0 )| (1<< COM2B1);//Estos son los pwm, estamos ocupando el PD3 Y PB3 el simbolo | es para agregar
//Modo fast top=0xff
TCCR2A|= (1<<WGM20)||(1<<WGM21);//modo variable, podemos cambiar lo que queramos, y el otro , se queda fijo
}

void config_uart(){
//(8000000/(16*BAUD))-1 = 51
UBRR0L = 51;
UCSR0C = (3<<UCSZ00);//1<<UCSZ01  1<<UCSZ00
UCSR0B = (1<<TXEN0) | (1<<RXEN0);  //RX es envío y Tx recepción
}

int main(){
    config_uart();
    unsigned int msj=4;
    DDRC=0xFF;

    config_pwm();
    DDRB=1<<PINB3;//DECLARAR COMOS SALIDA son enable
    DDRD=1<<PIND3;//DECLARAR COMOS SALIDA son los enable
    int guardar=0;

    while(1){
        //Empieza pwm
        OCR2A=20; //255 equivale a todo prendido --> lo tenemos ala reves por lo que en 0, giran ambos
        //Acaba pwm

        if(UCSR0A&(1<<RXC0)){
            msj=UDR0;
            while(!(UCSR0A&(1<<UDRE0)));
            UDR0=msj;
        
        if(msj==0){//ADELANTE
            if(guardar!=0){
                //_delay_ms(200);
                PORTC= 0<<PINC0;//0b000000100 in1 1 y in2 0 y eso lo hace antihorario
                PORTC|= 0<<PINC2;//0b000001001, el simbolo es para que no se sobrescriba|--> in3 1 y in4 es 0-- antihorario
            }
            _delay_ms(200);
            OCR2A=20;
            guardar=0;
            PORTC= 1<<PINC1;// in1 1 y in2 0 y eso lo hace horario No está funcionando
            PORTC|= 1<<PINC2;//0b000001001, el simbolo es para que no se sobrescriba|--> in3 1 y in4 es 0-- antihorario
        }
        else if(msj==1){//ATRÁS
            if(guardar!=1){
                //_delay_ms(200);
                PORTC= 0<<PINC0;//0b000000100 in1 1 y in2 0 y eso lo hace antihorario
                PORTC|= 0<<PINC2;//0b000001001, el simbolo es para que no se sobrescriba|--> in3 1 y in4 es 0-- antihorario
            }
            guardar=1;
            OCR2A=20;
            _delay_ms(200);
            PORTC= 1<<PINC0;//0b000000100 in1 1 y in2 0 y eso lo hace antihorario} No está funcionando
            PORTC|= 1<<PINC3;//0b000001001, el simbolo es para que no se sobrescriba|--> in3 1 y in4 es 0-- antihorario
        }
        else if(msj==2){//DERECHA
            if(guardar!=2){
                //_delay_ms(200);
                PORTC= 0<<PINC0;//0b000000100 in1 1 y in2 0 y eso lo hace antihorario
                PORTC|= 0<<PINC2;//0b000001001, el simbolo es para que no se sobrescriba|--> in3 1 y in4 es 0-- antihorario
            }
            _delay_ms(200);
            OCR2A=64;
            guardar=2;
            PORTC= 1<<PINC0;//0b000000100 in1 1 y in2 0 y eso lo hace antihorario
            PORTC|= 1<<PINC2;//0b000001001, el simbolo es para que no se sobrescriba
        }
        else if(msj==3){ //IZQUIERDA
            if(guardar!=3){
                //_delay_ms(200);
                PORTC= 0<<PINC0;//0b000000100 in1 1 y in2 0 y eso lo hace antihorario
                PORTC|= 0<<PINC2;//0b000001001, el simbolo es para que no se sobrescriba|--> in3 1 y in4 es 0-- antihorario
            }
            guardar=3;
            _delay_ms(200);
            OCR2A=64;
            PORTC= 1<<PINC1;//0b000000100 in1 1 y in2 0 y eso lo hace antihorario
            PORTC|= 1<<PINC3;//0b000001001, el simbolo es para que no se sobrescriba
            
        }
        else if(msj==4){//STOP
            guardar=4;
            PORTC= 0<<PINC0;//0b000000100 in1 1 y in2 0 y eso lo hace antihorario
            PORTC|= 0<<PINC2;//0b000001001, el simbolo es para que no se sobrescriba|--> in3 1 y in4 es 0-- antihorario
        }
        else{
            guardar=5;
            PORTC= 0<<PINC0;//0b000000100 in1 1 y in2 0 y eso lo hace antihorario
            PORTC|= 0<<PINC2;//0b000001001, el simbolo es para que no se sobrescriba|--> in3 1 y in4 es 0-- antihorario
        }
        
    }
    }

}


