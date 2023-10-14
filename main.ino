#include <avr/io.h>
#include <string.h>
#include "uart.h"

#define BAUD 9600 //defining the BAUD rate globally

#define PUSH_BUTTON 7 //push button at digital pin 7
#define LED 0 //LED at digital pin 8

#define button_onText "Button Pressed\n"  
#define button_offText "Button not Pressed\n"
#define compare_onCmd "led_on"
#define compare_offCmd "led_off"

char receive_string[8];
int counter = 0;
char received;

int main(void){
  DDRD |= 1 << PUSH_BUTTON;
  DDRB &= ~(1 << LED);


  uart_init(BAUD); //initialize the BAUDRate setup

  int prevButton_state = PIND & (1<<PUSH_BUTTON); //put the previous state in a variable, used to read the rising edge signal of a button

  while(1){ //loop for running system
    if((UCSR0A)&(1<<RXC0)){

      received = uart_receive(); //user input command for LED

      if(received != '\n'){
        receive_string[counter++] = received;
        if(counter >= 8) counter = 0;
      }

      else{
        if( counter == 6 && strncmp(receive_string,compare_onCmd,6)==0 ){ //compare users inputted command with correct command
          uart_transmit_array("LED Is On\n",10);
          PORTB |= 1 << LED;
        }

        else if( counter == 7 && strncmp(receive_string,compare_offCmd,7)==0){ //compare users inputted command with correct command
          uart_transmit_array("LED Is Off\n",11);
          PORTB &= ~(1 << LED);
        }

        else{
          uart_transmit_array("ERROR Incorrect Command Inputted\n",33); //error message if command incorrect
        }

        counter = 0;
        }
    }

    int Button_state = PIND & (1<<PUSH_BUTTON); //put the state of the button in a variable

    if(Button_state != 0 && Button_state != prevButton_state){ //check for rising edge signal of a button
      uart_transmit_array(button_onText,15);
    }

    else if (Button_state == 0 && Button_state != prevButton_state){
      uart_transmit_array(button_offText,19);
    }

    prevButton_state  = Button_state; //setting new previous button state for rising edge detection
  }

  return 0;
}

