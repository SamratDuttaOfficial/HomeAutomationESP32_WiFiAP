/*
 * Home Automation with ESP32 
 * We use ESP32 as an Access Point (AP) for our Web Server. 
 * The web server page will have the buttons for ON/OFF switches for home appliances.
 * Github: https://github.com/samratduttaofficial/HomeAutomationESP32_WiFiAP
 * 
 * A Project by Samrat Dutta and Jinia Parvin.
 * Samrat's Github: https://github.com/samratduttaofficial
 * Jinia's Github: https://github.com/jiniaparvin
 * This project was built on a ESP32 development board.
*/

// Load Wi-Fi library
// #include <WiFi.h> for ESP32 boards. #include <ESP8266WiFi.h> for ESP8266 boards.
#include <WiFi.h>

// We will use ESP32 as an WiFi access point.
// Credentials of the ESP32 WiFi access point
const char* ssid = "My Home Appliances";
const char* password = "ABCD2021";

// Set port 80 as the web-server port.
WiFiServer server(80);

// Store the HTTP request in a variable.
String header;

// Create switch varibales and assign them to GPIO pins
const int switch1 = 12;
const int switch2 = 13;
const int switch3 = 14;
const int switch4 = 27;

// Set the states of all the switches as "off" primarily.
String switch1_state = "off";
String switch2_state = "off";
String switch3_state = "off";
String switch4_state = "off";


void setup() {
  //Start the serial monitor
  Serial.begin(115200);
  
  // Set the switch pins as outputs
  pinMode(switch1, OUTPUT);
  pinMode(switch2, OUTPUT);
  pinMode(switch3, OUTPUT);
  pinMode(switch4, OUTPUT);
 
  // We are using active low type relay module, so, set outputs to high
  digitalWrite(switch1, HIGH);
  digitalWrite(switch2, HIGH);
  digitalWrite(switch3, HIGH);
  digitalWrite(switch4, HIGH);

  // Setting up WiFi access point
  Serial.print("Setting up WiFi Acess Point…");
  WiFi.softAP(ssid, password);  // Don't pass the password parameter for an open access point (unsafe).

  IPAddress IP = WiFi.softAPIP();
  Serial.print("Access Point IP address: ");
  Serial.println(IP);

  // Start the web server.
  server.begin();
}


void loop(){
  // Keep listening to incoming clients
  WiFiClient client = server.available();

  // When a new client connects to the server
  if (client){
    Serial.println("New Client Got Connected"); 
    String currentLine = "";
    // Loop while the client is connected           
    while (client.connected()){
      // Read a byte if there's bytes to read from the client.            
      if (client.available()){
        char c = client.read();
        // Print the byte we read from the client.
        Serial.write(c);
        header += c;
        if (c == '\n'){   // if the byte we read from the client is a newline character
          if (currentLine.length() == 0){  // If the current line is blank
            // HTTP headers start with a response code. Example: HTTP/1.1 200 OK
            client.println("HTTP/1.1 200 OK");
            // Content-type so that the client knows what's coming (text/html in this case)
            client.println("Content-type:text/html");
            client.println("Connection: close");
            // HTTP response starts with a blank line.
            client.println();
            
            // Let's build the On/Off mechanism for the GPIO pins.
            // GPIO12
            if (header.indexOf("GET /switch1/turn_on") >= 0) 
            {
              Serial.println("Switch 1 (GPIO 12) ON");
              switch1_state = "on";
              digitalWrite(switch1, LOW);
            } 
            else if (header.indexOf("GET /switch1/turn_off") >= 0) 
            {
              Serial.println("Switch 1 (GPIO 12) OFF");
              switch1_state = "off";
              digitalWrite(switch1, HIGH);
            }
            // GPIO13
            else if (header.indexOf("GET /switch2/turn_on") >= 0)
            {
              Serial.println("Switch 2 (GPIO 13) ON");
              switch2_state = "on";
              digitalWrite(switch2, LOW);
            } 
            else if (header.indexOf("GET /switch2/turn_off") >= 0)
            {
              Serial.println("Switch 2 (GPIO 13) OFF");
              switch2_state = "off";
              digitalWrite(switch2, HIGH);
            }
             // GPIO14
             else if (header.indexOf("GET /switch3/turn_on") >= 0)
            {
              Serial.println("Switch 3 (GPIO 14) ON");
              output14State = "on";
              digitalWrite(switch3, LOW);
            } 
            else if (header.indexOf("GET /switch3/turn_off") >= 0)
            {
              Serial.println("Switch 3 (GPIO 14) OFF");
              output14State = "off";
              digitalWrite(switch3, HIGH);
            }
            // GPIO27
            else if (header.indexOf("GET /switch4/turn_on") >= 0)
            {
              Serial.println("Switch 4 (GPIO 27) ON");
              output27State = "on";
              digitalWrite(switch4, LOW);
            } 
            else if (header.indexOf("GET /switch4/turn_off") >= 0)
            {
              Serial.println("Switch 4 (GPIO 27) OFF");
              output27State = "off";
              digitalWrite(switch4, HIGH);
            }
          
            // Display the HTML webpage
            // Create the HTML webpage
            client.println("<!DOCTYPE html><html>");
            // Head
            client.println("<head>");
            client.println("<meta name='viewport' content='width=device-width, initial-scale=1'>");
            // Let's make the CSS for syling the webpage. 
            client.println("<style>");
            client.println("html{font-family: Arial; display: inline-block; margin: 10px; text-align: center;}");
            client.println(".button{color: #fcece3; background-color: #ff742e; padding: 10px; margin: 10px; font-size: 30px;}");
            client.println(".button2{color: #fcece3; background-color: #d6692b; padding: 10px; margin: 10px; font-size: 30px;}");
            client.println("</style>");
            client.println("</head>");
            // Body
            client.println("<body>");
            // Heading
            client.println("<h1>My Home Appliances</h1>");
            // Display Switch 1 (GPIO12) state.
            client.println("<h3>Switch 1</h3>");
            // Display ON/OFF based on Switch 1 state.       
            if (switch1_state=="off") {
              client.println("<p><a href='/switch1/turn_on'><button class='button'>ON</button></a></p>");
            } else {
              client.println("<p><a href='/switch1/turn_off'><button class='button2'>OFF</button></a></p>");
            } 
            // Display Switch 2 (GPIO13) state.
            client.println("<h3>Switch 2</h3>");
            // Display ON/OFF based on Switch 2 state.       
            if (switch2_state=="off") {
              client.println("<p><a href='/switch2/turn_on'><button class='button'>ON</button></a></p>");
            } else {
              client.println("<p><a href='/switch2/turn_off'><button class='button2'>OFF</button></a></p>");
            }
            // Display Switch 3 (GPIO14) state.
            client.println("<h3>Switch 3</h3>");
            // Display ON/OFF based on Switch 3 state.n       
            if (switch3_state=="off") {
              client.println("<p><a href='/switch3/turn_on'><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href='/switch3/turn_off'><button class='button2'>OFF</button></a></p>");
            }
            // Display Switch 4 (GPIO27) state.
            client.println("<h3>Switch 4</h3>");
            // Display ON/OFF based on Switch 4 state.      
            if (switch4_state=="off") {
              client.println("<p><a href='/switch4/turn_on'><button class='button'>ON</button></a></p>");
            } else {
              client.println("<p><a href='/switch1/turn_off'><button class='button2'>OFF</button></a></p>");
            }
            client.println("</body>");
            client.println("</html>");
            // The HTTP response ends with another blank line
            client.println();            
            // Break the loop
            break;
          } else {    // clear currentLine if the current line is not blank.
            currentLine = "";
          }
        } else if (c != '\r') {  
          // if it gets anything other than a carriage return character, add it to the end of the currentLine.
          currentLine += c; 
        }
      }
    }
    // Clearing the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("Goodbye!");
  }
}
