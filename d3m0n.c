#include "Keyboard.h"
#include <stdio.h>
#include "i2c-display-lib.h"
#include "d3m0n.h"
#include <string.h>
#include <WiFi.h>


//setup buttons
int button_up = 20;
int button_ok = 19;
int button_down = 18;

char ssid[] = "D3M0N-X1";
char pass[] = "changeme";
String mac_address;
String ip_address;

WiFiServer server(80);


void setup() {
  Serial.begin(9600); 
  Serial1.begin(9600);
  Serial.print("yo");
  
  //setup keyboard
  Keyboard.begin();
  //setup lcd display
  lcd_setAddr(0x3f);
  lcd_init(16, 17);

  //setup ap
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  boolean result = WiFi.softAP(ssid, pass);
  if(result){
    ip_address=IpAddress2String(WiFi.localIP());
    display("IP address:", 0);
    display(ip_address, 1);
    mac_address = WiFi.softAPmacAddress().c_str();
    //setup server
    server.begin(80);
  }else{
    display("AP setup Failed", 0);
  }
  
  pinMode(button_up, INPUT_PULLUP); //up
  pinMode(button_ok, INPUT_PULLUP); //ok
  pinMode(button_down, INPUT_PULLUP); //down

  delay(1000);
  display("    D3M0N-X1    ", 0);
  display("  by 4RE5 team  ", 1);
  delay(2000);

  d3m0n_begin();
}

void loop(){
 WiFiClient client = server.available();  // Listen for incoming clients
 String header;
 if (client) 
 {
    String currentLine = "";               // make a String to hold incoming data from the client
    while (client.connected()) {  // loop while the client's connected
      if (client.available()) {  // if there's bytes to read from the client,
        char c = client.read();  // read a byte, then
        Serial.write(c);         // print it out the serial monitor
        header += c;
        if (c == '\n') {  // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            // turns the GPIOs on and off
            if (header.indexOf("GET /LED_BUILTIN/ON") >= 0) 
            {
              Serial.println("LED_BUILTIN ON");
            } 
            else if (header.indexOf("GET /LED_BUILTIN/OFF") >= 0) 
            {
              Serial.println("LED_BUILTIN OFF");
            }
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head> <title>Pico W Web Server</title> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html {font-family: Helvetica; display:inline-block; margin: 0px auto; text-align: center;}");
            client.println(" h1{color: #0F3376; padding: 2vh;} p{font-size: 1.5rem;}");
            client.println(".button{background-color: #4286f4; display: inline-block; border: none; border-radius: 4px; color: white; padding: 16px 40px;text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2{background-color: #4286f4;display: inline-block; border: none; border-radius: 4px; color: white; padding: 16px 40px;text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}</style></head>");
            // Web Page Heading
            client.println("<body><h1>Pico W Web Server</h1>");
            client.println("<p>GPIO state: null</p>");
            client.println("<p><a href=\"/LED_BUILTIN/ON\"><button class=\"button button2\">ON</button></a></p>");
            client.println("<p><a href=\"/LED_BUILTIN/OFF\"><button class=\"button button2\">OFF</button></a></p>");
            client.println("</body></html>");
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else {  // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
  if (digitalRead(button_down) == LOW) {
    //Send the message
    Keyboard.print("Hello");
    while(digitalRead(button_ok) == LOW){
      //do nothin
    }
  }
}

void display(String text, int line)
{
  lcd_setCursor(line, 0);
  lcd_print(text.c_str());
}

String IpAddress2String(const IPAddress& ipAddress)
{
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3])  ; 
}
