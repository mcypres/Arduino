void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
class A{
 protected:  static int i;
};

class B: public A{
  int getI(){
    return i++; 
  }
};



class C: public A{
  int getI(){
    return -i; 
  }
};
