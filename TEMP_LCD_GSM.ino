//Libraries
#include <DHT.h>
#include "LiquidCrystal.h"
#include <SoftwareSerial.h>

// initialize the library by providing the nuber of pins to it
LiquidCrystal lcd(8,9,4,5,6,7);
SoftwareSerial sms(18, 19);
SoftwareSerial gprs(18,19);// RX10, TX11 Pins
String apn = "unet";
String url = "http://128.199.96.255/GDP/add.php";  //URL for HTTP-POST-REQUEST

//Constants
#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino


//Variables
int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value
String byteRead;
String rainstatus;
// lowest and highest sensor readings:
const int sensorMin = 0;     // sensor minimum
const int sensorMax = 1024;  // sensor maximum
int suhu = 70;


int ledMerah = 31; 
int ledKuning = 33; 
int ledHijau = 35;
int buzzer = 37;
int buzzerState = LOW;
unsigned long previousMillis = 1000;
const long interval = 1000; 
int counter = 0;

void setup()
{
  pinMode(ledMerah, OUTPUT);
  pinMode(ledKuning, OUTPUT);
  pinMode(ledHijau, OUTPUT);
  pinMode(buzzer, OUTPUT);
  dht.begin();
  lcd.begin(16,2);
  sms.begin(9600);
  gprs.begin(9600);
  Serial.begin(9600);
  lcd.setCursor(0,0);
  lcd.print("  Greetings!!");
  delay(3000);
}

void loop()
{
/*----------------------------------------------------------------------------------------
 
    //check if serial connection available
    if (Serial.available()) {
      // read the most recent byte
      char c = Serial.read();
      // byteRead(String) is equivalent to c(char)
      byteRead += c;
     if(byteRead = "A")
        {
        Serial.print("      Interupted");
        }}

*///-----------------------------------------------------------------------------------------
    //Read data and store it to variables hum and temp
    hum = dht.readHumidity();
    temp= dht.readTemperature();
    //Print temp and humidity values to serial monitor
    //delay(2000);
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print(" %, Temp: ");
    Serial.print(temp);
    Serial.println(" Celsius");

    rain_sensor();
          

//----------------------------------------------------------------------------------------

//show value sensor to LCD
   // lcd.setCursor(0,0);
    //text to print
    lcd.begin(16,2);
    lcd.setCursor(0,0);
    lcd.print("   HUMID:");
    lcd.print(hum);
    // set cusor position to start of next line
    lcd.setCursor(0,1);
    lcd.print("   TEMP:");
    lcd.print(temp);
    //delay(1000);
    Serial.println(counter);

    
//----------------------------------------------------------------------------------------
    
      //SEND SMS
      //if temp > 40, send SMS
      if (temp >= 40){   
        digitalWrite(ledMerah, HIGH);
        digitalWrite(ledKuning, LOW);
        digitalWrite(ledHijau, LOW);

        unsigned long currentMillis = millis();
         if (currentMillis - previousMillis >= interval) {
         // save the last time you blinked the LED
         previousMillis = currentMillis;
         // if the LED is off turn it on and vice-versa:
         if (buzzerState == LOW) {
             buzzerState = HIGH;
           }   else {
             buzzerState = LOW;
           }
         }    
         digitalWrite(buzzer, buzzerState);

            if (counter == 0){
            // set the LED with the ledState of the variable:
            //delay(1000);
            sms.println("AT+CMGF=1");    //To send SMS in Text Mode
            delay(1000);
            sms.println("AT+CMGS=\"+601137823855\"\r"); // change to the phone number you using 
            Serial.println("AT+CMGS=\"+601137823855\"\r");
            delay(1000);
            sms.print("Humidity: ");//the content of the message
            delay(200);
            sms.print(hum);
            delay(200);
            sms.print("  Temp: ");//the content of the message
            delay(200);
            sms.print(temp);
            delay(200);
            sms.println((char)26);//the stopping character
            delay(200);
            counter = 1; 
            } 
      }
        
        else if ( temp > 30 &&  temp < 40){
         digitalWrite(ledKuning, HIGH);
         digitalWrite(ledMerah, LOW);
         digitalWrite(ledHijau, LOW);
         counter = 0;
         buzzerState = LOW;
         digitalWrite(buzzer, buzzerState);
      }
    
      else if (temp < 30){
        digitalWrite(ledHijau, HIGH);
        digitalWrite(ledKuning, LOW);
        digitalWrite(ledMerah, LOW);
        counter = 0;
        buzzerState = LOW;
        digitalWrite(buzzer, buzzerState);
      }
      
      gsm_sendhttp();
      
}  

//-------------------------------------------------------------------
void gsm_sendhttp() {
  gprs.println("AT");
  delay(500);
  gprs.println("AT+CSCLK=0");
  gprs.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  delay(500);
  gprs.println("AT+SAPBR=3,1,\"APN\",\"unet\"");
  delay(500);
  gprs.println("AT+SAPBR=1,1");
  delay(500);
  gprs.println("AT+HTTPINIT");
  delay(500);
  gprs.println("AT+HTTPPARA=\"CID\",1");
  delay(500);
  gprs.print("AT+HTTPPARA=\"URL\",\"");
  gprs.print("http://johanhussin.me/GDP/add.php\?temp1=");
  gprs.print(temp);
  gprs.print("&&moi1=");
  gprs.print(hum);
  gprs.print("&&rain=");
  gprs.print(rainstatus);
  gprs.println("\"");
  
  Serial.print("AT+HTTPPARA=\"URL\",\"");
  Serial.print("http://128.199.96.255/GDP/add.php\?temp1=");
  Serial.print(temp);
  Serial.print("&&moi1=");
  Serial.print(hum);
  Serial.print("&&rain=");
  Serial.print(rainstatus);
  Serial.println("\"");
  delay(1000);
  gprs.println("AT+HTTPACTION=1");
  delay(1000);
  gprs.println("AT+HTTPTERM");
  delay(1000);
}

//------------------------------------------------------------------------
void rain_sensor() {
    // read the sensor on analog A0:
  int sensorReading = analogRead(A0);
  // map the sensor range (four options):
  // ex: 'long int map(long int, long int, long int, long int, long int)'
  int range = map(sensorReading, sensorMin, sensorMax, 0, 2);
  
  // range value:
  switch (range) {
 case 0:    // Sensor getting wet
    rainstatus = "Raining";
    Serial.println("Raining");
    break;
 case 1:    // Sensor dry
    rainstatus = "Not Raining";
    Serial.println("Not Raining");
    break;
  }
  delay(500);  // delay between reads
  }
    

   

       







      
  


