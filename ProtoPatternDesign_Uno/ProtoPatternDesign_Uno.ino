#define DEBUG
#ifdef DEBUG
 #define DEBUG_PRINT(x)     Serial.print (x)
 #define DEBUG_PRINTDEC(x)     Serial.print (x, DEC)
 #define DEBUG_PRINTLN(x)  Serial.println (x)
#else
 #define DEBUG_PRINT(x)
 #define DEBUG_PRINTDEC(x)
 #define DEBUG_PRINTLN(x) 
#endif

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 9
#define NUM 60

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM, PIN, NEO_GRBW + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code
  Serial.begin(9600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

//void TheaterChaseWTwoColor(unit32_t c1, unit32_t c2);



// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void MixColorTheaterChase() {
  for (int j=0; j<100; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel(random(255)));    //turn every third pixel on
      }
      strip.show();

      delay(80);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
//        strip.setPixelColor(i+q, Wheel(random(255)));        //turn every third pixel off
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

void TheaterChaseWTwoColor(uint32_t c1 = Wheel(random(255)), uint32_t c2 = Wheel(random(255))) {
  c1 = Wheel(random(255));
  for (int j=0; j<80; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c1);    //turn every third pixel on
      }
      strip.show();

      delay(80);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
//        strip.setPixelColor(i+q, Wheel(random(255)));        //turn every third pixel off
        strip.setPixelColor(i+q, c2);        //turn every third pixel off
      }
    }
  }
}
void TwoDirectionScanner(byte num = 1, uint32_t Color1 = Wheel(random(255))){
  while(num--){
    for(int j = 0; j < strip.numPixels(); j++){
      for (int i = 0; i < strip.numPixels(); i++)
      {
          if (i == j)  // Scan Pixel to the right
          {
               strip.setPixelColor(i, Color1);
          }
          else if (i == strip.numPixels() - j) // Scan Pixel to the left
          {
               strip.setPixelColor(i, Color1);
          }
          else // Fading tail
          {
               strip.setPixelColor(i, DimColor(strip.getPixelColor(i)));
          }
      }
      strip.show();
      delay(50);
    }
  }
}

void fade(byte steps = 400, uint32_t Color1 = Wheel(random(20, 40)), uint32_t Color2 = Wheel(random(50,200))){
  while(steps){
    uint8_t red = ((Red(Color1) * (steps)) + (Red(Color2))) / steps;
    uint8_t green = ((Green(Color1) * (steps)) + (Green(Color2))) / steps;
    uint8_t blue = ((Blue(Color1) * (steps)) + (Blue(Color2))) / steps;
    
    ColorSet(strip.Color(red, green, blue));
    delay(120);
    steps--;
  }
}
void loop() { 
  DEBUG_PRINTLN("Scanner from two direction w/ random color...");
  TwoDirectionScanner(3);
  TwoDirectionScanner();
  TwoDirectionScanner();
  
  DEBUG_PRINTLN("Color gradual chage...");
  for(int i = 0; i < NUM; i++){
    strip.setPixelColor(i, 255 - i * 4, i, i * 3, 0);
  }
  strip.show();
  delay(3000); 

  DEBUG_PRINTLN("Blue, Green, Red");
  for(int i = 0; i < 20; i++){
    strip.setPixelColor(i, 0,0,255,0);
    strip.setPixelColor(i+20, 0,255,0,0);
    strip.setPixelColor(i+40, 255,0,0,0);
  }
  strip.show();
  delay(3000);

  DEBUG_PRINTLN("Color wiping...");
  colorWipe(strip.Color(255, 0, 0), 20); // Red
  colorWipe(strip.Color(0, 255, 0), 20); // Green
  colorWipe(strip.Color(0, 0, 255), 20); // Blue
  colorWipe(strip.Color(10, 120, 120), 20); //
  colorWipe(strip.Color(150, 150, 10), 20); //
  colorWipe(strip.Color(150, 0, 100), 20); //
  colorWipe(strip.Color(0, 0, 0, 255), 50); // White RGBW

  DEBUG_PRINTLN("Single color theater chasing...");
  theaterChase(strip.Color(127, 127, 127), 50); // White
  theaterChase(strip.Color(127, 0, 0), 50); // Red
  theaterChase(strip.Color(0, 0, 127), 50); // Blue

  DEBUG_PRINTLN("Random two color theater chasing...");
  TheaterChaseWTwoColor();

  DEBUG_PRINTLN("Tandom mixed color theater chasing...");
  MixColorTheaterChase();

  DEBUG_PRINTLN("Rainbow theater chasing...");
  theaterChaseRainbow(40);

  DEBUG_PRINTLN("Rainbow...");
  rainbow(10);

  DEBUG_PRINTLN("Rainbow cycling...");
  rainbowCycle(5);
}
