#include <Arduino.h>
#include <FastLED.h>
#include <IRremote.h>
//#include <Adafruit_NeoPixel.h>

#define NUM_LEDS 20
CRGB leds[NUM_LEDS];
// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);


//Heavy inspiration taken from https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/#LEDStripEffectFadeInandFadeOutRedGreenandBlue


const int NeoPixelPin = 4;
const int IRReceiver = 2; // Signal Pin of IR receiver to Arduino 

/*-----( Declare objects )-----*/
IRrecv irrecv(IRReceiver);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'


int selection;
int brightness;

//prototypes

void translateIR();
//void showStrip();
//void setPixel(int, byte, byte, byte);
//void setAll(byte, byte, byte);
void RGBLoop();
void strobe(byte, byte, byte, int, int, int);
void twinkleRandom(int, int, boolean);
//void leftToRight(byte, byte, byte, int, int, int);
//void rightToLeft(byte, byte, byte, int, int, int);
//void outsideToCenter(byte, byte, byte, int, int, int);
//void centerToOutside(byte, byte, byte, int, int, int);
void newKITT(byte, byte, byte, int, int, int);
void cylonBounce(byte, byte, byte, int, int, int);
void rainbowCycle(int);
void fire(int, int, int);
void bouncingColoredBalls(int, byte[][3]);
void meteorRain(byte, byte, byte, byte, byte, boolean, int);
void flashlight(byte, byte, byte);


void setup()
{
  Serial.begin(9600);
  FastLED.addLeds<WS2811, NeoPixelPin, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);


  /* USE FOR NEOPIXEL LIBRARY */
  //strip.begin();
  //strip.show(); // Initialize all pixels to 'off'
  irrecv.enableIRIn(); // Start the receiver
  
  Serial.println("IR Receiver Button Decode"); 

  selection = 0;
  brightness = 153;
  FastLED.setBrightness(brightness);
}


void loop()
{
  if (irrecv.decode(&results)) // have we received an IR signal?
  {
    translateIR(); 
    irrecv.resume(); // receive the next value
  }  

  // ---> here we call the effect function <---
  switch (selection)
  {
    case 0:
    {
      RGBLoop();
      break;
    }
    case 1:
    {
      twinkleRandom(20, 100, false);
      break;
    }
    case 2:
    {
      rainbowCycle(20);
      break;
    }
    case 3:
    {
      cylonBounce(0xff, 0, 0, 4, 10, 50);
      break;
    }
    case 4:
    {  
      newKITT(0xff, 0, 0, 8, 10, 50);
      break;
    }
    case 5:
    { 
      byte colors[3][3] = {{0xff, 0,0}, {0xff, 0xff, 0xff}, {0, 0, 0xff}};
      bouncingColoredBalls(3, colors);
      break;
    }
    case 6:
    {
      fire(55,120,15);
      break;
    }
    case 7:
    {
      meteorRain(0xff,0xff,0xff,10, 64, true, 30);
      break;
    }
    case 8:
    {  
      strobe(0xff, 0xff, 0xff, 10, 50, 1000);
      break;
    }
    case 9:
    {  
      flashlight(0xff, 0xff, 0xff);
      break;
    }
    default:
    {  
      break;
    }
  }
}




void translateIR() // takes action based on IR code received
{
  switch(results.value)
  {    
    case 0xFF629D: 
      Serial.println(" FORWARD");
      if (brightness < 255) brightness += 51;
      FastLED.setBrightness(brightness);
      break;

    case 0xFF22DD: 
      Serial.println(" LEFT");    
      (selection > 0) ? selection-- : selection = 9;
      break;
    
    case 0xFF02FD: 
      Serial.println(" -OK-");    
      break;
    
    case 0xFFC23D: 
      Serial.println(" RIGHT");   
      (selection < 9) ? selection++ : selection = 0;
      break;
    
    case 0xFFA857: 
      Serial.println(" DOWN"); 
      if (brightness > 0) brightness -= 51;
      FastLED.setBrightness(brightness);
      break;
    
    case 0xFF4AB5: 
      Serial.println(" 0");    
      selection = 0;
      break;
    
    case 0xFF6897:
      Serial.println(" 1");    
      selection = 1;
      break;
    
    case 0xFF9867: 
      Serial.println(" 2");    
      selection = 2;
      break;
    
    case 0xFFB04F: 
      Serial.println(" 3");    
      selection = 3;
      break;
    
    case 0xFF30CF: 
      Serial.println(" 4");    
      selection = 4;
      break;
    
    case 0xFF18E7: 
      Serial.println(" 5");    
      selection = 5;
      break;
    
    case 0xFF7A85: 
      Serial.println(" 6");    
      selection = 6;
      break;

    case 0xFF10EF: 
      Serial.println(" 7");   
      selection = 7;
      break;
    
    case 0xFF38C7: 
      Serial.println(" 8");    
      selection = 8;
      break;
    
    case 0xFF5AA5: 
      Serial.println(" 9");    
      selection = 9;
      break;
    
    case 0xFF42BD: 
      Serial.println(" *");   
      break;
    
    case 0xFF52AD: 
      Serial.println(" #");    
      break;
    
    case 0xFFFFFFFF: 
      Serial.println(" REPEAT");
      break;  
    
    default: 
      Serial.println(" other button:   ");
      Serial.println(results.value);
      break;
      // End Case
  delay(500); // Do not get immediate repeat
  } 
}

void showStrip()
{
#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  strip.show();
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  FastLED.show();
#endif
}


void setPixel(int Pixel, byte red, byte green, byte blue)
{
#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  strip.setPixelColor(Pixel, strip.Color(red, green, blue));
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  leds[Pixel].r = red;
  leds[Pixel].g = green;
  leds[Pixel].b = blue;
#endif
}


void setAll(byte red, byte green, byte blue)
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    setPixel(i, red, green, blue);
  }
  showStrip();
}


void RGBLoop()
{
  for (int j = 0; j < 3; j++)
  {
    // Fade IN
    for (int k = 0; k < 256; k++)
    {
      switch (j)
      {
      case 0:
        setAll(k, 0, 0);
        break;
      case 1:
        setAll(0, k, 0);
        break;
      case 2:
        setAll(0, 0, k);
        break;
      }
      showStrip();
      delay(3);
    }
    // Fade OUT
    for (int k = 255; k >= 0; k--)
    {
      switch (j)
      {
      case 0:
        setAll(k, 0, 0);
        break;
      case 1:
        setAll(0, k, 0);
        break;
      case 2:
        setAll(0, 0, k);
        break;
      }
      showStrip();
      delay(3);
    }
  }
}


void strobe(byte red, byte green, byte blue, int StrobeCount, int FlashDelay, int EndPause)
{
  for (int j = 0; j < StrobeCount; j++)
  {
    setAll(red, green, blue);
    showStrip();
    delay(FlashDelay);
    setAll(0, 0, 0);
    showStrip();
    delay(FlashDelay);
  }

  delay(EndPause);
}

void twinkleRandom(int Count, int SpeedDelay, boolean OnlyOne)
{
  setAll(0, 0, 0);

  for (int i = 0; i < Count; i++)
  {
    setPixel(random(NUM_LEDS), random(0, 255), random(0, 255), random(0, 255));
    showStrip();
    delay(SpeedDelay);
    if (OnlyOne)
    {
      setAll(0, 0, 0);
    }
  }

  delay(SpeedDelay);
}


void cylonBounce(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay)
{
  for(int i = 0; i < NUM_LEDS-EyeSize-2; i++)
  {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++)
    {
      setPixel(i+j, red, green, blue); 
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }

  delay(ReturnDelay);

  for(int i = NUM_LEDS-EyeSize-2; i > 0; i--)
  {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++)
    {
      setPixel(i+j, red, green, blue); 
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }

  delay(ReturnDelay);
}


void centerToOutside(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay)
{
  for (int i = ((NUM_LEDS - EyeSize) / 2); i >= 0; i--)
  {
    setAll(0, 0, 0);

    setPixel(i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++)
    {
      setPixel(i + j, red, green, blue);
    }
    setPixel(i + EyeSize + 1, red / 10, green / 10, blue / 10);

    setPixel(NUM_LEDS - i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++)
    {
      setPixel(NUM_LEDS - i - j, red, green, blue);
    }
    setPixel(NUM_LEDS - i - EyeSize - 1, red / 10, green / 10, blue / 10);

    showStrip();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}

void outsideToCenter(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay)
{
  for (int i = 0; i <= ((NUM_LEDS - EyeSize) / 2); i++)
  {
    setAll(0, 0, 0);

    setPixel(i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++)
    {
      setPixel(i + j, red, green, blue);
    }
    setPixel(i + EyeSize + 1, red / 10, green / 10, blue / 10);

    setPixel(NUM_LEDS - i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++)
    {
      setPixel(NUM_LEDS - i - j, red, green, blue);
    }
    setPixel(NUM_LEDS - i - EyeSize - 1, red / 10, green / 10, blue / 10);

    showStrip();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}

void leftToRight(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay)
{
  for (int i = 0; i < NUM_LEDS - EyeSize - 2; i++)
  {
    setAll(0, 0, 0);
    setPixel(i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++)
    {
      setPixel(i + j, red, green, blue);
    }
    setPixel(i + EyeSize + 1, red / 10, green / 10, blue / 10);
    showStrip();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}

void rightToLeft(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay)
{
  for (int i = NUM_LEDS - EyeSize - 2; i > 0; i--)
  {
    setAll(0, 0, 0);
    setPixel(i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++)
    {
      setPixel(i + j, red, green, blue);
    }
    setPixel(i + EyeSize + 1, red / 10, green / 10, blue / 10);
    showStrip();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}

void newKITT(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay)
{
  rightToLeft(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  leftToRight(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  outsideToCenter(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  centerToOutside(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  leftToRight(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  rightToLeft(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  outsideToCenter(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  centerToOutside(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
}


byte* wheel(byte wheelPos)
{
  static byte c[3];
  
  if(wheelPos < 85)
  {
   c[0]=wheelPos * 3;
   c[1]=255 - wheelPos * 3;
   c[2]=0;
  }
  else if(wheelPos < 170)
  {
   wheelPos -= 85;
   c[0] = 255 - wheelPos * 3;
   c[1] = 0;
   c[2] = wheelPos * 3;
  }
  else
  {
   wheelPos -= 170;
   c[0] = 0;
   c[1] = wheelPos * 3;
   c[2] = 255 - wheelPos * 3;
  }

  return c;
}

void rainbowCycle(int SpeedDelay)
{
  byte* c;
  uint16_t i, j;

  for(j=0; j<256*5; j++)
  { // 5 cycles of all colors on wheel
    for(i=0; i< NUM_LEDS; i++)
    {
      c=wheel(((i * 256 / NUM_LEDS) + j) & 255);
      setPixel(i, *c, *(c+1), *(c+2));
    }
    showStrip();
    delay(SpeedDelay);
  }
}


void setPixelHeatColor (int Pixel, byte temperature)
{
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature / 255.0) * 191);
 
  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252
 
  // figure out which third of the spectrum we're in:
  if( t192 > 0x80)
  {                     // hottest
    setPixel(Pixel, 255, 255, heatramp);
  }
  else if( t192 > 0x40 )
  {             // middle
    setPixel(Pixel, 255, heatramp, 0);
  }
  else
  {                               // coolest
    setPixel(Pixel, heatramp, 0, 0);
  }
}

void fire(int Cooling, int Sparking, int SpeedDelay)
{
  static byte heat[NUM_LEDS];
  int cooldown;
  
  // Step 1.  Cool down every cell a little
  for( int i = 0; i < NUM_LEDS; i++)
  {
    cooldown = random(0, ((Cooling * 10) / NUM_LEDS) + 2);
    
    if(cooldown>heat[i])
    {
      heat[i]=0;
    }
    else
    {
      heat[i] = heat[i] - cooldown;
    }
  }
  
  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for( int k = NUM_LEDS - 1; k >= 2; k--)
  {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }
    
  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if( random(255) < Sparking )
  {
    int y = random(7);
    heat[y] = heat[y] + random(160,255);
    //heat[y] = random(160,255);
  }

  // Step 4.  Convert heat to LED colors
  for( int j = 0; j < NUM_LEDS; j++)
  {
    setPixelHeatColor(j, heat[j] );
  }

  showStrip();
  delay(SpeedDelay);
}


void bouncingColoredBalls(int BallCount, byte colors[][3])
{
  float Gravity = -9.81;
  int StartHeight = 1;
  
  float Height[BallCount];
  float ImpactVelocityStart = sqrt(-2 * Gravity * StartHeight);
  float ImpactVelocity[BallCount];
  float TimeSinceLastBounce[BallCount];
  int   Position[BallCount];
  long  ClockTimeSinceLastBounce[BallCount];
  float Dampening[BallCount];
  
  for (int i = 0 ; i < BallCount ; i++)
  {   
    ClockTimeSinceLastBounce[i] = millis();
    Height[i] = StartHeight;
    Position[i] = 0; 
    ImpactVelocity[i] = ImpactVelocityStart;
    TimeSinceLastBounce[i] = 0;
    Dampening[i] = 0.90 - float(i) / pow(BallCount,2); 
  }

  while (true)
  {
    for (int i = 0 ; i < BallCount ; i++)
    {
      TimeSinceLastBounce[i] =  millis() - ClockTimeSinceLastBounce[i];
      Height[i] = 0.5 * Gravity * pow(TimeSinceLastBounce[i] / 1000 , 2.0 ) + ImpactVelocity[i] * TimeSinceLastBounce[i] / 1000;
  
      if ( Height[i] < 0 )
      {                      
        Height[i] = 0;
        ImpactVelocity[i] = Dampening[i] * ImpactVelocity[i];
        ClockTimeSinceLastBounce[i] = millis();
  
        if (ImpactVelocity[i] < 0.01)
        {
          ImpactVelocity[i] = ImpactVelocityStart;
        }
      }
      Position[i] = round( Height[i] * (NUM_LEDS - 1) / StartHeight);
    }
  
    for (int i = 0 ; i < BallCount ; i++)
    {
      setPixel(Position[i],colors[i][0],colors[i][1],colors[i][2]);
    }
    
    showStrip();
    setAll(0,0,0);
  }
}

void fadeToBlack(int ledNo, byte fadeValue)
{
 #ifdef ADAFRUIT_NEOPIXEL_H 
    // NeoPixel
    uint32_t oldColor;
    uint8_t r, g, b;
    int value;
    
    oldColor = strip.getPixelColor(ledNo);
    r = (oldColor & 0x00ff0000UL) >> 16;
    g = (oldColor & 0x0000ff00UL) >> 8;
    b = (oldColor & 0x000000ffUL);

    r=(r<=10)? 0 : (int) r-(r*fadeValue/256);
    g=(g<=10)? 0 : (int) g-(g*fadeValue/256);
    b=(b<=10)? 0 : (int) b-(b*fadeValue/256);
    
    strip.setPixelColor(ledNo, r,g,b);
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[ledNo].fadeToBlackBy(fadeValue);
 #endif  
}

void meteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay)
{  
  setAll(0,0,0);
  
  for(int i = 0; i < NUM_LEDS+NUM_LEDS; i++)
  {  
    // fade brightness all LEDs one step
    for(int j=0; j<NUM_LEDS; j++)
    {
      if((!meteorRandomDecay) || (random(10) > 5))
      {
        fadeToBlack(j, meteorTrailDecay );        
      }
    }
    
    // draw meteor
    for(int j = 0; j < meteorSize; j++)
    {
      if((i - j < NUM_LEDS) && (i - j >= 0))
      {
        setPixel(i - j, red, green, blue);
      } 
    }
   
    showStrip();
    delay(SpeedDelay);
  }
}


void flashlight(byte red, byte green, byte blue)
{
  setAll(red, green, blue);
}