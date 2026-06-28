#include <WiFi.h>
#include <WebServer.h>

// ========== 硬件引脚定义 ==========
const int TOUCH_PIN = 4;    // 触摸引脚 T0 -> GPIO4（参考 lab4 触摸实验）
const int LED_PIN = 2;      // LED引脚 GPIO2（参考 lab2 基础IO、lab5 Web控制）
const int TOUCH_THRESHOLD = 500; // 触摸触发阈值，低于该值判定为触摸，可按实际校准

// ========== WiFi配置 ==========
const char* ssid = "羽目禾的Xiaomi 14";
const char* password = "Zrc819726@";

WebServer server(80);

// ========== 系统状态变量 ==========
bool isArmed = false;    // 布防状态：true=已布防，false=已撤防
bool isAlarm = false;    // 报警状态：true=报警触发中

// ========== LED非阻塞闪烁变量 ==========
unsigned long previousMillis = 0;
const long blinkInterval = 100; // 报警闪烁间隔，数值越小闪烁越快
bool ledState = false;

// ========== 生成网页页面 ==========
String makePage() 
{
  String statusText;
  if (isAlarm) 
  {
    statusText = "<span style='color:red; font-weight:bold;'>报警中</span>";
  } 
  else if (isArmed) 
  {
    statusText = "<span style='color:orange; font-weight:bold;'>已布防</span>";
  } 
  else 
  {
    statusText = "<span style='color:green; font-weight:bold;'>已撤防</span>";
  }
  
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>物联网安防报警器</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      text-align: center;
      margin-top: 80px;
    }
    .container {
      max-width: 400px;
      margin: 0 auto;
      padding: 20px;
    }
    .btn {
      padding: 12px 30px;
      font-size: 18px;
      margin: 10px;
      border: none;
      border-radius: 5px;
      cursor: pointer;
    }
    .btn-arm {
      background-color: #e74c3c;
      color: white;
    }
    .btn-disarm {
      background-color: #2ecc71;
      color: white;
    }
    .status {
      font-size: 24px;
      margin: 30px 0;
    }
  </style>
</head>
<body>
  <div class="container">
    <h1>物联网安防报警器</h1>
    <div class="status">当前状态：)rawliteral" + statusText + R"rawliteral(</div>
    <a href="/arm"><button class="btn btn-arm">布防 Arm</button></a >
    <a href="/disarm"><button class="btn btn-disarm">撤防 Disarm</button></a >
  </div>
</body>
</html>
)rawliteral";
  return html;
}

// ========== Web路由处理 ==========
// 根路径: 返回主页面
void handleRoot() {
  server.send(200, "text/html; charset=UTF-8", makePage());
}

// 布防接口：开启警戒，重置报警状态
void handleArm() 
{
  isArmed = true;
  isAlarm = false;
  digitalWrite(LED_PIN, LOW);
  server.sendHeader("Location", "/");
  server.send(303); // 重定向回主页
}

// 撤防接口：解除警戒，停止报警
void handleDisarm() 
{
  isArmed = false;
  isAlarm = false;
  digitalWrite(LED_PIN, LOW);
  server.sendHeader("Location", "/");
  server.send(303);
}

// ========== 初始化 ==========
void setup() 
{
  Serial.begin(115200);
  delay(1000);
  
  // 初始化LED引脚为输出模式（参考 lab2 基础IO）
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // 初始状态熄灭
  
  // 连接WiFi（参考 lab5 Web服务器）
  WiFi.begin(ssid, password);
  Serial.print("正在连接WiFi");
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi连接成功");
  Serial.print("网页访问地址：http://");
  Serial.println(WiFi.localIP());
  
  // 注册Web路由
  server.on("/", handleRoot);
  server.on("/arm", handleArm);
  server.on("/disarm", handleDisarm);
  server.begin();
  Serial.println("安防报警器系统已启动");
}

// ========== 主循环 ==========
void loop() 
{
  // 处理Web客户端请求（必须持续执行，保证页面响应）
  server.handleClient();
  
  // 1. 布防且未报警时，检测触摸信号（参考 lab4 触摸检测）
  if (isArmed && !isAlarm) 
  {
    int touchValue = touchRead(TOUCH_PIN);
    // 触摸值低于阈值，触发报警
    if (touchValue < TOUCH_THRESHOLD) 
    {
      isAlarm = true;
      Serial.println("检测到触碰，触发报警");
    }
  }
  
  // 2. 报警状态下，LED高频闪烁（非阻塞实现，不影响Web响应）
  if (isAlarm) 
  {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= blinkInterval) 
    {
      previousMillis = currentMillis;
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
    }
  }
  
  delay(10);
}