int cm_5 = 800; /*change this*/
unsigned char
	ir_front = 2,
	ir_left = 0,
	ir_right = 4,
	ir_left_45 = 1,
	ir_right_45 = 3,
	speed_left = 150, speed_right = 150;

volatile unsigned short blocks = 0;

void alignment(){ /*must be ran in loops*/
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

void kissMe(int dist, bool counter = false){
	m.setSpeed(speed_left,speed_right);
	while (analogRead(ir_front) < dist){ /*alignment trigger*/
		alignment();
	}
	m.setSpeed(0,0); /*brakes*/
}
void blockCounterLeft(unsigned short number_of_blocks){
	m.setSpeed(speed_left,speed_right);
	blocks = 0;
	attachInterrupt(0, adder, FALLING);
	while (blocks < number_of_blocks){ /*alignment*/
		alignment();
	}
	detachInterrupt(0);
	m.setSpeed(0,0); /*finished counting*/
}
void blockCounterRight(unsigned short number_of_blocks){
	m.setSpeed(speed_left,speed_right);
	blocks = 0;
	attachInterrupt(1, adder, FALLING);
	while (blocks < number_of_blocks){ /*alignment*/
		alignment();
	}
	detachInterrupt(1);
	m.setSpeed(0,0); /*finished counting*/
}
void adder(){ //interrupt function
	blocks++;
}


/***Turning algorithm****/
int
	calibrated_turn90 = 300, /*300ms to turn */
	calibrated_turn180 = 600; /*600ms to turn */
