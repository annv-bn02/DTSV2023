#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
#include "OTA.h"




const char* host = "esp32";
const char* ssid = "artng";
const char* password = "12345678";
int success_flag = 0, success_cnt = 0;  // ledState used to set the LED
String str_ota_state;
WebServer server(80);

/* Style */
String style =
"<style>#file-input,input{width:100%;height:44px;border-radius:4px;margin:10px auto;font-size:15px}"
"input{background:#f1f1f1;border:0;padding:0 15px}body{background:#5bdb34;font-family:sans-serif;font-size:14px;color:#777}"
"#file-input{padding:0;border:1px solid #ddd;line-height:44px;text-align:left;display:block;cursor:pointer}"
"#bar,#prgbar{background-color:#f1f1f1;border-radius:10px}#bar{background-color:#3498db;width:0%;height:10px}"
"form{background:#fff;max-width:258px;margin:75px auto;padding:30px;border-radius:5px;text-align:center}"
".btn{background:#db343c;color:#fff;cursor:pointer}</style>";

/* Login page */
String loginIndex = 
"<form name=loginForm>"
"<h1>ESP32 Login</h1>"
"<input name=userid placeholder='User ID'> "
"<input name=pwd placeholder=Password type=Password> "
"<input type=submit onclick=check(this.form) class=btn value=Login></form>"
"<script>"
"function check(form) {"
"if(form.userid.value=='annv' && form.pwd.value=='admin')"
"{window.open('/serverIndex')}"
"else"
"{alert('Error Password or Username')}"
"}"
"</script>" + style;
 
String serverIndex =
"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
"<input type='file' name='update' id='file' onchange='sub(this)' style=display:none>"
"<label id='file-input' for='file'>Choose file...</label>"
"<input type='submit' class=btn value='Update'>"
"<br><br>"
"<div id='prg'></div>"
"<br><div id='prgbar'><div id='bar'></div></div><br>"
"</form>"
"<script>"
"function sub(obj){"
"var fileName = obj.value.split('\\\\');"
"document.getElementById('file-input').innerHTML = '   '+ fileName[fileName.length-1];"
"};"

"function getData() {"
"  $.get('/data', function(response) {"
"    $('#sensor-value').text(response);"
"  });"
"};"
"setInterval(getData, 1000);"

"$('form').submit(function(e){"
"e.preventDefault();"
"var form = $('#upload_form')[0];"
"var data = new FormData(form);"
"$.ajax({"
"url: '/update',"
"type: 'POST',"
"data: data,"
"contentType: false,"
"processData:false,"
"xhr: function() {"
"var xhr = new window.XMLHttpRequest();"
"xhr.upload.addEventListener('progress', function(evt) {"
"if (evt.lengthComputable) {"
"var per = evt.loaded / evt.total;"
"$('#prg').html('progress: ' + Math.round(per*100) + '%');"
"$('#bar').css('width',Math.round(per*100) + '%');"
"}"
"}, false);"
"return xhr;"
"},"
"success:function(d, s) {"
"console.log('success!');"
"},"
"error: function (a, b, c) {"
"}"
"});"
"});"
"</script>" 
"<p style='background:#fff;max-width:256px;margin:75px auto;padding:30px;border-radius:5px;text-align:center;'>FOTA state: <span id='sensor-value'></span></p>"
+ style;



void send_update_message();
void send_ota_start();
void send_ota_end();
void send_ota_data(uint8_t *data, uint16_t length);
void send_ota_header(int dataLength);
void calculate_fram_data(uint8_t *buf, uint32_t length);
uint16_t CheckSum(uint8_t *buf, uint8_t len);

/* setup function */
void setup(void) {

  Serial.begin(9600);
  Serial2.begin(9600);

  // Connect to WiFi network
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  /*use mdns for host name resolution*/
  if (!MDNS.begin(host)) { //http://esp32.local
    while (1) {
      delay(1000);
    }
  }
  /*return index page which is stored in serverIndex */
  server.on("/", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", loginIndex);
  });
  server.on("/serverIndex", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", serverIndex);
  });
  server.on("/data", HTTP_GET, []() {
  String data = str_ota_state;
  server.send(200, "text/plain", data);
  });
  /*handling uploading firmware file */
  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");

  }, []() {
    HTTPUpload& upload = server.upload();
    // Serial.write(upload.buf, upload.currentSize);
    if(upload.totalSize > 0)
    {
      calculate_fram_data(upload.buf, upload.currentSize);
    }    
  });
  server.begin();
}

void loop(void) {
  server.handleClient();
  if(success_flag)
  {
    success_cnt++;
    if(success_cnt == 10000)
    {
      str_ota_state = " ";
      success_flag = 0;
      success_cnt = 0;
    }
  }
  delay(1);
}
void send_update_message() {
    unsigned char sof = 0xAA;
    unsigned char update_1 = 0x10;
    unsigned char update_2 = 0x11;
    unsigned char eof = 0xBB;

    unsigned char sampleBuffer[4];
    sampleBuffer[0] = sof;
    sampleBuffer[1] = update_1;
    sampleBuffer[2] = update_2;
    sampleBuffer[3] = eof;

    Serial2.write(sampleBuffer, 4);
}

void send_ota_start() {
    unsigned char sof = 0xAA;
    unsigned char packet_type = 0x00;
    unsigned char data_len[2] = {0x00, 0x01};
    unsigned char cmd = 0x00;
    unsigned char eof = 0xBB;

    unsigned char sampleBuffer[8];
    sampleBuffer[0] = sof;
    sampleBuffer[1] = packet_type;
    sampleBuffer[2] = data_len[1];
    sampleBuffer[3] = data_len[0];
    sampleBuffer[4] = cmd;

    // Calculate and set crc
    data_format_uint8_16_t input_convert;
    input_convert.data_uint16 = CheckSum(sampleBuffer, 5);
    sampleBuffer[5] = input_convert.bytes[0];
    sampleBuffer[6] = input_convert.bytes[1];
    sampleBuffer[7] = eof;

    Serial2.write(sampleBuffer, 8);
}

void send_ota_end() {
    unsigned char sof = 0xAA;
    unsigned char packet_type = 0x00;
    unsigned char data_len[2] = {0x00, 0x01};
    unsigned char cmd = 0x01;
    unsigned char eof = 0xBB;

    unsigned char sampleBuffer[8];
    sampleBuffer[0] = sof;
    sampleBuffer[1] = packet_type;
    sampleBuffer[2] = data_len[1];
    sampleBuffer[3] = data_len[0];
    sampleBuffer[4] = cmd;

    // Calculate and set crc
    data_format_uint8_16_t input_convert;
    input_convert.data_uint16 = CheckSum(sampleBuffer, 5);
    sampleBuffer[5] = input_convert.bytes[0];
    sampleBuffer[6] = input_convert.bytes[1];
    sampleBuffer[7] = eof;

    Serial2.write(sampleBuffer, 8);
}

void send_ota_header(int dataLength) {
    unsigned char sof = 0xAA;
    unsigned char packet_type = 0x02;
    unsigned char data_len[2] = {0x00, 0x04};
    // unsigned char sizedata[4] = Convert_From_Int_To_Bytes(dataLength);
    // Convert dataLength to bytes and store in sizedata

    unsigned char eof = 0xBB;

    unsigned char sampleBuffer[11];
    sampleBuffer[0] = sof;
    sampleBuffer[1] = packet_type;
    sampleBuffer[2] = data_len[1];
    sampleBuffer[3] = data_len[0];
    data_format_int_bytes input_convert;
    input_convert.data_int = dataLength;
    sampleBuffer[4] = input_convert.bytes[0];
    sampleBuffer[5] = input_convert.bytes[1];
    sampleBuffer[6] = input_convert.bytes[2];
    sampleBuffer[7] = input_convert.bytes[3];
    // Calculate and set crc
    data_format_uint8_16_t uint16_convert;
    uint16_convert.data_uint16 = CheckSum(sampleBuffer, 8);
    sampleBuffer[8] = uint16_convert.bytes[0];
    sampleBuffer[9] = uint16_convert.bytes[1];
    sampleBuffer[10] = eof;

    Serial2.write(sampleBuffer, 11);
}

void send_ota_data(uint8_t *data, uint16_t length) {
    unsigned int index = 0;

    unsigned char sof = 0xAA;
    unsigned char packet_type = 0x01;
    // unsigned char data_len[2] = Convert_From_Uint16_To_Bytes(length);
    // Convert length to bytes and store in data_len

    unsigned char eof = 0xBB;

    unsigned int bufferLength = 7 + length;
    unsigned char sampleBuffer[bufferLength];
    sampleBuffer[0] = sof;
    sampleBuffer[1] = packet_type;
    data_format_uint8_16_t input_convert;
    input_convert.data_uint16 = length;
    sampleBuffer[2] = input_convert.bytes[0];
    sampleBuffer[3] = input_convert.bytes[1];
    index = 4;

    for (unsigned int i = 0; i < length; i++) {
        sampleBuffer[index] = data[i];
        index++;
    }

    // Calculate and set crc
    input_convert;
    input_convert.data_uint16 = CheckSum(sampleBuffer, index);
    sampleBuffer[index] = input_convert.bytes[0];
    sampleBuffer[index + 1] = input_convert.bytes[1];
    sampleBuffer[index + 2] = eof;

    Serial2.write(sampleBuffer, index + 3);
}

uint16_t CheckSum(uint8_t *buf, uint8_t len)
{
	uint16_t crc = 0xFFFF, pos = 0, i = 0;
	for (pos = 0; pos < len; pos++)
	{
		crc ^= (uint16_t)buf[pos]; // XOR byte into least sig. byte of crc
		for (i = 8; i != 0; i--)   // Loop over each bit
		{
			if ((crc & 0x0001) != 0) // If the LSB is set
			{
				crc >>= 1; // Shift right and XOR 0xA001
				crc ^= 0xA001;
			}
			else // Else LSB is not set
			{
				crc >>= 1; // Just shift right
			}
		}
	}
	return crc;
}

void calculate_fram_data(uint8_t *buf, uint32_t length) {
    unsigned int chia = length / 128;
    unsigned int index = 0;
    send_update_message();
    Serial.print("\n\n\n<<<<<<Send_update_message>>>>>>>>\n");
    delay(2000);

    send_ota_start();
    Serial.print("<<<<<<<<Send_ota_start>>>>>>>>\n");
    delay(2000);

    send_ota_header((chia + 1) * 128);
    Serial.print("<<<<<<<Send_ota_header>>>>>>>>\n");
    delay(2000);

    Serial.print("<<<<<<<<<<,Send_ota_data>>>>>>>>>>\n");
    if (chia * 128 == length) {
        unsigned int bufferLength = 128;
        uint8_t sampleBuffer[bufferLength];
        for (unsigned int i = 0; i < chia; i++) {
            for (unsigned int j = 0; j < 128; j++) {
                sampleBuffer[j] = buf[index];
                index++;
            }
            send_ota_data(sampleBuffer, 128);
            Serial.println(i);
            delay(100);
        }
    } else {
        unsigned int bufferLength = 128;
        uint8_t sampleBuffer[bufferLength];
        for (unsigned int i = 0; i < chia + 1; i++) {
            if (i < chia) {
                for (unsigned int j = 0; j < 128; j++) {
                    sampleBuffer[j] = buf[index];
                    index++;
                }
            } else {
                unsigned int t = length - (chia * 128);
                unsigned int count = 0;
                for (unsigned int j = 0; j < 128; j++) {
                    if (count < t) {
                        sampleBuffer[j] = buf[index];
                    } else {
                        sampleBuffer[j] = 0xFF;
                    }
                    index++;
                    count++;
                }
            }
            send_ota_data(sampleBuffer, 128);
            Serial.println(i);
            delay(100);
        }
    }
    delay(2000);
    // Delay for 2.5 seconds
    Serial.print("<<<<<<Send_ota_end>>>>>>>\n");
    send_ota_end();
    str_ota_state = "Success";
    success_flag = 1;
  }