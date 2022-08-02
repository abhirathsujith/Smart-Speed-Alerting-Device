#include <TinyGPS.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);
SoftwareSerial gsm(8,9);
SoftwareSerial ss(6,5);
String phone_no1 = "8971504592";
//String phone_no2 = "";
TinyGPS gps;
float value=0;
float rev=0;
int rpm;
int oldtime=0;
int time;
char v_no = "KA05MG3232";
long randNumber;
void sendsms (String phone_no1,float s);
void sendsmsrto (String phone_no2,float s);
const int BUZZ = 8;
 
void isr() //interrupt service routine
{
rev++;
}
 
void setup()
{
Serial.begin(9600);
gsm.begin(9600);
ss.begin(9600);
lcd.init();
  lcd.clear();
  lcd.backlight();
  pinMode(BUZZ, OUTPUT);

 

attachInterrupt(0,isr,RISING); //attaching the interrupt
}
 
void loop()
{

delay(1000);
detachInterrupt(0); //detaches the interrupt
time=millis()-oldtime; //finds the time
rpm=(rev/time)*60000*7; //calculates rpm for blades
oldtime=millis(); //saves the current time
rev=0;
float r=0.0375;
float c=2*(22/7)*r;
float s=abs(c*rpm);
//s=(s/1000)*6;
s=0;


//Serial.print(rpm);
//Serial.print("\n");
  lcd.setCursor(0,0);
  lcd.print(v_no);
  lcd.setCursor(0,1);
  lcd.print("SPEED=");
  lcd.print(s);
  lcd.print("Kmh");
if (s>=0)
{
  sendsms(phone_no1,  s);
  digitalWrite(BUZZ, HIGH);
  delay(3000);
  digitalWrite(BUZZ, LOW);
 // sendsmsrto(phone_no2, s);
  
}

attachInterrupt(0,isr,RISING);
}




void sendsms(String phoneno,float s)
{
  
  bool newddata = false;
  unsigned long chars;
  unsigned short sentences,failed;
  float flat,flon;
  unsigned long age;
  randNumber = random(10000,99999);
  gps.f_get_position(&flat,&flon,&age);
  gsm.print("AT+CMGF=1\r");
  delay(400);
  gsm.print("AT+CMGS=\"");
  gsm.print(phoneno);
  gsm.println("\"");
  delay(300);
  Serial.print("Your e-challan No is ");
  Serial.print(randNumber);
  gsm.print("Your e-challan No is ");
  gsm.print(randNumber);
  
  //lcd.print(randNumber);

  
  
  Serial.println("\nFor the Vehicle No. ");
  Serial.print(v_no);
  Serial.print("\nFound OVER SPEEDING at \n");
  Serial.print(s);
  Serial.print("Km/hr");
  Serial.print("\n");
  gsm.print("\nFor the Vehicle No. ");
  gsm.print(v_no);
  gsm.print("\nFound OVER SPEEDING at ");
  gsm.print(s);
  gsm.print("Km/hr");
  gsm.print("\n");
  
  


  
  //gsm.print("\n");

  
  Serial.println("Having total challan amount as Rs 1000\nPlease pay the amount to the nearest RTO\n ");
  
  
  gsm.print("https://maps.google.com/maps?=loc:");
  //gsm.print("\n");
  gsm.print(flat==TinyGPS::GPS_INVALID_ANGLE ? 0.0 : flat,6);
  Serial.print(",");
  Serial.print("\n");
  gsm.print(",");
  gsm.print(flon==TinyGPS::GPS_INVALID_ANGLE ? 0.0 : flon,6);
  delay(200);
  

  Serial.print(flat);
  Serial.print("\n");
  Serial.print(flon);
  Serial.print("\n");
  gsm.println((char)26);
  delay(200);
  gsm.println();
  
}

/*void sendsmsrto(String phoneno2,float s)
{
  bool newddata = false;
  unsigned long chars;
  unsigned short sentences,failed;
  float flat,flon;
  unsigned long age;
  gps.f_get_position(&flat,&flon,&age);
  gsm.print("AT+CMGF=1\r");
  delay(400);
  gsm.print("AT+CMGS=\"");
  gsm.print(phoneno2);
  gsm.println("\"");
  delay(300);
  gsm.print("speed of vechile is");
  gsm.print(s);
  gsm.print("\n");
  gsm.print("https://maps.google.com/maps?=loc:");
  gsm.print("\n");
  gsm.print(flat==TinyGPS::GPS_INVALID_ANGLE ? 0.0 : flat,6);
  Serial.print(",");
  Serial.print("\n");
  gsm.print(",");
  gsm.print(flon==TinyGPS::GPS_INVALID_ANGLE ? 0.0 : flon,6);
  delay(200);
  gsm.print("\n");
  gsm.print("Address:");
  gsm.print("\n");
  gsm.print("Bike Plate No:");
  gsm.print("\n");
  Serial.print(flat);
  Serial.print("\n");
  Serial.print(flon);
  Serial.print("\n");
  gsm.println((char)26);
  delay(200);
  gsm.println();
  
  

}*/
