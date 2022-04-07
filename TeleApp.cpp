

#include "TeleApp.hpp"    

#include <FS.h>  //required to take out csv file
#include <SPIFFS.h>

char file_name[] = "/seziureLog.csv"; 
File myFile;   //file = class , myfile= object , initializing object 
LoggerSPIFFS myLog(file_name);  //


// Wifi network station credentials
#define WIFI_SSID "xxxxxxxxx"
#define WIFI_PASSWORD "xxxxxxxxx"

// Time & Date set up
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Variables to save date and time
String formattedDate;
String dayStamp;
String timeStamp;


WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

void send_log_csv(void);

void DateTime_loop(void * parameter);

void DateTime_Setup(void){
  // Initialize a NTPClient to get time  Wi-Fi Setup
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800 Singapore
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(28800);
  
   xTaskCreate(
    DateTime_loop,    // Function that should be called
    "Date & Time task",   // Name of the task (for debugging)
    10000,            // Stack size (bytes)
    NULL,            // Parameter to pass
    1,               // Task priority
    NULL             // Task handle
  );
}

void DateTime_loop(void * parameter) {
  while(1){
   while(!timeClient.update()) {
      timeClient.forceUpdate();
    }
    // The formattedDate comes with the following format:
    // 2018-05-28T16:00:13Z
    // We need to extract date and time
    formattedDate = timeClient.getFormattedDate();
//    Serial.println(formattedDate);
  
    // Extract date
    int splitT = formattedDate.indexOf("T");
    dayStamp = formattedDate.substring(0, splitT);
//    Serial.print("DATE: ");
//    Serial.println(dayStamp);
    // Extract time
    timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
//    Serial.print("HOUR: ");
//    Serial.println(timeStamp);
    delay(1000);
  }
}
telegramMessage msg;
void handleNewMessages(int numNewMessages)
{
  Serial.print("handleNewMessages ");
  Serial.println(numNewMessages);
  
  String answer;
  for (int i = 0; i < numNewMessages; i++)   // loop through every single messages
  {
//    telegramMessage &msg = bot.messages[i];
    msg = bot.messages[i];   
    Serial.println("Received " + msg.text);
    if (msg.text == "/get_history"){
//      bot.sendMessage(msg.chat_id, "test", "");
      send_log_csv();   
      myLog.flush(); //clear log and send
    }
    else if (msg.text == "/emergency"){
      answer = "STAY with the person!\nTime how long the seizures lasts, especially the stiffening and jerking movements.\nHelp the person lie down, and place something soft under the head and neck.\nTurn the person onto one SIDE with the head and mouth angled toward the ground.\nThis helps saliva or fluids to drain out of the mouth.\nIt also prevents the tongue from blocking the airway.\nDo not put any object in the person's mouth.\nThe tongue cannot be swallowed during a seizure, yet you could get hurt.\nThe muscles for chewing are very strong, so a finger can be bitten, or an object can be bitten off and cause choking. ";
       bot.sendMessage(msg.chat_id, answer, "Markdown");
    }
  }
}


void bot_setup(void)
{
  const String commands = F("["
                            "{\"command\":\"emergency\",  \"description\":\"What to do during a seizure episode\"},"
                            "{\"command\":\"get_history\", \"description\":\"Past seizure records\"}"
                            "]");
  
//  F("["
//                            "{\"command\":\"emergency\", \"description\":\"What to do during a seizure episode\"},"
//                            "{\"command\":\"get_history\", \"description\":\"Past seizure records\"}"
//                            "]");
  bot.setMyCommands(commands);
  //bot.sendMessage("25235518", "Hola amigo!", "Markdown");
}


bool isMoreDataAvailable()
{
  return myFile.available();
}

byte getNextByte()
{
  return myFile.read();
}


bool senderHelp(char* buffer, int n){  //buffer is where they store in log file , converting byte array to string 
  int index=0;
  Serial.println("send log");
//  myFile = SPIFFS.open(file_name);
  
    String sendThis="";
  // Check if there is another string to print
  while(index<n && strlen(&buffer[index])>0){
    Serial.print("---");
    int byteSent=0;
    Serial.print(&buffer[index]);
    
    while(buffer[index+byteSent]){
      sendThis += buffer[index+byteSent];
      byteSent++;
    }
    sendThis += "\n";
  
    Serial.println("---");
    // +1, the '\0' is processed
    index += byteSent+1;
  }
  bot.sendMessage(msg.chat_id, sendThis, "");
//  if (myFile)
//  {
//    Serial.print(file_name);
//    Serial.print("....");
//
//    String sent = bot.sendFileByBinary(msg.chat_id, "text/csv", myFile.size(), file_name,
//                                        isMoreDataAvailable,
//                                        getNextByte, nullptr, nullptr);
//
//    if (sent)
//    {
//      Serial.println("was successfully sent");
//    }
//    else
//    {
//      Serial.println("was not sent");
//    }
//
//    myFile.close();
//  }
//  else
//  {
//    // if the file didn't open, print an error:
//    Serial.println("error opening file");
//  }
 
//bot.sendMessage(CHAT_ID, sendThis, "");
  return true;
}

void send_log_csv(void){
  myFile = SPIFFS.open(file_name);  //SPIFFS =  a place to store stuff in esp32
    if (myFile) // if opened successfully 
  {
    Serial.print(file_name); 
    Serial.print("....");

    String sent = bot.sendFileByBinary(msg.chat_id, "text/csv", myFile.size(), file_name,   //sending file to telegram modified codes
                                        isMoreDataAvailable,
                                        getNextByte, nullptr, nullptr);

    if (sent)
    {
      Serial.println("was successfully sent");
    }
    else
    {
      Serial.println("was not sent");
    }

    myFile.close();
  }
  else
  {
    // if the file didn't open, print an error:
    Serial.println("error opening file");
  }
}


void log_setup(void)
{
   myLog.begin();
//  myLog.setSizeLimit(100);
  myLog.setFlusherCallback(senderHelp);
}

void setup_Wifi(void) {
//  Serial.begin(115200);

 // attempt to connect to Wifi network:
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Retrieving time: ");
  configTime(0, 0, "pool.ntp.org"); // get UTC time via NTP
  time_t now = time(nullptr);
  while (now < 24 * 3600)
  {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);
  bot_setup();
//  setup_Commands();
  bot.sendMessage(CHAT_ID, "MAASFEP Bot started up, please connect inputs.", "");
}
