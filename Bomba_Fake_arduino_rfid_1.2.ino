//Bomba Fake usando Arduino + Liquid Crystal + Rfid522
//Author: Alex Sandro
//License: Open Source

#include <LiquidCrystal.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);	// Create MFRC522 instance.

LiquidCrystal lcd(8, 6, 5, 4, 3, 2); // Create LCD
//Variables:
int led1 = 7; //LED PIN
int minu = 1; //Minutes (MAX 9!)
int seco = 9; //Seconds
//int greenPin = 7;
String conteudo = "";
int reset = 0;

// RS(12) EN(11) DB4(5) DB5(4) DB6(3) DB7(2)
//Connect VSS to GND, VDD to 5V and V0 to a potentiometer
//A and K are used for LCD's backlight

void setup() {
  Serial.begin(9600);	// inicializa comunicação serial
  lcd.begin(16, 2); //inicializa o LCD resolução 16x2
  pinMode(led1, OUTPUT); //Define o pino do led como saida
  SPI.begin();			// Inicializa SPI bus
  mfrc522.PCD_Init();	// Inicializa Rfid - MFRC522 card
  menu();
}

void menu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.println("Virus H1Z1 v1.0     ");
  lcd.setCursor(0, 1);
  lcd.println("Aproxime o Card   ");
}

void completa() {
  while (reset ==0) {
  lcd.clear();
  lcd.setCursor(0, 2);
  //lcd.autoscroll();
  lcd.print("BOOOOMMMMMM");
  
  tone(led1, 1915, 3830);
  delay(1000); 
  tone(led1, 1700, 3400);
  delay(1000); 
  tone(led1, 1519, 3038);
  delay(1000);
  tone(led1, 1432, 2864);
  delay(1000);
   tone(led1, 1275, 2550);
  delay(1000); 
  tone(led1, 1136, 2272);
  delay(1000); 
  tone(led1, 1014, 2028);
  delay(1000);
  tone(led1, 956, 1912);
  delay(1000);
  verifica();
}
}
void desarmar() {
  while (reset ==1) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Bomba");
  lcd.setCursor(0, 1);
  lcd.print("Desativada");
  verifica();
  delay(1000);
  }

}
void contagem() {
  minu=1;
  seco=9;
  //Print Time Left: on the top of the display
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Time Left");
  //Print the :
  lcd.setCursor(2, 2);
  lcd.print(":");
  //Print minutes left

  if ((minu > 0) && (minu < 9)) {
    lcd.setCursor(0, 1);
    lcd.print("0");
    lcd.setCursor(1, 1);
    lcd.print(minu);
    seco = 60;
  }
  else {
    lcd.setCursor(0, 1);
    lcd.print(minu);
  }
  
  // inicio do Loop 
  
  while (minu >= 0) {
    while (seco > 0) {
      seco = seco - 1;
      verifica();
      if (minu <= 0 and seco <= 0) {
        completa();
      }
      delay(1000);
      if (seco > 9) {
        lcd.setCursor(3, 2);
        lcd.print(seco);
      } else {
        lcd.setCursor(3, 2);
        lcd.print("0");
        lcd.setCursor(4, 2);
        lcd.print(seco);
         
      }
    }
    
    if (minu > 0 and minu < 9) {
      minu = minu - 1;
      lcd.setCursor(0, 1);
      lcd.print("0");
      lcd.setCursor(1, 1);
      lcd.print(minu);
      seco = 60;
    }
    else {
      minu = minu - 1;
      if (minu > 9) {
        lcd.setCursor(0, 1);
      } else {
        lcd.setCursor(0, 1);
        lcd.print("0");
      }
      lcd.print(minu);
      seco = 60;
          
    }
   
    
    }
  
  
      
}

void verifica() {

  Serial.flush();
  conteudo = "";

  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Dump debug info about the card. PICC_HaltA() is automatically called.
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));



  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);

    conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Mensagem : ");
  conteudo.toUpperCase();

  if (conteudo.substring(1) == "75 DF B6 76") //UID 1 - Chaveiro
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Iniciando");
    lcd.setCursor(0, 1);
    lcd.print("Detonacao");
    lcd.blink();
    delay(3000);
    lcd.clear();
    lcd.noBlink();
    
    reset = 0;
    contagem();
  }
  if  (conteudo.substring(1) == "3D BE 73 65") //UID 1 - Chaveiro
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Cancelando");
    lcd.setCursor(0, 1);
    lcd.print("Detonacao");
    lcd.blink();
    delay(3000);
    lcd.clear();
    lcd.noBlink();
    
    reset = 1;
    desarmar();
  }
}
void loop() {

  verifica();
}



