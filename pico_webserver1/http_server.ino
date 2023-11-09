
void http_response_header(EthernetClient client)
{
  // send a standard HTTP response header
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");  // the connection will be closed after completion of the response
  client.println();
}

void http_response_network(EthernetClient client)
{
  cnf_network netinfo;
  get_networkinfo(&netinfo);

  http_response_header(client);
  String html ="<!DOCTYPE html> <html lang=\"en\"> <head> <meta charset=\"UTF-8\"> <meta name=\"Author\" content=\"\"> <meta name=\"Keywords\" content=\"\"> <meta name=\"Description\" content=\"\"> <title>Document</title> </head> <body> <div> <a href=\"/network\">network</a> </div> <div> <form action=\"/network\" method=\"post\"> <table> <tbody> <tr> <td>Ip Address</td> <td> <input type=\"text\" name=\"ip1\" value=\"TXT_IP1_TXT\"> <input type=\"text\" name=\"ip2\" value=\"TXT_IP2_TXT\"> <input type=\"text\" name=\"ip3\" value=\"TXT_IP3_TXT\"> <input type=\"text\" name=\"ip4\" value=\"TXT_IP4_TXT\"> </td> </tr> <tr> <td>subnet mask</td> <td> <input type=\"text\" name=\"netmask1\" value=\"TXT_NETMASK1_TXT\"> <input type=\"text\" name=\"netmask2\" value=\"TXT_NETMASK2_TXT\"> <input type=\"text\" name=\"netmask3\" value=\"TXT_NETMASK3_TXT\"> <input type=\"text\" name=\"netmask4\" value=\"TXT_NETMASK4_TXT\"> </td> </tr> <tr> <td>gateway</td> <td> <input type=\"text\" name=\"gateway1\" value=\"TXT_GATEWAY1_TXT\"> <input type=\"text\" name=\"gateway2\" value=\"TXT_GATEWAY2_TXT\"> <input type=\"text\" name=\"gateway3\" value=\"TXT_GATEWAY3_TXT\"> <input type=\"text\" name=\"gateway4\" value=\"TXT_GATEWAY4_TXT\"> </td> </tr> <tr> <td>DNS</td> <td> <input type=\"text\" name=\"dns1\" value=\"TXT_DNS1_TXT\"> <input type=\"text\" name=\"dns2\" value=\"TXT_DNS2_TXT\"> <input type=\"text\" name=\"dns3\" value=\"TXT_DNS3_TXT\"> <input type=\"text\" name=\"dns4\" value=\"TXT_DNS4_TXT\"> </td> </tr> <tr> <td>Mac Address</td> <td> <input type=\"text\" name=\"mac1\" value=\"TXT_MAC1_TXT\"> <input type=\"text\" name=\"mac2\" value=\"TXT_MAC2_TXT\"> <input type=\"text\" name=\"mac3\" value=\"TXT_MAC3_TXT\"> <input type=\"text\" name=\"mac4\" value=\"TXT_MAC4_TXT\"> <input type=\"text\" name=\"mac5\" value=\"TXT_MAC5_TXT\"> <input type=\"text\" name=\"mac6\" value=\"TXT_MAC6_TXT\"> </td> </tr> </tbody> </table> <input type=\"submit\"> </form> </div> </body> </html>";



  html.replace("TXT_IP1_TXT", String(netinfo.ip[0]));
  html.replace("TXT_IP2_TXT", String(netinfo.ip[1]));
  html.replace("TXT_IP3_TXT", String(netinfo.ip[2]));
  html.replace("TXT_IP4_TXT", String(netinfo.ip[3]));

  html.replace("TXT_NETMASK1_TXT", String(netinfo.netmask[0]));
  html.replace("TXT_NETMASK2_TXT", String(netinfo.netmask[1]));
  html.replace("TXT_NETMASK3_TXT", String(netinfo.netmask[2]));
  html.replace("TXT_NETMASK4_TXT", String(netinfo.netmask[3]));

  html.replace("TXT_GATEWAY1_TXT", String(netinfo.gateway[0]));
  html.replace("TXT_GATEWAY2_TXT", String(netinfo.gateway[1]));
  html.replace("TXT_GATEWAY3_TXT", String(netinfo.gateway[2]));
  html.replace("TXT_GATEWAY4_TXT", String(netinfo.gateway[3]));

  html.replace("TXT_DNS1_TXT", String(netinfo.dns[0]));
  html.replace("TXT_DNS2_TXT", String(netinfo.dns[1]));
  html.replace("TXT_DNS3_TXT", String(netinfo.dns[2]));
  html.replace("TXT_DNS4_TXT", String(netinfo.dns[3]));


  html.replace("TXT_MAC1_TXT", String(netinfo.mac[0], HEX));
  html.replace("TXT_MAC2_TXT", String(netinfo.mac[1], HEX));
  html.replace("TXT_MAC3_TXT", String(netinfo.mac[2], HEX));
  html.replace("TXT_MAC4_TXT", String(netinfo.mac[3], HEX));
  html.replace("TXT_MAC5_TXT", String(netinfo.mac[4], HEX));
  html.replace("TXT_MAC6_TXT", String(netinfo.mac[5], HEX));




  client.println(html);
}

void http_route(String http_raw, EthernetClient client)
{
  //http_header header = http_parse_header(http_raw);
  int idx_line_end = http_raw.indexOf("\n");
  String line = http_raw.substring(0, idx_line_end);
  //serial_debug(line);

  //---- http_method
  int pos_method = line.indexOf(" ");
  String s_method = line.substring(0, pos_method);
  serial_debug(s_method);

  //---- uri
  int pos_uri = line.indexOf(" ", pos_method+1);
  String uri = line.substring(pos_method+1, pos_uri);
  serial_debug(uri);

  if(s_method == "GET")
  {
    //serial_debug("GET");
    if(uri == "/network" || uri == "/")
    {
      http_response_network(client);    
    }

  }
  else if (s_method == "POST") 
  {
    if(uri == "/network")
    {
      
      String http_header_raw;
      String http_body_raw;
      int length = http_raw.length();
      int last_idx_header;
      for(int i=0; i<length; i++)
      {        
        //end of header
        if (http_raw[i] == '\n' && ((i+1) < length) &&  (http_raw[i+1] == '\n' || http_raw[i+1] == '\r' )) 
        {
          last_idx_header = i-1;
          break;
        }
      }
      
      http_body_raw = http_raw.substring(last_idx_header+1);
      http_body_raw.trim();

      cnf_network netinfo;
      bool set_ip_ok = false;
      bool set_netmask_ok = false;
      bool set_gateway_ok = false;
      bool set_dns_ok = false;      
      bool set_mac_ok = false;
      
      String ip1_in = "";
      String ip2_in = "";
      String ip3_in = "";
      String ip4_in = "";
      String ip5_in = "";
      String ip6_in = "";

      bool ret_ip1 = false;
      bool ret_ip2 = false;
      bool ret_ip3 = false;
      bool ret_ip4 = false;
      bool ret_ip5 = false;
      bool ret_ip6 = false;

      bool is_netinfo_ok = get_networkinfo(&netinfo);

      
      //==== Ip ====================================================
      ret_ip1 = get_req_val(http_body_raw, "ip1", 4, ip1_in);
      ret_ip2 = get_req_val(http_body_raw, "ip2", 4, ip2_in);
      ret_ip3 = get_req_val(http_body_raw, "ip3", 4, ip3_in);
      ret_ip4 = get_req_val(http_body_raw, "ip4", 4, ip4_in);

      set_ip_ok = false;
      if(ret_ip1 && ret_ip2 && ret_ip3 && ret_ip4)
      {
        serial_debug("Received Ip Addr : "+ip1_in+" "+ip2_in+" "+ip3_in+" "+ip4_in);          
        
        netinfo.ip[0] = byte(ip1_in.toInt());
        netinfo.ip[1] = byte(ip2_in.toInt());
        netinfo.ip[2] = byte(ip3_in.toInt());
        netinfo.ip[3] = byte(ip4_in.toInt());
        set_ip_ok = true;          
      
      }

      //==== netmask ====================================================
      if(set_ip_ok)
      {
        
        ip1_in = "";
        ip2_in = "";
        ip3_in = "";
        ip4_in = "";
        ret_ip1 = get_req_val(http_body_raw, "netmask1", 4, ip1_in);
        ret_ip2 = get_req_val(http_body_raw, "netmask2", 4, ip2_in);
        ret_ip3 = get_req_val(http_body_raw, "netmask3", 4, ip3_in);
        ret_ip4 = get_req_val(http_body_raw, "netmask4", 4, ip4_in);
serial_debug("Received Netmask Addr Start : "+ip1_in+" "+ip2_in+" "+ip3_in+" "+ip4_in);           
        set_netmask_ok = false;
        if(ret_ip1 && ret_ip2 && ret_ip3 && ret_ip4)
        {
          serial_debug("Received Netmask Addr : "+ip1_in+" "+ip2_in+" "+ip3_in+" "+ip4_in);          
        
          netinfo.netmask[0] = byte(ip1_in.toInt());
          netinfo.netmask[1] = byte(ip2_in.toInt());
          netinfo.netmask[2] = byte(ip3_in.toInt());
          netinfo.netmask[3] = byte(ip4_in.toInt());
          set_netmask_ok = true;          
          
        }
      }

      //==== gateway ====================================================
      if(set_netmask_ok)
      { 
        ip1_in = "";
        ip2_in = "";
        ip3_in = "";
        ip4_in = "";
        ret_ip1 = get_req_val(http_body_raw, "gateway1", 4, ip1_in);
        ret_ip2 = get_req_val(http_body_raw, "gateway2", 4, ip2_in);
        ret_ip3 = get_req_val(http_body_raw, "gateway3", 4, ip3_in);
        ret_ip4 = get_req_val(http_body_raw, "gateway4", 4, ip4_in);

        set_gateway_ok = false;
        if(ret_ip1 && ret_ip2 && ret_ip3 && ret_ip4)
        {
          serial_debug("Received Gateway Addr : "+ip1_in+" "+ip2_in+" "+ip3_in+" "+ip4_in);                    
          netinfo.gateway[0] = byte(ip1_in.toInt());
          netinfo.gateway[1] = byte(ip2_in.toInt());
          netinfo.gateway[2] = byte(ip3_in.toInt());
          netinfo.gateway[3] = byte(ip4_in.toInt());
          set_gateway_ok = true;  
        }
      }

       //==== Dns ====================================================
      if(set_gateway_ok)
      { 
        ip1_in = "";
        ip2_in = "";
        ip3_in = "";
        ip4_in = "";
        ret_ip1 = get_req_val(http_body_raw, "dns1", 4, ip1_in);
        ret_ip2 = get_req_val(http_body_raw, "dns2", 4, ip2_in);
        ret_ip3 = get_req_val(http_body_raw, "dns3", 4, ip3_in);
        ret_ip4 = get_req_val(http_body_raw, "dns4", 4, ip4_in);

        set_dns_ok = false;
        if(ret_ip1 && ret_ip2 && ret_ip3 && ret_ip4)
        {
          serial_debug("Received DNS Addr : "+ip1_in+" "+ip2_in+" "+ip3_in+" "+ip4_in);                    
          netinfo.dns[0] = byte(ip1_in.toInt());
          netinfo.dns[1] = byte(ip2_in.toInt());
          netinfo.dns[2] = byte(ip3_in.toInt());
          netinfo.dns[3] = byte(ip4_in.toInt());
          set_dns_ok = true;  
        }
      }

       //==== MAC ====================================================
      
      if(set_dns_ok)
      { 
        ip1_in = "";
        ip2_in = "";
        ip3_in = "";
        ip4_in = "";
        ip5_in = "";
        ip6_in = "";
        ret_ip1 = get_req_val(http_body_raw, "mac1", 6, ip1_in);
        ret_ip2 = get_req_val(http_body_raw, "mac2", 6, ip2_in);
        ret_ip3 = get_req_val(http_body_raw, "mac3", 6, ip3_in);
        ret_ip4 = get_req_val(http_body_raw, "mac4", 6, ip4_in);
        ret_ip5 = get_req_val(http_body_raw, "mac5", 6, ip5_in);
        ret_ip6 = get_req_val(http_body_raw, "mac6", 6, ip6_in);

        set_mac_ok = false;
        if(ret_ip1 && ret_ip2 && ret_ip3 && ret_ip4 && ret_ip5 && ret_ip6)
        {
          serial_debug("Received MAC Addr : "+ip1_in+" "+ip2_in+" "+ip3_in+" "+ip4_in+" "+ip5_in+" "+ip6_in);   
          /*
          netinfo.mac[0] = byte(ip1_in.toInt());
          netinfo.mac[1] = byte(ip2_in.toInt());
          netinfo.mac[2] = byte(ip3_in.toInt());
          netinfo.mac[3] = byte(ip4_in.toInt());
          netinfo.mac[4] = byte(ip5_in.toInt());
          netinfo.mac[5] = byte(ip6_in.toInt());
          */
          char mac1[3];
          char mac2[3];
          char mac3[3];
          char mac4[3];
          char mac5[3];
          char mac6[3];

          //mac1[0]  //e
          //mac1[1]  //d
          //mac1[2]  //\0
          ip1_in.toCharArray(mac1, 3);
          ip2_in.toCharArray(mac2, 3);
          ip3_in.toCharArray(mac3, 3);
          ip4_in.toCharArray(mac4, 3);
          ip5_in.toCharArray(mac5, 3);
          ip6_in.toCharArray(mac6, 3);
          
          
          //serial_debug(String((char*)mac1));  

          //unsigned long  ==> 4byte
          netinfo.mac[0] = byte(strtoul(mac1,  NULL, 16));
          netinfo.mac[1] = byte(strtoul(mac2,  NULL, 16));
          netinfo.mac[2] = byte(strtoul(mac3,  NULL, 16));
          netinfo.mac[3] = byte(strtoul(mac4,  NULL, 16));
          netinfo.mac[4] = byte(strtoul(mac5,  NULL, 16));
          netinfo.mac[5] = byte(strtoul(mac6,  NULL, 16));
          
          serial_debug(String(netinfo.mac[0])+":"+String(netinfo.mac[1])+":"+String(netinfo.mac[2])+":"+String(netinfo.mac[3])+":"+String(netinfo.mac[4])+":"+String(netinfo.mac[5]));
          set_mac_ok = true;  
        }
      }
      

      if(set_ip_ok && set_netmask_ok && set_gateway_ok && set_dns_ok && set_mac_ok)  
      {
        EEPROM.put(CNF_OFFSET_NETWORK, netinfo);
        EEPROM.commit();          
        serial_debug("Network Info Saved");
      }
      else
      {
        serial_debug("Network Info Save Failed");
      }
      //ip1=192&ip2=168&ip3=1&ip4=137
      serial_debug(http_body_raw);

      http_response_network(client);

    }

    serial_debug("POST");  
  }
  else 
  {
    serial_debug("not allowed method");    
  }

  
}

/*
//ip1=192&ip2=168&ip3=1&ip4=137
key => ip1, ip2...
str => ip1=192&ip2=168&ip3=1&ip4=137
max_dataset => 4
*/

bool get_req_val(String str, String key, int max_dataset, String& retval)
{
  //ip1=192&ip2=168&ip3=1&ip4=137
  int start_pos = 0;
  int end_pos = 0;
  String dataset = "";
  String ret = "";
  
  //ip1=192
  String sub_dataset = "";
  String fkey = "";
  String val = "";
  int sub_spos = 0;
  int sub_epos = 0;

  for(int i=0; i<50; i++)
  { 
    if(start_pos > str.length() || end_pos > str.length())  
    {
      return false;
    }
    end_pos = str.indexOf("&", start_pos);
    dataset = str.substring(start_pos, end_pos);
    //serial_debug(dataset);


    /* ip1=192 */
    sub_epos = dataset.indexOf("=");
    fkey = dataset.substring(0, sub_epos);
    //serial_debug("0 :"+String(sub_epos)+": "+fkey);
    if(fkey == key)
    {
      val = dataset.substring(sub_epos+1);
      //serial_debug(val);
      if(val != "") {
        retval = val;
        return true;
      }
    }
    
    start_pos = end_pos+1;
  }
  
  return false;
}



int init_http_server()
{
  serial_debug("Ethernet WebServer Example"); 

  // start the server
  server.begin();
  serial_debug("server is at " + Ethernet.localIP().toString());

  return 0;
}

void http_server_main()
{
  //http_header reqHeader = {HTTP_METHOD_NONE, ""};
  EthernetClient client = server.available();
  if (client) {
    serial_debug(">>new client");
    serial_debug("==================================================================");
   
    char http_req_buf_[1024*4];
    memset(http_req_buf_, 0, sizeof(http_req_buf_));
    //serial_debug(String(sizeof(http_req_buf_)));
    int idx_buf = -1;
    while (client.connected()) {

      //number of bytes available for reading 
      if (client.available()) {
       // serial_debug(String(idx_buf));
       // char c = client.read();
        idx_buf++;
        http_req_buf_[idx_buf] = client.read();
        
      } else {
        
        String s_http_req = http_req_buf_;
        //serial_debug(s_http_req);
        serial_debug(">>Client Data Send Finish, Start Route");
        http_route(s_http_req, client);


        s_http_req = "";
        memset(http_req_buf_, 0, sizeof(http_req_buf_));
        
        break;
      }
        
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    serial_debug("client disconnected");

  }
}
