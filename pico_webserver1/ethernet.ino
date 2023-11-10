
cnf_network get_default_network()
{
  cnf_network netinfo;

  netinfo.ip[0] = 192;
  netinfo.ip[1] = 168;
  netinfo.ip[2] = 1;
  netinfo.ip[3] = 137;

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

void init_ethernet()
{
  cnf_network netinfo;
  
  EEPROM.get(CNF_OFFSET_NETWORK, netinfo);
  if(netinfo.ip[0] == 0) {
    netinfo = get_default_network();
  }

  Ethernet.init(17);
    
  /*
  byte mac[] = {
    0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
  };
    IPAddress ip(192, 168, 1, 137);
  IPAddress myDns(8, 8, 8, 8);
  IPAddress gateway(192, 168, 1, 1);
  IPAddress netmask(255, 255, 255, 0);
  */

  Ethernet.begin(
    netinfo.mac, 
    netinfo.ip, 
    netinfo.dns, 
    netinfo.gateway, 
    netinfo.netmask
  );

  

  //Ethernet.begin(mac, ip, myDns, gateway, netmask);
  
  delay(100);

  //print_network_info();

    /*
  Ethernet.setGatewayIP(IPAddress (netinfo.gateway[0], netinfo.gateway[1], netinfo.gateway[2], netinfo.gateway[3]));
  Ethernet.setDnsServerIP(IPAddress(netinfo.dns[0], netinfo.dns[1], netinfo.dns[2], netinfo.dns[3]));
  Ethernet.setSubnetMask(IPAddress(netinfo.netmask[0], netinfo.netmask[1], netinfo.netmask[2], netinfo.netmask[3]));  
  */

  
  //EEPROM.put(CNF_OFFSET_NETWORK, netinfo);
  //EEPROM.commit();
}

int is_ethernet_ok()
{
  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    serial_debug("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    /*
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
    */
    return -1;
  }

  if (Ethernet.linkStatus() == LinkOFF) {
    serial_debug("Ethernet cable is not connected.");
    return -2;
  }

  return 0;
  
}


