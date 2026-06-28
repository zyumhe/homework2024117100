// ex06/police_strobe.ino
// 警车双闪灯效 - 两个LED反相渐变闪烁
// LED_A 渐亮时 LED_B 渐灭，反之亦然

#define LED_A_PIN 2    // 灯A
#define LED_B_PIN 5    // 灯B（使用PWM引脚）

const int STEP = 2;          // 每次增减步长
const int DELAY_TIME = 15;   // 每步延时（ms）

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(LED_A_PIN, OUTPUT);
  pinMode(LED_B_PIN, OUTPUT);
  Serial.println("警车双闪灯效启动");
}

void loop() {
  // 阶段1：A渐亮（0→255），B渐灭（255→0）
  for (int duty = 0; duty <= 255; duty += STEP) {
    analogWrite(LED_A_PIN, duty);           // A 越来越亮
    analogWrite(LED_B_PIN, 255 - duty);     // B 越来越暗（反相）
    delay(DELAY_TIME);
  }

  // 阶段2：A渐灭（255→0），B渐亮（0→255）
  for (int duty = 255; duty >= 0; duty -= STEP) {
    analogWrite(LED_A_PIN, duty);           // A 越来越暗
    analogWrite(LED_B_PIN, 255 - duty);     // B 越来越亮（反相）
    delay(DELAY_TIME);
  }
}