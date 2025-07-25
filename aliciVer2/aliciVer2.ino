#include <WiFi.h>
#include <esp_now.h>

int m12_r = D0;
int m12_l = D1;
int m34_r = D2;
int m34_l = D3;
int m5_r = D4;
int m5_l = D5;
int m6_r = D6;
int m6_l = D7;

uint8_t gelenKomut = 0;

void onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  if (len > 0) {
    gelenKomut = incomingData[0]; // İlk byte'ı al
    Serial.print("Gelen Komut (raw uint8): ");
    Serial.println(gelenKomut); // Sadece sayı olarak
  }
}

void setup() {
  delay(2000); // 🛠 ESP kararlı açılması için gecikme
  Serial.begin(9600);

  pinMode(m12_r, OUTPUT);
  pinMode(m12_l, OUTPUT);
  pinMode(m34_r, OUTPUT);
  pinMode(m34_l, OUTPUT);
  pinMode(m5_r, OUTPUT);
  pinMode(m5_l, OUTPUT);
  pinMode(m6_r, OUTPUT);
  pinMode(m6_l, OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW başlatılamadı!");
    return;
  }

  esp_now_register_recv_cb(onDataRecv);
  Serial.println("ESP-NOW Alıcı Hazır.");
}

void loop() {
  if (gelenKomut != 0) {
    Serial.print("İşlenecek Komut: ");
    Serial.println(gelenKomut);
    
    switch (gelenKomut) {
      case 1: Serial.println("İleri komutu alındı"); ileri(); break;
       case 2: Serial.println("Geri komutu alındı"); geri(); break;
         case 3: Serial.println("Sağ komutu alındı"); sol(); break;
            case 4: Serial.println("Sol komutu alındı"); sag(); break;
              case 5: Serial.println("ileri sag komutu alındı"); sagileri(); break;
              case 6: Serial.println("ileri sol komutu alındı"); solileri(); break;
           case 7: Serial.println("geri sag komutu alındı"); saggeri(); break;
         case 8: Serial.println("geri Sol komutu alındı"); solgeri(); break;
        case 9: Serial.println("Dur komutu alındı"); dur(); break;
      default: Serial.println("Bilinmeyen komut"); break;
    }

    gelenKomut = 0; // Resetle
  }
  delay(50);
}

void ileri() {
  analogWrite(m12_r, 255); analogWrite(m34_r, 0);
  analogWrite(m5_r, 255); analogWrite(m6_r, 0);
  analogWrite(m12_l, 0);   analogWrite(m34_l, 255);
  analogWrite(m5_l, 0);    analogWrite(m6_l, 255);
}

void geri() {
  analogWrite(m12_r, 0);   analogWrite(m34_r, 255);
  analogWrite(m5_r, 0);    analogWrite(m6_r, 255);
  analogWrite(m12_l, 255); analogWrite(m34_l, 0);
  analogWrite(m5_l, 255);  analogWrite(m6_l, 0);
}

void sag() {
  analogWrite(m12_r, 255); analogWrite(m34_r, 255);
  analogWrite(m5_r, 255);  analogWrite(m6_r, 255);
  analogWrite(m12_l, 0);   analogWrite(m34_l, 0);
  analogWrite(m5_l, 0);    analogWrite(m6_l, 0);
}

void sol() {
  analogWrite(m12_r, 0);   analogWrite(m34_r, 0);
  analogWrite(m5_r, 0);    analogWrite(m6_r, 0);
  analogWrite(m12_l, 255); analogWrite(m34_l, 255);
  analogWrite(m5_l, 255);  analogWrite(m6_l, 255);
}

void sagileri() {
  analogWrite(m12_r, 0); analogWrite(m34_r, 0);
  analogWrite(m5_r, 0);  analogWrite(m6_r, 0);
  analogWrite(m12_l, 0); analogWrite(m34_l, 0);
  analogWrite(m5_l, 0);  analogWrite(m6_l, 0);
}

void dur() {
  analogWrite(m12_r, 0); analogWrite(m34_r, 0);
  analogWrite(m5_r, 0);  analogWrite(m6_r, 0);
  analogWrite(m12_l, 0); analogWrite(m34_l, 0);
  analogWrite(m5_l, 0);  analogWrite(m6_l, 0);
}


void saggeri() {
  analogWrite(m12_r, 0); analogWrite(m34_r, 0);
  analogWrite(m5_r, 0);  analogWrite(m6_r, 0);
  analogWrite(m12_l, 0); analogWrite(m34_l, 0);
  analogWrite(m5_l, 0);  analogWrite(m6_l, 0);
}

void solileri() {
  analogWrite(m12_r, 0); analogWrite(m34_r, 0);
  analogWrite(m5_r, 0);  analogWrite(m6_r, 0);
  analogWrite(m12_l, 0); analogWrite(m34_l, 0);
  analogWrite(m5_l, 0);  analogWrite(m6_l, 0);
}

void solgeri() {
  analogWrite(m12_r, 0); analogWrite(m34_r, 0);
  analogWrite(m5_r, 0);  analogWrite(m6_r, 0);
  analogWrite(m12_l, 0); analogWrite(m34_l, 0);
  analogWrite(m5_l, 0);  analogWrite(m6_l, 0);
}