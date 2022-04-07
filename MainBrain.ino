//header file , basically copying the whole file over
// cpp file usually for source code for functions

#include "temp.hpp"
#include "ldr.hpp"
#include "gsr.hpp"
#include "emg.hpp"
#include "RGB.hpp"
#include "TeleApp.hpp"
#include "pulsesensor.hpp"

const int BUZZER = 4 ;
//const int LED_RED = 33 ;
//const int LED_GREEN = 25 ;
//const int LED_BLUE = 32 ;

const unsigned long BOT_MTBS = 1000; // mean time between scan messages
unsigned long bot_lasttime; // last time messages' scan has been done

//void SeizureAlertPhoto(bool onOff) {
//  if (onOff) {
//    Serial.println("Potential Photo Seizure !");
//    bot.sendMessage(CHAT_ID, "Potential Photo Seizure !", "");
//    //   digitalWrite(LED_PURPLE,HIGH);
//    showAnalogRGB(CRGB(0xcc33ff)); //purple color
//    digitalWrite(BUZZER , HIGH);
//  }
//  else {
//    //   digitalWrite(LED_PURPLE,LOW);
//    showAnalogRGB(CRGB::Black);
//    digitalWrite(BUZZER , LOW);
//  }
//}
//
//
//void SeizureAlertTonic(bool onOff) {
//  if (onOff) {
//    Serial.println("Potential Tonic Seizure !");
//    bot.sendMessage(CHAT_ID, "Potential Tonic Seizure !", "");
//    //   digitalWrite(LED_RED,HIGH);
//    showAnalogRGB(CRGB(0xcc0000)); //red color
//    digitalWrite(BUZZER , HIGH);
//  }
//  else {
//    //   digitalWrite(LED_RED,LOW);
//    showAnalogRGB(CRGB::Black);
//    digitalWrite(BUZZER , LOW);
//  }
//}



void setup() {
  Serial.begin(115200);                                            // usual number for esp32(STARTHERE)
                                                      //  pinMode(LED_RED, OUTPUT) ;
  pinMode(BUZZER , OUTPUT) ;
  setup_Wifi();
  DateTime_Setup();
  setup_rgb();
  setup_temp();
  setup_ldr();
  setup_gsr();
  setup_AVGtemp();
  setup_emg();
  log_setup();
  setup_pulsesensor();
}

void loop() {
  //  Serial.println(LDRfreq);
  //  Serial.println(gsr_average);
  //  Serial.println(getTemp()); // '()' because it is a function
  //  if (LDRfreq > FreqThres && (getTemp() > (Get_Avg_Temp() + 1.0))) {
  //    SeizureAlertPhoto(true);
  //  }
  //  else {
  //    SeizureAlertPhoto(false);
  //  }
  //  if (LDR_timeout) {
  //    SeizureAlertPhoto(false);
  //  }
  //
  //  if (Tonic_Seizure == true ) {
  //    SeizureAlertTonic(true);
  //  }
  //  else {
  //    SeizureAlertTonic(false);
  //  }
  //   delay(500);
  //  Serial.print("Tonic_Seizure");
  //  Serial.println(Tonic_Seizure);
  //  delay(500);
  float temp = getTemp(), avgTemp = Get_Avg_Temp();
  Serial.print("Temp=");
  Serial.println(temp);
  Serial.print("AvgTemp=");
  Serial.println(avgTemp);

  if (LDRfreq > FreqThres && (temp > (avgTemp + 1.0)) && (myBPM > (avgBPM + 15.0)) ) {
    digitalWrite(BUZZER , HIGH);
    String storeStr = "PhotoSeizure, \
Temp: " + String(temp) + ", \
AvgTemp: " + String(avgTemp) + ", \
Freq: " + String(LDRfreq) + ", \
Date: " + String(dayStamp) + ", \
Hour: " + String(timeStamp) + "\n";

    bool success = myLog.append(storeStr.c_str(), false);
    if (success) {
      Serial.println("Event stored!");
    } else {
      if (myLog.isFull()) {
        Serial.println("Event NOT stored! You had filled the available space, flush or reset the log");
      } else {
        Serial.println("Event NOT stored!");
      }
    }

    showAnalogRGB(CRGB(0xcc33ff));
    bot.sendMessage(CHAT_ID, "Potential Seizure detected! Please contact patient!", "");

  }
  else if (Tonic_Seizure == true && (temp > (avgTemp + 1.0)) && (myBPM > (avgBPM + 15.0)) ) {
    digitalWrite(BUZZER , HIGH);

    String storeStr = "TonicSeizure, \
Temp: " + String(temp) + ", \
AvgTemp: " + String(avgTemp)+", \
Date: " + String(dayStamp) + ", \
Hour: " + String(timeStamp) + "\n";

    bool success = myLog.append(storeStr.c_str(), false);
    if (success) {
      Serial.println("Event stored!");
    } else {
      if (myLog.isFull()) {
        Serial.println("Event NOT stored! You had filled the available space, flush or reset the log");
      } else {
        Serial.println("Event NOT stored!");
      }
    }

    showAnalogRGB(CRGB(0xcc0000));
    bot.sendMessage(CHAT_ID, "Tonic Seizure detected! Please contact 995!", "");
  }
  else if (LDR_timeout) {
    //    SeizureAlertPhoto(false);
    //    Serial.println("timeout");
    showAnalogRGB(CRGB::Black);
    digitalWrite(BUZZER , LOW);
  }
  else {
    //    SeizureAlertPhoto(false);
    showAnalogRGB(CRGB::Black);
    digitalWrite(BUZZER , LOW);
  }
  if (millis() - bot_lasttime > BOT_MTBS)   // check every 1second for new messages. If current time is more than smth
  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);   //checking if there are new messages annd return no of new messages

    while (numNewMessages)  //keep doing as long as there are messages 
    {
      Serial.println("got response");   
      handleNewMessages(numNewMessages);  //check for what command was written
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    bot_lasttime = millis();
  }

  delay(10);
}
