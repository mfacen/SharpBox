// ########################################
//  INPUT
// ########################################

class Input : public CompositeHtml {

  public:

    float value=0;
    int minValue=0;
    int maxValue=0;
    String unit;
    String text="";
    
    //String getHtml() {};

};

// ########################################
//  BUTTON HTML
// ########################################

class Button: public Input {
  public:
    Button(String n , String t, ElementsHtml* e = 0  ) {
      name = n;
      id = n;
      parent = e;  
      text = t;     //ERROR ERROR ERROR ERROR NO HACER ESTO Serial.println (t);
      pushElement(this);          // Los elementos basicos se registran solos en el AllHTMLElemens !!
    }
    String postCallBack(ElementsHtml* e,String postValue) {
      if (parent) return parent->postCallBack(this,postValue);
      
      //return ("console.log('postCallBack of " + name+" parent: "+parent->name+"'); ");
      return "";
    } 
    String getHtml() { 
    htmlAdd( "<button type='button' width='40' id='");htmlAdd( id.c_str());htmlAdd( "' value ='") ;htmlAdd( text.c_str());htmlAdd( "' onclick=\"btnClickText('");
    htmlAdd(id.c_str()); htmlAdd( "','"); htmlAdd(text.c_str()); htmlAdd("')\" >");htmlAdd( text.c_str()); htmlAdd( "</button>\n");
     return String(ElementsHtml::htmlGet());
     }
    void update() {};
};



// ########################################
//  Combo Box HTML
// ########################################
class ComboBox: public Input {
  public:
  String *fields;
  int selected = 0;
    ComboBox(String n, int s , String _fields[] ,  ElementsHtml* e=0) {
      name = n;
      text="";
      id = n;
      fields=_fields;
      fieldsCount = s;
      value = 0;
       pushElement(this);          // Los elementos basicos se registran solos en el AllHTMLElemens !!
       
      if (e!=0) parent = e;                                                                            //ERROR ERROR ERROR ERROR NO HACER ESTO Serial.println (t);
      }

   
    String getHtml() {String html = "<select id='" + id + "' onchange=\"btnClickText('"+id+"', this.selectedIndex)\">\n";
          for(int i = 0; i <= fieldsCount ; i++) {
              html+="<option value='"+fields[i]+"'>"+fields[i]+"</option>";
          }
              html+="</select>";return  html;}
    void update() {
      text = fields[selected];
      value = selected;
      javaQueue.add("document.getElementById('" + id + "').selectedIndex='" + String(selected) + "'; console.log('"+name+" update value="+String(value)+"');");
      
    }
    String postCallBack(ElementsHtml* e,String postValue) {
                  selected = postValue.toInt();
                  update();
                  if (parent) parent->postCallBack(this,postValue);
                  return "";//"document.getElementById('" + id + "').innerHTML='" + text + "';";
    }
    private:
      int fieldsCount;
};



// ########################################
//  DSB 18B20 IN
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
     // html = " <div id='"+id+"'><h4>" + name + "</h4>"+descriptor+"<br>Temperature:" + String(value) + "</div>";
       pushElement(this);          // Los elementos basicos se registran solos en el AllHTMLElemens !!
       oneWire = new  OneWire(pin);        // Set up a OneWire instance to communicate with OneWire devices
       tempSensors = new DallasTemperature (oneWire); // Create an instance of the temperature sensor class
         tempSensors->setWaitForConversion(false); //  block the program while the temperature sensor is reading
       tempSensors->begin();
    }
    //~Dsb18B20(ElementsHtml::deleteElement(this));

    String getHtml() { return " <div id='"+id+"'><h4>" + name + "</h4>"+descriptor+"<br>Temperature:" + String(value) + "</div>";  }
    
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

    String postCallBack(ElementsHtml* e,String postValue) {update();}
  private:
  bool tempRequested = false;
  unsigned long lastTemperatureRequest;
  int intervalTemperature = 1500;
};
// ########################################
//  DHT11 IN
// ########################################

class Dht11: public Input {
    public:

    int pin;
    DHT* dhtSensor;
  
    Dht11 ( int _pin , String  _name) {
      pin = _pin;
      name = _name;
      id = _name;
      minValue = 0;
      maxValue = 150;
      unit = "grados";
      descriptor = "DHT11 T/H";
     // html = " <div id='"+id+"'><h4>" + name + "</h4>"+descriptor+"<br>Temperature:" + String(value) + "</div>";
       pushElement(this);          // Los elementos basicos se registran solos en el AllHTMLElemens !!
        dhtSensor = new DHT (pin, DHT11 );
        dhtSensor->begin();
    }
    //~Dsb18B20(ElementsHtml::deleteElement(this));

    String getHtml() { return " <div id='"+id+"'><h4>" + name + "</h4>"+descriptor+"<br>Temperature:" + String(value) + "</div>";  }
    
    void update() {
       if ( ( millis() - lastTemperatureRequest) > intervalTemperature ) {
        value = dhtSensor->readTemperature(); // Request the temperature from the sensor (it takes some time to read it)
        //tempRequested=true;
        lastTemperatureRequest = millis();
      }

      javaQueue.add( "document.getElementById('" + id + "').innerHtml='Temperature:" + String(value) + "';");

    }

    String postCallBack(ElementsHtml* e,String postValue) {update();}
  private:
  bool tempRequested = false;
  unsigned long lastTemperatureRequest=0;
  int intervalTemperature = 2000;
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
       pushElement(this);          // Los elementos basicos se registran solos en el AllHTMLElemens !!
      
    }

    String getHtml() {  return " <div id='"+id+"'><h4>" + name + "</h4>"+descriptor+"<br>"+ label->getHtml() +  "</div>";
    }
//    class Label;
//class Output;
//Label::getHtml();
    void update() {
      value = analogRead(pin);
      label->update(String(value));
    }

    String postCallBack(ElementsHtml* e,String postValue) {update();}

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
      label = new Label(name+"lbl","",this); addChild(label);
       pushElement(this);          // Los elementos basicos se registran solos en el AllHTMLElemens !!
      
    }

    String getHtml() {return " <div id='"+id+"'><h4>" + name + "</h4>"+descriptor+"<br>"+ label->getHtml() +  "</div>"; }

    void update() {
      value = digitalRead(pin);
      label->update(String(value));
    }

    String postCallBack(ElementsHtml* e,String postValue) {update();}

};


// ########################################
//  EDIT BOX HTML
// ########################################

class EditBox: public Input {
  public:
    EditBox(String n, String t, ElementsHtml* e=0) {
      name = n;
      text=t;
      id = n;
      value = 0;
       pushElement(this);          // Los elementos basicos se registran solos en el AllHTMLElemens !!
       
      if (e!=0) parent = e;                                                                            //ERROR ERROR ERROR ERROR NO HACER ESTO Serial.println (t);

    }

    String getHtml() {return  "<input type='text' id='" + id + "' width='20' onkeyup=\"btnClickText('"+id+"',(this.value))\"></input>\n";}
    void update() {
      value = text.toInt();
      javaQueue.add("document.getElementById('" + id + "').value='" + text + "';");
      
    }
    String postCallBack(ElementsHtml* e,String postValue) {
                  text = postValue;
                  update();
                  if (parent) parent->postCallBack(this,postValue);
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
//  KEYPAD HTML
// ########################################
class KeyPad: public Input  {  // Aqui lo he cambiado !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  Antes era input
  public:
    int value=0;
    String state ="ooo";
    EditBox *edt;
    EditBox *edtLabel;
          Button* buttons[11];

    KeyPad(String n) {
      name = n;
      id = n;
      edt = new EditBox(name+ "Edit","");
      edtLabel = new EditBox(name+"Label","");
//      addChild(edt);addChild(edtLabel);
      for (int i = 0; i < 10; i++ ) {

        buttons[i] = new Button (name+"btn"+String(i), String(i),this);  // esto indica que tiene pariente 
//        addChild (buttons[i]);
//yield();
      }
      buttons[10] = new Button (name, "delete",this);

    }
    String getHtml() {
      String h="<div id='"+name+"'><h4>" + name + "</h4>\n\t";
            for (int i = 0; i < 11; i++ ) {
              h+= buttons[i]->getHtml(); 
              if ((i==2)||(i==5)||(i==8)) h+="<br>";
            }
            h+=edt->getHtml()+edtLabel->getHtml()+"</div>"; 
            return h;
    }
    String postCallBack(ElementsHtml* e,String postValue) {
      if (postValue == "delete")edt->deleteChar();
      else  edt->appendText (postValue);
      update();
      return "";//( "console.log('postCallBack of "+name+"'); ");
    };
    void update() {
      (edt->text == "1234") ? value = 1 : value = 0 ; //updateDisplay();
      !value ? state = "Locked" : state = "Unlocked";
      edtLabel->setText(state);
    };
};



// ########################################
//  ULTRA SOUND PROBE
// ########################################

class UltraSoundProbe: public Input {
  public:
  Label* label;
  UltraSoundProbe ( String _name, int _pinTrig, int _pinEcho ) {
    trigPin = _pinTrig;
    echoPin = _pinEcho;
    name = _name;
    id = _name;
    label = new Label(name+"lbl","",this); addChild(label);

      pinMode(trigPin, OUTPUT);
      pinMode(echoPin, INPUT); 
  }

  void update(){
  value = getDistance();
        label->update("Distance: "+String(value));

  }

  String getHtml() {  return " <div id='"+id+"'><h4>" + name + "</h4>"+descriptor+"<br>"+ label->getHtml() +  "</div>";
    }

    
  float getDistance(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Measure the response from the HC-SR04 Echo Pin
 
  float duration = pulseIn(echoPin, HIGH);
  
  // Determine distance from duration
  // Use 343 metres per second as speed of sound
  
  return (duration / 2) * 0.0343;
  }

  
  private:
    int trigPin;
    int echoPin;
};

