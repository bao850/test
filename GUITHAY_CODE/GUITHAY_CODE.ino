#include <WiFi.h>
#include <FirebaseESP32.h>

// Thông tin WiFi
#define WIFI_SSID "IoT Lab"
#define WIFI_PASSWORD "IoT@123456"

// Thông tin Firebase
#define FIREBASE_HOST "https://tt-iot-uteee-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "CR8pNdfjyUp8V3TdKdrEuEpidiobmWKG0r1SRvA8"

// Chân GPIO của ESP32
int counter = 0;

// Khởi tạo Firebase
FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;
String path = "/";

void setup() {
  pinMode(2, OUTPUT); // Cài đặt chân LED là OUTPUT
  digitalWrite(2, LOW); // Tắt LED ban đầu
  Serial.begin(115200); // Khởi tạo Serial Monitor với baud rate 115200
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // Kết nối WiFi
  // Chờ kết nối WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected.");

  // Cấu hình Firebase
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;

  // Kết nối Firebase
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  // Gửi biến counter lên Firebase
  if (Firebase.setInt(firebaseData, "/counter", counter)) {
    Serial.println("Counter sent to Firebase");
  } else {
    Serial.print("Failed to send counter: ");
    Serial.println(firebaseData.errorReason());
  }

  // Nhận lệnh điều khiển đèn từ Firebase
  if (Firebase.getBool(firebaseData, "/ledStatus")) {
    bool ledStatus = firebaseData.boolData();
    digitalWrite(2, ledStatus ? HIGH : LOW);
    Serial.println(ledStatus ? "LED is ON" : "LED is OFF");
  } else {
    Serial.print("Failed to get ledStatus: ");
    Serial.println(firebaseData.errorReason());
  }
  Serial.print("Counter value: ");
  Serial.println(counter);

  // Tăng biến đếm và đợi 1 giây
  counter++;
  delay(1000);
}