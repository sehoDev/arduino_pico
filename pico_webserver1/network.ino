
bool get_networkinfo(cnf_network* netinfo)
{
  IPAddress ip =  Ethernet.localIP();
  IPAddress netmask =  Ethernet.subnetMask();
  IPAddress gateway =  Ethernet.gatewayIP();
  IPAddress dns = Ethernet.dnsServerIP();

  netinfo->ip[0] = ip[0];
  netinfo->ip[1] = ip[1];
  netinfo->ip[2] = ip[2];
  netinfo->ip[3] = ip[3];

  netinfo->netmask[0] = netmask[0];
  netinfo->netmask[1] = netmask[1];
  netinfo->netmask[2] = netmask[2];
  netinfo->netmask[3] = netmask[3];

  netinfo->gateway[0] = gateway[0];
  netinfo->gateway[1] = gateway[1];
  netinfo->gateway[2] = gateway[2];
  netinfo->gateway[3] = gateway[3];

  netinfo->dns[0] = dns[0];
  netinfo->dns[1] = dns[1];
  netinfo->dns[2] = dns[2];
  netinfo->dns[3] = dns[3];


  byte macBuffer[6];
  Ethernet.MACAddress(macBuffer);

  netinfo->mac[0] = macBuffer[0];
  netinfo->mac[1] = macBuffer[1];
  netinfo->mac[2] = macBuffer[2];
  netinfo->mac[3] = macBuffer[3];
  netinfo->mac[4] = macBuffer[4];
  netinfo->mac[5] = macBuffer[5];

  return true;
}

void print_network_info()
{
  
  IPAddress ip =  Ethernet.localIP();
  IPAddress netmask =  Ethernet.subnetMask();
  IPAddress gateway =  Ethernet.gatewayIP();
  IPAddress dns = Ethernet.dnsServerIP();
  
  byte macBuffer[6];
  Ethernet.MACAddress(macBuffer);

  //222 173 190 239 254 237
  serial_debug(ip.toString());
  serial_debug(netmask.toString());
  serial_debug(gateway.toString());
  serial_debug(dns.toString());
  serial_debug(String(macBuffer[0])+":"+String(macBuffer[1])+":"+String(macBuffer[2])+":"+String(macBuffer[3])+":"+String(macBuffer[4])+":"+String(macBuffer[5]));
}







