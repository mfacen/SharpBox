// ########################################
//  Commands
// ########################################
class Program;
;
class Commands: public ElementsHtml{
  public:
  bool executing=false;
  virtual bool run(){};
  //String getHtml(){};
};

// #######################################
//  Command   SET
// ########################################

class Set:public Commands {
public:
  Output* output;
  String f[2] = { "OFF" , "ON" };
  //ComboBox* comboBox1; 
  //EditBox* editOutput;
  Button* btnON;
  Button* btnOFF;
  int value;
  Set(String n, Output* out){
    output = out;
    name = n;
    id = n;
   //comboBox1 = new ComboBox ( name+"combo",2,f);
    btnON = new Button ("btnON"+id,"ON",this);
    btnOFF = new Button ("btnOFF"+id,"OFF",this);
  }
  bool run () {
    //output->update(comboBox1->value);
    return true;
  }
  String getHtml(){  String s = "<div class='";s+=name;s+="' ";s+=style;s+=" id='";s+=name;s+="'><h6>";s+=name;s+="</h6>";s+=output->getHtml();
                                s+="<br>"; s+= btnON->getHtml(); s+= btnOFF->getHtml() ; s+= "</div>"; return s; }
  String postCallBack(ElementsHtml* e,String postValue){ if ( e==btnON )  output->update(0); if (e==btnOFF) output->update(1);Serial.println(e->id);return "";}
void update(){}
  
};

// #######################################
//  Command   InputPanel
// ########################################       
class InputPanel:public Commands {
  public:
     Input* input;
     int value;
     InputPanel(String n, Input* inp){
    input = inp;
    name = n;
    id = n;
  }
  bool run () {
    input->update();
    return true;
  }
  String getHtml(){  return "<div class='"+name+"' "+style+" id='"+name+"'><h6>"+name+"</h6>"+input->getHtml() +"</div>";  }
  String postCallBack(ElementsHtml* e,String postValue){};
void update(){ input->update();}
//void update ( String s ) { input->update(s);}
};

// #######################################
//  Command   InputPanelText
// ########################################       
class InputPanelText:public Commands {
  public:
     InputPanelText(String n, String s, String editText){
    name = n;
    id = n;
    label = new Label ( "lbl"+name , s );
    edit = new EditBox ( "edt"+name , editText, "number",this);
  }
  bool run () {
    edit->update();
    return true;
  }
  String getHtml(){  return "<div class='"+name+"' "+style+" id='"+name+"'><h6>"+name+"</h6>"+ label->getHtml() + edit->getHtml()  +"</div>";  }
  String postCallBack(ElementsHtml* e,String postValue){Serial.print("Post Call Back de "+this->name+" value: "+postValue); return "";};
void update(){ edit->update();}
void update ( String s ) { label->update(s);}
float getValue () { return edit->value;Serial.println("InputPanelText.getValue()");}
  void readOnly(){edit->setDisabled(true);}
  private:
  EditBox* edit;
  Label* label;
  
};



// #######################################
//  Command   PAUSE
// ########################################             // Pause No tenia PostCallBack !!!!!!!!!  Era ese el problema ????
class Pause: public Commands {
    public:
    int value;
    bool firstRun=true;
    Pause(String s , int i ){
      name=s;interval=i;
      value=i;
      editTime  = new EditBox (name+"edtBox",String(interval),"text",this);
//      addChild(editTime);
      }

    void setInterval(int i){ interval = i ; value = i;}
    
   String getHtml(){String s = "<div class='"; s+= name; s+= "' "; s+= style; s+=" id='"; s+= name; s+= "'><h4>"; s+= name; s+= "</h4><br>Time: ";
                              s+=editTime->getHtml(); s+= "</div>"; return s;
   }
  bool run(){  
      if (firstRun){firstRun=false; start();}
      if  (  (lastTimeCheck - millis()  ) > 1000 ) { lastTimeCheck = millis (); update();if(value==0)value=interval;value-=1; }
      if (( millis()-lastUpdate ) > interval*1000 ) {firstRun=true;return true;} else return false;
  };
  String postCallBack(ElementsHtml* e,String postValue){return "";}
  void start( ) { lastUpdate  = millis();}
  void update(){} //javaQueue.add("document.getElementById('" + editTime->id + "').setAttribute('innerHTML', '"+String(value)+"');");    }
  private:
    unsigned long lastUpdate;
    int interval;
    unsigned long currentMillis;
    unsigned long lastTimeCheck;
        EditBox* editTime;

};

// ########################################
//  Command Logger
// ########################################
class Logger: public Commands {
  public:
    Logger ( String s , String _fileName , int i = 10) { 
        id=s;name=s;fileName=_fileName;
        label= new Label ("lbl"+name,"Stoped.");
         pause = new Pause("tmr"+name,10);
         comboBox = new ComboBox ( "combo"+id, 2 , f,this );
         edtInterval = new EditBox ("edt"+id,"","number",this);
         interval = i;
    }

    void addInput ( Input *i ) { if (index<9) {inputArray[index]=i;index++;}}
    void addOutput ( Output *o ) { if (indexO<9) {outputArray[indexO]=o;indexO++;}}
    void addFloat ( String name, float *f ) {
                                               if (indexF<9) {
                                        names[indexF]=name;floatArray[indexF]=f;indexF++;}}
    bool run(){return logData();}

    void update(){
                if ( (index>0) && ( pause->run() ) )  {
                  for ( int i=0; i<index; i++ ) {  inputArray[i]->update(); }
                  pause->setInterval(interval);
                  logData();
                }
                label->update("Tmr: "+String(pause->value) );
              }
    void startTimer(){pause->run();}
    void setInterval ( int _interval) { interval = _interval ;pause->setInterval(interval); edtInterval->update(String(interval)); }

    bool logData(){
            bool r=false;

      if (comboBox->value){
      File tempLog ;
      tempLog = SPIFFS.open(fileName , "a"); // Write the time and the temperature to the csv file
      if (index>0){
        for ( int i=0; i<index; i++ ) {
         tempLog.println(inputArray[i]->getName()+" , "+String(inputArray[i]->value));
        }
      }
      if (indexO>0){
        for ( int i=0; i<indexO; i++ ) {
         tempLog.println(outputArray[i]->getName()+" , "+String(outputArray[i]->value));
        }
      }
      if (indexF>0){
        for ( int i=0; i<indexF; i++ ) {
          float temp = *floatArray[i];
         tempLog.println(names[i]+" , "+String(temp));
        }
      }

            tempLog.close();
            #ifndef debug
            Serial.println("Data Logged");
            #endif

    }
          return r;

    }
      String getHtml(){
        String str= "<div class='";str+=name;str+="' ";str+= style; str+=" id='"; str+= name ; str+= "'><h6>"; str+= name; str += "</h6>"; str += comboBox->getHtml();
        str+= edtInterval->getHtml(); str += "<br>Inputs:<br>";
        if (index!=0) {
         for ( int i=0; i<index; i++ ) { str += inputArray[i]->getName()  + "<br>"; }
         } 
                 if (indexO!=0) {
         for ( int i=0; i<indexO; i++ ) { str += outputArray[i]->getName() + "<br>"; }
         }  
                 if (indexF!=0) {
         for ( int i=0; i<indexF; i++ ) { str += names[i] + "<br>"; }
         }  
                 str += label->getHtml()+"</div>";

         return str;
      }
      //              Atencion si falta una de los metodos static se produce un Error que no se detecta y no hay HTML
  String postCallBack(ElementsHtml* e,String postValue){ if (e==edtInterval) interval = edtInterval->value;pause->setInterval(interval); return "";}

  private:
    Input* inputArray[10];
    Output* outputArray[10];
    float * floatArray[10];
     String names [10];

    Label* label;
    String f[2] = { "OFF" , "ON" };
ComboBox* comboBox;
EditBox* edtInterval;
    String fileName;
    int index = 0;
    int indexO = 0;
    int indexF = 0;
    Pause * pause;
    int interval=60;
};





class CommandsComposite: public Commands {
  public:

  bool run(){
    for ( int i=0; i<command_count; i++) {
      commands[i]->run();
    }
  }  String getHtml(){}
  void addCommand( Commands* c) {commands [ command_count ] = c; command_count++; }
  //private:
      int command_count = 0;

  Commands* commands[20];
};



// ########################################
//  Command IF
// ########################################
class IfCommand: public CommandsComposite {
  public:
  IfCommand(String s, Input* inLL, Input* inRR ){name=s;id=s;inR=inRR;inL=inLL; }//   ERROR ERROR ERROR ERROR ERROR  Serial.print("Created IfCommand "+name); };

  String getHtml(){
    String html = "<div class='"+name+"'><h4>"+name+"</h4>"+inL->getHtml()+"  > " + inR->getHtml()+"<br>";
    for ( int i=0; i<command_count; i++) {
      html+=commands[i]->getHtml();
    }
    return html;
  }
     bool run(){}
    String postCallBack(ElementsHtml* e,String postValue){return "";};
 void update(){};
  private:
  Input* inL;
  Input* inR;
};


// ########################################
//  Command IF UNITARIO = Active Control
// ########################################
class ActiveControl:public Commands {
  public:
  //String name;
  Input* inputLeft;
  Input* inputRight;
  Output* output;
  Input* inputEdit;
  //String op = ">";
  String op = "=";
  ActiveControl(String n,Input* inL,String _op, Input* inR, Output* out,Input* inpEdit):inputLeft{inL},inputRight{inR},output{out},inputEdit{inpEdit} {
       op = _op;
       name = n;
          id=name; 
          
    inputEdit->parent=this;
    
    };
    
  String getHtml(){String s= "<div class='";s+=name;s+="' id='";s+=id;s+="'><h4>";s+=name;s+="</h4> If ";s+= inputLeft->getHtml();s+="  ";s+= op ;
                          s+= "  "; s+= inputRight->getHtml(); s+=  "  Then  " ; s+= output->name ;s+= " =  "; s+=inputEdit->getHtml(); s+= "</div>"; return s;}

  void update(){
    inputEdit->update();
    inputLeft->update();
    inputRight->update();
    
    if (op==">") {
      if ( inputLeft->value > inputRight->value ) {
          output->update(  inputEdit->value );
      }
    }
        if (op=="=") {
      if ( inputLeft->value == inputRight->value ) {
          output->update(  inputEdit->value );
      }
    }
    if (op=="<") {
      if ( inputLeft->value < inputRight->value ) {
          output->update(  inputEdit->value );
      }
    }
  }
  bool run(){
    update();
    return true;
  }
   String postCallBack(ElementsHtml* e,String postValue) {   return "";  }
};




class ActivePause: public ActiveControl {
public:
int value=0;
//     ActivePause (String n,Input* inL,String _op, Input* inR) {
//     //ActivePause (String n) {
//      inputLeft=inL;inputRight=inR;
//       op = _op;
//       name = n;
//          id=name; 
//          
//    html="<div id='"+id+"'><h4>"+name+"</h4> If "+ inputLeft->getHtml() +"  "+ op + "  " + inputRight->getHtml()+ " Then Continue </div>";
//    inputEdit->parent=this;
//    
//    };
      void update(){
    value=0;
    if (op=">") {
      if ( inputLeft->value > inputRight->value ) {
          value=1;
      }
    }
        if (op="=") {
      if ( inputLeft->value == inputRight->value ) {
          value=1;
      }
    }
    
  }
  bool run(){
    update();
    if (value==0) return false; else return true;
  }
    String postCallBack(ElementsHtml* e,String postValue){ return "";};

};

// ########################################
//  Command Keypad Control
// ########################################
class KeypadControl: public Commands {
  public:
  KeyPad* keypad;
   KeypadControl( String n ){
    name = n;
    id = n;
    keypad = new KeyPad(name+"kpd");
   }
   String getHtml(){ return "<div><h4>"+name+"</h4>"+keypad->getHtml()+"</div>"; }
     bool run(){
    //update();
    if (keypad->value==0) return false; else return true;
  }
    String postCallBack(ElementsHtml* e,String postValue){ return "";};
void update(){};

};

