#define cm_5 800 /*change this*/
#define cal_left 500
#define cal_right 500
unsigned char
	//IRs
	ir_front = 3,
	ir_left = 5,
	ir_right = 1,
	ir_left_45 = 4,
	ir_right_45 = 2,
	//headlights
	headlght_left = 13,
	headlight_right = 12,
	//grounds
	ground_left = 5,
	ground_right = 4,
	//statics
	speed_left = cal_left,
	speed_right = cal_right,
	trigger_dist_phase2 = 500;

void frontTouch(){
	forward();
	while (!digitalRead(headlght_left) || !digitalRead(headlight_right));
	if (digitalRead(headlght_left)){
		//// change the right speed
		m.setSpeed(0, 100);
		while (!digitalRead(headlight_right));
		m.setSpeed(0,0);
	}
	else {
		///// change left speed
		m.setSpeed(100, 0);
		while (!digitalRead(headlght_left));
		m.setSpeed(0,0);
	}
}

unsigned int present = 500;
void forward(){
	speed_left = cal_left, speed_right = cal_right;
	m.setSpeed(speed_left,speed_right);
}
void checkBlock(boolean side, boolean stop_at /*no block (false) or until block is present (true)*/){
	unsigned char side_sense = ir_right;
	if (side) side_sense = ir_left;
	if (stop_at) while (analogRead(side_sense) > present);
	else while(analogRead(side_sense) < present);
	m.setSpeed(0,0);
}

// attach to a distance 1 block away
void kissMe(int dist){
	forward();
	while (analogRead(ir_front) < dist){ /*alignment trigger*/
		if (analogRead(ir_left)>cm_5){
			speed_left++;
			speed_right--;
			m.setSpeed(speed_left,speed_right);
		}
		if (analogRead(ir_right)>cm_5){
			speed_right++;
			speed_left--;
			m.setSpeed(speed_left, speed_right);
		}
	}
	m.setSpeed(0,0); /*brakes*/
}

////////////Calibrate delays (*Caution hypothetical*)
void cross_bridge(){
	forward();
	while (analogRead(ir_front) < trigger_dist_phase2){
		if(!digitalRead(ground_left)){
			m.setSpeed(-speed_left, -speed_right);
			delay(100);
			m.setSpeed(0,0);
			m.setSpeed(speed_left + 10, speed_right);
			delay(100);
			forward();
		}
		if(!digitalRead(ground_right)){
			m.setSpeed(-speed_left, -speed_right);
			delay(100);
			m.setSpeed(0,0);
			m.setSpeed(speed_left, speed_right + 10);
			delay(100);
			forward();
		}
	}
}

/***Turning algorithm****/
int
	calibrated_turn90 = 300, /*300ms to turn */
	calibrated_turn180 = 600; /*600ms to turn */
//from front touch
void turn_type1(){

}