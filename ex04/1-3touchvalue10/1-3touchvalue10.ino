#define TOUCH_PIN 4
#define LED_PIN 2
#define THRESHOLD 500    // 触摸触发阈值，需根据实际硬件调试
#define DEBOUNCE_MS 200 // 防抖时间，单位毫秒

bool ledState = false;
volatile bool touchFlag = false; // 中断标志位，volatile 防止编译器优化
unsigned long lastTouchTime = 0; // 上次有效触摸时间，用于防抖

// 中断服务函数 - 仅做标记，不执行业务逻辑
void gotTouch() {
  touchFlag = true;
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, ledState); // 初始化LED状态
  
  // 绑定触摸中断：引脚、中断函数、触发阈值
  touchAttachInterrupt(TOUCH_PIN, gotTouch, THRESHOLD);
}

void loop() {
  // 检测触摸中断标志
  if (touchFlag) {
    // 防抖判断：距离上次触发超过防抖时间，才执行状态翻转
    if (millis() - lastTouchTime > DEBOUNCE_MS) {
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
      Serial.print("LED状态: ");
      Serial.println(ledState ? "点亮" : "熄灭");
      
      lastTouchTime = millis(); // 更新触摸时间
    }
    touchFlag = false; // 清除中断标志
  }
  
  // 主循环可添加其他任务
  delay(10);
}