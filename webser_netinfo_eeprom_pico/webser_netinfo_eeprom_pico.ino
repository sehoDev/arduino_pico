/*
 Web Server

 A simple web server that shows the value of the analog input pins.
 using an Arduino WIZnet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 modified 02 Sept 2015
 by Arturo Guadalupi
 
 */

#include <SPI.h>
#include <Ethernet.h>
#include <EEPROM.h>

int CNF_OFFSET_NETWORK = 0;

struct cnf_network {
  byte ip[4];
  byte netmask[4];
  byte gateway[4];
  byte dns[4];
  byte mac[6];
};

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 117);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup() {
  cnf_network netinfo1;
  cnf_network netinfo2;

  //Call before the first use of the EEPROM data for read or write. 
  //It makes a copy of the emulated EEPROM sector in RAM to allow random update and access.
  EEPROM.begin(256);

  //net info를 eeprom 에 먼저 저장
  netinfo1 = get_default_network();
  EEPROM.put(CNF_OFFSET_NETWORK, netinfo1);
  EEPROM.commit();

  //저장된 네트웍 정보를 다시 블러온다.
  EEPROM.get(CNF_OFFSET_NETWORK, netinfo2);

  // You can use Ethernet.init(pin) to configure the CS pin
  //Ethernet.init(10);  // Most Arduino shields
  //Ethernet.init(5);   // MKR ETH Shield
  //Ethernet.init(0);   // Teensy 2.0
  //Ethernet.init(20);  // Teensy++ 2.0
  //Ethernet.init(15);  // ESP8266 with Adafruit FeatherWing Ethernet
  //Ethernet.init(33);  // ESP32 with Adafruit FeatherWing Ethernet
  Ethernet.init(17);

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Ethernet WebServer Example");

  // start the Ethernet connection and the server:
  //Ethernet.begin(mac, ip);
  
  Ethernet.begin(
    netinfo2.mac, 
    netinfo2.ip, 
    netinfo2.dns, 
    netinfo2.gateway, 
    netinfo2.netmask
  );

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  // start the server
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());


}


cnf_network get_default_network()
{
  cnf_network netinfo;

  netinfo.ip[0] = 192;
  netinfo.ip[1] = 168;
  netinfo.ip[2] = 1;
  netinfo.ip[3] = 117;

  netinfo.netmask[0] = 255;
  netinfo.netmask[1] = 255;
  netinfo.netmask[2] = 255;
  netinfo.netmask[3] = 0;

  netinfo.gateway[0] = 192;
  netinfo.gateway[1] = 168;
  netinfo.gateway[2] = 1;
  netinfo.gateway[3] = 1;

  netinfo.dns[0] = 8;
  netinfo.dns[1] = 8;
  netinfo.dns[2] = 8;
  netinfo.dns[3] = 8;

  netinfo.mac[0] = 0xDE;
  netinfo.mac[1] = 0xAD;
  netinfo.mac[2] = 0xBE;
  netinfo.mac[3] = 0xEF;
  netinfo.mac[4] = 0xFE;
  netinfo.mac[5] = 0xED;

  return netinfo;
  
}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an HTTP request ends with a blank line
    bool currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the HTTP request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard HTTP response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of each analog input pin
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            int sensorReading = analogRead(analogChannel);
            client.print("analog input ");
            client.print(analogChannel);
            client.print(" is ");
            client.print(sensorReading);
            client.println("<br />");
          }
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}
