#include "captest2.h"
#include "arduino.h"

//Function to carry out Test2
void captest2(void)
{
  const float CapOne = 24.59; // USER CALIBRATE
  const float Res_Pullup = 34.9;
  const int MaxADC_Value = 1023;
  
  //lcd.PCF8574_LCDClearScreen();
  //lcd.PCF8574_LCDGOTO(lcd.LCDLineNumberOne, 0);

  pinMode(Cap2InPin, INPUT);
  digitalWrite(Cap2OutPin, HIGH);
  int val = analogRead(Cap2InPin);
  digitalWrite(Cap2OutPin, LOW);

  if (val < 1000)
  {
    pinMode(Cap2InPin, OUTPUT);
    // Cu = VA2 * C1 / (VA3 - VA2)
    float capacitance = (float)val * CapOne / (float)(MaxADC_Value - val);
    Serial.print(F("Capacitance Value = "));
    Serial.print(capacitance, 3);
    Serial.print(F(" pF ADC = ("));
    Serial.print(val);
    Serial.println(F(") "));
    //lcd.print(capacitance, 3);
    //lcd.print(" pF");
    //lcd.PCF8574_LCDGOTO(lcd.LCDLineNumberTwo, 0);
    //lcd.print("ADC=");
    //lcd.print(val);
  }
  else
  {
    pinMode(Cap2InPin, OUTPUT);
    delay(1);
    pinMode(Cap2OutPin, INPUT_PULLUP);
    unsigned long u1 = micros();
    unsigned long t;
    int digVal;
    do
    {
      digVal = digitalRead(Cap2OutPin);
      unsigned long u2 = micros();
      // condition ? result_if_true : result_if_false
      t = u2 > u1 ? u2 - u1 : u1 - u2;
    } while ((digVal < 1) && (t < 400000L));

    pinMode(Cap2OutPin, INPUT);
    val = analogRead(Cap2OutPin);

    //discharge
    digitalWrite(Cap2InPin, HIGH);
    int dischargeTime = (int)(t / 1000L) * 5;
    delay(dischargeTime);
    pinMode(Cap2OutPin, OUTPUT);
    digitalWrite(Cap2OutPin, LOW);
    digitalWrite(Cap2InPin, LOW);

    float capacitance = -(float)t / Res_Pullup
                        / log(1.0 - (float)val / (float)MaxADC_Value);
    Serial.print(F("Capacitance Value = "));
    if (capacitance > 1000.0)
    {
      Serial.print(capacitance / 1000.0, 2);
      Serial.print(F(" uF"));
      //lcd.print(capacitance / 1000.0, 2);
      //lcd.print(" uF");
    }
    else
    {
      Serial.print(capacitance, 2);
      Serial.print(F(" nF"));
      //lcd.print(capacitance, 2);
      //lcd.print(" nF");
    }

    Serial.print(F(" ("));
    // condition ? result_if_true : result_if_false
    Serial.print(digVal == 1 ? F("Normal") : F("HighVal"));
    Serial.print(F(", t= "));
    Serial.print(t);
    Serial.print(F(" us, ADC= "));
    Serial.print(val);
    Serial.println(F(")"));
    //lcd.print(" A=");
    //lcd.print(val);
    //lcd.PCF8574_LCDGOTO(lcd.LCDLineNumberTwo, 0);
    //lcd.print("t=");
    //lcd.print(t);
    //lcd.print("uS");
  }
  while (millis() % 1000 != 0);
  delay(4000);
}