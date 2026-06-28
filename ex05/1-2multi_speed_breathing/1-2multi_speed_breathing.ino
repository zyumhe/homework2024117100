// ex05/multi_speed_breathing.ino
// 多档位触摸调速呼吸灯
// 每触摸一次，速度档位循环切换（1->2->3->1）

#define TOUCH_PIN 4
#define LED_PIN 2
#define THRESHOLD 500
#define DEBOUNCE_MS 200

int speedLevel = 1;               // 当前速度档位（1/2/3）
bool ledState = false;
volatile bool touchFlag = false;
unsigned long lastTouchTime = 0;

// 不同档位对应的步长和延时（档位越高，呼吸越快）
// 档位1：步长2，延时15ms  → 慢速呼吸
// 档位2：步长5，延时10ms  → 中速呼吸
// 档位3：步长10，延时5ms  → 快速呼吸
const int STEPS[] = {0, 2, 5, 10};   // 索引从1开始
const int DELAYS[] = {0, 15, 10, 5};

void gotTouch() {
  touchFlag = true;
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(LED_PIN, OUTPUT);
  touchAttachInterrupt(TOUCH_PIN, gotTouch, THRESHOLD);
  Serial.println("多档位呼吸灯启动，当前档位：1");
}

void loop() {
  // 检测触摸切换档位
  if (touchFlag) {
    if (millis() - lastTouchTime > DEBOUNCE_MS) {
      speedLevel++;
      if (speedLevel > 3) {
        speedLevel = 1;
      }
      Serial.print("切换到档位：");
      Serial.println(speedLevel);
      lastTouchTime = millis();
    }
    touchFlag = false;
  }

  // PWM 呼吸灯效果
  int step = STEPS[speedLevel];
  int delayTime = DELAYS[speedLevel];

  // 渐亮
  for (int duty = 0; duty <= 255; duty += step) {
    analogWrite(LED_PIN, duty);
    delay(delayTime);
    // 在呼吸过程中也检测触摸
    checkTouch();
  }

  // 渐灭
  for (int duty = 255; duty >= 0; duty -= step) {
    analogWrite(LED_PIN, duty);
    delay(delayTime);
    checkTouch();
  }
}

// 在呼吸循环中检测触摸的函数
void checkTouch() {
  if (touchFlag) {
    if (millis() - lastTouchTime > DEBOUNCE_MS) {
      speedLevel++;
      if (speedLevel > 3) {
        speedLevel = 1;
      }
      Serial.print("切换到档位：");
      Serial.println(speedLevel);
      lastTouchTime = millis();
    }
    touchFlag = false;
  }
}