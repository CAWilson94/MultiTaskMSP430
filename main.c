#include <stdio.h>
#include <intrinsics.h>
#include <msp430.h>

int flag = 0;
int counter = 0;
unsigned int arr[8] = {1,2,3,4,8,7,6,5};

#pragma vector=PORT1_VECTOR
__interrupt void toggle(void)
{
  P1IFG &= ~0x04; 
  flag = 1;
}

void ConfigTimerA(unsigned int delayCycles)
{
    TA0CCTL0 |= CCIE;	//Enable Interrupts on Timer
    TA0CCR0 = delayCycles;	//Number of cycles in the timer counts to 
    TA0CTL |= TASSEL_1;	//Use ACLK as source for timer
    TA0CTL |= MC_1;	//Use UP mode timer
}


#pragma vector=TIMER0_A0_VECTOR //(56 * 2u) /* 0xFFE8 Timer0_A3 CC0 */
__interrupt void Timer_A0(void){
     P1OUT ^= 0x01; // Toggle the LED
     P1OUT ^= BIT7; // Toggle the LED
     counter++; 
}

void caseLED(void){
  
   switch(arr[opt]) {
      case 1 :
        P8OUT  |= BIT0; 
        P5OUT  &= ~BIT1;
        P5OUT  &= ~BIT2;
        P5OUT  &= ~BIT3;
        P2OUT  &= ~BIT7;
        
        break;
         
      case 2 :
        P8OUT  &= ~BIT0; 
        P5OUT |= BIT1;
        P5OUT  &= ~BIT2;
        P5OUT  &= ~BIT3;
        P2OUT  &= ~BIT7; 
         break;
         
      case 3 :
        P8OUT  &= ~BIT0; 
        P5OUT &= ~BIT1;
        P5OUT  |= BIT2;
        P5OUT  &= ~BIT3;
        P2OUT  &= ~BIT7; 
         
         break;
         
      case 4 :
        P8OUT  &= ~BIT0; 
        P5OUT &= ~BIT1;
        P5OUT  &= ~BIT2;
        P5OUT  |= BIT3;
        P2OUT  &= ~BIT7; 
         
         break;
         
      case 5 :
        P8OUT  &= ~BIT0; 
        P5OUT |= BIT1;
        P5OUT  |= BIT2;
        P5OUT  |= BIT3;
        P2OUT  |= BIT7;
        
         break;
         
      case 6 :
        P8OUT  |= BIT0; 
        P5OUT &= ~BIT1;
        P5OUT  |= BIT2;
        P5OUT  |= BIT3;
        P2OUT  |= BIT7;
         
         break;
         
      case 7 :
        P8OUT  |= BIT0; 
        P5OUT  |= BIT1;
        P5OUT  &= ~BIT2;
        P5OUT  |= BIT3;
        P2OUT  |= BIT7;
        
         break;
         
      case 8 :
        P8OUT  |= BIT0; 
        P5OUT  |= BIT1;
        P5OUT  |= BIT2;
        P5OUT  &= ~BIT3;
        P2OUT  |= BIT7;
       
         break;
         
      default :
        P8OUT  |= BIT0; 
        P5OUT  |= BIT1;
        P5OUT  |= BIT2;
        P5OUT  &= ~BIT3;
        P2OUT  |= BIT7;
        break;
        
   }
  
}
 
void main(void)
{
 WDTCTL = WDTPW + WDTHOLD; // switch watch dog off
 PM5CTL0 &= ~LOCKLPM5; // turn off low power mode.. ?
 
// set LED direction to output
//P1DIR=0x01; // 0000 0001
 
P1DIR |= BIT0 + BIT7; // P1.0 and P1.7 outputs : safer to turn it on with the or, want to only touch one bit
 
P1REN=0x04; // 0000 1000 // sets pull up: resistor enable
P1OUT= 0x04; // 0000 0101 // port 1 bit 0 to be an output: direction of 1 -> whatever in output goes out to it 

/* Set INT1 interrupt edge select reg */
P1IES = 0;
/* Set Port 1 interrupt enable reg */
P1IE = 0x04; // 0000 1010
P1IFG &= ~0x04; // clear flag before program runs forever


__enable_interrupt();
/* Loop and wait for interrupt */

while(1)
{
  
  if(flag==1){
    P1OUT ^= 0x01; 
    flag = 0;
  }
   if(counter==120){
        P1OUT = 0;
       counter=0;
     }

}

} 