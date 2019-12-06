/*

   This is revision 1.1 by Jeffrey Fitzpatrick
   1.0 - Removes LED Functionality. 
   1.1 - Adds a second buzzer, this is a passive buzzer that will keep beeping throughout the menus.
         This is intended to movitate the user to close the menu so as to avoid the condition where
         an alarm is prevented due to the device being left in one of the menus (perhaps by accident).

         Minor changes include the renaming of variable so it'll be easier to comprehend.

         The buzzer assignments need not be specifically a passive or active, it just happens that 
         they were available for use.
   1.2 - Adds the ability to turn off the LCD in order to conserve battery life. The display will be
         turned on automatically an hour prior to alarm. As such, waking up earlier than the alarm
         and seeing the LCD on will give you a good indication to get moving.
*/


/*
  :Project:Allarm_Clock
  :Author: Tiziano Bianchettin
  :Date: 10/02/2017
  :Revision: 2
  :License: Public Domain
  thanks to:
  http://arduinoenonsolo.blogspot.it/2012/12/orologio-con-arduino-e-il-ds1307.html
  http://www.mauroalfieri.it/
  http://www.danielealberti.it/
  http://www.maffucci.it/
  My electronics laboratory professor "Perito Carli"
*/
//************libraries**************//
#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>

//************************************//
LiquidCrystal_I2C lcd(0x27, 20, 4); // Display  I2C 20 x 4
RTC_DS1307 RTC;

//************Button*****************//
int P1 = 6; // Button SET MENU'
int P2 = 7; // Button +
int P3 = 8; // Button -
int P4 = 9; // SWITCH Alarm
int P5 = 3; // LCD on/off

//**************Alarm***************//

#define buzzer 10
#define buzzer2 4

//************Variables**************//
int hourUpdate;
int minUpdate;
int yearupg;
int monthUpdate;
int dayUpdate;
int menu = 0;
int setAll = 0;

uint8_t alarmHours = 0, alarmMinutes = 0;  // Holds the current alarm time


void setup()
{

  lcd.begin();
  lcd.backlight();
  lcd.clear();

  pinMode(P1, INPUT_PULLUP); // https://www.arduino.cc/en/Tutorial/InputPullupSerial
  pinMode(P2, INPUT_PULLUP);
  pinMode(P3, INPUT_PULLUP);
  pinMode(P4, INPUT_PULLUP);
  pinMode(P5, INPUT_PULLUP);

  pinMode(buzzer, OUTPUT); // Set buzzer as an output
  pinMode(buzzer2, OUTPUT);
  printAllOff();
  Serial.begin(9600);
  Wire.begin();
  RTC.begin();

  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // Set the date and time at compile time
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  //RTC.adjust(DateTime(__DATE__, __TIME__)); //removing "//" to adjust the time
  // The default display shows the date and time
  int menu = 0;
}

void loop()
{

  // check if you press the SET button and increase the menu index
  if (digitalRead(P1) == LOW)
  {
    menu = menu + 1;
  }
  if ((digitalRead(P2) == LOW) && (digitalRead(P3) == LOW))
  {

    DisplaySetHourAll();
    DisplaySetMinuteAll();
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("ALARM");
    lcd.setCursor(5, 1);
    lcd.print(alarmHours, DEC);
    lcd.print(":");
    lcd.print(alarmMinutes, DEC);
    delay(1000);
    lcd.clear();
  }
  // in which subroutine should we go?
  if (menu == 0)
  {
    DisplayDateTime(); // Display  DateTime
    turnOff(); // 
    Alarm(); // Alarm control
  }
  if (menu == 1)
  {
    DisplaySetHour();
    buzz(900);
  }
  if (menu == 2)
  {
    DisplaySetMinute();
    buzz(800);
  }
  if (menu == 3)
  {
    DisplaySetYear();
    buzz(700);
  }
  if (menu == 4)
  {
    DisplaySetMonth();
    buzz(600);
  }
  if (menu == 5)
  {
    DisplaySetDay();
    buzz(500);
  }
  if (menu == 6)
  {
    buzz(400);
    StoreAgg();
    delay(500);
    menu = 0;
  }
  delay(100);
}

void DisplayDateTime ()
{
  // We show the current date and time
  DateTime now = RTC.now();

  lcd.setCursor(0, 2);
  lcd.print("Hour : ");

  if (now.hour() <= 9)
  {
    lcd.print("0");
  }
  lcd.print(now.hour(), DEC);
  hourUpdate = now.hour();
  lcd.print(":");
  if (now.minute() <= 9)
  {
    lcd.print("0");
  }
  lcd.print(now.minute(), DEC);
  minUpdate = now.minute();
  lcd.print(":");
  if (now.second() <= 9)
  {
    lcd.print("0");
  }
  lcd.print(now.second(), DEC);


  lcd.setCursor(0, 1);
  lcd.print("Date : ");
  if (now.day() <= 9)
  {
    lcd.print("0");
  }
  lcd.print(now.day(), DEC);
  dayUpdate = now.day();
  lcd.print("/");
  if (now.month() <= 9)
  {
    lcd.print("0");
  }
  lcd.print(now.month(), DEC);
  monthUpdate = now.month();
  lcd.print("/");
  lcd.print(now.year(), DEC);
  yearupg = now.year();

  char DOW[][10] = {"Sunday   ", "Monday   ", "Tuesday  ", "Wednesday", "Thursday ", "Friday   ", "Saturday "};
  lcd.setCursor(0, 0);
  lcd.print("Day  : ");
  lcd.print(DOW[now.dayOfTheWeek()]); // if it appears error in the code, enter the code given below
  //lcd.print(DOW[now.dayOfWeek()]);
}

void DisplaySetHour()
{
  // time setting
  lcd.clear();
  DateTime now = RTC.now();
  if (digitalRead(P2) == LOW)
  {
    if (hourUpdate == 23)
    {
      hourUpdate = 0;
    }
    else
    {
      hourUpdate = hourUpdate + 1;
    }
  }
  if (digitalRead(P3) == LOW)
  {
    if (hourUpdate == 0)
    {
      hourUpdate = 23;
    }
    else
    {
      hourUpdate = hourUpdate - 1;
    }
  }
  lcd.setCursor(0, 0);
  lcd.print("Set time:");
  lcd.setCursor(0, 1);
  lcd.print(hourUpdate, DEC);
  delay(200);
}

void DisplaySetMinute()
{
  // Setting the minutes
  lcd.clear();
  if (digitalRead(P2) == LOW)
  {
    if (minUpdate == 59)
    {
      minUpdate = 0;
    }
    else
    {
      minUpdate = minUpdate + 1;
    }
  }
  if (digitalRead(P3) == LOW)
  {
    if (minUpdate == 0)
    {
      minUpdate = 59;
    }
    else
    {
      minUpdate = minUpdate - 1;
    }
  }
  lcd.setCursor(0, 0);
  lcd.print("Set Minutes:");
  lcd.setCursor(0, 1);
  lcd.print(minUpdate, DEC);
  delay(200);
}

void DisplaySetYear()
{
  // setting the year
  lcd.clear();
  if (digitalRead(P2) == LOW)
  {
    yearupg = yearupg + 1;
  }
  if (digitalRead(P3) == LOW)
  {
    yearupg = yearupg - 1;
  }
  lcd.setCursor(0, 0);
  lcd.print("Set Year:");
  lcd.setCursor(0, 1);
  lcd.print(yearupg, DEC);
  delay(200);
}

void DisplaySetMonth()
{
  // Setting the month
  lcd.clear();
  if (digitalRead(P2) == LOW)
  {
    if (monthUpdate == 12)
    {
      monthUpdate = 1;
    }
    else
    {
      monthUpdate = monthUpdate + 1;
    }
  }
  if (digitalRead(P3) == LOW)
  {
    if (monthUpdate == 1)
    {
      monthUpdate = 12;
    }
    else
    {
      monthUpdate = monthUpdate - 1;
    }
  }
  lcd.setCursor(0, 0);
  lcd.print("Set Month:");
  lcd.setCursor(0, 1);
  lcd.print(monthUpdate, DEC);
  delay(200);
}

void DisplaySetDay()
{
  // Setting the day
  lcd.clear();
  if (digitalRead(P2) == LOW)
  {
    if (dayUpdate == 31)
    {
      dayUpdate = 1;
    }
    else
    {
      dayUpdate = dayUpdate + 1;
    }
  }
  if (digitalRead(P3) == LOW)
  {
    if (dayUpdate == 1)
    {
      dayUpdate = 31;
    }
    else
    {
      dayUpdate = dayUpdate - 1;
    }
  }
  lcd.setCursor(0, 0);
  lcd.print("Set Day:");
  lcd.setCursor(0, 1);
  lcd.print(dayUpdate, DEC);
  delay(200);
}

void StoreAgg()
{
  // Variable saving
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SAVING IN");
  lcd.setCursor(0, 1);
  lcd.print("PROGRESS");
  RTC.adjust(DateTime(yearupg, monthUpdate, dayUpdate, hourUpdate, minUpdate, 0));
  delay(200);
}
void DisplaySetHourAll()// Setting the alarm minutes
{
  while (digitalRead(P1) == HIGH) {

    lcd.clear();

    if (digitalRead(P2) == LOW)
    {
      if (alarmHours == 23)
      {
        alarmHours = 0;
      }
      else
      {
        alarmHours = alarmHours + 1;
      }
    }
    if (digitalRead(P3) == LOW)
    {
      if (alarmHours == 0)
      {
        alarmHours = 23;
      }
      else
      {
        alarmHours = alarmHours - 1;
      }
    }
    lcd.setCursor(0, 0);
    lcd.print("Set HOUR Alarm:");
    lcd.setCursor(0, 1);
    lcd.print(alarmHours, DEC);
    delay(200);
  }
  delay(200);
}

void DisplaySetMinuteAll()// Setting the alarm minutes
{
  while (digitalRead(P1) == HIGH) {

    lcd.clear();
    if (digitalRead(P2) == LOW)
    {
      if (alarmMinutes == 59)
      {
        alarmMinutes = 0;
      }
      else
      {
        alarmMinutes = alarmMinutes + 1;
      }
    }
    if (digitalRead(P3) == LOW)
    {
      if (alarmMinutes == 0)
      {
        alarmMinutes = 59;
      }
      else
      {
        alarmMinutes = alarmMinutes - 1;
      }
    }
    lcd.setCursor(0, 0);
    lcd.print("Set MIN. Alarm:");
    lcd.setCursor(0, 1);
    lcd.print(alarmMinutes, DEC);
    delay(200);
  }
  delay(200);
}
void printAllOn() {
  lcd.setCursor(0, 3);
  lcd.print("Alarm: ");



  if (alarmHours <= 9)
  {
    lcd.print("0");
  }
  lcd.print(alarmHours, DEC);

  lcd.print(":");
  if (alarmMinutes <= 9)
  {
    lcd.print("0");
  }
  lcd.print(alarmMinutes, DEC);

}
void printAllOff() {
  lcd.setCursor(0, 3);
  lcd.print("Alarm: Off  ");
}
void Alarm() {
  if (digitalRead(P4) == LOW)
  {
    setAll = setAll + 1;
  }
  if (setAll == 0)
  {
    printAllOff();
    noTone (buzzer);
    ;
  }
  if (setAll == 1)
  {

    printAllOn();

    DateTime now = RTC.now();
    if ( now.hour() == alarmHours && now.minute() == alarmMinutes )
    {
      lcd.noBacklight();
      DateTime now = RTC.now();

      tone(buzzer, 880); //play the note "A5" (LA5)
      delay (300);
      tone(buzzer, 698); //play the note "F6" (FA5)
      lcd.backlight();
    }
    else {
      noTone (buzzer);

    }

  }
  if (setAll == 2)
  {
    setAll = 0;
  }
  delay(200);
}


/*
   Added Code functions below.
*/

void buzz(int freq)
{
  tone(buzzer2, freq);
  delay(200);
  noTone(buzzer2);
}


void turnOff()
{
  while(
  lcd.backlight();
}
