#include <WiFi.h>
#include <WebServer.h>

// ========== 配置项目 ==========
const char* ssid = "羽目禾的Xiaomi 14";      
const char* password = "Zrc819726@";  

const int ledPin = 2;          // LED引脚，对应板载LED GPIO2
const int freq = 5000;         // PWM频率，参考lab3默认5000Hz
const int resolution = 8;      // PWM分辨率，8位对应范围0-255

WebServer server(80);          // Web服务器，端口80

// ========== 网页页面处理 ==========
void handleRoot() 
{
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32 无极调光器</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      text-align: center;
      margin-top: 80px;
    }
    .container {
      max-width: 420px;
      margin: 0 auto;
      padding: 20px;
    }
    #brightnessSlider {
      width: 100%;
      margin: 25px 0;
      height: 8px;
    }
    #valueShow {
      font-size: 28px;
      font-weight: bold;
      color: #2c3e50;
    }
  </style>
</head>
<body>
  <div class="container">
    <h1>LED 无极调光器</h1>
    <p>拖动滑动条实时调节 LED 亮度</p >
    <input type="range" min="0" max="255" value="0" id="brightnessSlider">
    <p>当前亮度值：<span id="valueShow">0</span></p >
  </div>

  <script>
    const slider = document.getElementById('brightnessSlider');
    const valueShow = document.getElementById('valueShow');

    // 监听滑动条拖动事件，实时发送调光请求
    slider.addEventListener('input', function() {
      const dutyValue = this.value;
      valueShow.textContent = dutyValue;
      // 异步发送GET请求, 不阻塞页面滑动
      fetch(`/set?duty=${dutyValue}`)
        .catch(error => console.log('调光请求失败:', error));
    });
  </script>
</body>
</html>
)rawliteral";
  server.send(200, "text/html; charset=UTF-8", html);
}

// ========== 调光接口处理 ==========
void handleSetDuty() 
{
  // 从URL参数中获取亮度值
  String dutyStr = server.arg("duty");
  int dutyValue = dutyStr.toInt();

  // 范围校验，防止超出8位PWM的取值范围
  if (dutyValue < 0) dutyValue = 0;
  if (dutyValue > 255) dutyValue = 255;

  // 设置PWM占空比，控制LED亮度
  ledcWrite(ledPin, dutyValue);

  // 返回当前亮度值作为响应
  server.send(200, "text/plain", String(dutyValue));
}

// ========== 初始化 ==========
void setup() 
{
  Serial.begin(115200);

  // 配置PWM：直接绑定引脚、频率、分辨率（参考lab3新版用法）
  ledcAttach(ledPin, freq, resolution);
  ledcWrite(ledPin, 0); // 初始状态LED熄灭

  // 连接WiFi（参考lab5连接逻辑）
  WiFi.begin(ssid, password);
  Serial.print("正在连接WiFi");
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi连接成功");
  Serial.print("网页访问地址: http://");
  Serial.println(WiFi.localIP());

  // 注册Web路由
  server.on("/", handleRoot);        // 根路径：返回调光页面
  server.on("/set", handleSetDuty);  // 调光接口：接收亮度值

  server.begin();
  Serial.println("Web服务器已启动");
}

// ========== 主循环 ==========
void loop() 
{
  server.handleClient(); // 处理客户端请求
}
