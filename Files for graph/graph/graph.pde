import processing.serial.*;
import javax.swing.*; 

float[] vals, absolute;
int scale = 200, pinNumber = 0, pre_height = 200, last;
Serial uno;
boolean pause = false;
void setup() {
  try { 
    UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
  } 
  catch (Exception e) { 
    e.printStackTrace();
  } 
  size(800,250);
  smooth();
  String x, f;
  String[] k;
  
  do{
    f = "";
    k = uno.list();
    for(int i=0; i<k.length; i++){
      f+= "\n ["+ Integer.toString(i) + "] \"" + k[i] + "\"";
    }
    x = JOptionPane.showInputDialog(frame, "Enter Serial Port Index:" + f);
  }while(x == null || x == "");
  uno = new Serial(this, uno.list()[int(x)], 9600);
  uno.bufferUntil('\n');
  // An array of random values
  //frameRate(60);
  vals = new float[scale];
  absolute = new float[width];
  
  for (int i = 0; i < vals.length; i++) {
    vals[i] = pre_height; //random(1024)/1024 * pre_height
  }
  for (int i = 0; i < absolute.length; i++){
    absolute[i] = pre_height;
  }
}
void draw(){
  pushMatrix();
  translate(0,height-pre_height);
  String inString = uno.readStringUntil('\n'); //"300,500,600,400,900,100"; //Float.toString(random(1024))+","+Float.toString(random(1024))+","+Float.toString(random(1024));
  if(inString != null){
    inString = trim(inString);
    inString = split(inString, ",")[pinNumber];
    background(255);
    
    //make grid
    for (int i = 0; i < scale; i++){
      stroke(220);
      strokeWeight(1);
      /*line((float)i/(24-1)*width, 0, (float)i/(24-1)*width, pre_height);//*/line((float)i/(scale-1)*width, 0, (float)i/(scale-1)*width, pre_height);
    }
    
    //display grid voltage
    for(int i = 0; i < 6; i++){
      stroke(220);
      strokeWeight(1);
      line(0,(float)i*pre_height/5,width,(float)i*pre_height/5);
      textSize((float)11/200*pre_height);
      textAlign(LEFT, CENTER);
      text(Integer.toString(i),(float).01 *width,(float)pre_height - i*pre_height/5);
    }
    //DRAW pinNumber
    textAlign(RIGHT, CENTER);
    textSize((float)22/200*pre_height);
    text("A"+Integer.toString(pinNumber), width, (float)-(height - pre_height)/2);
    /////Draw Graph
    for (int i = 0; i < vals.length-1; i++) {
      fill(0,255,38,123);
      noStroke();
      quad((float)i/(scale-1)*width,vals[i], (float)i/(scale-1)*width, pre_height, (float)(i+1)/(scale-1)*width, pre_height, (float)(i+1)/(scale-1)*width,vals[i+1]);
      stroke(0,201,30);
      strokeWeight(1);
      line((float)i/(scale-1)*width,vals[i], (float)(i+1)/(scale-1)*width,vals[i+1]);
    }
    //Write voltage
    if(!pause){
    fill(124);
    noStroke();
    textSize((float)24/200*pre_height);
    textAlign(RIGHT);
    text(inString + " : " + String.format("%.2f",(float)float(inString)/1024*5) + "v", (float)width*0.9, vals[scale-1]-10);
    stroke(0);
    //Draw current line
    line(0, vals[scale-1], width, vals[scale-1]);
    // Slide everything down in the array
    for (int i = 0; i < vals.length-1; i++) {
      vals[i] = vals[i+1]; 
    }
    for(int i = 0; i < absolute.length -1; i++){
      absolute[i] = absolute[i+1];
    }
    // Add a new random/update value
    
    vals[vals.length-1] = pre_height - ((float)float(inString)/1024 * pre_height);
    absolute[absolute.length-1] = vals[vals.length-1];
    last = int(inString);
    }
    else{
      fill(124);
      noStroke();
      textSize((float)24/200*pre_height);
      textAlign(RIGHT);
      text(Integer.toString(last) + " : " + String.format("%.2f",(float)last/1024*5) + "v", (float)width*0.9, vals[scale-2]-10);
      stroke(0);
      //Draw current line
      line(0, pre_height - ((float)last/1024 * pre_height), width, pre_height - ((float)last/1024 * pre_height));
    }
  }
  popMatrix();
}
void keyPressed() {
  if (key == '+'){
    if (scale > 10){
      scale-=10;
      vals = new float[scale];
      for(int i = 0; i < scale; i ++){
        vals[scale-1-i] = absolute[absolute.length-1-i];
      }
    }
  }
  else if(key == '-'){
    if (scale < 400){
      scale+=10;
      vals = new float[scale];
      for(int i = 0; i < scale; i ++){
        vals[scale-1-i] = absolute[absolute.length-1-i];
      }
    }
  }
  if(key == ' '){
    pause = !pause;
  }
  for(int i = 0; i<6; i++){
    if(key == (char)(i+'0') ){
      pinNumber = i;
    }
  }
}
