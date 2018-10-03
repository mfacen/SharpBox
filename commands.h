// ########################################
//  Commands
// ########################################
class Program;
;
class Commands: public ElementsHTML{
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
  ComboBox* comboBox1; 
  //EditBox* editOutput;
 // Button* btn;
  int value;
  Set(String n, Output* out){
    output = out;
    name = n;
    id = n;
    //editOutput = new EditBox ( "edt"+name,"0");
    
   comboBox1 = new ComboBox ( name+"combo",2,f);
   
 //   btn = new Button ("btn"+name,"btnAction","Action",this);
  }
  bool run () {
    output->update(comboBox1->value);
    return true;
  }
  String getHtml(){  return "<div><h4>"+name+"</h4>"+output->name+"  "+ comboBox1->getHtml() +"</div>";  }
  String postCallBack(ElementsHTML* e,String postValue){ if (postValue == "btnAction") output->update(comboBox1->value);};
  void update(){output->update(comboBox1->value);}
  
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
      editTime  = new EditBox (name+"edtBox",String(interval));
//      addChild(editTime);
      }
      
    
   String getHtml(){
    return "<div><h4>"+name+"</h4>"+"<br>Time: "+editTime->getHtml()+"</div>";
   }
  bool run(){  
      if (firstRun){firstRun=false; start();}
      if  (  (lastTimeCheck - millis()  ) > 1000 ) { lastTimeCheck = millis (); value = value - 1; update(); }
      if (( millis()-lastUpdate ) > interval*1000 ) {firstRun=true;return true;} else return false;
  };
  String postCallBack(ElementsHTML* e,String postValue){};
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
    Logger ( String s , Input* in) {id=s;name=s;input=in;}
    bool run(){return logData(input);}
    void update(){input->update();}
    bool logData( Input* i){
      File tempLog ;
      tempLog = SPIFFS.open("/dataLog.csv", "a"); // Write the time and the temperature to the csv file
      bool r=tempLog;
      tempLog.println(i->getName()+": "+String(i->value));
      tempLog.close();
      return r;
    }
      String getHtml(){return "<div><h4>Logger "+name+"</h4>Input: "+input->getName()+"</div>";}      //              Atencion si falta una de los metodos static se produce un Error que no se detecta y no hay HTML
  String postCallBack(ElementsHTML* e,String postValue){};

  private:
    Input* input;
    
};





class CommandsComposite: public Commands {
  public:
    int command_count = 0;

  Commands* commands[];
  //bool run();
  String getHtml(){}
  void addCommand( Commands* c) {commands [ command_count ] = c; command_count++; }
  private:
  
};



// ########################################
//  Command IF
// ########################################
class IfCommand: public CommandsComposite {
  public:
  IfCommand(String s, Input* inLL, Input* inRR ){name=s;id=s;inR=inRR;inL=inLL; }//   ERROR ERROR ERROR ERROR ERROR  Serial.print("Created IfCommand "+name); };
  bool run(){
    for ( int i=0; i<command_count; i++) {
      commands[i]->run();
    }
  }
  String getHtml(){
    String html = "<div><h4>"+name+"</h4>"+inL->getHtml()+"  > " + inR->getHtml()+"<br>";
    for ( int i=0; i<command_count; i++) {
      html+=commands[i]->getHtml();
    }
    return html;
  }
  void addCommand( Commands* c) {commands [ command_count ] = c; command_count++; }
    String postCallBack(ElementsHTML* e,String postValue){};
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
    
  String getHtml(){return "<div id='"+id+"'><h4>"+name+"</h4> If "+ inputLeft->getHtml() +"  "+ op + "  " + inputRight->getHtml()+ "  Then  " + output->name + " =  "+inputEdit->getHtml()+"</div>";}

  void update(){
    inputEdit->update();
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
   String postCallBack(ElementsHTML* e,String postValue) {   return "";  }
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
    String postCallBack(ElementsHTML* e,String postValue){};

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
    String postCallBack(ElementsHTML* e,String postValue){};
void update(){};

};

