// ########################################
//  OUTPUT
// ########################################


  class Output:public ElementsHTML {

    
  public:
  
  String stateStr;
  int value;
  int minValue;
  int maxValue;
  String unit;

  virtual void update( int newValue)=0;
 // virtual String postCallBack(String postValue);
  virtual String getHtml(){};
    };

// ########################################
//  RELAY OUTPUT
// ########################################


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
                      html="<span>"+name+" - "  + "<button type='button' data-value='toogle' name='"+ descriptor + "' id='" + id +"' onclick='btnClick(this)'>Toogle</button></span>";

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
    String postCallBack(String postValue) {
      if (postValue=="toogle"){
      toogle();
      //return "Toggled "+name;
      return "document.getElementById('"+id+"').innerHTML='"+stateStr+"';";
      }
    }
  };


