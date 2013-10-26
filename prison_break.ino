#include <L298.h>
#include <Servo.h>

L298 m(9,10,5,6);

int v = 1;

void setup(){

}
void loop(){
	m.setSpeed(100*v,100*v);
	delay(500);
	m.setSpeed(175*v,175*v);
	delay(500);
	m.setSpeed(255*v,255*v);
	delay(500);
	m.setSpeed(0,0); /*breaks without coasting*/
	delay(100);
	v = -v; /*means next time it runs setSpeed(-255,-255) aka reverse*/
}
