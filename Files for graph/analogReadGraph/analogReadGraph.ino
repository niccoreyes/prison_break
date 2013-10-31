#include <L298.h>

#define s Serial
L298 m(10,11,8,9);

void setup(){
  s.begin(9600);
  m.setSpeed(0,0);
}
void loop(){
  for(int i =0; i <5; i++){
    s.print(analogRead(i));
    s.print(",");
  }
  s.println(analogRead(5));
  delay(100);
}
