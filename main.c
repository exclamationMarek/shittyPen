#include <avr/io.h>
#include <util/delay.h>
#include <avr/cpufunc.h>

uint16_t servoPosition;


void servoPulse(uint16_t angle){
	/*angle from 0 to 1000 coresponds to pulse between 500 and 2500us. Ish*/
	uint16_t i;
	if(angle>1000)
		angle = 1000;
	
	PORTD |= 0x01;
	_delay_us(500);
	for(i=0; i<angle;i++){_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();}
	PORTD &= 0xFE;
	for(; i<1000;i++){_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();}
	_delay_ms(7);
	_delay_us(500);
	servoPosition = angle;
}

void servoHold(uint16_t angle, uint16_t time){
	/*angle from 0 to 1000 coresponds to pulse between 500 and 2500us. Ish*/
	/*time in 10's of ms. 1=10ms 2=20ms etc*/

	while(time--){
		servoPulse(angle);
	}
}

void servoSwing(uint16_t end, uint8_t speed){
	if(speed==0)speed=1;
	if(speed>15)speed=15;
	if(end>servoPosition){
		while(servoPosition < end - speed){
			servoPulse(servoPosition);
			servoPosition+=speed;
		}
	}else{
		while(servoPosition > end + speed){
			servoPulse(servoPosition);
			servoPosition-=speed;
		}
	}
	servoPulse(end);
	servoPosition = end;
}

void waitForButton(){
	while(PIND & 0x10){_NOP();}
}

void waitForButtonRelease(){
	while((PIND & 0x10) == 0){_NOP();}
}

int main(void){
	PORTD = 0b00000000;
	DDRA = 0;
	DDRB = 0b00000001; //B0 = buck enable
	DDRC = 0;
	DDRD = 0b00000001; //D0 = servo signal
	
	PORTB |= 0b00000001; //turn on power to servos


	//set pull up for the button
	PORTD |= 0x10; //D4 = button


	//reset the servo at midpoint
	servoPosition = 600;
	servoHold(600, 50);
	

	while(1){
		waitForButton();

		servoHold(260, 50);
		servoHold(600, 50);	
	}
}

