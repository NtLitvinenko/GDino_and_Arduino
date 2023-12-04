// y 8, x 5 (custom symbol)

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // Устанавливаем дисплей
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

  lcd.backlight(); // Turn on the display backlight

  pinMode(2, INPUT); // Assuming pin 2 is for white button
  pinMode(3, INPUT); // Assuming pin 3 is for blue button
  pinMode(4, INPUT); // Assuming pin 4 is for red button
}

void gOver() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Game over!");
  while (true);
}
short cactus_pos=19;
short fuss = 0;
void loop() {
  static short costume_std = 0;
  static short costume_lay = 0;

  bool isWhite = digitalRead(2);
  bool isBlue = digitalRead(3);
  bool isTStopped = digitalRead(4);

  if (!isTStopped) {
    // Clear the display
    lcd.clear();

    // Update bird and cactus positions
    bird_pos--;
    cactus_pos--;

    // Check collision between bird and player
    if (bird_pos == 0) {
      if ((isWhite && bird_y == 1) || (isBlue && bird_y == 0) || (!isWhite && bird_y == 0)) {
        gOver();
      } else {
        // Respawn bird at new coordinates after a level ends
        bird_pos = 16;
        bird_y = random(0, 2);
      }
    }

    // Check collision between player and cactus
    if ((cactus_pos == 0) && (!isBlue)) {
      gOver();
    }
    else {
      // Respawn cactus at new coordinates after passing
      if (cactus_pos < 0) {
      
        cactus_pos = random(bird_pos+4, bird_pos+12);
      }
    }

    // Draw bird and cactus on the LCD
    lcd.setCursor(bird_pos, bird_y);
    lcd.write(4);

    lcd.setCursor(cactus_pos, 1);
    lcd.write(5);
    if (!isBlue) {fuss = 0; }
    // Player actions (jump or lay)
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
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Pause");
    delay(500);
  }
}
