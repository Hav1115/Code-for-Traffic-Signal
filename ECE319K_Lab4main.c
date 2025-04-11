/* ECE319K_Lab4main.c
 * Traffic light FSM
 * ECE319H students must use pointers for next state
 * ECE319K students can use indices or pointers for next state
 * Put your names here or look silly
  */

#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "../inc/Clock.h"
#include "../inc/UART.h"
#include "../inc/Timer.h"
#include "../inc/Dump.h"  // student's Lab 3
#include <stdio.h>
#include <string.h>
// put both EIDs in the next two lines
const char EID1[] = "AVS2377"; //  ;replace abc123 with your EID
const char EID2[] = "HK27356"; //  ;replace abc123 with your EID
// Hint implement Traffic_Out before creating the struct, make struct match your Traffic_Out

// initialize all 6 LED outputs and 3 switch inputs
// assumes LaunchPad_Init resets and powers A and B
void Traffic_Init(void){ // assumes LaunchPad_Init resets and powers A and B
 // write this 
IOMUX->SECCFG.PINCM[PB0INDEX] = (uint32_t) 0x00000081; // OUTPUT
IOMUX->SECCFG.PINCM[PB1INDEX] = (uint32_t) 0x00000081;
IOMUX->SECCFG.PINCM[PB2INDEX] = (uint32_t) 0x00000081;

IOMUX->SECCFG.PINCM[PB16INDEX] = (uint32_t) 0x00040081; // input

GPIOB ->DOE31_0 |= 0x00000007; // does input friendly

IOMUX->SECCFG.PINCM[PB6INDEX] = (uint32_t) 0x00000081; //OUTPUT
IOMUX->SECCFG.PINCM[PB7INDEX] = (uint32_t) 0x00000081;
IOMUX->SECCFG.PINCM[PB8INDEX] = (uint32_t) 0x00000081;


IOMUX->SECCFG.PINCM[PB15INDEX] = (uint32_t) 0x00040081; // INPUT

GPIOB ->DOE31_0 |= 0x000001C0; // does input friendly

IOMUX->SECCFG.PINCM[PB17INDEX] = (uint32_t) 0x00040081; // INPUT
}
/* Activate LEDs
* Inputs: west is 3-bit value to three east/west LEDs
*         south is 3-bit value to three north/south LEDs
*         walk is 3-bit value to 3-color positive logic LED on PB22,PB26,PB27
* Output: none
* - west =1 sets west green
* - west =2 sets west yellow
* - west =4 sets west red
* - south =1 sets south green
* - south =2 sets south yellow
* - south =4 sets south red
* - walk=0 to turn off LED
* - walk bit 22 sets blue color
* - walk bit 26 sets red color
* - walk bit 27 sets green color
* Feel free to change this. But, if you change the way it works, change the test programs too
* Be friendly*/
void Traffic_Out(uint32_t west, uint32_t south, uint32_t walk){
  GPIOB ->DOUT31_0 = (GPIOB -> DOUT31_0 | (west << 6)) | (GPIOB->DOUT31_0 | south) | (GPIOB->DOUT31_0 | walk); // All we are doing here is shifting the bits to there proper pin in the output and then orring them to activate all of them
}
/* Read sensors
 * Input: none
 * Output: sensor values
 * - bit 0 is west car sensor
 * - bit 1 is south car sensor
 * - bit 2 is walk people sensor
* Feel free to change this. But, if you change the way it works, change the test programs too
 */
uint32_t Traffic_In(void){
    return ((GPIOB->DIN31_0 & 0x00038000) >> 15); // USES the input bits and 
     //ands them to get the value specified
}

//FSMMMMMMMMM STARTS HERE

//Struct (Defines the 13 states we are using)
#define stays 0
#define switchs 1
#define stops 2
#define stayw 3
#define switchw 4
#define stopw 5
#define staywalk 6
#define flash 7
#define flash2 8
#define flash3 9
#define flash4 10
#define flash5 11
#define flash6 12

struct stru{
  uint32_t output; // this is what is gonna dosplay lights wise
  uint32_t delay; // This is used as a delay within the code to make sure we can see change in LED
  uint32_t traffic[8]; //This is the 8 possible directions I would go depending on the 8 possible inputs I have (2^3)
};
typedef const struct stru styp; // styp is made as a new struct. const means it is read only

styp FSM[13] = {
  {0x04000101, 80, {stays , stays, switchs, switchs, switchs, switchs, switchs, switchs}}, // We at South
  {0x04000102, 120, {stops , stops, stops, stops, stops, stops, stops, stops}}, // Yellow light still South
  {0x04000104, 80, {stayw , stays, stayw, stayw, staywalk, staywalk, staywalk, staywalk}}, // Red light for south 
  {0x04000044, 120, {stayw , switchw, stayw, switchw, switchw, switchw, switchw, switchw}}, // We at West
  {0x04000084, 80, {stopw , stopw, stopw, stopw, stopw, stopw, stopw, stopw}}, //Yellow Light for West
  {0x04000104, 120, {stays , stays, stayw, stays, staywalk, staywalk, staywalk, stays}}, //Red Light both
  {0x0C400104, 80, {staywalk, flash, flash, flash, staywalk, flash, flash, flash}}, //We are walking//setting bits 22, 26, 27 and flashing both red lights
  {0x00000104, 80, {staywalk, flash2, flash2, flash2, staywalk, flash2, flash2, flash2}}, //flash off
  {0x04000104, 120, {staywalk, flash3, flash3, flash3, staywalk, flash3, flash3, flash3}}, //flash on
  {0x00000104, 80, {staywalk, flash4, flash4, flash4, staywalk, flash4, flash4, flash4}}, //flashoff
  {0x04000104, 120, {staywalk, flash5, flash5, flash5, staywalk, flash5, flash5, flash5}}, //flashon
  {0x00000104, 80, {staywalk, flash6, flash6, flash6, staywalk, flash6, flash6, flash6}}, //flash Off
  {0x04000104, 120, {staywalk, stays, stayw, stays, staywalk, stays, stayw, stayw}} //flashon
};
// use main1 to determine Lab4 assignment
void Lab4Grader(int mode);
void Grader_Init(void);
int main1(void){ // main1
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Lab4Grader(0); // print assignment, no grading
  while(1){
  }
}
// use main2 to debug LED outputs
// at this point in ECE319K you need to be writing your own test functions
// modify this program so it tests your Traffic_Out  function
int main2(void){ // main2
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Grader_Init(); // execute this line before your code
  Debug_Init();
  LaunchPad_LED1off();
  Traffic_Init(); // your Lab 4 initialization
  if((GPIOB->DOE31_0 & 0x20)==0){
    UART_OutString("access to GPIOB->DOE31_0 should be friendly.\n\r");
  }
  UART_Init();
  UART_OutString("Lab 4, Spring 2025, Step 1. Debug LEDs\n\r");
  UART_OutString("EID1= "); UART_OutString((char*)EID1); UART_OutString("\n\r");
  UART_OutString("EID2= "); UART_OutString((char*)EID2); UART_OutString("\n\r");
  while(1){
    // write code to test your Traffic_Out
    Traffic_Out(1, 0, 0x00000000);
    Debug_Dump(GPIOB->DOUT31_0);
    Traffic_Out(2, 0, 0x00400000);
    Debug_Dump(GPIOB->DOUT31_0);
    Traffic_Out(4, 0, 0x00000000);
    Debug_Dump(GPIOB->DOUT31_0);

    Traffic_Out(1, 1, 0x00000000);
    Debug_Dump(GPIOB->DOUT31_0);
    Traffic_Out(0, 2, 0x00000000);
    Debug_Dump(GPIOB->DOUT31_0);
    Traffic_Out(0, 4, 0x00000000);
    Debug_Dump(GPIOB->DOUT31_0);

    Traffic_Out(0, 2, 0x00400000);
    Debug_Dump(GPIOB->DOUT31_0);
    Traffic_Out(1, 0, 0x0C400000);
    Debug_Dump(GPIOB->DOUT31_0);

    Traffic_Out(1, 0, 0x00000000);
    Debug_Dump(GPIOB->DOUT31_0);
    Traffic_Out(2, 4, 0x00400000);
    Debug_Dump(GPIOB->DOUT31_0);
    Traffic_Out(4, 0, 0x00000000);
    Debug_Dump(GPIOB->DOUT31_0);
    if((GPIOB->DOUT31_0&0x20) == 0){
      UART_OutString("DOUT not friendly\n\r");
    }
  }
}
// use main3 to debug the three input switches
// at this point in ECE319K you need to be writing your own test functions
// modify this program so it tests your Traffic_In  function
int main3(void){ // main3
  uint32_t last=0,now;
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Traffic_Init(); // your Lab 4 initialization
  Debug_Init();   // Lab 3 debugging
  UART_Init();
  __enable_irq(); // UART uses interrupts
  UART_OutString("Lab 4, Spring 2025, Step 2. Debug switches\n\r");
  UART_OutString("EID1= "); UART_OutString((char*)EID1); UART_OutString("\n\r");
  UART_OutString("EID2= "); UART_OutString((char*)EID2); UART_OutString("\n\r");
  while(1){
    now = Traffic_In(); // Your Lab4 input
    if(now != last){ // change
      UART_OutString("Switch= 0x"); UART_OutUHex(now); UART_OutString("\n\r");
      Debug_Dump(now);
    }
    last = now;
    Clock_Delay(800000); // 10ms, to debounce switch
  }
}
// use main4 to debug using your dump
// proving your machine cycles through all states
int main4(void){// main4
uint32_t input;
  Clock_Init80MHz(0);
  LaunchPad_Init();
  LaunchPad_LED1off();
  Traffic_Init(); // your Lab 4 initialization
 // set initial state
  Debug_Init();   // Lab 3 debugging
  UART_Init();
  __enable_irq(); // UART uses interrupts
  UART_OutString("Lab 4, Spring 2025, Step 3. Debug FSM cycle\n\r");
  UART_OutString("EID1= "); UART_OutString((char*)EID1); UART_OutString("\n\r");
  UART_OutString("EID2= "); UART_OutString((char*)EID2); UART_OutString("\n\r");
// initialize your FSM
  uint32_t x;
  uint32_t y;
  uint32_t south;
  uint32_t west;
  uint32_t walk;
  uint32_t dump;
  x = stays;
  //x = Traffic_Out(4, 1, 0x04000000)
  Traffic_Init();
  while(1){
      // 1) output depending on state using Traffic_Out
      // call your Debug_Dump logging your state number and output
      // 2) wait depending on state
      // 3) hard code this so input always shows all switches pressed
      // 4) next depends on state and input
        SysTick_Init();   // Initialize SysTick for software waits
       GPIOB->DOUT31_0 = (GPIOB->DOUT31_0 & ((~0X0d4001C7))) | FSM[x].output; // Giving the Dout the first state in FSM
      SysTick_Wait10ms(FSM[x].delay);
      y = 7;
      x = FSM[x].traffic[y];
    walk = (((GPIOB->DOUT31_0)&0x0D400000)>>22);
    south = ((GPIOB->DOUT31_0)&7);
    west = (((GPIOB->DOUT31_0)&0x000001D0)>>6);
     dump = (walk) + (south<<8) + (west<<16) + (x<<24);
     Debug_Dump(dump);
  }
}
// use main5 to grade
int main(void){// main5
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Grader_Init(); // execute this line before your code
  LaunchPad_LED1off();
  Traffic_Init(); // your Lab 4 initialization
// initialize your FSM
  SysTick_Init();   // Initialize SysTick for software waits
  // initialize your FSM
  uint32_t test;
  uint32_t test1;
  test = stays;
  Traffic_Init();
  Lab4Grader(1); // activate UART, grader and interrupts
  while(1){
      // 1) output depending on state using Traffic_Out
      // call your Debug_Dump logging your state number and output
      // 2) wait depending on state
      // 3) input from switches
      // 4) next depends on state and input
      GPIOB->DOUT31_0 = (GPIOB->DOUT31_0 & ((~0X0d4001C7))) | FSM[test].output;
      SysTick_Wait10ms(FSM[test].delay);
      test1 = Traffic_In();
      test = FSM[test].traffic[test1];
  
  }
}
