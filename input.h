// ########################################
//  INPUT
// ########################################

class Input: public ElementsHTML {

  public:

    float value;
    int minValue;
    int maxValue;
    String unit;
    String text="";

    //String getHtml() {};

};

// ########################################
//  BUTTON
// ########################################

class Button: public Input {
  public:
    Button(String n, String dataValue , String t, ElementsHTML* e = 0  ) {
      name = n;
      id = n;
      if (e!=0) parent = e;                                                                            //ERROR ERROR ERROR ERROR NO HACER ESTO Serial.println (t);
      pushElement(this);          // Los elementos basicos se registran solos en el AllHTMLElemens !!
      html = "<button type='button' width='40' id='" + id + "' value ='" + name + "' name='" + name + "' data-value='" + dataValue + "' onclick=\"btnClickText('" + id + "','" + name + "','" + dataValue + "')\" >" + t + "</button>\n";
    }

    String postCallBack(ElementsHTML* e,String postValue, String postDataValue) {
      if (parent) return parent->postCallBack(this,postValue,postDataValue);
      
      //return ("console.log('postCallBack of " + name+" parent: "+parent->name+"'); ");
      return "";
    } 
    String getHtml() {  return html; }
    void update() {};
};


// ########################################
//  Combo Box
// ########################################
class ComboBox: public Input {
  public:
  String *fields;
  int selected = 0;
    ComboBox(String n, int s , String* _fields ,  ElementsHTML* e=0) {
      name = n;
      text="";
      id = n;
      fields=_fields;
      html = "<select id='" + id + "' onchange=\"btnClickText('"+id+"', this.value , this.selectedIndex)\">\n";
          for(int i = 0; i <=s ; i++) {
              html+="<option value='"+_fields[i]+"'>"+_fields[i]+"</option>";
          }
              html+="</select>";
      value = 0;
       pushElement(this);          // Los elementos basicos se registran solos en el AllHTMLElemens !!
       
      if (e!=0) parent = e;                                                                            //ERROR ERROR ERROR ERROR NO HACER ESTO Serial.println (t);
      }

   
    String getHtml() {return  html;}
    void update() {
      text = fields[selected];
      value = selected;
      javaQueue.add("document.getElementById('" + id + "').selectedIndex='" + String(selected) + "'; console.log('"+name+" update value="+String(value)+"');");
      
    }
    String postCallBack(ElementsHTML* e,String postValue, String postDataValue) {
                  selected = postDataValue.toInt();
                  update();
                  if (parent) parent->postCallBack(this,postValue,postDataValue);
                  return "";//"document.getElementById('" + id + "').innerHTML='" + text + "';";
    }
};



// ########################################
//  DSB 18B20
// ########################################

class Dsb18B20: public Input {
    public:

    int pin;
    DallasTemperature* tempSensors;
    OneWire* oneWire;

    Dsb18B20 ( int _pin , String  _name) {
      pin = _pin;
      name = _name;
      id = _name;
      minValue = 0;
      maxValue = 150;
      unit = "grados";
      descriptor = "Temperature Sensor  Dsb18B20";
      html = " <div id='"+id+"'><h4>" + name + "</h4>"+descriptor+"<br>Temperature:" + String(value) + "</div>";
       pushElement(this);          // Los elementos basicos se registran solos en el AllHTMLElemens !!
       oneWire = new  OneWire(pin);        // Set up a OneWire instance to communicate with OneWire devices
       tempSensors = new DallasTemperature (oneWire); // Create an instance of the temperature sensor class
         tempSensors->setWaitForConversion(false); //  block the program while the temperature sensor is reading
       tempSensors->begin();
    }
    //~Dsb18B20(ElementsHTML::deleteElement(this));

    String getHtml() {

      return html;
    }
    
    void update() {
      if  (!tempRequested)  {
        tempSensors->requestTemperatures(); // Request the temperature from the sensor (it takes some time to read it)
        tempRequested=true;
        lastTemperatureRequest = millis();
      }
      if ( ( millis() - lastTemperatureRequest > intervalTemperature ) && tempRequested ) {
        value = tempSensors->getTempCByIndex(0); // Get the temperature from the sensor
        tempRequested=false;
      }
      //value = tempSensors->getTempCByIndex(0);
      javaQueue.add( "document.getElementById('" + id + "').innerHtml='Temperature:" + String(value) + "';");

    }

    String postCallBack(ElementsHTML* e,String postValue, String postDataValue) {update();}
  private:
  bool tempRequested = false;
  unsigned long lastTemperatureRequest;
  int intervalTemperature = 1500;
};

// ########################################
//  Analog IN
// ########################################
//class Label;
//class Output;
//Label::getHtml();

class AnalogIn: public Input {
    public:
    int pin;

    Label* label;
    AnalogIn ( int _pin , String  _name) {
      pin = _pin;
      name = _name;
      id = _name;
      minValue = 0;
      maxValue = 150;
      unit = "milliVolts";
      descriptor = "Ana In ";
      label = new Label(name+"lbl","",this);
      html = " <div id='"+id+"'><h4>" + name + "</h4>"+descriptor+"<br>"+ label->getHtml() +  "</div>";
       pushElement(this);          // Los elementos basicos se registran solos en el AllHTMLElemens !!
      
    }

    String getHtml() {

      return html;
    }
//    class Label;
//class Output;
//Label::getHtml();
    void update() {
      value = analogRead(pin);
      label->update(String(value));
    }

    String postCallBack(ElementsHTML* e,String postValue, String postDataValue) {update();}

};

// ########################################
//  Digital IN
// ########################################

class DigitalIn: public Input {
    public:
    int pin;

    Label* label;
    DigitalIn ( int _pin , String  _name) {
      pin = _pin;
      name = _name;
      id = _name;
      minValue = 0;
      maxValue = 1;
      unit = "milliVolts";
      descriptor = "D In "+String(pin);
      label = new Label(name+"lbl","",this);
      html = " <div id='"+id+"'><h4>" + name + "</h4>"+descriptor+"<br>"+ label->getHtml() +  "</div>";
       pushElement(this);          // Los elementos basicos se registran solos en el AllHTMLElemens !!
      
    }

    String getHtml() {return html; }

    void update() {
      value = digitalRead(pin);
      label->update(String(value));
    }

    String postCallBack(ElementsHTML* e,String postValue, String postDataValue) {update();}

};


// ########################################
//  EDIT BOX
// ########################################

class EditBox: public Input {
  public:
    EditBox(String n, String t, ElementsHTML* e=0) {
      name = n;
      text=t;
      id = n;
      html = "<input type='text' id='" + id + "' width='20' onkeyup=\"btnClickText('"+id+"','"+name+"',(this.value))\"></input>\n";
      value = 0;
       pushElement(this);          // Los elementos basicos se registran solos en el AllHTMLElemens !!
       
      if (e!=0) parent = e;                                                                            //ERROR ERROR ERROR ERROR NO HACER ESTO Serial.println (t);

    }

    String getHtml() {return  html;}
    void update() {
      value = text.toInt();
      javaQueue.add("document.getElementById('" + id + "').value='" + text + "';");
      
    }
    String postCallBack(ElementsHTML* e,String postValue, String postDataValue) {
                  text = postDataValue;
                  update();
                  if (parent) parent->postCallBack(this,postValue,postDataValue);
                  return "document.getElementById('" + id + "').innerHTML='" + text + "';";
    }
    
    void appendText (String t) {
      text += t;
      //Serial.println("EditBox->appendText()");
      update();
    }
    void setText (String t) {
      text = t;
      update();
    }
    void deleteChar () {
      text = text.substring(0, text.length() - 1);
      update();
    }
};



// ########################################
//  KEYPAD
// ########################################
class KeyPad: public Input {  // Aqui lo he cambiado !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  Antes era input
  public:
    int value=0;
    String state ="ooo";
    EditBox *edt;
    EditBox *edtLabel;
          Button* buttons[11];

    String html;
    KeyPad(String n) {
      name = n;
      id = n;
      html += "<div><h4>" + name + "</h4>\n\t";
      edt = new EditBox(name+ "Edit","");
      edtLabel = new EditBox(name+"Label","");
//      addChild(edt);addChild(edtLabel);
      for (int i = 0; i < 10; i++ ) {

        buttons[i] = new Button (name+"btn"+String(i), String(i), String(i),this);  // esto indica que tiene pariente 
//        addChild (buttons[i]);
        buttons[i]->datavalue = name;   ///   AQUI HE CAMBIADO
        html += buttons[i]->getHtml()+"\n";
        if ((i==2)||(i==5)||(i==8)) html+="<br>";
      }
      buttons[10] = new Button (name, "delete", "del",this);
  //    addChild (buttons[10]);
      html += buttons[10]->getHtml();
      html += edt->getHtml();
      html += edtLabel->getHtml();
      html += "</div>";
      //javaQueue.add("document.getElementById('" + edtLabel->id + "').innerHTML='" + state + "';");

    }
    String getHtml() {
      return html;
    }
    String postCallBack(ElementsHTML* e,String postValue, String postDataValue) {
      if (postDataValue == "delete")edt->deleteChar();
      else  edt->appendText (postDataValue);
      update();
      return "";//( "console.log('postCallBack of "+name+"'); ");
    };
    void update() {
      (edt->text == "1234") ? value = 1 : value = 0 ; //updateDisplay();
      !value ? state = "Locked" : state = "Unlocked";
      edtLabel->setText(state);
    };

   


};

