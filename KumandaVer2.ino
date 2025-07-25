#include <WiFi.h>
#include <esp_now.h>
#include <Deneyap_KumandaKolu.h>

Joystick KumandaKolu2;  // Joystick 1 - Ana yön
Joystick KumandaKolu3;  // Joystick 2'nin Y'si
Joystick KumandaKolu4;  // Joystick 2'nin X'i

uint8_t peer[] = {0x7C, 0x9E, 0xBD, 0xD8, 0x8C, 0xAC};

uint8_t komut = 9;
uint8_t onceki_komut = 255;

const int merkez = 500;
const int esik = 150;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Gönderim durumu: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Başarılı" : "Başarısız");
}

void setup() {
  Serial.begin(115200);
  
  KumandaKolu2.begin(0x1A);
  KumandaKolu3.begin(0x1C);
  KumandaKolu4.begin(0x1D);

  WiFi.mode(WIFI_STA);
  delay(100);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW başlatılamadı!");
    while (1);
  }

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, peer, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Peer eklenemedi!");
    while (1);
  }

  esp_now_register_send_cb(OnDataSent);

  Serial.println("Başarılı şekilde başlatıldı");
}

void loop() {
  // Joystick 1 verileri (Deneyap)
  uint16_t joy1X = abs(KumandaKolu2.xRead()-1024);
  uint16_t joy1Y = abs(KumandaKolu2.yRead()-1024);

  // Joystick 2 verileri (Deneyap)
  uint16_t joy2Y = abs(KumandaKolu3.yRead()-1024);
  uint16_t joy2X = KumandaKolu4.yRead();

  // Yön belirleme - Deneyap joystick ortası 500, eşik 100
  bool ileri = joy2Y > (merkez + esik);   // 400’den küçük
  bool geri  = joy2Y < (merkez - esik);   // 600’den büyük
  bool sol   = joy2X < (merkez - esik);   // 400’den küçük
  bool sag   = joy2X > (merkez + esik);   // 600’den büyük

  uint8_t yeni_komut;

  if (ileri && sag)        yeni_komut = 5;
  else if (ileri && sol)   yeni_komut = 6;
  else if (geri && sag)    yeni_komut = 7;
  else if (geri && sol)    yeni_komut = 8;
  else if (ileri)          yeni_komut = 1;
  else if (geri)           yeni_komut = 2;
  else if (sag)            yeni_komut = 3;
  else if (sol)            yeni_komut = 4;
  else                     yeni_komut = 9;  // Nötr

  if (yeni_komut != onceki_komut) {
    komut = yeni_komut;
    esp_err_t result = esp_now_send(peer, &komut, sizeof(komut));
    Serial.print("Gönderilen Komut: ");
    Serial.println(komut);
    onceki_komut = komut;
  }

  Serial.print("Joy1 X: "); Serial.print(joy1X);
  Serial.print("\tY: "); Serial.print(joy1Y);
  Serial.print("\tJoy2 X: "); Serial.print(joy2X);
  Serial.print("\tJoy2 Y: "); Serial.print(joy2Y);
  Serial.print("\tKomut: "); Serial.println(komut);

  delay(100);
}
