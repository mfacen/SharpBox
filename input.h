// ########################################
//  INPUT
// ########################################

class Input:public ElementsHTML {

  public:

  int value;
  int minValue;
  int maxValue;
  String unit;

    virtual void update(){}; 
 virtual String getHtml(){};
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
    id = _name;
        minValue = 0;
        maxValue = 150;
        unit = "grados";
        descriptor = "Temperature Sensor";
        html = "<span>Temperature:" + String(value) + "</span>";
        javascript = "";
        postRequest = "";
    
  }
  //~Dsb18B20();
  
    String getHtml(){ return html;  }
    
  void update(){
              //tempSensors.requestTemperatures(); // Request the temperature from the sensor (it takes some time to read it)
        html = "<span>" + name+"  Temperature:" + String(value) + "</span>";

}
  String postCallBack(String postValue,String postDataValue){}

};

// ########################################
//  EDIT BOX
// ########################################

class EditBox: public Input {
  public:
  EditBox(String _name){
    name=_name;
    id=_name;
    html="<span><input type='number' id='"+id+"' onclick='btnClick(this)'></input></span>";
            javascript = "";
        postRequest = _name;
        
  }
  
      String getHtml(){  return html; }
  void update(){   
  }
 String postCallBack(String postValue, String postDataValue) {
      //return "document.getElementById('"+id+"').innerHTML='"+name+"';";
      value = postValue.toInt();
      return "";
    }
};

class Button: public ElementsHTML {
    public:
    Button(String n){
      name = n;
      id = n;
      html="<button type='button' id='"+id+"' name='"+name+"' onclick='btnClick(this)'</button>";
    }
  
   String postCallBack(String postValue, String postDataValue){
    
    }; // es virtual, lo tienen que implementar los hijos       ATENCION CUANDO DICE VTABLE ES QUE HE DEJADO UNA FUNCION SIN DEFINIR
      String getHtml(){  return html; }

};
