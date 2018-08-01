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
 String getHtml(){};
 
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
        postRequest=id;
//      page.addElement(this);
  }
  //~Dsb18B20();
  
    String getHtml(){ return html;  }
    
  void update(){
              //tempSensors.requestTemperatures(); // Request the temperature from the sensor (it takes some time to read it)
        html =   name+"  Temperature:" + String(value) ;

}
  String postCallBack(String postValue,String postDataValue){}

};

// ########################################
//  EDIT BOX
// ########################################

class EditBox: public Input {
  public:
  String text;
  EditBox(String _name){
    value = 0;
    name=_name;
    id=_name;
    html="<input type='text' id='"+id+"' width='40' ></input>";
            javascript = "";
        postRequest=id;
        
  }
  
      String getHtml(){  return html; }
  void update(){    javascript = "document.getElementById('"+name+"').value='"+text+"';";  }
 String postCallBack(String postValue, String postDataValue) {
      return "document.getElementById('"+id+"').innerHTML='"+name+"';";
      value = postValue.toInt();
      //return "";
    }
    void appendText (String _text){  text += _text; update(); }
    void setText (String _text){  text = _text; update(); }
    void deleteChar (){text=text.substring(0,text.length()-1);update(); }
};

// ########################################
//  BUTTON
// ########################################

class Button: public Input {
    public:
    Button(String n, String dataValue , String t) {
      name = n;
      id = n;
        postRequest=id;
      //Serial.println (t);
      html="<button type='button' width='40' id='"+id+"' value ='"+name+"' name='"+name+"' data-value='"+dataValue+"' onclick=\"btnClickText('"+id+"','"+name+"','"+dataValue+"')\" >"+t+"</button>";
    }

   String postCallBack(String postValue, String postDataValue){
    return ("console.log('btn clicked ")+name;} // es virtual, lo tienen que implementar los hijos       ATENCION CUANDO DICE VTABLE ES QUE HE DEJADO UNA FUNCION SIN DEFINIR
      String getHtml(){  return html; }
      void update(){};
};

// ########################################
//  KEYPAD
// ########################################
class KeyPad: public Input {
  public:
    int value;
    String state;
      EditBox *edt;
      EditBox *edtLabel;

  KeyPad(String n){
    name = n;
    id = n;
    postRequest=n;
    Button* buttons[11];
    html+= "<div><h4>"+name+"</h4>";
              edt = new EditBox("keyPadEdit");
              edtLabel = new EditBox("keyPadEditLabel");

    for (int i=0; i<10; i++ ) {

      buttons[i] = new Button (name,String(i),String(i));
      buttons[i]->postRequest = name;   ///   AQUI HE CAMBIADO
      buttons[i]->datavalue = name;   ///   AQUI HE CAMBIADO
      html+= buttons[i]->getHtml();
    }
    buttons[10] = new Button (name,"delete","del");
    html+=buttons[10]->getHtml();
    html+= edt->getHtml();
    html+= edtLabel->getHtml();
    html+="</div>";
        javascript = "document.getElementById('"+edtLabel->name+"').innerHTML='"+state+"';";

  }
        String getHtml(){  return html; }
   String postCallBack(String postValue, String postDataValue){
    if (postDataValue=="delete")edt->deleteChar();
    else  edt->appendText (postDataValue);
    update();
      return javascript+edt->javascript;
      
    }; 
    void update(){ (edt->text == "1234")? value=1:value=0 ; //updateDisplay();
         !value?state="Locked":state="Unlocked";
         edtLabel->setText(state);
          javascript = edtLabel->javascript;//+";document.getElementById('"+edtLabel->name+"').setAttribute('background-color')='red'";

    };
    void updateDisplay(){
//      if (value=0) addToJavaQueue(javascript);
         }
    

};

