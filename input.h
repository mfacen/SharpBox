// ########################################
//  INPUT
// ########################################

class Input : public ElementsHtml {

  public:

    float factor=1;
    int minValue=0;
    int maxValue=0;
    String unit;
    String text="";
    
    //String getHtml() {};
    void setFactor ( float _factor ) { factor = _factor ; }
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
                  Serial.println("Button-Post-Call-Back"+id);

      if (parent) parent->postCallBack(this,postValue);

      //return ("console.log('postCallBack of " + name+" parent: "+parent->name+"'); ");
      return "";
    } 
  String getHtml() {String  s = "<button "; s+= style ; s+=" type='button' width='40' id='";s+=id; s+= "' value ='"; s+= text; s+= "' onclick=\"btnClickText('"; s+= id;s+= "',this.value)\" >";
                      s+=text; s+= "</button>\n"; return s;  }
  //String getHtml() { String s= "<button id='";s+=id;s+="'></button> ";javaQueue.add("btn('"+id+"','"+text+"');\n"); return s;}

// void addHtml() {
//     htmlAdd("<button "); htmlAdd(style.c_str()); htmlAdd(" type='buttn' width='40' id='"); htmlAdd( id.c_str()); htmlAdd ("' value ='"); htmlAdd(text.c_str());
//     htmlAdd("' onclick=\"btnClickText('");
//     htmlAdd(id.c_str()); htmlAdd("',this.value)\" >");htmlAdd(text.c_str()); htmlAdd ("</button>\n");
// }
    void update(String sss) {
           String s= "var a=";s=+docIdStr;s+= id;s+= "'); a.value='";s+=sss;s+="'; a.textContent='";s+= sss ;s+= "';"; javaQueue.add(s);

      };
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

   
    String getHtml() {String s = "<select "; s+=style;s+=" id='";s+= id ; s+= "' onchange=\"btnClickText('";s+=id;s+="', this.selectedIndex)\">\n";
          for(int i = 0; i < fieldsCount ; i++) {
              s+="<option value='";s+=fields[i];s+="'>";s+=fields[i];s+="</option>";
          }
              s+="</select>";return  s;}
    void update() {
        if (value!=lastValue){
            text = fields[selected];
            //value = selected;
            lastValue=value;
             String s=docIdStr;s+=id ;s+= "').selectedIndex=" ;s+= String(selected);s+=";";javaQueue.add(s );//+ "'; console.log('"+name+" update value="+String(value)+"');");

          }

            
    }
    String postCallBack(ElementsHtml* e,String postValue) {
                  selected = postValue.toInt();
                  value = selected;
                  update();
                  if (parent) parent->postCallBack(this,postValue);
                  return "";//docIdStr + id + "').innerHTML='" + text + "';";
    }
    private:
      int fieldsCount;
      int lastValue;
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
      descriptor = "Dsb18B20";
      label = new Label("lbl"+id,"Temp: ",this);
     // html = " <div id='"+id+"'><h4>" + name + "</h4>"+descriptor+"<br>Temperature:" + String(value) + "</div>";
       pushElement(this);          // Los elementos basicos se registran solos en el AllHTMLElemens !!
       oneWire = new  OneWire(pin);        // Set up a OneWire instance to communicate with OneWire devices
       tempSensors = new DallasTemperature (oneWire); // Create an instance of the temperature sensor class
         tempSensors->setWaitForConversion(false); //  block the program while the temperature sensor is reading
       tempSensors->begin();
    }
    //~Dsb18B20(ElementsHtml::deleteElement(this));

    String getHtml() {String  s= " <div ";s+=style;s+="><h4>";s+= name ;s+= "</h4>";s+=descriptor;s+="<br>";s+=label->getHtml() ;s+= "</div>"; return s;  }
    
    void update() {
      if  (!tempRequested)  {
        tempSensors->requestTemperatures(); // Request the temperature from the sensor (it takes some time to read it)
        tempRequested=true;
        lastTemperatureRequest = millis();
      }
      if ( ( millis() - lastTemperatureRequest > intervalTemperature ) && tempRequested ) {
        value = tempSensors->getTempCByIndex(0); // Get the temperature from the sensor
        tempRequested=false;
        label->update(String(value));
      }
      //value = tempSensors->getTempCByIndex(0);
    }

    String postCallBack(ElementsHtml* e,String postValue) {return "";}
  private:
    Label* label;
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

    String getHtml() { return " <div "+style+"  id='"+id+"'><h4>" + name + "</h4>"+descriptor+"<br>Temperature:" + String(value) + "</div>";  }
    
    void update() {
       if ( ( millis() - lastTemperatureRequest) > intervalTemperature ) {
        value = dhtSensor->readTemperature(); // Request the temperature from the sensor (it takes some time to read it)
        //tempRequested=true;
        lastTemperatureRequest = millis();
      }

      javaQueue.add( docIdStr + id + "').innerHtml='Temperature:" + String(value) + "';");

    }

    String postCallBack(ElementsHtml* e,String postValue) {update(); return "";}
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
    AnalogIn ( int _pin , String  _name , String _unit = "anLevel" , float _factor=1 ) {
      pin = _pin;
      name = _name;
      id = _name;
      factor = _factor;
      minValue = 0;
      maxValue = 150;
      unit = _unit;
      descriptor = "Ana In ";
      label = new Label(name+"lbl","",this);
       pushElement(this);          // Los elementos basicos se registran solos en el AllHTMLElemens !!
      
    }

    String getHtml() {  String s= " <div ";s+=style;s+=" id='"; s+= id ; s+= "'><h4>" ;s+= name ;s+= "</h4>";s+=descriptor;s+="<br>";s+= label->getHtml() ;s+= unit ;s+= "</div>"; return s;
    }
//    class Label;
//class Output;
//Label::getHtml();
    void update() {
      value = analogRead(pin) * factor;
      label->update(String(value));
    }

    String postCallBack(ElementsHtml* e,String postValue) {update(); return "";}

};

// ########################################
//  Digital IN
// ########################################

class DigitalIn: public Input {
    public:

    DigitalIn ( int _pin , String  _name) {
      pin = _pin;
     // pinMode(pin,INPUT);
       
      name = _name;
      id = _name;
      minValue = 0;
      maxValue = 1;
      unit = "n/a";
      label = new Label(name+"lbl","",this); //addChild(label);
       img = new Image ("img"+id,"power-button.jpg",this);
       pushElement(this);          // Los elementos basicos se registran solos en el AllHTMLElemens !!
      
    }

    String getHtml() {String s=" <div ";s+=style;s+=" id='";s+=id;s+="'><h6>" ;s+= name ;s+= "</h6>";s+= label->getHtml() ;s+= img->getHtml() ;s+= "</div>"; return s;}

    void update() {
      value = digitalRead(pin);
      label->update(String(value));
      img->update( (value==0)?"power-button.jpg":"power-buttonON.jpg" );
    }

    String postCallBack(ElementsHtml* e,String postValue) {update();return "";}
      private:
      int pin;
      Label* label;
      Image* img;
};


// ########################################
//  EDIT BOX HTML
// ########################################

class EditBox: public Input {
  public:
    EditBox(String n, String t , String _type, ElementsHtml* e=0) {
      name = n;
      text=t;
      id = n;
      type = _type;
      value = text.toFloat();
       pushElement(this);          // Los elementos basicos se registran solos en el AllHTMLElemens !!
       
      //if (e!=0)
       parent = e;                                                                            //ERROR ERROR ERROR ERROR NO HACER ESTO Serial.println (t);

    }

    String getHtml() { String s=  F("<input ");s+=style;s+=F(" type='") ;s+= type; s+=F( "' id='"); s+= id ;s+= F("' value='");s+=text;
                              s+=F("' onchange=\"btnClickText('");s+=id; if (type=="checkbox") s+=F("',(this.checked))\">");
                                      else s=s+F("',(this.value))\">");
                                       return(s);}
    String postCallBack(ElementsHtml* e,String postValue) {

                  text = postValue;
                   //     if (text.toFloat()) value = text.toFloat();
                  update();
                  if (parent) parent->postCallBack(this,postValue);
                  //return docIdStr + id + "').innerHTML='" + text + "';";
                                    //Serial.println("EditBox->postCallBack()");
                  return "";
    }

    void disable (){
            javaQueue.add(docIdStr + id + "').setAttribute('disabled','disabled');");
    }
    void appendText (String t) {
      text += t;
      update();
    }
    void update (String t) {
                  Serial.println("EditBox->Update(String)");

      text = t;
      update();
    }
    void deleteChar () {
      text = text.substring(0, text.length() - 1);
      update();
    }
    void update() {
      if (text && ( text!=lastValue)) { value = text.toFloat(); if (text=="false") value=0;if (text=="true") value=1; lastValue = text;

         String s=docIdStr; s+= id ;s+= "').value='" ;s+= text ;s+= "';";javaQueue.add(s);
                  //Serial.println("EditBox->Update()");

      }
    }
  private:
    String type;
    String lastValue;
};



// ########################################
//  KEYPAD HTML
// ########################################
class KeyPad: public Input  {  // Aqui lo he cambiado !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  Antes era input
  public:
    int value=0;
    String state ="ooo";
    EditBox *edt;
    Label *edtLabel;
          Button* buttons[11];

    KeyPad(String n) {
      name = n;
      id = n;
      edt = new EditBox(id+ "Edit","","text",this);
      edtLabel = new Label(id+"Label","Locked",this);
//      addChild(edt);addChild(edtLabel);
      for (int i = 0; i < 10; i++ ) {

        buttons[i] = new Button (id+"btn"+String(i), String(i),this);  // esto indica que tiene pariente 
//        addChild (buttons[i]);
//yield();
      }
      buttons[10] = new Button (name, "delete",this);

    }
    String getHtml() {
      String s="<div "; s+=style;s+=" id='";s+=id;s+="'><h4>" ;s+= name ; "</h4>\n\t";
            for (int i = 0; i < 11; i++ ) {
              s+= buttons[i]->getHtml(); 
              if ((i==2)||(i==5)||(i==8)) s+="<br>";
            }
            s+=edt->getHtml();s+=edtLabel->getHtml();s+="</div>"; 
            return s;
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
      edtLabel->update(state);
    };
};



// ########################################
//  ULTRA SOUND PROBE
// ########################################

class UltraSoundProbe: public Input {
  public:
  Label* label;
  AverageModule average;
  UltraSoundProbe ( String _name, int _pinTrig, int _pinEcho ) {
    trigPin = _pinTrig;
    echoPin = _pinEcho;
    name = _name;
    id = _name;
    label = new Label(name+"lbl","",this); //addChild(label);

      pinMode(trigPin, OUTPUT);
      pinMode(echoPin, INPUT); 
  }

  void update(){
  value = getDistance();
  average.addValue(value);
  value=average.getAverage();
  
        label->update("Distance: "+String(value));

  }

  String getHtml() {  return " <div "+style+" id='"+id+"'><h6>" + name + "</h6>"+descriptor+"<br>"+ label->getHtml() +  "</div>";
    }

    
  float getDistance(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Measure the response from the HC-SR04 Echo Pin
 
  float duration = pulseIn(echoPin, HIGH , 30000 );
  
  // Determine distance from duration
  // Use 343 metres per second as speed of sound
  
  return (duration / 2) * 0.0349;
  }

  
  private:
    int trigPin;
    int echoPin;
};

