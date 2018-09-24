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
  String f[5] = { "OFF" , "ON" };
  ComboBox* comboBox1; 
  //EditBox* editOutput;
  Button* btn;
  int value;
  Set(String n, Output* out){
    output = out;
    name = n;
    id = n;
    //editOutput = new EditBox ( "edt"+name,"0");
    
   comboBox1 = new ComboBox ( name+"combo",2,f);
   
    btn = new Button ("btn"+name,"btnAction","Action",this);
      html="<div><h4>"+name+"</h4>"+output->name+":"+output->stateStr +" "+ comboBox1->getHtml() + btn->getHtml()+"</div>";
  }
  bool run () {
    output->update(comboBox1->value);
    return true;
  }
  String getHtml(){
    return html;
  }
  String postCallBack(String postValue,String postDatavalue){ if (postValue == "btnAction") output->update(comboBox1->value);};

};


// #######################################
//  Command   PAUSE
// ########################################
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
  void start( ) { lastUpdate  = millis();}
  void update(){
                //javaQueue.add("document.getElementById('" + editTime->id + "').setAttribute('innerHTML', '"+String(value)+"');");

  }
  private:
    unsigned long lastUpdate;
    int interval;
    unsigned long currentMillis;
    unsigned long lastTimeCheck;
        EditBox* editTime;

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
  Input* inL;
  Input* inR;
  IfCommand(String s, Input* inLL, Input* inRR ){name=s;id=s;inR=inRR;inL=inLL; }//   ERROR ERROR ERROR ERROR ERROR  Serial.print("Created IfCommand "+name); };
  bool run(){
    for ( int i=0; i<command_count; i++) {
      commands[i]->run();
    }
  }
  String getHtml(){
    html = "<div><h4>"+name+"</h4>"+inL->getHtml()+"  > " + inR->getHtml()+"<br>";
    for ( int i=0; i<command_count; i++) {
      html+=commands[i]->getHtml();
    }
  }
  void addCommand( Commands* c) {commands [ command_count ] = c; command_count++; }
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
          
    html="<div id='"+id+"'><h4>"+name+"</h4> If "+ inputLeft->getHtml() +"  "+ op + "  " + inputRight->getHtml()+ "  Then  " + output->name + " =  "+inputEdit->getHtml()+"</div>";
    inputEdit->parent=this;
    
    };
    
  String getHtml(){return html;}

  void update(){
    
    if (op=">") {
      if ( inputLeft->value > inputRight->value ) {
                  //javaQueue.add("console.log('greater');");
          output->update(  inputEdit->value );
      }
    }
        if (op="=") {
      if ( inputLeft->value == inputRight->value ) {
          //javaQueue.add("console.log('equal');");
          output->update(  inputEdit->value );
      }
    }
  }
  bool run(){
    update();
    return true;
  }
   String postCallBack(String postValue, String postDataValue) {
   return "";
    }
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
//      void update(){
//    value=0;
//    if (op=">") {
//      if ( inputLeft->value > inputRight->value ) {
//          value=1;
//      }
//    }
//        if (op="=") {
//      if ( inputLeft->value == inputRight->value ) {
//          value=1;
//      }
//    }
//    
//  }
  bool run(){
    update();
    if (value==0) return false; else return true;
  }
};

// ########################################
//  Command Keypad Control
// ########################################
class KeyPadCommand: public Commands {
  public:
  KeyPad* keypad;
   KeyPadCommand( String n ){
    name = n;
    id = n;
    keypad = new KeyPad(name+"kpd");
   }
   String getHtml(){ return "<div><h4>"+name+"</h4>"+keypad->getHtml()+"</div>"; }
     bool run(){
    //update();
    if (keypad->value==0) return false; else return true;
  }
};

