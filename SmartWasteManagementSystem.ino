#include <Wire.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
const int echoPin = 2; // Echo Pin của cảm biến siêu âm
const int pingPin = 3; // Trigger Pin của cảm biến siêu âm
const int threshold = 50; // Ngưỡng mức rác
const int ledPin = 13;
const int wetThreshold = 800; // Ngưỡng độ ẩm cho rác ướt
const int dryThreshold = 400; // Ngưỡng độ ẩm cho rác khô
long duration; int cm;
float latitude = -1.6848579, longitude = 37.1690756;
SoftwareSerial SIM900(9, 10);
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);
TinyGPSPlus gps; // Tạo một đối tượng TinyGPS++
float lat = -1.6848579, lon = 37.1690756;
void setup()
{
  Serial.begin(9600); // tôc độ Baud Rate (bits/s)//Bắt đầu giao tiếp Serial
  SIM900.begin(9600);
  delay(100);
  pinMode(pingPin, OUTPUT); // Khởi tạo chân 3 là đầu ra
  pinMode(echoPin, INPUT);  // Khởi tạo chân 2 là đầu vào
  Serial.println("                      GIAM SAT THUNG RAC TU XA");
  Serial.println("_______________________________________________________________________");
  SIM900.println("         SENDING...          ");
  lcd.begin(20, 4); ///dong va cot cua LCD
  lcd.setCursor(0, 2);
  lcd.print("Phan loai:");
}
void loop()
{
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin, LOW);
  duration = pulseIn(echoPin, HIGH); // Sử dụng pulseIn để tính thời gian tổng cộng trong micro giây
  cm = (duration * 0.034) / 2;     // Chuyển đổi từ micro giây sang mm
  cm = constrain(cm, 1, 120);
  lcd.setCursor(0, 0);
  lcd.print("Muc rac: "); //HIEN THI MUC RAC:
  lcd.print(120 - cm);
  lcd.setCursor(15, 0);
  lcd.print("cm");
  for (unsigned long start = millis(); millis() - start < 1000;) //thiet lap VI TRI THUNG RAC
  {
    while (Serial.available() > 0) {
      char c = Serial.read();
      if (gps.encode(c)) {
        if (gps.location.isValid()) // Kiểm tra dữ liệu vị trí hợp lệ
        {
          lat = gps.location.lat();
          lon = gps.location.lng();
        }}}}
  guiSMS();
  if (cm > 100) {
    lcd.setCursor(0, 1);
    lcd.println("    welcome     ");}
  if (cm < 10) {
    lcd.setCursor(0, 1);
    lcd.println("thung rac da day");}
  if (cm<100 and cm>10) {
    lcd.setCursor(0, 1);
    lcd.println("                ");}
  lcd.setCursor(13, 2);
  int moistureValue = analogRead(A0);
  if (moistureValue >= wetThreshold){
    lcd.print("Rac uot");}
  else if (moistureValue <= dryThreshold){
    lcd.print("Rac kho");}
  else {
    lcd.print("                ");}
  delay(100);
}
void guiSMS()// Hàm gửi tin nhắn thông báo và điều khiển hiển thị LCD
{
  if (cm < 10) {
    SIM900.println("Sending to: 0971763191");
    Serial.println("SMS from: 0965968397");
    Serial.println("!!!!!!THUNG RAC DA DAY!!!!!!");
    Serial.print("Muc rac trong thung:");
    Serial.print(120 - cm);
    Serial.println(" cm");
    Serial.print("vi tri cua thung rac day: [");
    Serial.print("vi do: ");
    Serial.print(lat, 6);
    Serial.print(",");
    Serial.print("kinhdo: ");
    Serial.print(lon, 6);
    Serial.println("]");
    Serial.println((char)26);
    Serial.println();
    delay(1000);}
  if (cm > 100) {
    SIM900.println("Sending to: 0971763191");
    Serial.println("SMS from: 0965968397");
    Serial.println("!!!!!!THUNG RAC DA DUOC DO!!!!!!");
    Serial.print("Muc rac trong thung:");
    Serial.print(120 - cm);
    Serial.println(" cm");
    Serial.print("vi tri cua thung rac da duoc do: [");
    Serial.print("vi do: ");
    Serial.print(lat, 6);
    Serial.print(",");
    Serial.print("kinhdo: ");
    Serial.print(lon, 6);
    Serial.println("]");
    Serial.println((char)26);
    Serial.println();
    delay(1000);}
}
