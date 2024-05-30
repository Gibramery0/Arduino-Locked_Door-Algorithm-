#include "Keypad.h"        // Tuş takımı için gerekli kütüphane
#include <Wire.h>           // Haberleşme için gerekli kütüphane
#include <EEPROM.h>    // Girilen şifrenin arduino belleğinde tutulması için gerekli kütüphane
#include <LiquidCrystal_I2C.h>         //LCD ekran için gerekli kütüphane
#define buzzer 12                               //Buzzerin bulunduğu pin değişkeni

int led=10;                                              //Yeşil Led 13. pine giricek
int led2=11;                                           //Kırmızı Led 12. pine giricek
LiquidCrystal_I2C lcd(0x27, 16, 2);                    //LCD  ekranın pinleri   

char okunan=0;                                       //Kullanılan değişkenler
const byte satir = 4; //4 satir
const byte sutun = 3; //3 sutun
char password[4];
char pass[4],pass1[4];
int i=0;

char rakamlar[satir][sutun] = {                // Keypadin tuşlarını tanımlama
{'1','2','3'},
{'4','5','6'},
{'7','8','9'},
{'*','0','#'}
};
byte satirpin[satir] = {8, 7, 6, 5};     // Tanımlanan tuşlar için satır pinleri
byte sutunpin[sutun] = {4, 3, 2};     // Tanımlanan tuşlar için sutun pinleri

Keypad butonlar = Keypad(makeKeymap(rakamlar), satirpin, sutunpin, satir, sutun);                     // Çağırma


void setup()  // Arduino ilk çalıştığında yapılması gereken işlemler ve bulundukları pinler
{

  lcd.init();
  lcd.backlight();
  pinMode(led, OUTPUT);           // Yeşil ledin arduinonun 13 pindeki çıkısı
  pinMode(led2, OUTPUT);         // Kırmızı ledin arduinonun 13 pindeki çıkısı
  pinMode(buzzer, OUTPUT);     //Buzzerin yukarıdaki degiskende belirtiği 15 pin yeri
  lcd.print(" Elektronik ");             //2 bölümden oluşan ledin üst kısmında yazan yazı
  lcd.setCursor(0, 1);                      //LCD nin alt kısmına geçiş 
  lcd.print(" Sifreli Kilit ");           //2 bölümden oluşan ledin alt kısmında yazan yazı 
  delay(2000);                               //2   bekleme süresi
  lcd.clear();                                  //LCD ekranın temizlenmesi
  lcd.print("Sifreyi Girin: ");        

 /*bu kod ilk şifre belirleniken aktif edilecektir.
 for(int j=0;j<4;j++)
 EEPROM.write(j, j+49);
 for(int j=0;j<4;j++)
 pass[j]=EEPROM.read(j);
*/

}
void loop()     // Ana Fonksiyon
{
 okunan = butonlar.getKey();  // Girilen tuşların okunması 
 if(okunan=='#')
 sifredegisim(); // # basıldığında şifre değiştirmesi için aşagıda bulunan fonksiyonu çağıracak
 if (okunan)
 {
 password[i++]=okunan;  
 lcd.print(okunan);
 tussesi();
 }
 if(i==4)
 {
 delay(200);                                   // bekleme süresi
 for(int j=0;j<4;j++)
 pass[j]=EEPROM.read(j);           // şifrenin eeproma kaydedilmesi
 if(!(strncmp(password, pass,4))) // karsılastırma 2 string ifadeyi
 {
 digitalWrite(led, HIGH);             // Yeşil ledin yanması
 tussesi();                                               // tışa basılırken buzzer sesi

 lcd.clear();                                   //  lcd ekranını sıfırlayıp

 lcd.print("Sifre onaylandi!");      // Ekrana yazını yazılması
 delay(2000); 
 lcd.setCursor(0,1);                       // lcd ekranın alt kızmı
 lcd.clear();
 lcd.print("Sifreyi Girin:");
 lcd.setCursor(0,1);
 i=0;
 digitalWrite(led, LOW);               // ledin sönmesi
 }

 else
 {
 digitalWrite(buzzer, HIGH);        // buzzer sesi
 lcd.clear(); 
 lcd.print("Tekrar deneyin...");
 digitalWrite(led2, HIGH);            // Kırmızı ledin yanması 
 lcd.setCursor(0,1); 
 delay(1000);
 digitalWrite(led2, LOW);              // Kırmızı ledin sönmesi 
 lcd.clear();
 lcd.print("Sifreyi girin:");
 lcd.setCursor(0,1);
 i=0;
 digitalWrite(buzzer, LOW);          // buzzerin susması
 }
 }
}
void sifredegisim()                        // Şifre değişim Fonksiyonu
{
 int j=0;
 lcd.clear();
 lcd.print("Gecerli Parola Girin ");  //Mevcut şifreyi girmemizi istiyor
 lcd.setCursor(0,1);
 while(j<4)
 {
 char key=butonlar.getKey();
 if(key)
 {
 pass1[j++]=key;
 lcd.print(key);
 tussesi();
 }
 key=0;
 }
 delay(500);

 if((strncmp(pass1, pass, 4)))           // karşılaştırma aynıysa mesaj veriyor
 {
 lcd.clear();
 lcd.print("Hatali...");
 digitalWrite(led2, HIGH);
 lcd.setCursor(0,1);
 lcd.print("Tekrar deneyin");
 delay(1000);
 digitalWrite(led2, LOW);
 }
 else
 {
 j=0;

 lcd.clear();
 lcd.print("Yeni Parola Girin:");        //Yeni parola giriyoruz
 lcd.setCursor(0,1);
 while(j<4)
 {
 char key=butonlar.getKey();
 if(key)
 {
 pass[j]=key;
 lcd.print(key);
 EEPROM.write(j,key);
 j++;
 tussesi();
 }
 }
 lcd.print(" Basarili......");   
 delay(1000);
 }
 lcd.clear();
 lcd.print("Sifreyi Girin:");
 lcd.setCursor(0,1);
 okunan=0;
}
void tussesi()                      // tusa basıldığında buzzerin ses çıkarma fonksiyonu
{
 digitalWrite(buzzer, HIGH);
 delay(30);
 digitalWrite(buzzer, LOW);
}

