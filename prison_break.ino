#include <L298.h>

L298 m(10,11,8,9);


/**************************************************/
//direction of turns parameter
#define left_t true
#define right_t false

//IRs
#define	ir_front 3
#define	ir_left 5
#define	ir_right 1
#define	ir_left_45 4
#define	ir_right_45 2
//headlights
#define	headlight_left 2
#define	headlight_right 3
//grounds
#define ground_left 5
#define	ground_right 4


/**************************************************/


//motor calibration
//max of 230
#define max_left 160
#define max_right 160


//pivot calibration

/*left*/
//power
#define pivot_power 100
//delay ms
#define pivotLeft_t 960
/*right*/
#define pivotRight_t 1000

//time
#define max_360 450





/*******************************************/
//statics
unsigned char
	speed_left = max_left,
	speed_right = max_right;
/*******************************************/
void setup(){
  //accelerate(100,100);
  Serial.begin(9600);
  //attachInterrupt(0, tryMe, RISING);
  accelerate(100,100);
  while(analogRead(ir_front) < 500);
  decelerate(100,100);
  delay(10);
  turn(left_t, 107);
}
void tryMe(){
  Serial.println("hi");
}
void loop(){
  
}
