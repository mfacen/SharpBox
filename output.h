// ########################################
//  OUTPUT
// ########################################

  class Output:public ElementsHtml {

    
  public:
  
  String stateStr;
  //float value=0;
  int minValue;
  int maxValue;
  String unit;
  bool invertedLogic = true;
   virtual void update( float newValue){};
  String getHtml(){}
      //void toogle(){ value? value=0:value=1;}

    };

  //Output operator++ () { value++;}
// ########################################
//  LABEL
// ########################################
#include <umm_malloc/umm_malloc.h> // for computing memory fragmentation

class Label: public Output {
  public:
          String text="Output:Label";

    Label ( String n , String t, ElementsHtml* e = 0 ){
      name = n;
      id = n;
      text = t;
      parent = e;
             pushElement(this);          // Los elementos basicos se registran solos en el AllHTMLElemens !!

    }
    String getHtml(){String  s= F("<span id='");s+=id;s+= "'>";s+=text;s+="</span>"; return s; }
    //void addHtml(){ String s= "<span id='";s+=id;s+= "'>";s+=text;s+="</span>"; htmlAdd(s.c_str()); }
    String postCallBack(ElementsHtml* e,String postValue) { if(parent) return parent->postCallBack(this,postValue); }
    void update ( String newValue ) {  {text= newValue; update();} }
    void update(){ if (lastText!=text) {String s=docIdStr; s+= id ; s+= F("').innerHTML='");s+=text;s+="';"; javaQueue.add(s); lastText=text;} } 
    void update(float newValue){update(String(newValue));}
    void append(String appendValue) { text += appendValue; update();}

  private:
      String lastText = "&^%$";

};

const size_t block_size = 8;
size_t getTotalAvailableMemory() {
  umm_info(0, 0);
  return ummHeapInfo.freeBlocks * block_size;
}

size_t getLargestAvailableBlock() {
  umm_info(0, 0);
  return ummHeapInfo.maxFreeContiguousBlocks * block_size;
}


class LabelFreeHeap: public Label {
  public:
  using Label::Label;
  void update(){ 

    int percentage =  100 - getLargestAvailableBlock() * 100.0 / getTotalAvailableMemory();
     String s=docIdStr;s+= id ;s+= F("').innerHTML='Heap: ");s+=String(ESP.getFreeHeap(),DEC);s+=F(" Frag: ");s+=String(percentage);s+="%';";javaQueue.add(s);// for size_t
  }
};

class TimeLabel: public Output {
        String text="Output:Label";

public:
      TimeLabel ( String n , String t, ElementsHtml* e = 0 ){
      name = n;
      id = n;
      text = t;
      parent = e;
      pushElement(this);          // Los elementos basicos se registran solos en el AllHTMLElemens !!
      String s="var now = new Date(); btnClickText('";s+=id;s+="',now.getTime()/1000);";
      javaQueue.addOnLoad(s);
    }
    String postCallBack(ElementsHtml* e,String postValue) { if(parent) return parent->postCallBack(this,postValue); }

        String getHtml(){ String s= "<span id='"; s+=id;s+= "'>";s+=text;s+="</span>"; return s;  }
       // void addHtml(){ String s= "<span id='"; s+=id;s+= "'>";s+=text;s+="</span>"; htmlAdd( s.c_str()) ;  }
  void update( uint32_t t ) { tt=t;value=t; String s=F("var now = new Date(");s+=String(t);s+="*1000); ";s+=docIdStr;s+= id; s+= F("').innerHTML=now.toString();"); javaQueue.add(s); }
  void update(){}
  void update(float t){}
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
    String getHtml(){ String s= "<div id='";s+=id;s+="div'><h5>";s+=text;s+="</h5><table id='";s+=id;s+= F("'><tr><td></td><td></td></tr></table></div>"); return s;  }
    //void addHtml(){ String s= "<div id='";s+=id;s+="div'><h5>";s+=text;s+="</h5><table id='";s+=id;s+= "'><tr><td></td><td></td></tr></table></div>"; htmlAdd( s.c_str());  }
    String postCallBack(ElementsHtml* e,String postValue) { if(parent) return parent->postCallBack(this,postValue); }
    //void update ( String newValue ) { text= newValue; javaQueue.add(docIdStr + id + "').innerHTML='<h5>"+text+"</h5>';");}
    void setTitle (String sss){text=sss;}
    void addRow ( String newRow ) { javaQueue.add(docIdStr + id + F("').insertRow().insertCell(0).innerHTML='")+newRow+"';");}
   // void makeTable(String csv){ javaQueue.add(docIdStr + id + "') = makeTable(csv);"); }
    void makeTable(String csv) { 
      
      String content = F("<table id='tableDir'><thead><tr><th>Peso (Kg)</th><th>Distancia (cm)</th></tr></thead>");
      int maxIndex = csv.length() - 1;
    int index=0;
    int next_index;
    String data_word;
    do{
 
        next_index=csv.indexOf(',',index);
        data_word=csv.substring(index, next_index);
        content += F("<tr><td onclick='clickedCell(this)'>") ;content+= data_word  +"</td>";
        index=next_index+1;
        next_index=csv.indexOf(',',index);
        data_word=csv.substring(index, next_index);
        
        content+="<td>" + data_word + "</td></tr>";
        //Serial.println(data_word);
        index=next_index+1;
     }while((next_index!=-1)&&(next_index<maxIndex));

      content += "</table>";
       javaQueue.add(docIdStr + id + "div').innerHTML=\""+content+"\";");  // Atencion si Content ya contiene semicolons simples no puedo usarlo aqui !!!
    }
    void changeCell ( String newText , int indexRow , int indexColumn ) {
      String s=F("var createdText=document.createTextNode('");s=s+newText;s=s+"');";s+= docIdStr; s+= id ;s+= F("').getElementsByTagName('tbody')[0].getElementsByTagName('tr')[");
                      s+=String(indexRow);s+=F("].getElementsByTagName('td')[");s+=String(indexColumn);s+=F("].childNodes[0]=createdText;"); javaQueue.add(s);//    insertRow().insertCell("+String(indexY)+").innerHTML='"+newText+"';");
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
    String getHtml(){ String s=F("<img heigth='"); s+=String(heigth); s+= F("' width='"); s+=String(width);s+="' id='";s+=id;s+= "' src='";s+=url;s+="'>";return s;  }
    String postCallBack(ElementsHtml* e,String postValue ) { if(parent) return parent->postCallBack(this,postValue); }
    void update ( String newValue ) { url= newValue;String s=docIdStr;s+= id;s+= "').src='";s+=url;s+="';"; javaQueue.add(s);}
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
    Graphic(String ss, ElementsHtml* e=0){ name=ss;id=ss;parent=e;}
    String getHtml(){ return "<canvas id='"+id+"' heigth='130'></canvas><script>var xPos=0;</script>";  }
    String postCallBack(ElementsHtml* e,String postValue ) { if(parent) return parent->postCallBack(this,postValue); }
    void update ( float newValue ) {
          String str;
          str+="var canvas = ";str+=docIdStr;str+=id;str+=F("');\n"
          "ctx=canvas.getContext('2d');\n"
          "ctx.strokeStyle = 'blue';\n" 
          "ctx.beginPath();\n"
          "ctx.arc(xPos*4,canvas.height-2 - ");
          str += String(newValue*5);
          str+=F(", 2, 0 , 2*Math.PI );\n"
          "ctx.fillStyle = 'blue';\n"
          "ctx.fill();\n"
          "ctx.stroke();\n"
         "ctx.closePath();\n"
         "xPos++; if ( xPos*4>canvas.width ) {  ctx.clearRect(0, 0, canvas.width, canvas.height);   xPos = 0;     generateCanvas();   }"
         "function generateCanvas(){ var canvas = document.getElementById('"); str+= id; str+= F("');  ctx=canvas.getContext('2d');"
                "  ctx.font = '10px serif';  ctx.fillStyle = 'blue';    ctx.fillText('");str+=name;str+=F("', 20, 15);    ctx.stroke();} ");
          javaQueue.add(str);
          //xPos++; if(xPos==
    }
    void update(String sss){update(sss.toInt());}
    void update(){ } 
      // void update( float newValue) {value=newValue;update();}

  private:
    //int xPos=0;
};

// ########################################
//  Chart
// ########################################
class Chart: public Output {
  public:
    Chart (String ss, ElementsHtml* e=0){ name=ss;id=ss;parent=e;}
    String getHtml(){ String s="<canvas id='"+id+"' heigth='130'></canvas>"; 
                      String ss="var i=document.createElement('script');i.src='/chart.js';";
                      ss+="var j=document.createElement('script');j.src='/Chart-min.js';";
                      ss+="var k=document.createElement('script');k.src='/moment.min.js';";
                       javaQueue.addOnLoad(ss); return(s);   }
    String postCallBack(ElementsHtml* e,String postValue ) { if(parent) return parent->postCallBack(this,postValue); }
    void update ( float newValue ) {
          String str = "addChartData("; str+= String(newValue);str+=");";
          
          javaQueue.add(str);
          //xPos++; if(xPos==
    }
    void update(String sss){update(sss.toFloat());}  
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
        unit = "on/off";
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
      void update(String sss){update(sss.toInt());};
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
            //javaQueue.add(docIdStr + label->id + "').innerHTML='"+stateStr+"';");
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
    String getHtml(){ String s= "<div><h6>"; s+=name; s+="</h6>"; s+=label->getHtml();s+="<br>";s+=img->getHtml();s+="</div>"; return s; }
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
    Gauge(String sss, ElementsHtml* e=0){ name=sss;id=sss;if(parent)parent=e;value =0;label=new Label("lbl"+id,"x");
    String s="var i=document.createElement('script');i.src='gauge.min.js';\nvar j=document.createElement('script');j.src='gaugeScript.js';\n";
    s+="var target = document.getElementById('";s+=id;s+="');window.";s+=id;s+=" = new Gauge(target).setOptions(opts);\n"; s+="window.Prueba.setMaxValue(40);\n"; // set max gauge value
    s+= "window.Prueba.setMinValue(0);\n";   javaQueue.addOnLoad(s);}

    String getHtml(){ String s="<div><h4>";s+=name+"</h4><canvas id='";
                                s+=id;s+="' height='90' width='140'></canvas><br><center>";s+=label->getHtml();s+="</div>";return(s);  }

    String postCallBack(ElementsHtml* e,String postValue ) { if(parent) return parent->postCallBack(this,postValue);else return ""; }
    void update(int v){value=v;update();}
       void update( float newValue) {value=newValue;update();}
    void update(){
      String str; if (firstRun) {str+="initiateGauge();"; firstRun=0;}
      str="window.";str+= id+".set("+String(value)+");";
      javaQueue.add(str);
      label->update(value);
    }
  private:
    Label* label;
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
      String s=docIdStr; s+=id; s+= "Meter').value="; s+=String(value)+";"; javaQueue.add(s);
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
    void update(){ String s=docIdStr; s+= id ; s+="Meter').value="; s+=String((int)value);s+=";";javaQueue.add(s); label->update(value); } 
    void update(float newValue){value=newValue;update();}

   private:
    int _max,_min; String unit;
};




