#ifndef DECODER_COMMANDE_H
#define DECODER_COMMANDE_H

class decoderCommande:public decoder{
	public decoder(String msg,String start){
		if msg.startsWith(start){
			action();
	}
	virtual private action()=0;
}
#endif