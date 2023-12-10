// y 8, x 5 (custom symbol)

#include <Wire.h> 
//#include "notes.h"

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // Устанавливаем дисплей

bool rbos = false;
byte std1[] = {
  B11111,
  B10001,
  B10001,
  B10001,
  B11111,
  B10010,
  B10010,
  B11011
};
byte std2[] = {
  B11111,
  B10001,
  B10001,
  B10001,
  B11111,
  B10010,
  B11011,
  B00000
};
byte lay1[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B10010,
  B11011
};
byte lay2[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B00000,
  B00000
};
byte bird[] = {
  B11111,
  B01110,
  B11111,
  B10001,
  B00000,
  B00000,
  B00000,
  B00000
};
byte cactus[] = {
  B00001,
  B00001,
  B00101,
  B00101,
  B10101,
  B11110,
  B00100,
  B00100
};

const short bPin = 11;
short bird_pos = 16;
short bird_y = random(0, 2);

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.createChar(0, std1);
  lcd.createChar(1, std2);
  lcd.createChar(2, lay1);
  lcd.createChar(3, lay2);
  lcd.createChar(4, bird);
  lcd.createChar(5, cactus);

  lcd.backlight();

  pinMode(2, INPUT); // 
  pinMode(3, INPUT); // 
  pinMode(4, INPUT); // 
  tone(bPin, 9600, 1000);
  tone(bPin, 96000, 1000);
  tone(bPin, 9600, 1000);
  tone(bPin, 96000, 1000);
}

void gOver() {
  tone(bPin, 900, 3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Game over!");
  while (true);
}
short cactus_pos=19;
short fuss = 0;
bool gamePaused = false;

void loop() {
  static short costume_std = 0;
  static short costume_lay = 0;

  bool isWhite = digitalRead(2);
  bool isBlue = digitalRead(3);
  bool isPauseButtonPressed = digitalRead(4);

  if (isPauseButtonPressed) {
    delay(300);
    lcd.setCursor(0, 0);
    lcd.print("Pause");
    
    while (!digitalRead(4)) {
    }
    gamePaused = !gamePaused; 
    delay(200); 
  }
  else {

    lcd.clear();
    bird_pos--;
    cactus_pos--;
    if (bird_pos == 0) {
      if ((isWhite && bird_y == 1) || (isBlue && bird_y == 0) || (!isWhite && bird_y == 0) || (!isWhite && !isBlue && bird_y==1)) {
        gOver();
      } else {
        bird_pos = 16;
        bird_y = random(0, 2);
      }
    }

    if ((cactus_pos == 0) && (!isBlue)) {
      gOver();
    }
    else {
      if (cactus_pos < 0) {
      
        cactus_pos = random(bird_pos+4, bird_pos+12);
      }
    }

    lcd.setCursor(bird_pos, bird_y);
    lcd.write(4);

    lcd.setCursor(cactus_pos, 1);
    lcd.write(5);
    if (!isBlue) {fuss = 0; }
    if (isWhite) {
      lcd.setCursor(0, 1);
      if (costume_lay == 1) {
        lcd.write(2);
        costume_lay = 0;
      } else {
        lcd.write(3);
        costume_lay = 1;
      }
    } else if (isBlue) {
      if (fuss >= 5) { gOver(); }
      else { fuss += 1; }
      lcd.setCursor(0, 0);
      if (costume_std == 1) {
        lcd.write(0);
        costume_std = 0;
      } else {
        lcd.write(1);
        costume_std = 1;
      }
    } else {
      lcd.setCursor(0, 1);
      if (costume_std == 1) {
        lcd.write(0);
        costume_std = 0;
      } else {
        lcd.write(1);
        costume_std = 1;
      }
    }
    delay(250);
  }
}
