#include "web.h"
const char* host = "esp32";

WebServer server(80);
int success_flag = 0, success_cnt = 0, ota_cnt = 0; 
uint8_t buffer_ota[10000], flag_ota = 0;
uint32_t buffer_ota_length = 0;
/* Style */
String style =
"<style>#file-input,input{width:100%;height:44px;border-radius:4px;margin:10px auto;font-size:15px}"
"input{background:#f1f1f1;border:0;padding:0 15px}body{background:#D8DBBD;font-family:sans-serif;font-size:14px;color:#777}"
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
"if(form.userid.value=='admin' && form.pwd.value=='admin')"
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
HTTPUpload upload_struct;
void Web_Setup()
{
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
    upload_struct = server.upload();
    // Serial.write(upload.buf, upload.currentSize);
    Serial.println(String(buffer_ota_length) + " " + String(upload_struct.totalSize) + " " + String(upload_struct.currentSize));
    for(int i = 0; i  < upload_struct.currentSize; i++)
    {
      buffer_ota[buffer_ota_length + i] = upload_struct.buf[i];
      // printf("%p ", buffer_ota[buffer_ota_length + i]);
      flag_ota = 1;
      ota_cnt = 0;
    }
    buffer_ota_length += upload_struct.currentSize;
  });
  server.begin();
}

void Web_Run()
{
    server.handleClient();
    if(success_flag == 1)
    {
        success_cnt++;
        if(success_cnt == 10000)
        {
            str_ota_state = " ";
            success_flag = 0;
            success_cnt = 0;
            buffer_ota_length = 0;
            ota_cnt = 0;
        }
    }
    if(flag_ota == 1)
    {
      ota_cnt++;
      if(ota_cnt > 100)
      {
        // for(int i = 0; i < upload_struct.totalSize; i++)
        // {
        //   printf("%p ", buffer_ota[i]);
        //   delay(1);
        // }
        // delay(10000);
        Ota_Run(buffer_ota, upload_struct.totalSize);
      }
    }

}
