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
int sirene = 7; //sirene PIN
int minu = 14; //Minutes (MAX 9!)
int seco = 60; //Seconds
int buzzerPin = A4;
int fio1 = A3;
int fio2 = A2;
int fio3 = A1;
String conteudo = "";
int reset = 0;
void setup() {
  Serial.begin(9600);	// inicializa comunicação serial
  lcd.begin(16, 2); //inicializa o LCD resolução 16x2
  pinMode(sirene, OUTPUT); //Define o pino da sirene como saida
  pinMode(buzzerPin, OUTPUT);
  pinMode(fio1, INPUT);
  pinMode(fio2, INPUT);
  pinMode(fio3, INPUT);
  SPI.begin();			// Inicializa SPI bus
  mfrc522.PCD_Init();	// Inicializa Rfid - MFRC522 card
  digitalWrite(sirene, LOW);
  menu();
}
void menu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.println("Virus H1Z1 v1.2     ");
  lcd.setCursor(0, 1);
  lcd.println("Aproxime o Card   ");
}
void completa() {
  while (reset == 0) {
    lcd.clear();
    lcd.setCursor(0, 2);
    //lcd.autoscroll();
    lcd.print("BOOOOMMMMMM");
    digitalWrite(sirene, HIGH);
    delay(5000);
    verifica();
    digitalWrite(sirene, LOW);
    delay(25000);
    verifica();
  }
}
void desarmar() {
  while (reset == 1) {
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
  minu = 14;
  seco = 60;
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
      tone(buzzerPin,1912,56);
      int leitura1= digitalRead(fio1);
  int leitura2= digitalRead(fio2);
  int leitura3= digitalRead(fio3);
  
  if (leitura1 ==0) {
        reset = 1;
        desarmar();
     }
      if (leitura2 ==0) {
        
        completa();
                     }
      if (leitura3 ==0) {
        seco = seco - 1;
     }
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



