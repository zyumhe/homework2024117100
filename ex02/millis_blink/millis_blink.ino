// 作业2：使用millis()实现LED 1Hz闪烁
// 引脚：D2，频率：1Hz（周期1秒，亮灭各0.5秒）
// ex02/millis_blink.ino
// 使用 millis() 实现 LED 以 1Hz 频率闪烁（亮0.5秒，灭0.5秒）

const int ledPin = 2;          // 改成你实际连接的引脚
unsigned long previousMillis = 0;
const long interval = 500;
bool ledState = LOW;

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
  }
}