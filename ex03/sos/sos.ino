// 作业3：使用millis()实现LED SOS闪烁信号
// S = 短闪150ms，O = 长闪450ms，循环间隔2秒
// ex03/sos.ino
// 使用 millis() 实现 LED SOS 信号
// S = 短闪（0.15秒）, O = 长闪（0.45秒）
// 同字母内间隔 0.1秒，字母间间隔 0.3秒，循环间停顿 2秒

const int ledPin = 2;
unsigned long previousMillis = 0;
int step = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();
  unsigned long duration;
  bool shouldBeOn;

  switch (step) {
    // 第一个 S（亮）
    case 0: shouldBeOn = true;  duration = 150; break;
    // 第一个 S（灭，字母内间隔）
    case 1: shouldBeOn = false; duration = 100; break;
    // 第二个 S（亮）
    case 2: shouldBeOn = true;  duration = 150; break;
    // 第二个 S（灭，字母内间隔）
    case 3: shouldBeOn = false; duration = 100; break;
    // 第三个 S（亮）
    case 4: shouldBeOn = true;  duration = 150; break;
    // 第三个 S（灭，字母间间隔）
    case 5: shouldBeOn = false; duration = 300; break;

    // 第一个 O（亮）
    case 6: shouldBeOn = true;  duration = 450; break;
    // 第一个 O（灭，字母内间隔）
    case 7: shouldBeOn = false; duration = 100; break;
    // 第二个 O（亮）
    case 8: shouldBeOn = true;  duration = 450; break;
    // 第二个 O（灭，字母内间隔）
    case 9: shouldBeOn = false; duration = 100; break;
    // 第三个 O（亮）
    case 10: shouldBeOn = true; duration = 450; break;
    // 第三个 O（灭，字母间间隔）
    case 11: shouldBeOn = false; duration = 300; break;

    // 第一个 S（亮）
    case 12: shouldBeOn = true;  duration = 150; break;
    // 第一个 S（灭，字母内间隔）
    case 13: shouldBeOn = false; duration = 100; break;
    // 第二个 S（亮）
    case 14: shouldBeOn = true;  duration = 150; break;
    // 第二个 S（灭，字母内间隔）
    case 15: shouldBeOn = false; duration = 100; break;
    // 第三个 S（亮）
    case 16: shouldBeOn = true;  duration = 150; break;
    // 第三个 S（灭，长停顿）
    case 17: shouldBeOn = false; duration = 2000; break;
  }

  if (currentMillis - previousMillis >= duration) {
    previousMillis = currentMillis;
    digitalWrite(ledPin, shouldBeOn ? HIGH : LOW);
    step++;
    if (step > 17) {
      step = 0;
    }
  }
}
