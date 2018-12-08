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
  virtual void update( int newValue){};
  String getHtml(){};
    };

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
    String getHtml(){ return "<span id='"+id+ "'><h5>"+text+"</h5></span>";  }
    String postCallBack(ElementsHtml* e,String postValue) { if(parent) return parent->postCallBack(this,postValue); }
    void update ( String newValue ) { text= newValue; javaQueue.add("document.getElementById('" + id + "').innerHTML='<h5>"+text+"</h5>';");}
    void update(){ javaQueue.add("document.getElementById('" + id + "').innerHTML='V: "+text+"';"); } 
    void update(int newValue){update(String(newValue));}
};

class LabelFreeHeap: public Label {
  public:
  using Label::Label;
  void update(){ javaQueue.add("document.getElementById('" + id + "').innerHTML='V: "+String(ESP.getFreeHeap(),DEC)+"';"); }
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
    String getHtml(){ return "<img id='"+id+ "' src='"+url+"'>";  }
    String postCallBack(ElementsHtml* e,String postValue ) { if(parent) return parent->postCallBack(this,postValue); }
    void update ( String newValue ) { url= newValue; javaQueue.add("document.getElementById('" + id + "').src='"+url+"';");}
    void update(int){ } 
    void update(){}
  private:
    String url = "";
};

// ########################################
//  Graphic
// ########################################

class Graphic: public Output {
  public:
    Graphic(String s, ElementsHtml* e=0){ name=s;id=s;parent=e;}
    String getHtml(){ return "<canvas id='"+id+"' heigth='130'></canvas><script>var xPos=0;</script>";  }
    String postCallBack(ElementsHtml* e,String postValue ) { if(parent) return parent->postCallBack(this,postValue); }
    void update ( int newValue ) {
          String str;
          str+="var canvas = document.getElementById('"+id+"');\n";
          str+="ctx=canvas.getContext('2d');\n";
          str+="ctx.strokeStyle = 'blue';\n"; 
          str+="ctx.beginPath();\n";
          str+="ctx.arc(xPos*4,canvas.height-2 - "+String(newValue*5)+", 2, 0 , 2*Math.PI );\n";
          str+="ctx.fillStyle = 'blue';\n";
          str+="ctx.fill();\n";
          str+="ctx.stroke();\n";
          str+="ctx.closePath();\n";
          str+="xPos++; if ( xPos*4>canvas.width ) {  ctx.clearRect(0, 0, canvas.width, canvas.height);   xPos = 0;     generateCanvas();   }";
          str+="function generateCanvas(){ var canvas = document.getElementById('"+id+"');  ctx=canvas.getContext('2d');"+
                "  ctx.font = '10px serif';  ctx.fillStyle = 'blue';    ctx.fillText('"+name+"', 20, 15);    ctx.stroke();} ";
          javaQueue.add(str);
          //xPos++; if(xPos==
    }
    void update(String s){update(s.toInt());}
    void update(){ } 
  private:
    //int xPos=0;
};


          
// ########################################
//  DIGITAL OUTPUT
// ########################################


  class DigitalOutput:public Output {

  public:
        DigitalOutput ( int _pin , String _name, String _id, bool inverted=false ) {          /// Constructor
        pin = _pin;
        id= _id;
        name = "D-Out" + id;
        minValue = 0;
        maxValue = 1;
        unit = "apagado/encendido";
        descriptor = name;
        pinMode ( pin , OUTPUT );
        stateStr="OFF";
        label = new Label ("lbl"+id,stateStr,this);
        img = new Image ("img"+id,"LampOn.bmp",this);
        pinMode(pin,OUTPUT);
        invertedLogic = inverted;
         };
      void update(){};
      void update(String s){update(s.toInt());};
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
            img->update( stateStr=="OFF"?"LampOff.bmp":"LampOn.bmp" );
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
    String getHtml(){ return "<div><h4>"+name+"</h4>"+label->getHtml()+img->getHtml()+"</div>";  }
    String postCallBack(ElementsHtml* e,String postValue ) {  return "";      }
    private:
      int pin;
      Label* label;
      Image* img;
  };

class RelayOutput: public DigitalOutput {
  using DigitalOutput::DigitalOutput;       // inherit the constructor
  public:
 // invertedLogic = true;

};

class Gauge: public Output {
  public:
  bool firstRun = true;
    Gauge(String s, ElementsHtml* e=0){ name=s;id=s;parent=e;}
    String getHtml(){ return "<script src='gauge.min.js'></script><script src='gaugeScript.js'></script><canvas id='"+id+"' heigth='60' width='100'></canvas>";  }
    String postCallBack(ElementsHtml* e,String postValue ) { if(parent) return parent->postCallBack(this,postValue); }
    void update(int v){value=v;update();}
    void update(){
      String str; if (firstRun) {str+="initiateGauge();"; firstRun=0;}
      str+= "injectedID='"+name; ///+"';ODgauge.set("+String(value)+");";
      javaQueue.add(str);
    }
};


class Meter: public Output {
  public:
    Meter ( String n , String _name, String unit_ , int min, int max, ElementsHtml* e = 0 ){
      name = _name;
      unit = unit_;
      id = n;
      _max=max;
      _min=min;
      parent = e;
             pushElement(this);          // Los elementos basicos se registran solos en el AllHTMLElemens !!

    }
    String getHtml(){ return "<div id='"+id+"'><h5>"+name+"</h5><meter id='"+id+ "Meter' min='"+String(_min)+"' max='"+String(_max)+"'></meter>"+unit+"</div>";  }
    String postCallBack(ElementsHtml* e,String postValue) { if(parent) return parent->postCallBack(this,postValue); }
    //void update ( String newValue ) { update(newValue.toInt());}
    void update(){ javaQueue.add("document.getElementById('" + id + "Meter').value="+String((int)value)+";"); } 
    void update(float newValue){value=newValue;update();}

   private:
    int _max,_min; String unit;
};




