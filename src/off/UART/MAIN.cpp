// Serial2

#ifdef ESP32
  #include <HardwareSerial.h>
  // HardwareSerial SerialX(1)  //if using UART1
  HardwareSerial SerialX(2);  //if using UART2
#else
  #include <SoftwareSerial.h>
  SoftwareSerial SerialX;
#endif
