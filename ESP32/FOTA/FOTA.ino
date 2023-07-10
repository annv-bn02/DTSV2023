#include <ESP32CAN.h>
#include <CAN_config.h>
#include "UDHttp.h"
#include "FS.h"
#include "SPIFFS.h"

#define CAN_CHUNK_SIZE     200
#define CAN_MSG_RESET     0x62
#define CAN_MSG_START     0x40
#define CAN_MSG_DATA      0x63
#define CAN_MSG_WAIT      0x4D
#define CAN_MSG_CONT      0x58
#define CAN_MSG_DONE      0x60

#define DOWNLOAD_HOST     "192.168.1.3"
/* the variable name CAN_cfg is fixed, do not change */
CAN_device_t CAN_cfg;
const char* ssid     = "artng";
const char* password = "12345678";

int state = 0;
File root;
UDHttp udh;

int wdataf(uint8_t *buffer, int len){
  //write downloaded data to file
  return root.write(buffer, len);
}

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("- failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("\tSIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

bool checkUpdate(){
  
  String download = "";
  String cur_ver = "";
  
  if(SPIFFS.exists("/SW.txt")){
    root = SPIFFS.open("/SW.txt", FILE_READ);
    if(!root) {
      Serial.println("error opening");
    }
    while (root.available()) {
      char ch = (char)root.read();
      if(ch != ';') {
        cur_ver += ch;
      } else {
        break;
      }
    }
    root.close();
  } else {
    cur_ver = "dummy";
  }
  Serial.println("cur_ver: " + cur_ver);
  root = SPIFFS.open("/NSW.txt", FILE_WRITE);
  if(!root){
    Serial.println("failed to open file for writing");
    return false;
  }
  int r = udh.download("http://"DOWNLOAD_HOST":8000/NSW.txt", "/NSW.txt", wdataf, NULL);
  if(r == -1)
  {
    Serial.println("error downloading");
    return false;
  }
  root.close();

  root = SPIFFS.open("/NSW.txt");
  if(!root) {
    Serial.println("error opening");
    return false;
  }
  bool check = false;
  while (root.available()) {
    char ch = (char)root.read();
    if(ch != ';') {
      download += ch;
    } else {
      if(check == false){
        check = true;
        Serial.println("download_ver: " + download);
        if(download != cur_ver){
          download = "";
          SPIFFS.remove("/SW.txt");
          SPIFFS.rename("/NSW.txt", "/SW.txt");
        } else {
          download = "";
          break;
        }
      }
    }
  }
  root.close();
  listDir(SPIFFS, "/", 0);
  Serial.println(download);
  if(download == ""){
    Serial.println("no update");
    return false;
  } else {
    root = SPIFFS.open("/App1.bin", FILE_WRITE);
    if(!root){
      Serial.println("failed to open file for writing");
      return false;
    }
    int r = udh.download((char *)download.c_str(), "App1.bin", wdataf, NULL);
    if(r == -1)
    {
      Serial.println("error downloading");
    }
    root.close();
    Serial.println("done downloading");
    
  }
  return true;
}

void setupCan(){
      /* set CAN pins and baudrate */
    CAN_cfg.speed=CAN_SPEED_500KBPS;
    CAN_cfg.tx_pin_id = GPIO_NUM_5;
    CAN_cfg.rx_pin_id = GPIO_NUM_4;
    /* create a queue for CAN receiving */
    CAN_cfg.rx_queue = xQueueCreate(10,sizeof(CAN_frame_t));
    //initialize CAN Module
    ESP32Can.CANInit();
}

void printBuf(uint8_t *buf, uint8_t len){
  for(int i = 0; i < len; i++){
    printf("%d\t", buf[i]);
  }
  printf("\n");
}

void sendFrame(uint8_t *buf, uint32_t id, uint8_t len){
    CAN_frame_t tx_frame;
    tx_frame.FIR.B.FF = CAN_frame_std;
    tx_frame.MsgID = id;
    tx_frame.FIR.B.DLC = len;
    if(buf != NULL && len> 0){
      memcpy(tx_frame.data.u8, buf, len);
    }
    ESP32Can.CANWriteFrame(&tx_frame);
}

int updateState(int curstate){
    CAN_frame_t rx_frame;
    if(xQueueReceive(CAN_cfg.rx_queue, &rx_frame, 3*portTICK_PERIOD_MS)==pdTRUE){
        if(rx_frame.FIR.B.FF == CAN_frame_std){
          printf("Msg 0x%08x %d\t",rx_frame.MsgID, rx_frame.FIR.B.DLC);
          printBuf(rx_frame.data.u8, rx_frame.FIR.B.DLC);
          if(rx_frame.MsgID == CAN_MSG_WAIT){
            return 4;
          } else if(rx_frame.MsgID == CAN_MSG_CONT){
            return 3;
          } else if(rx_frame.MsgID == CAN_MSG_START){
            return 2;
          }
      }
    }
    return curstate;
}


void setup() {
    Serial.begin(115200);
    
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    if(!SPIFFS.begin(true)){
      Serial.println("SPIFFS Mount Failed");
      return;
    }
    setupCan();
    state = 0;
}
int pre = 0;
uint8_t buf[8];
uint8_t idx = 0;
        
void loop() {
  if(pre != state){
    Serial.println(state);
    pre = state;
  }
  
  switch(state){
      case 0: {
        checkUpdate();
        state = 1;
      }
      break;
      case 1: {
        sendFrame(NULL, CAN_MSG_RESET, 0);
        state = 100;
      }
      break;
      case 2: {
        root = SPIFFS.open("/App1.bin", "rb");
        if(!root) {
          Serial.println("error opening");
          state = 100;
        } else {
          state = 3;
        }
      }
      break;
      case 3: {
        if (root.available()) {
          buf[idx] = root.read();
          idx++;
        } else {
          if(idx > 0){
            //printBuf(buf, idx);
            sendFrame(buf, CAN_MSG_DATA, idx);
          }
          root.close();
          Serial.println("done");
          sendFrame(NULL, CAN_MSG_DONE, 0);
          state = 100;
          break;
        }
        if(idx == 8){
          sendFrame(buf, CAN_MSG_DATA, idx);
          idx = 0;
        }
      }
      break;
      case 4: {
        delay(50);
      }
      break;  
      default:
      break;
  }
  state = updateState(state);
}
