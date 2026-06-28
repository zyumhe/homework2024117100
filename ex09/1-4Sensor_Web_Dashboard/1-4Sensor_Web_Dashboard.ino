#include <WiFi.h>
#include <WebServer.h>

// ========== 配置项目 ==========
const char* ssid = "羽目禾的Xiaomi 14";
const char* password = "Zrc819726@";

const int TOUCH_PIN = 4;  
WebServer server(80);

// ========== 仪表盘的主页面 ==========
void handleRoot() 
{
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>实时触摸传感器仪表盘</title>
  <style>
    * {
      margin: 0;
      padding: 0;
      box-sizing: border-box;
    }
    body {
      font-family: Arial, sans-serif;
      background-color: #f5f7fa;
      display: flex;
      justify-content: center;
      align-items: center;
      min-height: 100vh;
    }
    .dashboard {
      background: white;
      padding: 60px 80px;
      border-radius: 16px;
      box-shadow: 0 4px 20px rgba(0,0,0,0.1);
      text-align: center;
    }
    h1 {
      color: #2c3e50;
      margin-bottom: 40px;
      font-size: 24px;
    }
    .value-box {
      font-size: 96px;
      font-weight: bold;
      color: #3498db;
      margin: 20px 0;
      font-family: "Courier New", monospace;
    }
    .tip {
      color: #7f8c8d;
      font-size: 14px;
      margin-top: 20px;
    }
  </style>
</head>
<body>
  <div class="dashboard">
    <h1>触摸传感器实时数值</h1>
    <div class="value-box" id="touchValue">--</div>
    <p class="tip">手指靠近引脚时数值变小，离开后恢复</p >
  </div>

  <script>
    // AJAX 定时轮询，每 100ms 获取一次最新数值
    function updateValue() {
      fetch("/api/touch")
        .then(response => response.text())
        .then(value => {
          document.getElementById("touchValue").textContent = value;
        })
        .catch(err => console.log("获取数据失败:", err));
    }

    // 页面加载后立即执行，之后每 100ms 刷新一次
    updateValue();
    setInterval(updateValue, 100);
  </script>
</body>
</html>
)rawliteral";
  server.send(200, "text/html; charset=UTF-8", html);
}

// ========== 数据接口：返回触摸传感器实时数值 ==========
void handleGetTouch() 
{
  int value = touchRead(TOUCH_PIN);
  // 纯文本返回数值，方便前端解析
  server.send(200, "text/plain", String(value));
}

// ========== 初始化 ==========
void setup() 
{
  Serial.begin(115200);
  delay(1000);

  // 连接 WiFi（参考 lab5 Web服务器连接逻辑）
  WiFi.begin(ssid, password);
  Serial.print("正在连接WiFi");
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n WiFi连接成功");
  Serial.print("仪表盘访问地址: http://");
  Serial.println(WiFi.localIP());

  // 注册路由
  server.on("/", handleRoot);               // 根路径：返回仪表盘页面
  server.on("/api/touch", handleGetTouch);  // 数据接口：返回触摸数值

  server.begin();
  Serial.println("Web仪表盘已启动");
}

// ========== 主循环 ==========
void loop() 
{
  server.handleClient(); // 处理客户端请求
}