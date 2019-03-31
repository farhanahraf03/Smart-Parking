#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

Servo servo;

const char* ssid = "abc";
const char* password = "abc6969..";
ESP8266WebServer server(80);


String page = "";
String ps1_text = "";
String ps2_text = "";
String ps1_data = "vacant";
String ps2_data = "vacant";


void setup()
{

  pinMode(D7,INPUT); //Parking slot 1
  pinMode(D3,OUTPUT);

  pinMode(D6,INPUT); //Parking slot 2
  pinMode(D4,OUTPUT);

  pinMode(D0,INPUT); //entry gate IR
  servo.attach(D1); //D1 - entry gate servo
  servo.write(0);
  
  delay(1000);
  Serial.begin(115200);
  WiFi.begin(ssid, password); //begin WiFi connection
  Serial.println("Begin WIFI Connection-----");
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) 
  {
   delay(500);
   Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

   server.on("/ps1_data.txt", [](){
   ps1_text = (String)ps1_data;
   server.send(200, "text/html", ps1_text);
  });

   server.on("/ps2_data.txt", [](){
   ps2_text = (String)ps2_data;
   server.send(200, "text/html", ps2_text);
  });


   server.on("/", [](){
   page = "<h1>Sensor to Node MCU Web Server</h1><h1>Parking slot 1 - :<h1 id=\"data1\">""</h1></h1> <br> <h1>Parking slot 2 - :<h1 id=\"data2\">""</h1></h1>";
   page += "<script>\r";
   page += "var x1 = setInterval(function() {loadData(\"ps1_data.txt\",updateData1)}, 1000);\r";
   page += "var x2 = setInterval(function() {loadData(\"ps2_data.txt\",updateData2)}, 1000);\r";
   page += "function loadData(url, callback){\r";
   page += "var xhttp = new XMLHttpRequest();\r";
   page += "xhttp.onreadystatechange = function(){\r";
   page += " if(this.readyState == 4 && this.status == 200){\r";
   page += " callback.apply(xhttp);\r";
   page += " }\r";
   page += "};\r";
   page += "xhttp.open(\"GET\", url, true);\r";
   page += "xhttp.send();\r";
   page += "}\r";
   page += "function updateData1(){\r";
   page += " document.getElementById(\"data1\").innerHTML = this.responseText;\r";  
   page += "}\r";
   page += "function updateData2(){\r";
   page += " document.getElementById(\"data2\").innerHTML = this.responseText;\r";  
   page += "}\r";
   page += "</script>\r";
   server.send(200, "text/html", page);
   });

 server.begin();
 Serial.println("Web server started!");
 
}

void loop()
{
  int a = digitalRead(D7);
  int b = digitalRead(D6);
  int entry_gate = digitalRead(D0);
  
   ps1_data = String(a);

  if(a == HIGH)
  {
      digitalWrite(D3,LOW);
      ps1_data = "occupied";
  }

  else
  {
      digitalWrite(D3,HIGH);
      ps1_data = "vacant";
  }

  ps2_data = String(b);

  if(b == HIGH)
  {
      digitalWrite(D4,LOW);
      ps2_data = "occupied";
  }

  else
  {
      digitalWrite(D4,HIGH);
      ps2_data = "vacant";
  }

  server.handleClient();

    if(entry_gate == HIGH)
  {
    if(a == LOW || b == LOW)
    {
        servo.write(90);

        delay(2000);
    
        servo.write(0);
    
        delay(2000); 
    }
  }

  


}
