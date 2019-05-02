// ########################################
//  OUTPUT
// ########################################

  class Output:public ElementsHtml {

    
  public:
  
  String stateStr;
  float value=0;
  int minValue;
  int maxValue;
  String unit;
  bool invertedLogic = true;
   virtual void update( float newValue)=0;
  String getHtml(){}
      //void toogle(){ value? value=0:value=1;}

    };

  //Output operator++ () { value++;}
// ########################################
//  LABEL
// ########################################

class Label: public Output {
  public:
  String text="Output::Label";
    Label ( String n , String t, ElementsHtml* e = 0 ){
      name = n;
      id = n;
      text = t;
      parent = e;
             pushElement(this);          // Los elementos basicos se registran solos en el AllHTMLElemens !!

    }
    String getHtml(){ return "<span id='"+id+ "'>"+text+"</span>";  }
    String postCallBack(ElementsHtml* e,String postValue) { if(parent) return parent->postCallBack(this,postValue); }
    void update ( String newValue ) { text= newValue; update(); }
    void update(){ javaQueue.add("document.getElementById('" + id + "').innerHTML='<h5>"+text+"</h5>';"); } 
    void update(float newValue){update(String(newValue));}
    void append(String appendValue) { text += appendValue; update();}
};

class LabelFreeHeap: public Label {
  public:
  using Label::Label;
  void update(){ javaQueue.add("document.getElementById('" + id + "').innerHTML='V: "+String(ESP.getFreeHeap(),DEC)+"';"); }
};
class TimeLabel: public Label{
public:
      using Label::Label;
        String getHtml(){ return "<span id='"+id+ "'>"+text+"</span>";  }
  void update( long t ) { tt=t;value=t; javaQueue.add("var now = new Date("+String(t)+"*1000); document.getElementById('" + id + "').innerHTML=now.toString();"); }

private:
  long tt;
};
// ########################################
//  TABLE
// ########################################
class Table: public Output {
  public:
  String text="Table Title";
    Table ( String n , ElementsHtml* e = 0 ){
      name = n;
      id = n;
      parent = e;
             pushElement(this);          // Los elementos basicos se registran solos en el AllHTMLElemens !!

    }
    String getHtml(){ return "<div id='"+id+"div'><h5>"+text+"</h5><table id='"+id+ "'><tr><td></td><td></td></tr></table></div>";  }
    String postCallBack(ElementsHtml* e,String postValue) { if(parent) return parent->postCallBack(this,postValue); }
    //void update ( String newValue ) { text= newValue; javaQueue.add("document.getElementById('" + id + "').innerHTML='<h5>"+text+"</h5>';");}
    void setTitle (String s){text=s;}
    void addRow ( String newRow ) { javaQueue.add("document.getElementById('" + id + "').insertRow().insertCell(0).innerHTML='"+newRow+"';");}
   // void makeTable(String csv){ javaQueue.add("document.getElementById('" + id + "') = makeTable(csv);"); }
    void makeTable(String csv) { 
      
      String content = "<table id='tableDir'><thead><tr><th>Peso (Kg)</th><th>Distancia (cm)</th></tr></thead>";
      int maxIndex = csv.length() - 1;
    int index=0;
    int next_index;
    String data_word;
    do{
 
        next_index=csv.indexOf(',',index);
        data_word=csv.substring(index, next_index);
        content += "<tr><td onclick='clickedCell(this)'>" + data_word  +"</td>";
        index=next_index+1;
        next_index=csv.indexOf(',',index);
        data_word=csv.substring(index, next_index);
        
        content+="<td>" + data_word + "</td></tr>";
        //Serial.println(data_word);
        index=next_index+1;
     }while((next_index!=-1)&&(next_index<maxIndex));

      content += "</table>";
       javaQueue.add("document.getElementById('" + id + "div').innerHTML=\""+content+"\";");  // Atencion si Content ya contiene semicolons simples no puedo usarlo aqui !!!
    }
    void changeCell ( String newText , int indexRow , int indexColumn ) {
      javaQueue.add("var createdText=document.createTextNode('"+newText+"');"
      "document.getElementById('" + id + "').getElementsByTagName('tbody')[0].getElementsByTagName('tr')["+String(indexRow)+"].getElementsByTagName('td')["+String(indexColumn)+"].childNodes[0]=createdText;");//    insertRow().insertCell("+String(indexY)+").innerHTML='"+newText+"';");
    }
    void update(){ } 
       void update( float newValue) {value=newValue;update();}

    private:
    
};

// ########################################
//  IMAGE
// ########################################

class Image: public Output {
  public:
    Image ( String n , String t, ElementsHtml* e = 0 ){
      name = n;
      id = n;
      url = t;
      parent = e;
      pushElement(this);          // Los elementos basicos se registran solos en el AllHTMLElemens !!
    }
    String getHtml(){ return "<img heigth='"+String(heigth)+"' width='"+String(width)+"' id='"+id+ "' src='"+url+"'>";  }
    String postCallBack(ElementsHtml* e,String postValue ) { if(parent) return parent->postCallBack(this,postValue); }
    void update ( String newValue ) { url= newValue; javaQueue.add("document.getElementById('" + id + "').src='"+url+"';");}
    void update(float f){ } 
    void update(){}
    void setWidth(int w){width=w;}
    void setHeigth(int h){heigth=h;}
  private:
    String url = "";
    int width = 40;
    int heigth = 40;

};

// ########################################
//  Graphic
// ########################################

class Graphic: public Output {
  public:
    Graphic(String s, ElementsHtml* e=0){ name=s;id=s;parent=e;}
    String getHtml(){ return "<canvas id='"+id+"' heigth='130'></canvas><script>var xPos=0;</script>";  }
    String postCallBack(ElementsHtml* e,String postValue ) { if(parent) return parent->postCallBack(this,postValue); }
    void update ( float newValue ) {
          String str;
          str+="var canvas = document.getElementById('"+id+"');\n"
          "ctx=canvas.getContext('2d');\n"
          "ctx.strokeStyle = 'blue';\n" 
          "ctx.beginPath();\n"
          "ctx.arc(xPos*4,canvas.height-2 - "+String(newValue*5)+", 2, 0 , 2*Math.PI );\n"
          "ctx.fillStyle = 'blue';\n"
          "ctx.fill();\n"
          "ctx.stroke();\n"
         "ctx.closePath();\n"
         "xPos++; if ( xPos*4>canvas.width ) {  ctx.clearRect(0, 0, canvas.width, canvas.height);   xPos = 0;     generateCanvas();   }"
         "function generateCanvas(){ var canvas = document.getElementById('"+id+"');  ctx=canvas.getContext('2d');"
                "  ctx.font = '10px serif';  ctx.fillStyle = 'blue';    ctx.fillText('"+name+"', 20, 15);    ctx.stroke();} ";
          javaQueue.add(str);
          //xPos++; if(xPos==
    }
    void update(String s){update(s.toInt());}
    void update(){ } 
      // void update( float newValue) {value=newValue;update();}

  private:
    //int xPos=0;
};


          
// ########################################
//  DIGITAL OUTPUT
// ########################################
class PWM
{
  public:
  PWM (int _period ) {
    period = _period;
  previousMillis = 0;
  }
 
  bool update()
  {
    unsigned long currentMillis = millis();
     bool result;
    if(((currentMillis - previousMillis >= (period/10)*level))) { result = false; } 
    else 
      { result = true; }

    if  (  ( currentMillis - previousMillis )  > period ) { previousMillis = currentMillis; }
    return result;
    }
   void setLevel(int i){level=i;}
  private:
    unsigned long previousMillis;
    int period=0;
    int level=0; // level goes from 0ne to ten;
};

  class DigitalOutput:public Output {

  public:
        DigitalOutput ( int _pin , String _name, String _id, bool inverted=false ) {          /// Constructor
        pin = _pin;
        id= _id;
        name = "" + id;
        minValue = 0;
        maxValue = 1;
        unit = "apagado/encendido";
        descriptor = name;
        pinMode ( pin , OUTPUT );
        stateStr="OFF";
        label = new Label ("lbl"+id,stateStr,this);
        img = new Image ("img"+id,"power-button.jpg",this);
        pinMode(pin,OUTPUT);
        invertedLogic = inverted;
         };
      void setPWM ( PWM * _pwm) { pwm = _pwm; }
      void setLevel ( int l) { pwm->setLevel(l);}
      void update(){};
      void update(String s){update(s.toInt());};
      void update( float newValue) {
        Serial.println("Updating "+id+" Value: " + String(newValue));
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
            img->update( stateStr=="OFF"?"power-button.jpg":"power-buttonON.jpg" );
            label->update(stateStr);
       }
      void  updatePWM (){ if ( (stateStr=="ON") && (pwm!=0) )  ( pwm->update() ) ?  digitalWrite(pin,LOW): digitalWrite (pin,HIGH);   } 
      void changePWMLevel(int i ) { pwm->setLevel(i); }
      void toogle(){
        if (value==0){
          update(1);
        }
        else{
          update(0);
        }
      }
    String getState(){  return stateStr; }
    String getHtml(){ return "<div><h6>"+name+"</h6>"+label->getHtml()+img->getHtml()+"</div>";  }
    String postCallBack(ElementsHtml* e,String postValue ) {  return "";      }
    private:
      int pin;
      PWM* pwm;
      Label* label;
      Image* img;
  };

// ########################################
//  RELAY OUTPUT
// ########################################

class RelayOutput: public DigitalOutput {
  using DigitalOutput::DigitalOutput;       // inherit the constructor
  public:
 // invertedLogic = true;

};


// ########################################
//  GAUGE
// ########################################

class Gauge: public Output {
  public:
  bool firstRun = true;
    Gauge(String s, ElementsHtml* e=0){ name=s;id=s;parent=e;}
    String getHtml(){ return "<script src='gauge.min.js'></script><script src='gaugeScript.js'></script><canvas id='"+id+"' heigth='60' width='100'></canvas>";  }
    String postCallBack(ElementsHtml* e,String postValue ) { if(parent) return parent->postCallBack(this,postValue);else return ""; }
    void update(int v){value=v;update();}
       void update( float newValue) {value=newValue;update();}
    void update(){
      String str; if (firstRun) {str+="initiateGauge();"; firstRun=0;}
      str+= "injectedID='"+name; ///+"';ODgauge.set("+String(value)+");";
      javaQueue.add(str);
    }
};


// ########################################
//  METER
// ########################################


class Meter: public Output {
  public:
    Label* lblValue;
    Meter ( String n , String _name, String unit_ , int min, int max, ElementsHtml* e = 0 ){
      name = _name;
      unit = unit_;
      id = n;
      _max=max;
      _min=min;
      lblValue = new Label ( "lbl"+id,"",this);
      parent = e;
             pushElement(this);          // Los elementos basicos se registran solos en el AllHTMLElemens !!

    }
    String getHtml(){ return "<div id='"+id+"'><h5>"+name+"</h5>"+lblValue->getHtml()+"<meter id='"+id+ "Meter' min='"+String(_min)+"' max='"+String(_max)+"'></meter>"+unit+"</div>";  }
    String postCallBack(ElementsHtml* e,String postValue) { if(parent) return parent->postCallBack(this,postValue);else return ""; }
    //void update ( String newValue ) { update(newValue.toInt());}
    void update(){
      javaQueue.add("document.getElementById('" + id + "Meter').value="+String(value)+";");
      lblValue->update(String(value)+" kg");
    } 
    void update(float newValue){value=newValue;update();}

   private:
    int _max,_min; String unit;
};

// ########################################
//  SLIDER
// ########################################


class Slider: public Output {
  public:
    Label* label;
    Slider ( String n , String _name, String unit_ , int min, int max, ElementsHtml* e = 0 ){
      name = _name;
      unit = unit_;
      id = n;
      _max=max;
      _min=min;
      parent = e;
             pushElement(this);          // Los elementos basicos se registran solos en el AllHTMLElemens !!
    label = new Label ( "lbl"+name , "" , this );
    }
    String getHtml(){ return "<div id='"+id+"'><h5>"+name+"</h5><input type='range' id='"+id+ "Meter' min='"+String(_min)+"' max='"+String(_max)+"' onchange='btnClick(this.value)'>"+unit+label->getHtml()+"</div>";  }
    String postCallBack(ElementsHtml* e,String postValue) { if(parent) return parent->postCallBack(this,postValue); value = postValue.toFloat(); return ""; }
    //void update ( String newValue ) { update(newValue.toInt());}
    void update(){ javaQueue.add("document.getElementById('" + id + "Meter').value="+String((int)value)+";"); label->update(value); } 
    void update(float newValue){value=newValue;update();}

   private:
    int _max,_min; String unit;
};




