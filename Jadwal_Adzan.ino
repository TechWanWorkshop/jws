#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <DFRobotDFPlayerMini.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#include <NTPClient.h>
#include <WiFiUdp.h>

#include <TimeLib.h>
#include <TimeAlarms.h>
 
const char* ssid = "onlyone";
const char* password =  "islam130590";

HardwareSerial mySoftwareSerial(1);
DFRobotDFPlayerMini myDFPlayer;

LiquidCrystal_I2C lcd(0x27, 16, 2); 

//defining House Icon
byte housechar1[8]={B00000, B00001, B00011, B00011, //Row 0, Col 0
                  B00111, B01111, B01111, B11111,};
byte housechar2[8]={B11111, B11111, B11100, B11100, //Row 1, Col 0
                  B11100, B11100, B11100, B11100,};
byte housechar3[8]={B00000, B10010, B11010, B11010, //ROW 0, Col 1
                  B11110, B11110, B11110, B11111,};
byte housechar4[8]={B11111, B11111, B11111, B10001, //Row 1, Col 1
                    B10001, B10001, B11111, B11111,};

//NTP---------
#define NTP_OFFSET   7 * 60 * 60      // In seconds
#define NTP_ADDRESS  "pool.ntp.org"
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET);
int afternoon=0;

const char* Imsak ;
const char* Sunrise ;
const char* Fajr ;
const char* Dhuhr ;
const char* Asr ;
const char* Sunset ;
const char* Maghrib ;
const char* Isha ;
const char* Midnight ;

const char* Gregorian;

String jam ;
String menit ;
String detik ;

String t;




StaticJsonDocument<1024> doc;

//String imsak = "04:25";

 
void setup() {
 
  Serial.begin(115200);

  //Wire.begin();
  lcd.init();          
  lcd.backlight();
  lcd.setCursor(5, 0);
  lcd.print("Jadwal");
  lcd.setCursor(0, 1);
  lcd.print("  Waktu Sholat   ");
  

  mySoftwareSerial.begin(9600, SERIAL_8N1, 16, 17);  // speed, type, RX, TX

  while (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    
    Serial.println(myDFPlayer.readType(),HEX);
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    
  }
  
  Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms
  
  //----Set volume----
  myDFPlayer.volume(30);  //Set volume value (0~30).
  myDFPlayer.volumeUp(); //Volume Up
  myDFPlayer.volumeDown(); //Volume Down
  
  //----Set different EQ----
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  
  //----Set device we use SD as default----
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  


  
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");

  timeClient.update();
  String newtime = timeClient.getFormattedTime();
  Serial.print("the time is : ");
  Serial.println(newtime);
  Serial.print("Hour    : ");
  Serial.println((newtime.substring(0,2)).toInt());
  Serial.print("Minute  : ");
  Serial.println((newtime.substring(3,5)).toInt());
  Serial.print("Seconds : ");
  Serial.println((newtime.substring(6,8)).toInt());
 
  setTime((newtime.substring(0,2)).toInt(),(newtime.substring(3,5)).toInt(),(newtime.substring(6,8)).toInt(),1,1,20); 

  Alarm.alarmRepeat(0,10,0, pray);  // 8:30am every day
  
  //Alarm.timerRepeat(5, sensor);            // timer for every 15 seconds   

   pray();

   delay(1000);
   lcd.clear();
  
 
 
}
 
void loop() {
 
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status

    digitalClockDisplay();

    
    
    
    Serial.println (t);

    String test = "04:17:00";

    

    String imsak = String (Imsak);
    String pray_imsak = imsak+":00";
    
    String fajr = String (Fajr);
    String pray_fajr = fajr+":00";

    String dhuhr = String (Dhuhr);
    String pray_dhuhr = dhuhr+":00";
    
    String ashar = String (Asr);
    String pray_ashar = ashar+":00";

    String maghrib = String (Maghrib);
    String pray_maghrib = maghrib+":00";

    String isha = String (Isha);
    String pray_isha = isha+":00";

    

    if (t == pray_fajr)
    {
      adzan("Fajr");
      myDFPlayer.play(1);
      delay(180000);
     
    }

    else if (t == pray_dhuhr)
    {
      adzan("Dhzuhur");
      myDFPlayer.play(1);
      delay(180000);
     
    }

    else if (t == pray_ashar)
    {
      adzan("Ashar");
      myDFPlayer.play(1);
      delay(180000);
    }

    else if (t == pray_maghrib)
    {
      adzan("Maghrib");
      myDFPlayer.play(1);
      delay(180000);
    }

     else if (t == pray_isha)
    {
      adzan("Isha");
      myDFPlayer.play(1);
      delay(180000);
    }

    else
    {
      lcdview();
    }

    

    
    
 
    Alarm.delay(1000); // show clock every second

    

  }

  else
  {
    ESP.restart();
  }
 
    
 
 
 
}

void adzan(String i) {
  lcd.setCursor(3,1);
  lcd.print("        "); 

  String gregorian = String (Gregorian);
  lcd.createChar(1,housechar1);
  lcd.createChar(2,housechar2);
  lcd.createChar(3,housechar3);
  lcd.createChar(4,housechar4);
  lcd.setCursor(0,0);
  lcd.write(1);
  lcd.setCursor(0,1);
  lcd.write(2);
  lcd.setCursor(1,0);
  lcd.write(3);
  lcd.setCursor(1,1);
  lcd.write(4);
    
  lcd.setCursor(3,1);
  lcd.print("             "); 

  lcd.setCursor(3,0);
  lcd.print(gregorian);

  
  lcd.setCursor(3,1);
  lcd.print("Adzan "+i); 
  
}

void lcdview() {
  
    String gregorian = String (Gregorian);
    lcd.createChar(1,housechar1);
    lcd.createChar(2,housechar2);
    lcd.createChar(3,housechar3);
    lcd.createChar(4,housechar4);
    lcd.setCursor(0,0);
    lcd.write(1);
    lcd.setCursor(0,1);
    lcd.write(2);
    lcd.setCursor(1,0);
    lcd.write(3);
    lcd.setCursor(1,1);
    lcd.write(4);
    
    lcd.setCursor(3,1);
    lcd.print("             ");  

    lcd.setCursor(3,0);
    lcd.print(gregorian);

    
       
    lcd.setCursor(3,1);
    lcd.print(t); 
}



void pray() {

  HTTPClient http;
 
    http.begin("https://api.pray.zone/v2/times/today.json?city=jakarta"); //Specify the URL
    int httpCode = http.GET();                                        //Make the request
 
    if (httpCode > 0) { //Check for the returning code
 
        String payload = http.getString();
        //Serial.println(httpCode);
        //Serial.println(payload);

        DeserializationError error = deserializeJson(doc, payload);

        // extract the data
        JsonObject results = doc["results"];
        JsonObject results_datetime_0_times = results["datetime"][0]["times"];
        Imsak = results_datetime_0_times["Imsak"]; // "07:13"
        Sunrise = results_datetime_0_times["Sunrise"]; // "08:36"
        Fajr = results_datetime_0_times["Fajr"]; // "07:23"
        Dhuhr = results_datetime_0_times["Dhuhr"]; // "13:00"
        Asr = results_datetime_0_times["Asr"]; // "15:03"
        Sunset = results_datetime_0_times["Sunset"]; // "17:25"
        Maghrib = results_datetime_0_times["Maghrib"]; // "17:46"
        Isha = results_datetime_0_times["Isha"]; // "18:38"
        Midnight = results_datetime_0_times["Midnight"]; // "00:24"

        JsonObject results_datetime_0_date = results["datetime"][0]["date"];
        Gregorian = results_datetime_0_date["gregorian"]; // "2021-10-26"


        Serial.print("Date: ");
        Serial.println(Gregorian);
        Serial.print("Imsak: ");
        Serial.println(Imsak);
        Serial.print("Subuh: ");
        Serial.println(Fajr);
        Serial.print("Dhuhr: ");
        Serial.println(Dhuhr);
        Serial.print("Ashar: ");
        Serial.println(Asr);
        Serial.print("Maghrib: ");
        Serial.println(Maghrib);
        Serial.print("Isha: ");
        Serial.println(Isha);

        lcd.clear();

              
      }
 
    else {
      Serial.println("Error on HTTP request");
    }
 
    http.end(); //Free the resources
  }
  


void digitalClockDisplay() 
{
  // digital clock display of the time
    jam = String(hour());
    menit = String(minute());
    detik = String(second());
    if (hour()<10)
    jam = "0"+jam;
    if (minute()<10)
    menit = "0"+menit;
    if (second()<10)
    detik = "0"+detik;
    t = jam+":"+menit+":"+detik;
  

  
}

void printDigits(int digits) 
{
  Serial.print(":");
  if (digits < 10)
  Serial.print('0');
  Serial.print(digits);
}
