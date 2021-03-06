#include <WiFiClientSecure.h>
#include "../DS18B20.h"
#include "ThingSpeak.h"
#include "../config.h"

void thingSpeakSend() {
  float _targetTemp = saci.getTargetTemp() ;
  float _temp = beerTemp() ;

  char _payload[70];

  //Hack to print float values with sprintf. :)
  sprintf(_payload, "api_key=%s&field1=%d.%02d&field2=%d.%02d&field3=%d",\
         thingSpeakToken, (int)(_temp), (int)(_temp * 100)%100, (int)(_targetTemp),\
         (int)(_targetTemp * 100)%100, millis() /1000 ) ;

  //HTTPS GET/POST  works ok with WiFiClientSecurelibrary
  WiFiClientSecure client ;
  const char* host = "api.thingspeak.com";
  const int port = 443 ;
  if ( client.connect(host, port) ) {
    client.println("POST /update HTTP/1.1") ;
    client.print("Host: ") ;
    client.println(host) ;
    client.println("User-Agent: SACI/1.0");
    client.println("Connection: close");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(strlen(_payload));
    client.println();
    client.println(_payload);
  }
}
