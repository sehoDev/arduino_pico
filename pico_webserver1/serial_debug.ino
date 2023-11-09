

void serial_debug(String msg)
{
  if(IS_DEBUG_MODE && is_serial_open)
  {
    Serial.println(msg);
  }
}

void init_serial_debug()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  /*
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  */

  check_serial();
}

void check_serial()
{
  if(Serial)
  {
    is_serial_open = true;
  }
  else
  {
    is_serial_open = false;
  }
}