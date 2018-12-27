#include <Wire.h>
// Get the LCD I2C Library here:
// https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
#include <LiquidCrystal_I2C.h>
const int ledPin = 13;       
const int  zeroCrossPin = 2;    


boolean startButtonState = 0;
boolean lastStartButtonState = 0;

int buttonPushCounter = 0;   
int zeroCrossState = 0;         
int lastZeroCrossState = 0;     
int czasZgrzewaniaTriak = 0;
int czasZgrzewaniaTriakChange =0;
int czasZgrzewaniaTriak2 = 1;//wartość razy 10ms
int buttonValue1 = 0;
int potentiometerValue1 = 0;
int potentiometerValue2 = 0;
int pozwoleniePracy = 0;
int pozwoleniePracy2 = 0;
int krokCyklu=1;
int opoznienieZalaczenia = 0;
int podwojnyZgrzew =0;
int potentiometerValueChange1;
int potentiometerValueChange2;
float czasZgrzewaniaSek = 0;



LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Ustawienie adresu ukladu na 0x27


void setup()
{
  pinMode(3, OUTPUT); //moc3021
  pinMode(5, OUTPUT); //buzzer
  pinMode(7, INPUT);
  pinMode(zeroCrossPin, INPUT);
  digitalWrite(3, LOW);
  pinMode(A7, INPUT_PULLUP);
  pinMode(A6, INPUT_PULLUP);
  lcd.begin(16,2);   // Inicjalizacja LCD 2x16
  lcd.print("EMSI-Tech ");
  lcd.setCursor(0, 1);
  lcd.print("TR-I2C v.1.02a");
  tone(5,2600);
  delay(30);
  noTone(5); 
  delay(100);
  tone(5,2600);
  delay(30);
  noTone(5);
  delay(1000);
  potentiometerValue1 = analogRead(A7);
  potentiometerValue2 = analogRead(A6);
}/*--(end setup )---*/


void printEmsiLCD(){
  lcd.clear();
  if(czasZgrzewaniaTriak<100){
    lcd.setCursor(0, 0); 
    lcd.print("Czas zgrzewu:");
    lcd.setCursor(0, 1);
    lcd.print(czasZgrzewaniaTriakChange*10);
    lcd.print("ms");
    //lcd.setCursor(0, 1);
  }
  else{
    lcd.setCursor(0, 0); 
    lcd.print("Czas zgrzewu:");
    lcd.setCursor(0, 1);
    lcd.print(czasZgrzewaniaSek);
    lcd.print("s");
  }
  delay(100);

}

void loop() 
{
  // potentiometerValue1 = analogRead(A7);
  //potentiometerValue2 = analogRead(A6);

  potentiometerValueChange1 = abs(potentiometerValue1 - analogRead(A7));
  potentiometerValueChange2 = abs(potentiometerValue2 - analogRead(A6));




  //stabilizator odczytu potencjometru
  if (potentiometerValueChange1>1){
    potentiometerValue1 = analogRead(A7);
  }
  if (potentiometerValueChange2>1){
    potentiometerValue2 = analogRead(A6);
  }
  //koniec stabilizatora

  czasZgrzewaniaTriak = (potentiometerValue1/50) + (potentiometerValue2);    

  //działanie przy zmianie wartości czasu zgrzewu
  if (czasZgrzewaniaTriakChange != czasZgrzewaniaTriak){
    czasZgrzewaniaTriakChange =czasZgrzewaniaTriak;
    czasZgrzewaniaSek = czasZgrzewaniaTriak/100.00;
    tone(5,2000);
    delay(30);
    noTone(5);
    printEmsiLCD();

  }




  /*
  //start zgrzewania po puszczeniu przycisku
   if (digitalRead(7) == HIGH){
   while(digitalRead(7)== HIGH){
   delay(100);
   }
   pozwoleniePracy = 1;
   }
   */



  //start po naciśnięciu przycisku
  startButtonState = digitalRead(7);
  if (startButtonState != lastStartButtonState) {
    if (startButtonState == HIGH) {
      delay(100);
      pozwoleniePracy = 1;
      tone(5,2300);
      delay(30);
      noTone(5); 
      if (podwojnyZgrzew == 1) {
        pozwoleniePracy2 = 1;
      }
    }
  }
  lastStartButtonState = startButtonState;


  krokCyklu=1;
  //zgrzew właściwy
  while(pozwoleniePracy == 1){
    zeroCrossState = digitalRead(zeroCrossPin);
    if (zeroCrossState != lastZeroCrossState) {
      if (zeroCrossState == HIGH) {
        delayMicroseconds(6000);
        digitalWrite(3, HIGH);
        digitalWrite(13, HIGH);   
        delayMicroseconds(100);   
        krokCyklu++;

      } 
    }
    lastZeroCrossState = zeroCrossState;
    if (krokCyklu >czasZgrzewaniaTriak){
      pozwoleniePracy = 0;
      digitalWrite(3, LOW);
      digitalWrite(13, LOW);  
      tone(5,2300);
      delay(30);
      noTone(5); 
      tone(5,2300);
      delay(30);
      noTone(5);
     delay(500); 
      printEmsiLCD();
    }
  }

}/* --(end main loop )-- */


/* ( THE END ) */




































