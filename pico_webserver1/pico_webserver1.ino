
#include <SPI.h>
#include <Ethernet.h>
#include <EEPROM.h>
#include <Arduino.h>


/* serial Debug ========================================*/
const int IS_DEBUG_MODE = 1;
bool is_serial_open = false;
/* ======================================= serial Debug */


/* Ethernet Http Server ============================================= */
EthernetServer server(80);




/* ==== EEPROM ================================================= */
int CNF_OFFSET_NETWORK = 0;
struct cnf_network {
  byte ip[4];
  byte netmask[4];
  byte gateway[4];
  byte dns[4];
  byte mac[6];
};
/*==================================================================*/


void setup() {  
  EEPROM.begin(256);
  init_serial_debug();  
  init_ethernet();  
  init_http_server();  
}


void loop() {
  check_serial();  
  http_server_main();
  
}









