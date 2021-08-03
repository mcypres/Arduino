#ifndef ESPUDP_H
#define ESPUDP_H

#include "Arduino.h"
#include <WiFiUdp.h>

#define PAKET_MAX_SIZE 255
class EspUdp{
private:WiFiUDP Udp;
public:	EspUdp(unsigned int localUdpPort);
public: String getPaket();
}

#endif