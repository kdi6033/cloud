#include <AWS_IOT.h>
#include <WiFi.h>
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`
#include "images.h"

AWS_IOT hornbill;
SSD1306  display(0x3c, 4, 15);

char ssid[]="***";
char password[]="***";
char HOST_ADDRESS[]="***";
char CLIENT_ID[]= "client id";
char outTopic[] = "inTopic"; 
char inTopic[] = "outTopic"; 

int status = WL_IDLE_STATUS;
int tick=0,msgCount=0,msgReceived = 0;
char payload[512];
char rcvdPayload[512];
String sIP="0.0.0.0";

void mySubCallBackHandler (char *topicName, int payloadLen, char *payLoad)
{
    strncpy(rcvdPayload,payLoad,payloadLen);
    rcvdPayload[payloadLen] = 0;
    msgReceived = 1;
}

void DisplaySetup() {
  pinMode(16,OUTPUT);
  digitalWrite(16, LOW);    // set GPIO16 low to reset OLED
  delay(50); 
  digitalWrite(16, HIGH); // while OLED is running, must set GPIO16 in high
  // Initialising the UI will init the display too.
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
}

void drawFontFaceDemo() {
    display.clear();
    //ss="";
    //ss="count: ";
    //ss=ss+count;
    Serial.println(rcvdPayload);
    display.drawString(0, 0, rcvdPayload);
    display.display();
}

void setup() {
  DisplaySetup();
  Serial.begin(115200);
  delay(2000);

  while (status != WL_CONNECTED)
  {
      Serial.print("Attempting to connect to SSID: ");
      Serial.println(ssid);
      // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
      status = WiFi.begin(ssid, password);
      // wait 5 seconds for connection:
      delay(5000);
  }
  sIP=WiFi.localIP().toString();
  Serial.println(sIP);

  Serial.println("Connected to wifi");

  if(hornbill.connect(HOST_ADDRESS,CLIENT_ID)== 0)
  {
      Serial.println("Connected to AWS");
      delay(1000);

      if(0==hornbill.subscribe(inTopic,mySubCallBackHandler))
      {
          Serial.println("Subscribe Successfull");
      }
      else
      {
          Serial.println("Subscribe Failed, Check the Thing Name and Certificates");
          while(1);
      }
  }
  else
  {
      Serial.println("AWS connection failed, Check the HOST Address");
      while(1);
  }

  delay(2000);

}

void loop() {
    drawFontFaceDemo();
    if(msgReceived == 1)
    {
        msgReceived = 0;
        Serial.print("Received Message:");
        Serial.println(rcvdPayload);
    }
    if(tick >= 20)   // publish to topic every 5seconds
    {
        tick=0;
        sprintf(payload,"Hello ESP32 : %d",msgCount++);
        if(hornbill.publish(outTopic,payload) == 0)
        {        
            Serial.print("Publish Message:");
            Serial.println(payload);
        }
        else
        {
            Serial.println("Publish failed");
        }
    }  
    vTaskDelay(1000 / portTICK_RATE_MS); 
    tick++;
}

String toStringIp(IPAddress ip) {
  String res = "";
  for (int i = 0; i < 3; i++) {
    res += String((ip >> (8 * i)) & 0xFF) + ".";
  }
  res += String(((ip >> 8 * 3)) & 0xFF);
  return res;
}
