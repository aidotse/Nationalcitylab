#include <FastLED.h>

#define PIXEL_TYPE WS2812B
#define COLOR_ORDER GRB
#define BRIGHTNESS 255 //(0-255)
#define VOLTS 5
#define MAX_AMPS 2000

/*

Gamla

#define TransportButton A8
#define IndustryButton A9
#define GridButton A10
#define CommersialButton A11
#define RecidentialButton A12
#define PublicButton A13
#define GridStorageChargeButton A0
#define GridStorageSupplyButton A1

/*define TransportButtonLight 0
  #define IndustryButtonLight 1
  #define GridButtonLight 2
  #define CommersialButtonLight 3
  #define RecidentialButtonLight 4
  #define PublicButtonLight 5
  #define GridStorageChargeButtonLight 6
  #define GridStorageSupplyButtonLight 7*/

#define TransportButton A13
#define IndustryButton A12
#define GridButton A11
#define CommersialButton A10
#define RecidentialButton A9
#define PublicButton A8
#define GridStorageChargeButton A7
#define GridStorageSupplyButton A6


#define TransportIndicatorStrip 7
#define IndustryIndicatorStrip 2
#define GridIndicatorStrip 3
#define CommersialIndicatorStrip 4
#define RecidentialIndicatorStrip 5
#define PublicIndicatorStrip 6
#define GridStorageIndicatorStrip 8
#define PowerPlantStrip 9


#define NumLeds 15
#define DayRed CRGB(255, 0, 0)
#define DayGreen CRGB(0, 255, 0)
#define DayBlue CRGB(0, 0, 128)
#define NightRed CRGB(128, 0, 2)
#define NightGreen CRGB(0, 128, 2)
#define NightBlue CRGB(0, 0, 64)
#define Black CRGB(0, 0, 0)

CRGB TransportLEDs[NumLeds];
CRGB IndustryLEDs[NumLeds];
CRGB GridLEDs[NumLeds];
CRGB CommersialLEDs[NumLeds];
CRGB RecidentialLEDs[NumLeds];
CRGB PublicLEDs[NumLeds];
CRGB GridStorageLEDs[NumLeds];
CRGB PowerPlantLEDs[NumLeds];

//Time of day                  0    1    2    3    4    5    6    7    8    9    10   11   12   13   14   15   16   17   18   19   20   21   22   23
byte TransportDemand[48]   = { 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 5, 5, 6, 7, 8, 9, 8, 7, 6, 5, 5, 6, 7, 7, 7, 6, 5, 6, 7, 8, 9, 9, 8, 7, 7, 6, 6, 5, 5, 5, 5, 5, 5, 4, 4, 3, 3};
byte IndustryDemand[48]    = { 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5, 6, 7, 8, 9, 9, 9, 9, 9, 9, 8, 7, 8, 9, 9, 9, 8, 7, 8, 9, 9, 8, 7, 6, 5, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3};
byte GridDemand[48]        = { 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 5, 5, 6, 7, 8, 9, 8, 7, 6, 5, 5, 6, 7, 7, 7, 6, 5, 6, 7, 8, 9, 9, 8, 7, 7, 6, 6, 5, 5, 5, 5, 5, 5, 5, 4, 4, 3};
byte CommersialDemand[48]  = { 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 6, 7, 7, 8, 9, 9, 9, 9, 8, 7, 6, 5, 5, 6, 7, 8, 9, 9, 8, 7, 6, 5, 5, 4, 4, 4, 4, 4, 3, 4, 2};
byte RecidentialDemand[48] = { 2, 2, 2, 3, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 6, 7, 8, 6, 5, 4, 4, 4, 4, 5, 6, 6, 5, 5, 5, 4, 5, 6, 6, 7, 8, 9, 9, 9, 9, 9, 8, 8, 7, 6, 5, 4, 4, 3};
byte PublicDemand[48]      = { 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 6, 6, 7, 8, 9, 9, 9, 9, 9, 9, 8, 7, 8, 9, 9, 9, 6, 7, 8, 9, 9, 8, 7, 6, 6, 6, 6, 6, 4, 4, 4, 4, 4, 4, 3, 3};
byte Supply[48];
//{16, 16, 16, 17, 20, 20, 21, 21, 22, 25, 25, 28, 30, 33, 38, 39, 49, 46, 44, 42, 40, 41, 51, 42, 45, 46, 42, 39, 36, 37, 43, 49, 50, 48, 45, 43, 39, 37, 33, 33, 30, 30, 29, 28, 26, 23, 21, 17};
int SumDemand;
int SumSupply;
byte BurningCoal;
byte TransportWaste = 0;
byte IndustryWaste = 0;
byte GridWaste = 0;
byte CommersialWaste = 0;
byte RecidentialWaste = 0;
byte PublicWaste = 0;
boolean PowerPlantActive = false;
int PowerPlantActiveTimer = 0;
int TimeSinceLastPowerPlant = 0;
int GridStorageChargeLevel = 500;
int PowerPlantDim = 0;


byte counter = 0;

int Delay = 20;

CRGB CheckColor(char color[], byte TimeOfDay)
{
  if (TimeOfDay > 14 && TimeOfDay < 38)
  {
    if (color == "Green") return DayGreen;
    else if (color == "Blue") return DayBlue;
    else return DayRed;
  }
  else if (TimeOfDay == 14 || TimeOfDay == 38)
  {
    if (color == "Green") return NightGreen / 2 + DayGreen / 2;
    else if (color == "Blue") return NightBlue / 2 + DayBlue / 2;
    else return NightRed / 2 + DayRed / 2 ;
  }
  else
  {
    if (color == "Green") return NightGreen;
    else if (color == "Blue") return NightBlue;
    else return NightRed;
  }
}

void setup() {
  for (int i = 0; i < 48; i++)
  {
    float SinValue = sin(3.1415926 * (i - 5) / 48);
    Supply[i] = 35 + round(15 * SinValue * SinValue);
  }
  pinMode(TransportButton,         INPUT_PULLUP);
  pinMode(IndustryButton,          INPUT_PULLUP);
  pinMode(GridButton,              INPUT_PULLUP);
  pinMode(CommersialButton,        INPUT_PULLUP);
  pinMode(RecidentialButton,       INPUT_PULLUP);
  pinMode(PublicButton,            INPUT_PULLUP);
  pinMode(GridStorageChargeButton, INPUT_PULLUP);
  pinMode(GridStorageSupplyButton, INPUT_PULLUP);

  Serial.begin(9600);

  FastLED.addLeds<PIXEL_TYPE, TransportIndicatorStrip,   COLOR_ORDER>(TransportLEDs, NumLeds);
  FastLED.addLeds<PIXEL_TYPE, IndustryIndicatorStrip,    COLOR_ORDER>(IndustryLEDs, NumLeds);
  FastLED.addLeds<PIXEL_TYPE, GridIndicatorStrip,        COLOR_ORDER>(GridLEDs, NumLeds);
  FastLED.addLeds<PIXEL_TYPE, CommersialIndicatorStrip,  COLOR_ORDER>(CommersialLEDs, NumLeds);
  FastLED.addLeds<PIXEL_TYPE, RecidentialIndicatorStrip, COLOR_ORDER>(RecidentialLEDs, NumLeds);
  FastLED.addLeds<PIXEL_TYPE, PublicIndicatorStrip,      COLOR_ORDER>(PublicLEDs, NumLeds);
  FastLED.addLeds<PIXEL_TYPE, GridStorageIndicatorStrip, COLOR_ORDER>(GridStorageLEDs, NumLeds);
  FastLED.addLeds<PIXEL_TYPE, PowerPlantStrip,           COLOR_ORDER>(PowerPlantLEDs, NumLeds);

  FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS, MAX_AMPS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();

  FastLED.show();

}

void loop() {
  for (int TimeOfDay = 0; TimeOfDay < 48; TimeOfDay++)
  {
    boolean ChargeStorage = false;
    boolean SupplyFromStorage = false;
    int GridStorageEffect = 0;

    if (random(60) < TransportDemand[TimeOfDay]   && TransportWaste < 3)   TransportWaste ++;
    if (random(60) < IndustryDemand[TimeOfDay]    && IndustryWaste < 3)    IndustryWaste ++;
    if (random(60) < GridDemand[TimeOfDay]        && GridWaste < 3)        GridWaste ++;
    if (random(60) < CommersialDemand[TimeOfDay]  && CommersialWaste < 3)  CommersialWaste ++;
    if (random(60) < RecidentialDemand[TimeOfDay] && RecidentialWaste < 3) RecidentialWaste ++;
    if (random(60) < PublicDemand[TimeOfDay]      && PublicWaste < 3)      PublicWaste ++;

    for (int counter = 0; counter < 10; counter++)         //counter är räknaren som delar upp halvsekunden i tio delar
    {

      delay(50);
      if (digitalRead(TransportButton)          == LOW) TransportWaste = 0;
      if (digitalRead(IndustryButton)           == LOW) IndustryWaste = 0;
      if (digitalRead(GridButton)               == LOW) GridWaste = 0;
      if (digitalRead(CommersialButton)         == LOW) CommersialWaste = 0;
      if (digitalRead(RecidentialButton)        == LOW) RecidentialWaste = 0;
      if (digitalRead(PublicButton)             == LOW) PublicWaste = 0;
      if (digitalRead(GridStorageChargeButton)  == LOW) ChargeStorage = true;
      if (digitalRead(GridStorageSupplyButton)  == LOW) SupplyFromStorage = true;

      SumDemand = TransportDemand[TimeOfDay]    + PublicWaste;
      SumDemand += IndustryDemand[TimeOfDay]    + IndustryWaste;
      SumDemand += GridDemand[TimeOfDay]        + GridWaste;
      SumDemand += CommersialDemand[TimeOfDay]  + CommersialWaste;
      SumDemand += RecidentialDemand[TimeOfDay] + RecidentialWaste;
      SumDemand += PublicDemand[TimeOfDay]      + PublicWaste;

      for (int i = 0; i < GridStorageChargeLevel / 100 ; i++) GridStorageLEDs[i] = CheckColor("Blue", TimeOfDay);
      for (int i = GridStorageChargeLevel / 100; i < NumLeds; i++) GridStorageLEDs[i] = Black;

      if (ChargeStorage && GridStorageChargeLevel < 1500)
      {
        GridStorageLEDs[round(GridStorageChargeLevel / 1000.0 * (counter + 1))] *= 4;
        GridStorageEffect = max(5, Supply[TimeOfDay] - SumDemand );
        if (counter == 9) GridStorageChargeLevel += GridStorageEffect;
      }
      else if (SupplyFromStorage && GridStorageChargeLevel > 0)
      {
        GridStorageLEDs[round(GridStorageChargeLevel / 1000.0 * (10 - counter))] *= 4;
        GridStorageEffect = min(-5, Supply[TimeOfDay] - SumDemand);
        if (counter == 9) GridStorageChargeLevel += GridStorageEffect;
      }

      SumSupply = Supply[TimeOfDay] - GridStorageEffect;

      if (SumSupply < SumDemand)
      {
        PowerPlantActive = true;
        BurningCoal = 50;
      }
      else
      {
        PowerPlantActive = false;
        BurningCoal = 0;
        PowerPlantActiveTimer = 0;
      }

      if (PowerPlantActive && PowerPlantDim < 250) PowerPlantDim += 25;
      if (!PowerPlantActive && PowerPlantDim > 0) PowerPlantDim -= 25;

      for (int i = 0; i < 10; i++)
      {
        PowerPlantLEDs[i] = CheckColor("Red", TimeOfDay) / 4;
        PowerPlantLEDs[i] %= PowerPlantDim;
      }
      PowerPlantLEDs[counter] *= 4;



      FastLED.show();
    }
    if (PowerPlantActive && PowerPlantActiveTimer < 2) PowerPlantActiveTimer ++;
    if (PowerPlantActiveTimer > 1) TimeSinceLastPowerPlant = 0;
    else TimeSinceLastPowerPlant ++;
    if (!PowerPlantActive) PowerPlantActiveTimer = 0;
    
    for (int i = 0; i < TransportDemand[TimeOfDay]; i++) TransportLEDs[i] = CheckColor("Green", TimeOfDay);
    for (int i = TransportDemand[TimeOfDay]; i < TransportDemand[TimeOfDay] + TransportWaste; i++) TransportLEDs[i] = CheckColor("Red", TimeOfDay);
    for (int i = TransportDemand[TimeOfDay] + TransportWaste; i < NumLeds; i++) TransportLEDs[i] = Black;

    for (int i = 0; i < IndustryDemand[TimeOfDay]; i++) IndustryLEDs[i] = CheckColor("Green", TimeOfDay);
    for (int i = IndustryDemand[TimeOfDay]; i < IndustryDemand[TimeOfDay] + IndustryWaste; i++) IndustryLEDs[i] = CheckColor("Red", TimeOfDay);
    for (int i = IndustryDemand[TimeOfDay] + IndustryWaste; i < NumLeds; i++) IndustryLEDs[i] = Black;

    for (int i = 0; i < GridDemand[TimeOfDay]; i++) GridLEDs[i] = CheckColor("Green", TimeOfDay);
    for (int i = GridDemand[TimeOfDay]; i < GridDemand[TimeOfDay] + GridWaste; i++) GridLEDs[i] = CheckColor("Red", TimeOfDay);
    for (int i = GridDemand[TimeOfDay] + GridWaste; i < NumLeds; i++) GridLEDs[i] = Black;

    for (int i = 0; i < CommersialDemand[TimeOfDay]; i++) CommersialLEDs[i] = CheckColor("Green", TimeOfDay);
    for (int i = CommersialDemand[TimeOfDay]; i < CommersialDemand[TimeOfDay] + CommersialWaste; i++) CommersialLEDs[i] = CheckColor("Red", TimeOfDay);
    for (int i = CommersialDemand[TimeOfDay] + CommersialWaste; i < NumLeds; i++) CommersialLEDs[i] = Black;

    for (int i = 0; i < RecidentialDemand[TimeOfDay]; i++) RecidentialLEDs[i] = CheckColor("Green", TimeOfDay);
    for (int i = RecidentialDemand[TimeOfDay]; i < RecidentialDemand[TimeOfDay] + RecidentialWaste; i++) RecidentialLEDs[i] = CheckColor("Red", TimeOfDay);
    for (int i = RecidentialDemand[TimeOfDay] + RecidentialWaste; i < NumLeds; i++) RecidentialLEDs[i] = Black;

    for (int i = 0; i < PublicDemand[TimeOfDay]; i++) PublicLEDs[i] = CheckColor("Green", TimeOfDay);
    for (int i = PublicDemand[TimeOfDay]; i < PublicDemand[TimeOfDay] + PublicWaste; i++) PublicLEDs[i] = CheckColor("Red", TimeOfDay);
    for (int i = PublicDemand[TimeOfDay] + PublicWaste; i < NumLeds; i++) PublicLEDs[i] = Black;

    Serial.print(100);
    Serial.print(" ");
    Serial.print(TimeSinceLastPowerPlant);
    Serial.print(" ");
    Serial.print(SumSupply);
    Serial.print(" ");
    Serial.print(SumDemand);
    Serial.print(" ");
    Serial.print(BurningCoal);
    Serial.println(" ");

    FastLED.show();
  }
}
