#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD'nin I2C adresini ve boyutlarını tanımlıyoruz
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Röle pinleri
#define RELAY1_PIN 2
#define RELAY2_PIN 3
#define RELAY3_PIN 4
#define RELAY_PIN1 1  // İşlemler tamamlandığında açılacak röle

// Geri sayım fonksiyonu (millis ile donmadan çalışır)
void displayCountdown(const char* message, int duration) {
    unsigned long startTime = millis();
    int remainingTime = duration;
    
    while (remainingTime >= 0) {
        if (millis() - startTime >= 1000) { // Her saniyede bir güncelle
            startTime = millis();
            // LCD ekranda mesaj ve süreyi göster
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print(message);
            lcd.setCursor(0, 1);
            lcd.print("Sure: ");
            lcd.print(remainingTime);
            
            // Seri port ekranında mesaj ve süreyi göster
            Serial.print(message);
            Serial.print(" - Sure: ");
            Serial.println(remainingTime);

            remainingTime--;
        }
    }
}

void setup() {
    // Seri portu başlat
    Serial.begin(9600);
    lcd.begin();
    lcd.backlight();
    
    // Röle pinlerini çıkış olarak ayarla
    pinMode(RELAY1_PIN, OUTPUT);
    pinMode(RELAY2_PIN, OUTPUT);
    pinMode(RELAY3_PIN, OUTPUT);
    pinMode(RELAY_PIN1, OUTPUT);
    
    // Röleleri başlangıçta kapalı yap
    digitalWrite(RELAY1_PIN, HIGH);
    digitalWrite(RELAY2_PIN, HIGH);
    digitalWrite(RELAY3_PIN, HIGH);
    digitalWrite(RELAY_PIN1, HIGH);

    // LCD'de hoş geldiniz mesajı göster
    lcd.clear();
    lcd.print("Hos Geldiniz!");
    delay(3000);

    // Seri portta da hoş geldiniz mesajı göster
    Serial.println("Hos Geldiniz!");
}

void loop() {
    // 1. Sıvı Verme İşlemi
    digitalWrite(RELAY3_PIN, LOW);
    displayCountdown("1. SIVI VERILIYOR", 1);
    digitalWrite(RELAY3_PIN, HIGH);
    displayCountdown("Bekleniyor", 2);

    // 2. Sıvı Verme İşlemi
    digitalWrite(RELAY2_PIN, LOW);
    displayCountdown("2. SIVI VERILIYOR", 1);
    digitalWrite(RELAY2_PIN, HIGH);
    displayCountdown("Bekleniyor", 2);

    // Su Verme İşlemi
    digitalWrite(RELAY1_PIN, LOW);
    displayCountdown("SU VERILIYOR", 1);
    digitalWrite(RELAY1_PIN, HIGH);

digitalWrite(RELAY1_PIN, HIGH);
    digitalWrite(RELAY2_PIN, HIGH);
    digitalWrite(RELAY3_PIN, HIGH);
    digitalWrite(RELAY_PIN1, HIGH);

    // İşlemler Tamam mesajı ve röleyi açma
    digitalWrite(RELAY_PIN1, LOW);  // Röle açıldı (aktif)
    Serial.println("Islemler Tamam!");

    // 15 saniye boyunca her saniye ekrana "İşlemler Tamam" yazdır
    unsigned long tamamStartTime = millis();
    unsigned long lastUpdate = 0;

    while (millis() - tamamStartTime < 15000) {
        if (millis() - lastUpdate >= 1000) { // Her saniyede bir yenile
            lastUpdate = millis();
            lcd.setCursor(0, 0);
            lcd.print("Islemler Tamam    ");  // Satırı boşluklarla temizleyip yazdır
            Serial.println("Islemler Tamam!");
        }
    }

    digitalWrite(RELAY_PIN1, HIGH);  // Röle kapandı (pasif)
    lcd.setCursor(0, 0);  // LCD'deki yazıyı silmeden konumlandır
    lcd.print("                ");  // Ekranda kalan yazıyı temizle
}
