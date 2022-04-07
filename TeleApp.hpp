#ifndef TeleApp_H
#define TeleApp_H
#include <Arduino.h>
    
#include <logger_spiffs.h>

#include <WiFi.h>

#include <WiFiClientSecure.h>

// Time & Date set up
#include <NTPClient.h>
#include <WiFiUdp.h>

#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>  

// Use @myidbot (IDBot) to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "xxxxxxxxxxxxxxxxxxxxxxxxxx"


// Telegram BOT Token (Get from Botfather)
#define BOT_TOKEN "xxxxxxxxxxxxxxxxxxxxxxxxxxxx"

extern WiFiClientSecure secured_client;
extern UniversalTelegramBot bot;
extern LoggerSPIFFS myLog;

void setup_Wifi(void);
void DateTime_Setup(void);

void log_setup(void);
void handleNewMessages(int numNewMessages);

void bot_setup(void);


//Date and Time Set up
extern String dayStamp;
extern String timeStamp;
#endif
