

#include <SoftPWM.h>
#include <IRremote.h>
#include <LedStripe.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

int IR_RECV_PIN = 13;
IRrecv irrecv(IR_RECV_PIN);
decode_results results;

// commands
const unsigned long cmd_on = 0xF7C03F;
const unsigned long cmd_off = 0xF740BF;

const unsigned long cmd_bright_up = 0xF700FF;
const unsigned long cmd_bright_down = 0xF7807F;

const unsigned long cmd_red0 = 0xF720DF;
const unsigned long cmd_red1 = 0xF710EF;
const unsigned long cmd_red2 = 0xF730CF;
const unsigned long cmd_red3 = 0xF708F7;
const unsigned long cmd_red4 = 0xF728D7;

const unsigned long cmd_green0 = 0xF7A05F;
const unsigned long cmd_green1 = 0xF7906F;
const unsigned long cmd_green2 = 0xF7B04F;
const unsigned long cmd_green3 = 0xF78877;
const unsigned long cmd_green4 = 0xF7A857;

const unsigned long cmd_blue0 = 0xF7609F;
const unsigned long cmd_blue1 = 0xF750AF;
const unsigned long cmd_blue2 = 0xF7708F;
const unsigned long cmd_blue3 = 0xF748B7;
const unsigned long cmd_blue4 = 0xF76897;

const unsigned long cmd_white0 = 0xF7E01F;
const unsigned long cmd_white1 = 0xF7D02F;
const unsigned long cmd_white2 = 0xF7F00F;
const unsigned long cmd_white3 = 0xF7C837;
const unsigned long cmd_white4 = 0xF7E817;

const int nrOfStrips = 6;
LedStripe strip[nrOfStrips] = {
  LedStripe(10,A5,0),
  LedStripe(11,A4,1),
  LedStripe(12,A3,2),
  LedStripe( 8,A2,6),
  LedStripe( 9,A1,7),
  LedStripe( 4, 5,3)
};

void setup()
{
  SoftPWMBegin();
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  
  setAllOff();
}

void setAllColor(int red, int green, int blue) {
  for(int i=0;i<nrOfStrips;i++) {
    strip[i].SetColor(red,green,blue);
  }
}

void setOneToColor(int nr,int red, int green, int blue) {
  for(int i=0;i<nrOfStrips;i++) {
    if(i==nr) {
      strip[i].SetColor(red,green,blue);
    } else {
      strip[i].SetColor(0x00,0x00,0x00);
    }
  }
}

void setAllOff() {
  for(int i=0;i<nrOfStrips;i++) {
    strip[i].Off();
  }
}

void setAllOn() {
  for(int i=0;i<nrOfStrips;i++) {
    strip[i].On();
  }
}

void setAllIncBrightness() {
  for(int i=0;i<nrOfStrips;i++) {
    strip[i].IncBrightness();
  }
}

void setAllDecBrightness() {
  for(int i=0;i<nrOfStrips;i++) {
    strip[i].DecBrightness();
  }
}

int animation = 0;

void animation1() {
  static unsigned long j = 0;  
  
  for(int i=0;i<nrOfStrips;i++) {
    strip[i].SetColor(Wheel(((i * 256 / nrOfStrips) + j) & 255));
  }
  delay(50);
  j++; j%=256*nrOfStrips;
}

void animation2() {
  static int j = 0;
  
  for(int i=0;i<nrOfStrips;i++) {
    if(i==j) {
      strip[i].SetColor(0xFF,0xFF,0xFF);
    } else {
      strip[i].SetColor(0x00,0x00,0xFF);
    }
    
  }
  
  //delay(1000);
  
  j++;j%=nrOfStrips;
}

void animation4() {
  static int j = 0;
  
  for(int i=0;i<nrOfStrips;i++) {
    if(i==j) {
      strip[i].SetColor(0xFF,0x00,0x00);
    } else {
      strip[i].SetColor(0x00,0x00,0x00);
    }
    
  }
  
  //delay(1000);
  
  j++;j%=nrOfStrips;
}

void animation5() {
  static int j = 0;
  
  for(int i=0;i<nrOfStrips;i++) {
    if(i==j) {
      strip[i].SetColor(0x00,0xFF,0x00);
    } else {
      strip[i].SetColor(0x00,0x00,0x00);
    }
    
  }
  
  //delay(1000);
  
  j++;j%=nrOfStrips;
}

void animation6() {
  static int j = 0;
  
  for(int i=0;i<nrOfStrips;i++) {
    if(i==j) {
      strip[i].SetColor(0x00,0x00,0xFF);
    } else {
      strip[i].SetColor(0x00,0x00,0x00);
    }
    
  }
  
  //delay(1000);
  
  j++;j%=nrOfStrips;
}

void animation3() {
  static int j = 2;
  
  strip[(j-2)%nrOfStrips].SetColor(0x00,0x66,0xFF); // blau
  strip[(j-1)%nrOfStrips].SetColor(0x00,0xFF,0x00); // grün
  strip[(j+0)%nrOfStrips].SetColor(0xFF,0xFF,0x00); // gelb
  strip[(j+1)%nrOfStrips].SetColor(0xFF,0x66,0x00); // oange
  strip[(j+2)%nrOfStrips].SetColor(0xFF,0x00,0x00); // rot
    
  j++;
}

T_RGB Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  
  if(WheelPos < 85) {
    return T_RGB(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return T_RGB(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return T_RGB(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void changemode(unsigned long cmd) {
  T_RGB rgb;
  T_YUV yuv;
  
  Serial.println(cmd);
  
  switch(cmd) {
    case cmd_on: setAllOn(); animation = 0; break;
    case cmd_off: setAllOff(); animation = 0; break;
    case cmd_red0: setAllColor(0xFF,0x00,0x00); animation = 0; break;
    case cmd_red1: animation4(); animation = 0; break;
    case cmd_red2: setOneToColor(2,0xFF,0x00,0x00); animation = 0; break;      
    case cmd_red3: setOneToColor(3,0xFF,0x00,0x00); animation = 0; break;      
    case cmd_red4: setOneToColor(4,0xFF,0x00,0x00); animation = 0; break;
      
    case cmd_green0: setAllColor(0x00,0xFF,0x00); animation = 0; break;      
    case cmd_green1: animation5(); animation = 0; break;      
    case cmd_green2: setOneToColor(2,0x00,0xFF,0x00); animation = 0; break;
    case cmd_green3: setOneToColor(3,0x00,0xFF,0x00); animation = 0; break;
    case cmd_green4: setOneToColor(4,0x00,0xFF,0x00); animation = 0; break;
      
    case cmd_blue0: setAllColor(0x00,0x00,0xFF); animation = 0; break;
    case cmd_blue1: animation6(); animation = 0; break;      
    case cmd_blue2: setOneToColor(2,0x00,0x00,0xFF); animation = 0; break;
    case cmd_blue3: setOneToColor(3,0x00,0x00,0xFF); animation = 0; break;
    case cmd_blue4: setOneToColor(4,0x00,0x00,0xFF); animation = 0; break;
      
    case cmd_white0: setAllColor(255,255,255); animation = 0; break;
    case cmd_white1: animation = 1; break;
    case cmd_white2: animation2(); animation = 0; break;
    case cmd_white3: animation3(); animation = 0; break; 
    case cmd_bright_up: setAllIncBrightness(); break;
    case cmd_bright_down: setAllDecBrightness(); break;     
    default: break;
  }
}

unsigned long lastcmd = 0;


void loop()
{

  //delay(length*2);
  
  if (irrecv.decode(&results)) {
    if(results.value == REPEAT) {
      results.value = lastcmd;
    }
    
    changemode(results.value);
    
    lastcmd = results.value;
    
    irrecv.resume(); // resume receiver
  }
  
  switch(animation) {
    case 1:
      animation1();
      break;
    case 2:
      animation2();
      break;
    case 3:
      animation3();
      break;  
    default:
      break;
    
  }
}
