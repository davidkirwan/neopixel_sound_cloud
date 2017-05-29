/*
Sound Cloud - Sound Activated Thundercloud. 

keywords: attiny85, microphone, neopixel, thundercloud, art, arduino

David Kirwan - https://github.com/davidkirwan
USB -> UART TTL Serial: https://www.aliexpress.com/item/CJMCU-CP2102-MICRO-USB-to-UART-TTL-Module-6Pin-Serial-Converter-UART-STC-Replace-FT232-NEW/32499746290.html
Microphone module: https://www.aliexpress.com/item/High-Sensitivity-Sound-Microphone-Sensor-Detection-Module-For-Arduino-AVR-PIC/1540010132.html
*/

#include <Adafruit_NeoPixel.h>
//#include <SoftwareSerial.h>


#define MICPIN         A1
#define NEOPIXELPIN    0
#define NEOPIXELPIN2   3
#define NUMPIXELS      30
//#define RX             3
//#define TX             4

int peakhigh = 0;
int peaklow = 1023;
int peak = 0;
int threshold = 0;
int lvl = 0;
int counter = 0;
int val = 0;
int diff = 0;


Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIXELPIN, NEO_RGB + NEO_KHZ800); //NEO_KHZ800);
Adafruit_NeoPixel pixels2 = Adafruit_NeoPixel(NUMPIXELS, NEOPIXELPIN2, NEO_RGB + NEO_KHZ800); //NEO_KHZ800);
//SoftwareSerial mySerial(RX, TX);


void lightning_flash(int lvl){
  int r = random(0, 4);
  for(int i=0; i<lvl; i++){
    // GRB
    switch(r){
      case 0:
        pixels.setPixelColor(i, 255, 255, 255); // White
        pixels2.setPixelColor(i, 255, 255, 255); // White
        break;
      case 1:
        pixels.setPixelColor(i, 0, 255, 255); // Purple
        pixels2.setPixelColor(i, 0, 255, 255); // Purple
        break;
      case 2:
        pixels.setPixelColor(i, 0, 0, 255); // Blue
        pixels2.setPixelColor(i, 0, 0, 255); // Blue
        break;
      case 3:
        //pixels.setPixelColor(i, 255, 255, 0); // Yellow
        //pixels.setPixelColor(i, 255, 0, 0); // Green
        pixels.setPixelColor(i, 255, 0, 255); // Cyan
        pixels2.setPixelColor(i, 255, 0, 255); // Cyan
        break;
      case 4:
        pixels.setPixelColor(i, 0, 255, 0); // Red
        pixels2.setPixelColor(i, 0, 255, 0); // Red
        break;
    }
  }

  for(int i=NUMPIXELS-1; i>lvl; i--){
    pixels.setPixelColor(i, 0, 0, 0);
    pixels2.setPixelColor(i, 0, 0, 0);
  }
  pixels.show(); // This sends the updated pixel color to the hardware.
  pixels2.show(); // This sends the updated pixel color to the hardware. 
}



void setup(){
  //mySerial.begin(9600);
  pixels.begin(); // This initializes the NeoPixel library.
  pixels.show(); // Initiates the strip to off
  pixels2.begin();
  pixels2.show();
  delay(1000);
  //mySerial.println("setup");
}

void loop(){
  if(counter == 50){
    counter = 0;
    if(peakhigh != peaklow){
      peakhigh = threshold;
      peaklow = threshold;
    }
  }
  
  peak = 0;
  lvl = 0;
  val = 0;
  diff = 0;
  peak = analogRead(MICPIN);

  if(peak > peakhigh){ 
    peakhigh = peak;
  }

  if(peak < peaklow){ 
    peaklow = peak; 
  }

  threshold = ((peakhigh + peaklow) / 2);

  if(peak >= threshold){
    diff = abs(peakhigh - threshold); 
  } else{ 
    diff = abs(peaklow - threshold); 
  }
  
  val = abs(peak - threshold);
  lvl = map(val, 0, diff, 0, NUMPIXELS);
  
  
  if(lvl < 0 || peakhigh == peaklow){ lvl = 0; } // the map function was returning negative numbers..
  
  
  if(diff <= 2 && lvl >= NUMPIXELS/4){
    lvl = NUMPIXELS/10;
  }
  
  if(diff <= 4 && lvl >= NUMPIXELS/2){
    lvl = NUMPIXELS/4;
  }
  
  /*
  if(counter % 2 == 0){
    mySerial.print(peaklow);
    mySerial.print(", ");
    mySerial.print(peakhigh);
    mySerial.print(", ");
    mySerial.print(peak);
    mySerial.print(", ");
    mySerial.print(threshold);
    mySerial.print(", ");
    mySerial.println(lvl);
  }
  */
  
  lightning_flash(lvl);
  delay(50);
  
  counter += 1;
}
