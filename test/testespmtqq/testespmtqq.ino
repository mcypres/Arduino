
enum StateWaiting {
  start,
  commande,
  endTopic,
  endMessage
};

#define START_CHAR '#'
#define SEPA_CHAR '&'
#define END_CHAR '\n'

#include"Logger.h"
#define MSG_SIZE 200
#define TOPIC_SIZE 50
static StateWaiting state=start;
static char buffMsg[MSG_SIZE];
static char buffTopic[TOPIC_SIZE];
HardwareSerial *ptOutputSer;

Logger* myLogger;

void setup() {

  char test[]="#topic&msg";
  ptOutputSer=&Serial;
  ptOutputSer->begin(115200);
  myLogger= new Logger(ptOutputSer,"test",W);
  _LOG_PRINT(W,"setup ","Start");
}

void loop() {
  readSerial();
}

 void readSerial(){
  
static int posTopic,posMsg;
static char cmd;
  while (ptOutputSer->available() > 0) {
    char car=ptOutputSer->read();
   // _LOG_PRINT(D,"car ",(int)car);
    
    _LOG_PRINT(D,"***************car ",car);
    _LOG_PRINT(D,"***************state ",state);
    switch  (state){
      
      case start:
        if (car==START_CHAR){
          _LOG_PRINT(D,"decript","start");
          state=commande;
          posTopic=0;
          posMsg=0;
        }
        break;

      case commande:
      _LOG_PRINT(D,"commande",car);
        if(car=='p'||car=='s'||car=='u'){
          cmd=car;
          state=endTopic;
          posTopic=0;          
          _LOG_PRINT(D,"commnande= ",cmd); 
        }
        else{
              state=start; 
              _LOG_PRINT(M,"commnande inconnue (p,s,u) ",cmd);  
        }
        break;
        
      case endTopic:   //on lit topic
        _LOG_PRINT(D,"endTopic",car);
        if (car==SEPA_CHAR){                    // fin de topic
          buffTopic[posTopic++]='\0';           //
          state=endMessage;                     //
          _LOG_PRINT(D,"fin topic ",buffTopic); //
          if (posTopic>=TOPIC_SIZE){ 
            _LOG_PRINT(M," topic overflow ",buffTopic);//
            state=start;                        // cas d erreur on ignore la suite
          }                                     //
          if (cmd=='s'){ //suscribe              //  
            _LOG_PRINT(M,"subscribe ",buffTopic);// 
            //MQTTclient.subscribe(buffTopic);  //
            state=start;                        //
            break;                              //
          }                                     //
          if (cmd=='u'){ // unsubscribe          //
            _LOG_PRINT(M,"unsubscribe ",buffTopic);
           // MQTTclient.unsubscribe(buffTopic);//
            state=start;                        //
            break;                              //
          }                                     //
          //buffTopic[posTopic++]=car;          //
          
          break;                                //
        }
        //state=endMessage;
        buffTopic[posTopic++]=car;
        break;
        
      case endMessage:   // on lit message
        
        _LOG_PRINT(D,"endMessage",car);
        if (car==END_CHAR){                         // fin message
          buffMsg[posMsg++]='\0';                   //
          _LOG_PRINT(M,"publish topic ",buffTopic); //
          _LOG_PRINT(M,"publish msg ",buffMsg);     //
         // MQTTclient.publish(buffTopic, buffMsg); //
          state=start;                              //
          break;                                    //
        } 
        buffMsg[posMsg++]=car;
        if(posMsg>=MSG_SIZE){
          _LOG_PRINT(M," message overflow ",buffMsg);
          state=start;
        }
        break;
    }       
  }
}
