#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <fauxmoESP.h>
fauxmoESP fauxmo;

/*
 * Connect an LED strip of lights to ESP8266 Wifi Module to connect to Alexa
 * Old alexa works with Belkin emulation, new one does not so Fauxmo uses Philips Hue emulation instead
 * 
 * @author credit given to original creator Xose Pérez
 * @author contributions made by Kristina Kacmarova
 * Modifications made to fauxmoESP_Basic.ino from: https://bitbucket.org/xoseperez/fauxmoesp/src/master/
 * Additions made: void loop(), void christmasLightsOn(), void lightsOff(), boolean connectWifi()
 */

// IP Address
IPAddress ip(192, 168, 0, 247); 
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0); 
const char* ssid = "Insert Your Own";
const char* password = "Insert Your Own";

// use GPIO
#define LED_BLUE            5 // D1
#define LED_RED             4 // D2
#define LED_GREEN           0 // D3
#define BTN_BLUE            14 // D5
#define BTN_RED             12 // D6
#define BTN_GREEN           13 // D7

#define ID_BLUE             "blue lights"
#define ID_RED              "red lights"
#define ID_GREEN            "green lights"
#define ID_CHRISTMAS        "christmas lights"

#define OFF HIGH
#define ON LOW
#define BTN_OFF HIGH
#define BTN_ON LOW

int redValue;
int greenValue;
int blueValue;
int Xmas = 0;

byte stateB = OFF; 
byte stateR = OFF; 
byte stateG = OFF; 

/*
 * Setup Wifi
 */
void setup() { 
  
  Serial.begin(115200);
  
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  digitalWrite(LED_BLUE, OFF);
  digitalWrite(LED_RED, OFF);
  digitalWrite(LED_GREEN, OFF);
  pinMode(BTN_BLUE, INPUT_PULLUP);
  pinMode(BTN_RED, INPUT_PULLUP);
  pinMode(BTN_GREEN, INPUT_PULLUP);
    
  if (connectWifi()) {

     fauxmo.enable(false); 
     Serial.println("Adding LED devices");
     fauxmo.addDevice(ID_BLUE);
     fauxmo.addDevice(ID_RED);
     fauxmo.addDevice(ID_GREEN);
     fauxmo.addDevice(ID_CHRISTMAS);

     fauxmo.setPort(80);  
     fauxmo.enable(true); 

     fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
        /Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
        
        if (strcmp(device_name, ID_BLUE)==0) {
          if (Xmas == 1) lightsOff(); // state is true then turn on
          digitalWrite(LED_BLUE, state ? ON : OFF);
        } 
        else if (strcmp(device_name, ID_RED)==0) {
          if (Xmas == 1) lightsOff(); // state is true then turn on
          digitalWrite(LED_RED, state ? ON : OFF);
        } 
        else if (strcmp(device_name, ID_GREEN)==0) {
          if (Xmas == 1) lightsOff(); // state is true then turn on
          digitalWrite(LED_GREEN, state ? ON : OFF);
        } 
        else if (strcmp(device_name, ID_CHRISTMAS)==0) {
          if (state) { // state is true then turn on
              christmasLightsOn();
          } 
          else { // state is false then turn off
              lightsOff();
          }
        } 
    }); // end of fauxmo.onSetState
  } // end of if (connectWifi())
  Serial.println("Exit Setup...");
} 

/*
 * Start loop
 */
void loop() {
  fauxmo.handle();
  
  if (Xmas == 1) {
    
    #define delayTime 5 // fading time between colors

    // fades out red bring green full when i=255
    redValue = 255; 
    greenValue = 0;
    blueValue = 0;
    
    for(int i = 0; i < 255; i += 1) {
      redValue -= 1;
      greenValue += 1; 
      if (Xmas == 0) return;    
      digitalWrite(LED_RED, redValue);
      digitalWrite(LED_GREEN, greenValue);
      delay(delayTime);
    }

    // fades out green bring blue full when i=255
    redValue = 0;
    greenValue = 255;
    blueValue = 0;
    
    for(int i = 0; i < 255; i += 1) {
      greenValue -= 1;
      blueValue += 1;
      if (Xmas == 0) return;  
      digitalWrite(LED_GREEN, greenValue);
      digitalWrite(LED_BLUE, blueValue);
      delay(delayTime);
    }

    // fades out blue bring red full when i=255
    redValue = 0;
    greenValue = 0;
    blueValue = 255;
    
    for(int i = 0; i < 255; i += 1) {
      blueValue -= 1;
      redValue += 1;
      if (Xmas == 0) return;  
      digitalWrite(LED_BLUE, blueValue);
      digitalWrite(LED_RED, redValue);
      delay(delayTime);
    }
  } // end of if (Xmas == 1) 

  // if the state of the btn is not pressed
  if (stateB == OFF) {
    // if the btn is pressed
    if (digitalRead(BTN_BLUE) == BTN_ON) {
      stateB = ON; // now the state of the button is pressed
      // if btn is pressed and and light is off then turn light on
      if (digitalRead(LED_BLUE) == OFF) {
        digitalWrite(LED_BLUE, ON); 
      } else {
          // if btn is pressed and and light is on then turn light off
        digitalWrite(LED_BLUE, OFF);
      }
    }
  } else {
    // if the state of the btn is pressed
    if (digitalRead(BTN_BLUE) == BTN_OFF) {
      // if the btn is not pressed (released), will be pressed a second time
      stateB = OFF; // now the state of the button is not pressed
    }   
  }

  // if the state of the btn is not pressed
  if (stateR == OFF) {
    // if the btn is pressed
    if (digitalRead(BTN_RED) == BTN_ON) {
      stateR = ON; // now the state of the button is pressed
      // if btn is pressed and and light is off then turn light on
      if (digitalRead(LED_RED) == OFF) {
        digitalWrite(LED_RED, ON); 
      } else {
          // if btn is pressed and and light is on then turn light off
          digitalWrite(LED_RED, OFF);
      }
    }
  } else {
    // if the state of the btn is pressed
    if (digitalRead(BTN_RED) == BTN_OFF) {
      // if the btn is not pressed (released), will be pressed a second time
      stateR = OFF; // now the state of the button is not pressed
    }   
  }

  // if the state of the btn is not pressed
  if (stateG == OFF) {
    // if the btn is pressed
    if (digitalRead(BTN_GREEN) == BTN_ON) {
      stateG = ON; // now the state of the button is pressed
      // if btn is pressed and and light is off then turn light on
      if (digitalRead(LED_GREEN) == OFF) {
        digitalWrite(LED_GREEN, ON); 
      } else {
          // if btn is pressed and and light is on then turn light off
          digitalWrite(LED_GREEN, OFF);
      }
    }
  } else {
    if (digitalRead(BTN_GREEN) == BTN_OFF) {
      // if the btn is not pressed (released), will be pressed a second time
      stateG = OFF; // now the state of the button is not pressed
    }   
  }
} // end of void loop()

/*
 * Turn christmas lights on
 */
void christmasLightsOn() {
  Serial.print("Christmas lights turn on ...");
  Xmas = 1;
}

/*
 * Turn lights off
 */
void lightsOff() {
  Serial.print("All lights turn off ...");
  Xmas = 0;
  digitalWrite(LED_BLUE, OFF);
  digitalWrite(LED_RED, OFF);
  digitalWrite(LED_GREEN, OFF);
}

/*
 * connect to wifi – returns true if successful or false if not
 */
boolean connectWifi() {
  
  boolean state = true;
  int i = 0;
  
  WiFi.config(ip, gateway, subnet);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");
  
  // Wait for connection
  Serial.print("Connecting ...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 10) {
      state = false;
      break;
    }
    i++;
  }
  
  if (state) {
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  
  else {
    Serial.println("");
    Serial.println("Connection failed.");
  }
  return state;
}
