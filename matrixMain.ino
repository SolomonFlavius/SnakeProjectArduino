#include <LiquidCrystal.h>
#include <LedControl.h>
#include <EEPROM.h>

bool inGame = false;
bool inMenu = true;
bool inSettings = false;
byte settingsOption = 0;  //name,difficulty, brightness lcd, brightness matrix, sound, exit
byte settingsOptionsMax = 5;
bool inDifficulty = false;
bool inName = false;
bool inExit = false;
bool inHighscore = false;
byte highscorePos = 1;
bool inAbout = false;
bool inHowToPlay = false;
bool inGameOver = false;
byte menuOption = 0;  //0-start 1-highscore 2-settings 3-about 4-how to play
byte menuOptionsMax = 4;
byte highscorePosMax = 5;
bool inSound = false;
bool inBLCD = false;
bool inBMatrix = false;
short score = 0;
short difficulty = 1;
short minDifficulty = 1;
short maxDifficulty = 3;
short direction = 0;  //0 left 1 up 2 right 3 down
short snakeRow[64];
short snakeColumn[64];
short snakeLength = 1;
int speed = 200;
int timeLeft;
char const alphabet[] = { ' ', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
short alphabetLength = 26;
short name[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
short letterPos = 0;
const byte buzzerPin = 5;

//Joystick Controller
//Joystick pins
const int xPin = A0;
const int yPin = A1;
const int pinSw = 2;

const int minThreshold = 200;
const int maxThreshold = 600;
int xValue = 0;
int yValue = 0;
bool swState = HIGH;

//debounce for joystick
long lastTime = 0;
long lastJoyTime = 0;
int interval = 50;
byte reading = LOW;
byte joyState = LOW;
byte lastReading = LOW;
byte joyReading = LOW;
byte lastJoyReading = HIGH;
bool joyMoved = false;

//matrix controller
const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 10;

LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);  // DIN, CLK, LOAD, No. DRIVER

byte xPos = 0;
byte yPos = 0;
byte xLastPos = 0;
byte yLastPos = 0;


const byte matrixSize = 8;
bool matrixChanged = true;

byte matrix[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};

//LCD pins
const byte RS = 9;
const byte enable = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = A5;
const byte d7 = 4;
const byte brightnessPin = 3;

LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);


byte lcdCols = 16;
byte lcdRows = 2;
int maxBrightnessValue = 5;
int minBrightnessValue = 1;
int maxBrightnessValueRaw = 255;
int minBrightnessValueRaw = 0;

byte arrowUp[8] = {
  0b00000,
  0b00000,
  0b00100,
  0b01010,
  0b10001,
  0b00000,
  0b00000,
  0b00000
};

byte arrowDown[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b10001,
  0b01010,
  0b00100,
  0b00000,
  0b00000
};

byte arrowRight[8] = {
  B00000,
  B00100,
  B00010,
  B11111,
  B00010,
  B00100,
  B00000,
  B00000
};

// arrow left
byte arrowLeft[8] = {
  B00000,
  B00100,
  B01000,
  B11111,
  B01000,
  B00100,
  B00000,
  B00000
};
//full square
byte squareCharacter[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

byte aboutSlide = 0;

// in Eprom
byte lcdBrightness;
byte matrixBrightness;
bool soundOn;
char hName1[16];
char hName2[16];
char hName3[16];
char hName4[16];
char hName5[16];
short hScore1;
short hScore2;
short hScore3;
short hScore4;
short hScore5;
short difficulty1;
short difficulty2;
short difficulty3;
short difficulty4;
short difficulty5;

long int startedFlash = 0;
short intervalBlink = 500;
bool isPrinted = false;
String charToPrint = "_";

bool firstPlace = false;
bool secondPlace = false;
bool thirdPlace = false;
bool fourthPlace = false;
bool fifthPlace = false;
bool inGameOver2 = false;

long lastMoved;
int moveInterval = 100;
int rx = 3;
int ry = 3;
const short hardTime = 8;
const short mediumTime = 10;
const short easyTime = 12;
long lastBlink = 0;
int blinkTime = 200;
bool isShown = true;
long lastLCDTime = 0;
int oneSecond = 1000;

long lastTimeMove = 0;

void ReadFromEEPROM() {
  lcdBrightness = EEPROM.read(0);
  matrixBrightness = EEPROM.read(1);
  soundOn = EEPROM.read(2);
  for (int i = 0; i < 16; i++) {
    hName1[i] = EEPROM.read(3 + i);
    hName2[i] = EEPROM.read(19 + i);
    hName3[i] = EEPROM.read(35 + i);
    hName4[i] = EEPROM.read(51 + i);
    hName5[i] = EEPROM.read(67 + i);
  }
  hScore1 = EEPROM.read(83);
  hScore2 = EEPROM.read(85);
  hScore3 = EEPROM.read(87);
  hScore4 = EEPROM.read(89);
  hScore5 = EEPROM.read(91);
  difficulty1 = EEPROM.read(93);
  difficulty2 = EEPROM.read(95);
  difficulty3 = EEPROM.read(97);
  difficulty4 = EEPROM.read(99);
  difficulty5 = EEPROM.read(101);
}

void UpdateHighscores() {
  for (int i = 0; i < 16; i++) {
    EEPROM.update(3 + i, hName1[i]);
    EEPROM.update(19 + i, hName2[i]);
    EEPROM.update(35 + i, hName3[i]);
    EEPROM.update(51 + i, hName4[i]);
    EEPROM.update(67 + i, hName5[i]);
  }
  EEPROM.update(83, hScore1);
  EEPROM.update(85, hScore2);
  EEPROM.update(87, hScore3);
  EEPROM.update(89, hScore4);
  EEPROM.update(91, hScore5);
  EEPROM.update(93, difficulty1);
  EEPROM.update(95, difficulty2);
  EEPROM.update(97, difficulty3);
  EEPROM.update(99, difficulty4);
  EEPROM.update(101, difficulty5);
}



void CheckHighscore() {
  if (score > hScore1 || (score == hScore1 && difficulty > difficulty1)) {
    for (int i = 0; i < 16; i++) {
      hName5[i] = hName4[i];
    }
    difficulty5 = difficulty4;
    hScore5 = hScore4;
    for (int i = 0; i < 16; i++) {
      hName4[i] = hName3[i];
    }
    difficulty4 = difficulty3;
    hScore4 = hScore3;
    for (int i = 0; i < 16; i++) {
      hName3[i] = hName2[i];
    }
    difficulty3 = difficulty2;
    hScore3 = hScore2;
    for (int i = 0; i < 16; i++) {
      hName2[i] = hName1[i];
    }
    difficulty2 = difficulty1;
    hScore2 = hScore1;
    for (int i = 0; i < 16; i++) {
      hName1[i] = alphabet[name[i]];
    }
    difficulty1 = difficulty;
    hScore1 = score;
    firstPlace = true;
    UpdateHighscores();
  } else {
    if (score > hScore2 || (score == hScore2 && difficulty > difficulty2)) {
      for (int i = 0; i < 16; i++) {
        hName5[i] = hName4[i];
      }
      difficulty5 = difficulty4;
      hScore5 = hScore4;
      for (int i = 0; i < 16; i++) {
        hName4[i] = hName3[i];
      }
      difficulty4 = difficulty3;
      hScore4 = hScore3;
      for (int i = 0; i < 16; i++) {
        hName3[i] = hName2[i];
      }
      difficulty3 = difficulty2;
      hScore3 = hScore2;
      for (int i = 0; i < 16; i++) {
        hName2[i] = alphabet[name[i]];
      }
      difficulty2 = difficulty;
      hScore2 = score;
      secondPlace = true;
      UpdateHighscores();
    } else {
      if (score > hScore3 || (score == hScore3 && difficulty > difficulty3)) {
        for (int i = 0; i < 16; i++) {
          hName5[i] = hName4[i];
        }
        difficulty5 = difficulty4;
        hScore5 = hScore4;
        for (int i = 0; i < 16; i++) {
          hName4[i] = hName3[i];
        }
        difficulty4 = difficulty3;
        hScore4 = hScore3;
        for (int i = 0; i < 16; i++) {
          hName3[i] = alphabet[name[i]];
        }
        difficulty3 = difficulty;
        hScore3 = score;
        thirdPlace = true;
        UpdateHighscores();
      } else {
        if (score > hScore4 || (score == hScore4 && difficulty > difficulty4)) {
          for (int i = 0; i < 16; i++) {
            hName5[i] = hName4[i];
          }
          difficulty5 = difficulty4;
          hScore5 = hScore4;
          for (int i = 0; i < 16; i++) {
            hName4[i] = alphabet[name[i]];
          }
          difficulty4 = difficulty;
          hScore4 = score;
          fourthPlace = true;
          UpdateHighscores();
        } else {
          if (score > hScore5 || (score == hScore5 && difficulty > difficulty5)) {
            for (int i = 0; i < 16; i++) {
              hName5[i] = alphabet[name[i]];
            }
            difficulty5 = difficulty;
            hScore5 = score;
            fifthPlace = true;
            UpdateHighscores();
          }
        }
      }
    }
  }
}



void setup() {
  // put your setup code here, to run once:
  ReadFromEEPROM();
  JoystickSetup();
  MatrixSetup();
  LCDSetup();
  BuzzerSetup();
  score = 0;
  CharactersSetup();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  JoystickLoop();
  MatrixLoop();
  LCDLoop();
}

void CharactersSetup() {
  //arrows -> 1 up 2 down 3 left 4 right
  lcd.createChar(0, squareCharacter);
  lcd.createChar(1, arrowUp);
  lcd.createChar(2, arrowDown);
  lcd.createChar(3, arrowLeft);
  lcd.createChar(4, arrowRight);
}

void LCDSetup() {
  ClearLCD();
  lcd.begin(lcdCols, lcdRows);
  pinMode(brightnessPin, INPUT);
  ShowMenuOption();
  SetLCDBrightness(lcdBrightness);
}

void Ding() {
  if (soundOn == true) {
    tone(buzzerPin, 1000, 100);
  }
}

void BuzzerSetup() {
  pinMode(buzzerPin, OUTPUT);
}

void SetLCDBrightness(int brightness) {
  analogWrite(brightnessPin, map(brightness, minBrightnessValue, maxBrightnessValue, minBrightnessValueRaw, maxBrightnessValueRaw));
  Serial.println(analogRead(brightnessPin));
}

void SetMatrixBrightness(int brightness) {
  lc.setIntensity(0, map(brightness, minBrightnessValue, maxBrightnessValue, minBrightnessValueRaw, maxBrightnessValue + 10));
}

void StartWritingName() {
  lcd.setCursor(12, 0);
  lcd.write(1);
  lcd.write(2);
  lcd.write(3);
  lcd.write(4);
  letterPos = 0;
  lcd.setCursor(letterPos, 1);
}

void ShowMenuOption() {
  ClearLCD();
  if (menuOption == 0) {
    lcd.print("Start game     ");
    lcd.write(2);
  }
  if (menuOption == 1) {
    lcd.print("Highscore     ");
    lcd.write(1);
    lcd.write(2);
  }
  if (menuOption == 2) {
    lcd.print("Settings      ");
    lcd.write(1);
    lcd.write(2);
  }
  if (menuOption == 3) {
    lcd.print("About         ");
    lcd.write(1);
    lcd.write(2);
  }
  if (menuOption == 4) {
    lcd.print("How to play    ");
    lcd.write(1);
  }
}

void ShowSettingsOption() {
  ClearLCD();
  if (settingsOption == 0) {
    WriteName();
  }
  if (settingsOption == 1) {
    WriteDifficultyScreen();
  }
  if (settingsOption == 2) {
    WriteLCDBrightnessScreen();
  }
  if (settingsOption == 3) {
    WriteMatrixBrightnessScreen();
  }
  if (settingsOption == 4) {
    WriteSound();
  }
  if (settingsOption == 5) {
    WriteExit();
  }
}

void WriteDifficultyScreen() {
  ClearLCD();
  lcd.write("Difficulty");
  if (inDifficulty == true) {
    lcd.setCursor(14, 1);
    lcd.write(3);
    lcd.write(4);
  }
  lcd.setCursor(0, 1);
  lcd.print(difficulty);
}

void WriteExit() {
  ClearLCD();
  lcd.write("Exit");
}

void ClearLCD() {
  lcd.clear();
}

void WriteHighscorePos(int pos) {
  ClearLCD();
  lcd.print("Highscore");
  lcd.setCursor(0, 1);
  lcd.print(String(pos));
  lcd.print(".");
  for (int i = 0; i < 16; i++) {
    if (pos == 1) {
      lcd.print(hName1[i]);
    }
    if (pos == 2) {
      lcd.print(hName2[i]);
    }
    if (pos == 3) {
      lcd.print(hName3[i]);
    }
    if (pos == 4) {
      lcd.print(hName4[i]);
    }
    if (pos == 5) {
      lcd.print(hName5[i]);
    }
  }
  if (pos == 1) {
    lcd.setCursor(15, 0);
    lcd.write(2);
  }
  if (pos == 2) {
    lcd.setCursor(14, 0);
    lcd.write(1);
    lcd.write(2);
  }
  if (pos == 3) {
    lcd.setCursor(14, 0);
    lcd.write(1);
    lcd.write(2);
  }
  if (pos == 4) {
    lcd.setCursor(14, 0);
    lcd.write(1);
    lcd.write(2);
  }
  if (pos == 5) {
    lcd.setCursor(15, 0);
    lcd.write(1);
  }
}

void WriteSound() {
  ClearLCD();
  lcd.write("ON/OFF sound");
  if (inSound == true) {
    lcd.setCursor(14, 1);
    lcd.write(3);
    lcd.write(4);
  }
  lcd.setCursor(0, 1);
  if (soundOn == true) {
    lcd.print("ON");
  } else {
    lcd.print("OFF");
  }
}

void WriteName() {
  ClearLCD();
  lcd.setCursor(0, 0);
  lcd.write("Name");
  lcd.setCursor(0, 1);
  for (int i = 0; i < 16; i++) {
    lcd.print(alphabet[name[i]]);
  }
}

void JoystickSetup() {
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(pinSw, INPUT_PULLUP);
}

void JoystickLoop() {
  JoystickClickHandler();
  JoystickMovementHandler();
}


void JoystickClickHandler() {
  reading = digitalRead(pinSw);

  if (reading != lastReading) {
    lastTime = millis();
  }

  if (millis() - lastTime > interval) {
    if (reading != swState) {
      swState = reading;
      if (swState == LOW) {
        JoystickClicked();
      }
    }
  }
  lastReading = reading;
}

void JoystickMovementHandler() {
  ///joystick movements
  xValue = analogRead(xPin);
  yValue = analogRead(yPin);

  if ((xValue > minThreshold && xValue < maxThreshold) || (yValue > minThreshold && yValue < maxThreshold)) {
    joyReading = LOW;
  }
  if ((xValue < minThreshold || xValue > maxThreshold) || (yValue < minThreshold || yValue > maxThreshold)) {
    joyReading = HIGH;
  }

  if (joyReading != lastJoyReading) {
    lastJoyTime = millis();
  }

  if (millis() - lastJoyTime > interval)
    if (joyReading != joyState) {
      joyState = joyReading;
      if (joyReading == HIGH)
        joyMoved = false;
    }

  if (xValue > maxThreshold && joyMoved == false) {  // in sus
    joyMoved = true;
    JoystickUp();
  }
  if (xValue < minThreshold && joyMoved == false) {  //in jos
    joyMoved = true;
    JoystickDown();
  }
  if (yValue > maxThreshold && joyMoved == false) {  // in dreapta
    joyMoved = true;
    JoystickRight();
  }
  if (yValue < minThreshold && joyMoved == false) {  //in stanga
    joyMoved = true;
    JoystickLeft();
  }

  lastJoyReading = joyReading;
}



void GenerateRandomFood() {
  if (matrix[xPos][yPos] == 2) {
    Ding();
    snakeLength += 1;
    score += 1;
    timeLeft = MaxTimeLeft();
    speed -= 10;
    WriteGameScreen();
    rx = random(0, 8);
    ry = random(0, 8);
    while (matrix[rx][ry] == 1) {
      rx = random(0, 8);
      ry = random(0, 8);
    }
    matrix[rx][ry] = 2;
  } else {
    matrix[snakeRow[snakeLength]][snakeColumn[snakeLength]] = 0;
  }
}


int MaxTimeLeft() {
  if (difficulty == 1) {
    return easyTime;
  }
  if (difficulty == 2) {
    return mediumTime;
  }
  if (difficulty == 3) {
    return hardTime;
  }
}

void ShowAbout() {
  if (aboutSlide == 0) {
    ClearLCD();
    lcd.setCursor(0, 0);
    lcd.print("Game name      ");
    lcd.write(4);
    lcd.setCursor(0, 1);
    lcd.print("Snake");
  } else if (aboutSlide == 1) {
    ClearLCD();
    lcd.setCursor(0, 0);
    lcd.print("Author        ");
    lcd.write(3);
    lcd.write(4);
    lcd.setCursor(0, 1);
    lcd.print("Solomon Flavius");
  } else {
    ClearLCD();
    lcd.setCursor(0, 0);
    lcd.print("Github name    ");
    lcd.write(3);
    lcd.setCursor(0, 1);
    lcd.print("SolomonFlavius");
  }
}

void WriteGameScreen() {
  ClearLCD();
  lcd.print("Score:");
  lcd.print(score);
  lcd.setCursor(0, 1);
  lcd.print("Time left:");
}

void JoystickClicked() {
  if (inMenu == true) {  //0-start 1-highscore 2-settings 3-about 4-how to play
    inMenu = false;
    Ding();
    if (menuOption == 0) {
      inGame = true;
      snakeLength = 1;
      timeLeft = MaxTimeLeft();
      snakeRow[1] = 0;
      snakeColumn[1] = 0;
      WriteGameScreen();
    }
    if (menuOption == 1) {
      inHighscore = true;
      WriteHighscorePos(1);
    }
    if (menuOption == 2) {
      inSettings = true;
      WriteName();
    }
    if (menuOption == 3) {
      inAbout = true;
      ShowAbout();
    }
    if (menuOption == 4) {
      inHowToPlay = true;
      ClearLCD();
      lcd.print("Move joystick to");
      lcd.setCursor(0, 1);
      lcd.print("catch food");
    }
  } else if (inHighscore == true) {
    Ding();
    inHighscore = false;
    inMenu = true;
    ShowMenuOption();
  } else if (inSettings == true) {
    Ding();
    inSettings = false;
    if (settingsOption == 0) {
      inName = true;
      StartWritingName();
    }
    if (settingsOption == 1) {
      inDifficulty = true;
      WriteDifficultyScreen();
    }
    if (settingsOption == 2) {
      inBLCD = true;
      WriteLCDBrightnessScreen();
    }
    if (settingsOption == 3) {
      inBMatrix = true;
      WriteMatrixBrightnessScreen();
    }
    if (settingsOption == 4) {
      inSound = true;
      WriteSound();
    }
    if (settingsOption == 5) {
      inMenu = true;
      ShowMenuOption();
    }
  } else if (inName == true) {
    Ding();
    inName = false;
    inSettings = true;
    WriteName();
  } else if (inBLCD == true) {
    Ding();
    inSettings = true;
    inBLCD = false;
    WriteLCDBrightnessScreen();
    EEPROM.update(0, lcdBrightness);
  } else if (inBMatrix == true) {
    Ding();
    inSettings = true;
    inBMatrix = false;
    WriteMatrixBrightnessScreen();
    EEPROM.update(1, matrixBrightness);
  } else if (inSound == true) {
    Ding();
    inSound = false;
    inSettings = true;
    WriteSound();
    EEPROM.update(2, soundOn);
  } else if (inAbout == true) {
    Ding();
    inAbout = false;
    inMenu = true;
    ShowMenuOption();
  } else if (inHowToPlay == true) {
    Ding();
    inHowToPlay = false;
    inMenu = true;
    ShowMenuOption();
  } else if (inDifficulty == true) {
    Ding();
    inDifficulty = false;
    inSettings = true;
    WriteDifficultyScreen();
  } else if (inGameOver == true) {
    Ding();
    inGameOver = false;
    inGameOver2 = true;
    ClearLCD();
    if(firstPlace == true) {
        lcd.write("You reached 1st");
        lcd.setCursor(0,1);
        lcd.write("place");
        firstPlace = false;
    }
    else if(secondPlace == true) {
      lcd.write("You reached 2nd");
      secondPlace = false;
    }
    else if(thirdPlace == true) {
      lcd.write("You reached 3rd");
      lcd.setCursor(0,1);
      lcd.write("place");
      thirdPlace = false;
    }
    else if(fourthPlace == true) {
      lcd.write("You reached 4th");
      lcd.setCursor(0,1);
      lcd.write("place");
      fourthPlace = false;
    }
    else if(fifthPlace == true) {
      lcd.write("You reached 5th");
      lcd.setCursor(0,1);
      lcd.write("place");
      fifthPlace = false;
    }
    else {
      lcd.write("Well played!");
    }
  }
  else if(inGameOver2 == true) {
    Ding();
    inGameOver2 = false;
    inMenu = true;
    menuOption = 0;
    ShowMenuOption();
  }
}

void JoystickUp() {
  if (inMenu == true) {
    if (menuOption == 0) {
      menuOption = 0;
    } else {
      menuOption -= 1;
      Ding();
    }
    ShowMenuOption();
  }
  if (inHighscore == true) {
    if (highscorePos == 1) {
      highscorePos = 1;
    } else {
      highscorePos -= 1;
      Ding();
      WriteHighscorePos(highscorePos);
    }
  }
  if (inSettings == true) {
    if (settingsOption == 0) {
      settingsOption = 0;
    } else {
      settingsOption -= 1;
      Ding();
    }
    ShowSettingsOption();
  }
  if (inName == true) {
    if (name[letterPos] == alphabetLength) {
      name[letterPos] = alphabetLength;
    } else {
      name[letterPos] += 1;
    }
  }
  if (inGame == true) {
    direction = 1;
  }
}

void JoystickDown() {
  if (inMenu == true) {
    if (menuOption == menuOptionsMax) {
      menuOption = menuOptionsMax;
    } else {
      menuOption += 1;
      Ding();
    }
    ShowMenuOption();
  }
  if (inHighscore == true) {
    if (highscorePos == highscorePosMax) {
      highscorePos = highscorePosMax;
    } else {
      highscorePos += 1;
      Ding();
      WriteHighscorePos(highscorePos);
    }
  }
  if (inSettings == true) {
    if (settingsOption == settingsOptionsMax) {
      settingsOption = settingsOptionsMax;
    } else {
      settingsOption += 1;
      Ding();
    }
    ShowSettingsOption();
  }
  if (inName == true) {
    if (name[letterPos] == 0) {
      name[letterPos] = 0;
    } else {
      name[letterPos] -= 1;
    }
  }
  if (inGame == true) {
    direction = 3;
  }
}

void JoystickLeft() {

  if (inName == true) {
    if (letterPos == 0) {
      letterPos = 0;
    } else {
      lcd.print(alphabet[name[letterPos]]);
      letterPos -= 1;
    }
  }
  if (inBLCD == true) {
    if (lcdBrightness == minBrightnessValue) {
      lcdBrightness = minBrightnessValue;
    } else {
      lcdBrightness -= 1;
      
      SetLCDBrightness(lcdBrightness);
      Ding();
      WriteLCDBrightnessScreen();
    }
  }
  if (inBMatrix == true) {
    if (matrixBrightness == minBrightnessValue) {
      matrixBrightness = minBrightnessValue;
    } else {
      matrixBrightness -= 1;
      Ding();
      SetMatrixBrightness(matrixBrightness);
      WriteMatrixBrightnessScreen();
    }
  }
  if (inSound == true) {
    soundOn = !soundOn;
    Ding();
    WriteSound();
  }
  if (inAbout == true) {
    if (aboutSlide == 0) {
      aboutSlide = 0;
    } else {
      aboutSlide -= 1;
      Ding();
      ShowAbout();
    }
  }
  if (inDifficulty == true) {
    if (difficulty == minDifficulty) {
      difficulty = minDifficulty;
    } else {
      difficulty -= 1;
      Ding();
      WriteDifficultyScreen();
    }
  }
  if (inGame == true) {
    direction = 0;
  }
}

void WriteLCDBrightnessScreen() {
  ClearLCD();
  lcd.print("LCD Brightness");
  if (inBLCD == true) {
    lcd.setCursor(14, 1);
    lcd.write(3);
    lcd.write(4);
  }
  lcd.setCursor(0, 1);
  lcd.print("1");
  for (int i = minBrightnessValue; i <= maxBrightnessValue; i++) {
    if (i <= lcdBrightness) {
      lcd.print((char)0);
    } else {
      lcd.print(" ");
    }
  }
  lcd.print("5");
}

void WriteMatrixBrightnessScreen() {
  ClearLCD();
  lcd.print("Matrix Brightness");
  if (inBMatrix == true) {
    lcd.setCursor(14, 1);
    lcd.write(3);
    lcd.write(4);
  }
  lcd.setCursor(0, 1);
  lcd.print("1");
  for (int i = minBrightnessValue; i <= maxBrightnessValue; i++) {
    if (i <= matrixBrightness) {
      lcd.print((char)0);
    } else {
      lcd.print(" ");
    }
  }
  lcd.print("5");
}



void LCDLoop() {
  if (inName == true) {
    lcd.setCursor(letterPos, 1);
    if (millis() - startedFlash > intervalBlink) {
      if (isPrinted) {
        lcd.setCursor(letterPos, 1);
        lcd.print(alphabet[name[letterPos]]);
        isPrinted = false;
      } else {
        lcd.setCursor(letterPos, 1);
        lcd.print("_");
        isPrinted = true;
      }
      startedFlash = millis();
    }
  }
}

void JoystickRight() {
  if (inName == true) {
    if (letterPos == 16) {
      letterPos = 16;
    } else {
      lcd.print(alphabet[name[letterPos]]);
      letterPos += 1;
    }
  }
  if (inBLCD == true) {
    if (lcdBrightness == maxBrightnessValue) {
      lcdBrightness = maxBrightnessValue;
    } else {
      lcdBrightness += 1;
      Ding();
      SetLCDBrightness(lcdBrightness);
      WriteLCDBrightnessScreen();
    }
  }
  if (inBMatrix == true) {
    if (matrixBrightness == maxBrightnessValue) {
      matrixBrightness = maxBrightnessValue;
    } else {
      matrixBrightness += 1;
      Ding();
      SetMatrixBrightness(matrixBrightness);
      WriteMatrixBrightnessScreen();
    }
  }
  if (inSound == true) {
    soundOn = !soundOn;
    Ding();
    WriteSound();
  }
  if (inAbout == true) {
    if (aboutSlide == 2) {
      aboutSlide = 2;
    } else {
      aboutSlide += 1;
      Ding();
      ShowAbout();
    }
  }
  if (inDifficulty == true) {
    if (difficulty == maxDifficulty) {
      difficulty = maxDifficulty;
    } else {
      difficulty += 1;
      Ding();
      WriteDifficultyScreen();
    }
  }
  if (inGame == true) {
    direction = 2;
  }
}



void MatrixSetup() {
  lc.shutdown(0, false);  // turn off power saving, enables display
  SetMatrixBrightness(matrixBrightness);
  lc.clearDisplay(0);  // clear screen
  matrix[xPos][yPos] = 1;
  rx = random(0, 8);
  ry = random(0, 8);
  matrix[rx][ry] = 2;
}



void MatrixLoop() {
  if (inGame == true) {
    if (millis() - lastBlink > blinkTime) {
      if (isShown == true) {
        isShown = false;
      } else {
        isShown = true;
      }
      lc.setLed(0, rx, ry, isShown);
      lastBlink = millis();
    }
    if (millis() - lastTimeMove > speed) {
      if (direction == 0) {
        GameMoveLeft();
      }
      if (direction == 1) {
        GameMoveUp();
      }
      if (direction == 2) {
        GameMoveRight();
      }
      if (direction == 3) {
        GameMoveDown();
      }
      lastTimeMove = millis();
    }
    if (millis() - lastLCDTime > oneSecond) {
      timeLeft -= 1;
      UpdateLCDTime();
      lastLCDTime = millis();
    }
    if (matrixChanged = true) {
      // matrix display logic
      UpdateMatrix();
      matrixChanged = false;
    }
  }
}

void UpdateLCDTime() {
  lcd.setCursor(10, 1);
  lcd.print(timeLeft);
  if(timeLeft == 9)
  {
    Serial.println(timeLeft);
    lcd.setCursor(11,1);
    lcd.print(' ');
  }
}

void UpdateMatrix() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      if (matrix[row][col] != 2) {
        lc.setLed(0, row, col, matrix[row][col]);
      }
    }
  }
}


void GameMoveUp() {
  matrixChanged = true;
  if (xPos > 0) {
    xPos--;
  } else {
    xPos = matrixSize - 1;
  }
  GameOver();
  if (inGame == true) {
    GenerateRandomFood();
    for (int i = snakeLength; i >= 2; i--) {
      snakeRow[i] = snakeRow[i - 1];
      snakeColumn[i] = snakeColumn[i - 1];
      matrix[snakeRow[i]][snakeColumn[i]] = 1;
    }
    matrix[xPos][yPos] = 1;
    snakeRow[1] = xPos;
    snakeColumn[1] = yPos;
  }
}

void GameMoveDown() {
  matrixChanged = true;
  if (xPos < matrixSize - 1) {
    xPos++;
  } else {
    xPos = 0;
  }
  GameOver();
  if (inGame == true) {
    GenerateRandomFood();
    for (int i = snakeLength; i >= 2; i--) {
      snakeRow[i] = snakeRow[i - 1];
      snakeColumn[i] = snakeColumn[i - 1];
      matrix[snakeRow[i]][snakeColumn[i]] = 1;
    }
    matrix[xPos][yPos] = 1;
    snakeRow[1] = xPos;
    snakeColumn[1] = yPos;
  }
}

void GameMoveLeft() {
  matrixChanged = true;

  if (yPos > 0) {
    yPos--;
  } else {
    yPos = matrixSize - 1;
  }
  GameOver();
  if (inGame == true) {
    GenerateRandomFood();
    for (int i = snakeLength; i >= 2; i--) {
      snakeRow[i] = snakeRow[i - 1];
      snakeColumn[i] = snakeColumn[i - 1];
      matrix[snakeRow[i]][snakeColumn[i]] = 1;
    }
    matrix[xPos][yPos] = 1;
    snakeRow[1] = xPos;
    snakeColumn[1] = yPos;
  }
}

void GameMoveRight() {
  matrixChanged = true;

  if (yPos < matrixSize - 1) {
    yPos++;
  } else {
    yPos = 0;
  }
  GameOver();
  if (inGame == true) {
    GenerateRandomFood();
    for (int i = snakeLength; i >= 2; i--) {
      snakeRow[i] = snakeRow[i - 1];
      snakeColumn[i] = snakeColumn[i - 1];
      matrix[snakeRow[i]][snakeColumn[i]] = 1;
    }
    matrix[xPos][yPos] = 1;
    snakeRow[1] = xPos;
    snakeColumn[1] = yPos;
  }
}

void GameOver() {
  if (matrix[xPos][yPos] == 1 || timeLeft <= 0) {
    CheckHighscore();
    inGame = false;
    ClearLCD();
    lcd.print("Game over");
    lcd.setCursor(0, 1);
    lcd.print("Score:");
    lcd.print(score);
    score = 0;
    inGameOver = true;
    snakeLength = 0;
    for (int i = 1; i <= 64; i++) {
      snakeRow[i] = 0;
      snakeColumn[i] = 0;
    }
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        matrix[i][j] = 0;
      }
    }
    rx = random(0, 8);
    ry = random(0, 8);
    speed = 200;
    matrix[rx][ry] = 2;
    UpdateMatrix();
    lc.clearDisplay(0);
  }
}
