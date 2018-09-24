// ########################################
//  OUTPUT
// ########################################

  class Output:public CompositeHTML {

    
  public:
  
  String stateStr;
  int value=0;
  int minValue;
  int maxValue;
  String unit;

  virtual void update( int newValue){};
  String getHtml(){};
    };

// ########################################
//  LABEL
// ########################################

class Label: public Output {
  public:
  String text="Output::Label";
    Label ( String n , String t, ElementsHTML* e = 0 ){
      name = n;
      id = n;
      text = t;
      parent = e;
      html="<span id='"+id+ "'><h5>"+text+"</h5></span>"; 
             pushElement(this);          // Los elementos basicos se registran solos en el AllHTMLElemens !!

    }
    String getHtml(){ return html;  }
    String postCallBack(String postValue , String postDataValue) { if(parent) return parent->postCallBack(postValue,postDataValue); }
    void update ( String newValue ) { text= newValue; javaQueue.add("document.getElementById('" + id + "').innerHTML='<h5>"+text+"</h5>';");}
    void update(){ javaQueue.add("document.getElementById('" + id + "').innerHTML='V: "+text+"';"); } 

};

// ########################################
//  DIGITAL OUTPUT
// ########################################


  class DigitalOutput:public Output {
    int pin;
    Label* label;
  public:
        bool invertedLogic = false;
        
        DigitalOutput ( int _pin , String _name, String _id ) {          /// Constructor
        pin = _pin;
        id= _id;
        name = "Digi Out " + id;
        minValue = 0;
        maxValue = 1;
        unit = "apagado/encendido";
        descriptor = name;
        pinMode ( pin , OUTPUT );
        stateStr="OFF";
        label = new Label ("lbl"+id,stateStr,this);
        
        html="<div><h4>"+name+"</h4><br>State: "+label->getHtml()+"</div>";
        pinMode(pin,OUTPUT);
         };

      void update( int newValue) {
      value = newValue;
      if (value>1)value=1;
      if (  newValue != 0 ) {
        digitalWrite(pin, HIGH);
        invertedLogic?stateStr = "OFF":stateStr = "ON";
      }
      if ( newValue == 0 ) {
        digitalWrite(pin, LOW);
        invertedLogic?stateStr="ON":stateStr = "OFF";
      }
            //javaQueue.add("document.getElementById('" + label->id + "').innerHTML='"+stateStr+"';");
            label->update(stateStr);
       }
       
      void toogle(){
        if (value==0){
          update(1);
        }
        else{
          update(0);
        }
      }
    String getState(){  return stateStr; }
    String getHtml(){ return html;  }
    String postCallBack(String postValue , String postDataValue) {
   return "";
      //return "document.getElementById('"+id+"').innerHTML='"+stateStr+"';";
      }
    
  };

class RelayOutput: public DigitalOutput {
  using DigitalOutput::DigitalOutput;       // inherit the constructor
  bool invertedLogic = true;
  public:

};




