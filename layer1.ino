//distance calibration trigger points in IR
#define block_1_away 225
#define block_2_away 100








//trigger value for motor to stop to pass into phase 2 of game
#define trigger_dist_phase2 500

//infrared 45deg calibration if a block is present on the side
#define irSides 225





void resetSpeed(){
	speed_left = max_left, speed_right = max_right;
}

/*calibration functions*/

/*aligns*/
void align(){
	forward(100);
	while (analogRead(ir_front) < block_1_away);
	decelerate(100,0);
}

/*pivots*/
//CALIBRATE pivotLeft_t
void pivotLeft(int deg){
	m.setSpeed(0,map(pivot_power,0,100,0,speed_right));
	delay(map(deg ,0,360, 0,pivotLeft_t));
	brake();
}

//CALIBRATE pivotRight_t
void pivotRight(int deg){
	m.setSpeed(map(pivot_power,0,100,0,speed_left),0);
	delay(map(deg ,0,360, 0,pivotRight_t));
	brake();
}


/*FORWARD AND BACKWARD*/
//maps power as % from 0 to the max_power
void forward(unsigned char power){
	resetSpeed(); //statics speed_left and speed_right == max_power
	m.setSpeed(map(power,0,100,0,speed_left),map(power,0,100,0,speed_right));
}
void backward(unsigned char power){
	resetSpeed();
	m.setSpeed(-map(power,0,100,0,speed_left),-map(power,0,100,0,speed_right));
}


/*FULL FUNCTIONS*/

//decelerate by time, 0 means fast as possible
void decelerate(int power, int time){
	int i = power;
	while(i > 0){
		i--;
		forward(i);
		delay(time/power);
	}
}
void accelerate(int power, int time){
	int i = 0;
	while(i < power){
		i++;
		forward(i);
		delay(time/power);
	}
}

void frontTouch(){
	forward(50);
	attachInterrupt(0, touch_left,RISING);
	attachInterrupt(1, touch_right, RISING);
}


/*DO NOT EDIT, INTERRUPTS*/
void touch_left(){
	detachInterrupt(0);
	detachInterrupt(1);
	m.setSpeed(-map(20, 0,100, 0,speed_left), map(20, 0,100, 0,speed_right));
	attachInterrupt(1, brake, RISING);
}
void touch_right(){
	detachInterrupt(0);
	detachInterrupt(1);
	m.setSpeed(-map(20, 0,100, 0,speed_left), map(20, 0,100, 0,speed_right));
	attachInterrupt(0, brake, RISING);
}
void brake(){
	m.setSpeed(0,0);
}
//////////////////////////////

/*no-block (false) or block (true)*/
//delays the program until block or no block is seen
void checkBlock(boolean side, boolean stop_at){
	unsigned char side_sense = ir_right_45;
	if (side) side_sense = ir_left_45;
	if (stop_at) while (analogRead(side_sense) > irSides);
	else while(analogRead(side_sense) < irSides);
}

// attach to a distance 1 block away
void kissMe(int dist, int power){
	forward(power);
	while (analogRead(ir_front) < dist){
		//alignment
		if (!digitalRead(ir_left)){
			if(speed_left < 255 && speed_left < max_left +20)
			speed_left++;
			if(speed_right > max_right -20)
			speed_right--;
			m.setSpeed(speed_left,speed_right);
		}
		if (!digitalRead(ir_right)){
			if(speed_left > max_left -20)
				speed_left--;
			if(speed_right < 255 && speed_right < max_right +20)
				speed_right++;
			m.setSpeed(speed_left, speed_right);
		}
	}
	decelerate(power,0);
}

////////////Calibrate delays (*Caution hypothetical*)
void cross_bridge(){
	forward(100);
	while (analogRead(ir_front) < trigger_dist_phase2){
		if(!digitalRead(ground_left)){
			backward(50);
			//calibrate
			delay(100);
			brake();
			m.setSpeed(speed_left*.5,-speed_right*.5);
			//calibrate
			delay(100);
			forward(100);
		}
		if(!digitalRead(ground_right)){
			backward(50);
			//calibrate
			delay(100);
			brake();
			m.setSpeed(-speed_left*.5, speed_right*.5);
			//calibrate
			delay(100);
			forward(100);
		}
	}
}

//from front touch


//inset direction, and how many degrees
void turn(bool dir, int deg){
	resetSpeed();
	if(dir)m.setSpeed(-speed_left,speed_right);
	else m.setSpeed(speed_left,-speed_right);
	//calibrate
	delay(map(deg, 0,360, 0,max_360));
	m.setSpeed(0,0);
}
