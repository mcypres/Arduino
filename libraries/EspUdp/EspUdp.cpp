#include"EspUdp.h"

EspUdp::EspUdp(unsigned int localUdpPort){
	Udp.begin(localUdpPort);

}
EspUdp::String getPaket(){
	char incomingPacket[PAKET_MAX_SIZE];
	int packetSize = Udp.parsePacket();
	if (packetSize){
    // receive incoming UDP packets
    	int len = Udp.read(incomingPacket, PAKET_MAX_SIZE-1);
    	if (len > 0){
    		incomingPacket[len] = 0;
    		ptLogOutputSer->print("getPaket() msg:");
    		ptLogOutputSer->println(incomingPacket);
        return String(incomingPacket);
      }
    }
    return "";
	
}