// ########################################
// CLASES
// ########################################

class Elements{
  public:
  String name;
  String id;
  virtual void postCallBack(){};
};


// ########################################
//  INPUT

class Input:public Elements {

  public:

  int value;
  int minValue;
  int maxValue;
  //String name;
  String unit;
  String descriptor;
  String html;
  String javascript;
  String postRequest;



    ;


    virtual void update() = 0; 
  };

class Dsb18B20:public Input {
  int pin;

public:

   Dsb18B20 ( int _pin , String  _name) {
    pin = _pin;
    name = _name;
        minValue = 0;
        maxValue = 150;
        unit = "grados";
        descriptor = "Temperature Sensor";
        html = "<span>Temperature:" + String(value) + "</span>";
        javascript = "";
        postRequest = "";
    
  }
    String getHtml(){
    return html;
  }
  void update(){
              //tempSensors.requestTemperatures(); // Request the temperature from the sensor (it takes some time to read it)
        html = "<span>" + name+"  Temperature:" + String(value) + "</span>";

}

};



// ########################################
//  OUTPUT
// ########################################


  class Output:public Elements {

    
  public:
  
  String stateStr;
  int value;
  int minValue;
  int maxValue;
  String unit;
  //String name;
  String descriptor;
  String html;
  String javascript;
  String postRequest;
  virtual void update( int newValue)=0;
  virtual void postCallBack()=0;
    };



  class RelayOutput:public Output {
    int pin;

  public:
  
        RelayOutput ( byte _pin , String _name, String _id ) {
        pin = _pin;
        name = _name;
        minValue = 0;
        maxValue = 1;
        unit = "apagado/encendido";
        descriptor = name;
        id= _id;
        pinMode ( pin , OUTPUT );
        stateStr="OFF";
    };

      void update( int newValue) {
      value = newValue;
      if (  newValue == 1 ) {
        digitalWrite(pin, HIGH);
        stateStr = "OFF";
      }
      if ( newValue == 0 ) {
        digitalWrite(pin, LOW);
        stateStr = "ON";
      }
      String temp =  ( value==0?"ON":"OFF" ) ; 
              html="<span>"+name+" - " + temp   + "<button type='button' name='"+ descriptor + "' id='" + id +"' onclick='btnClick(this)'>Toogle</button></span>";

       }
      void toogle(){
        if (value==0){
          update(1);
        }
        else{
          update(0);
        }
      }
          String getState(){
        return stateStr;
      }
          String getHtml(){
    return html;
  }
    void postCallBack(){
      this->toogle();
    }
  };


// ########################################
//  Commands
// ########################################

class Commands {
  public:
  String descriptor;
  String html;
  String javascript;
  String postRequest;
  virtual bool run();
};

class Set:Commands {
public:
  Output* output;
  int value;
  Set(Output* out){
    output = out;
      html="<span>"+output->html+"   "+output->stateStr + "</span>";
  }
  bool run () {
    output->update(value);
    return true;
  }
  String getHtml(){
    return html;
  }
};


// ########################################
//  Program
// ########################################

class Program {
public:

  Commands listOfCommands[20];
   int commandCount = 0;
   int runIndex =  0;
  void addCommand (Commands com){
    listOfCommands[commandCount] = com;
    commandCount++;
  }
  void deleteCommand(int index){
    for ( int i=index; i<19; i++)
    listOfCommands[i]=listOfCommands[i+1];
    commandCount--;
  }
  void runProgram() {
    if (runIndex>0){
    if (  listOfCommands[runIndex].run() ) runIndex++;
    }
  }
};


// ########################################
//  Page
// ########################################


class Page {
public:

   Elements listOfElements[20];
   int elementCount = 0;

   //Page(){};
  void addElement (Elements el){
    listOfElements[elementCount] = el;
    elementCount++;
  }
  void deleteElement(int index){
    for ( int i=index; i<19; i++)
    listOfElements[i]=listOfElements[i+1];
    elementCount--;
  }

};

