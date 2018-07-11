// ########################################
//  INPUT
// ########################################

class Input:public ElementsHTML {

  public:

  int value;
  int minValue;
  int maxValue;
  String unit;
  //String name;
    ;


    virtual void update() = 0; 
  };

// ########################################
//  DSB 18B20
// ########################################

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
String postCallBack(String postValue){}

};

// ########################################
//  EDIT BOX
// ########################################

class EditBox: public Input {
  public:
  EditBox(String _name){
    name=_name;
    id=_name;
    html="<span><input type='number' data-value='qwerty' id='"+id+"' onclick='btnClick(this)'>number</input></span>";
  }
      String getHtml(){  return html; }
  void update(){   
  }
 String postCallBack(String postValue) {
      //return "document.getElementById('"+id+"').innerHTML='"+name+"';";
      value = postValue.toInt();
    }
};

