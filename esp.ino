
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char *ssid = "ESP-AP ";
const char *password = "12345678";

String start;

ESP8266WebServer server(80);

void handleRoot() {
  
if(digitalRead(BUILTIN_LED))//  проверка состояниея пина
{
  start ="0";
  }
else
{
  start ="1";
  }  
String out = R"=====(
<!DOCTYPE html>
<html>
<head>
<meta charset = "utf-8">
<meta name='viewport' content='width=device-width, initial-scale=1.0'/>
<title>Управление LED и AJAX запрос</title>
<style type="text/css">
.button {
  background-color: #4CAF50; 
  border: none;
  color: white;
  border-radius: 6px;
  padding: 12px 24px;
  text-align: center;
  text-decoration: none;
  display: inline-block;
  font-size: 15px;
}


.button:hover, .button:visited {
  font-weight: bold; 
  color: black;
}



</style>
</head>
<body style="background-color: #cccccc; Color: blue; ">
<center>
<div>
<h1>Управление индикатором</h1>
  <button class="button" onclick="send(1)">ВКЛ.</button>
  <button class="button" onclick="send(0)">ВЫКЛ.</button><BR>
</div>
 <br>
<div><h2>
  LED Статус: <span id="state"></span>
</h2>
<BR>
<h3>Отправить: <br>
<input type="text" length=10 id="valdat"/>
<button type="button" onclick="sendData()">отправить</button>
</h3>
<BR>
<h3>Время работы: <br>
<span style="display: inline-block; width: 200px; height: 20px; border: 1px solid black; background-color: white; color: black;" id="readdat"></span><br><br>

</h3>
</div>
<script>

function loadDoc()
{
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("state").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "led_set?state="+)=====";
  
  out+= start; 
  
  out+= R"=====(, true);
  xhttp.send();
}

function send(led_sts) 
{
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("state").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "led_set?state="+led_sts, true);
  xhttp.send();
}

function sendData() 
{
  var xhttp = new XMLHttpRequest();
  document.getElementById("valdat")
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("valdat").innerHTML = this.responseText;
          }
  };
  xhttp.open("GET", "but_st?value=" + document.getElementById("valdat").value, true);
  xhttp.send();
}
setInterval(function() 
{
  getData();
}, 1000); 

function getData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("readdat").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "read", true);
  xhttp.send();
}

loadDoc();

</script>
</center>
</body>
</html>
)=====";


   server.send ( 200, "text/html", out );
}


 
void read_data() 
{
  String value = server.arg("value");
  Serial.println(value);   
}

void send_data() 
{
 int timer = millis()/1000;
 String key = String(timer);
 //String key = value;
 server.send(200, "text/plane", key);
}

void led_control() 
{
 
 String state;
 String act_state = server.arg("state");

 if(act_state == "1")
 {
  digitalWrite(LED_BUILTIN,LOW); //LED ON
  state = "Вкл.";
 }
 
 else
 {
  digitalWrite(LED_BUILTIN,HIGH); //LED OFF
  state = "Выкл.";
 }
 server.send(200, "text/plane", state);
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  
  Serial.println();
  Serial.print("Configuring access point...");
  WiFi.softAP(ssid, password);
     pinMode(LED_BUILTIN, OUTPUT);
     digitalWrite(LED_BUILTIN,HIGH); //изначальный параметр индикатоа
  IPAddress myIP = WiFi.softAPIP();
 
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.on("/led_set", led_control);
  server.on("/but_st", read_data);
  server.on("/read", send_data);
  server.begin();
  Serial.println("HTTP server started");
}



void loop() {
  server.handleClient();
}
