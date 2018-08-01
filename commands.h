// ########################################
//  Commands
// ########################################

class Commands: public ElementsHTML{
  public:
  virtual bool run(){};
  String getHtml(){};
};

// ########################################
//  Command   SET
// ########################################

class Set:public Commands {
public:
  Output* output;
  
  EditBox* editOutput;
  Button* btn;
  int value;
  Set(String _name, Output* out){
    output = out;
    name = _name;
    id = _name;
    editOutput = new EditBox ( "edt"+name);
    btn = new Button (name,"qwerty","Action");

            postRequest=id;

      html="<div>"+name+output->html+"   "+output->stateStr +" "+ editOutput->getHtml() + btn->getHtml()+"<?div>";
  }
  bool run () {
    output->update(editOutput->value);
    return true;
  }
  String getHtml(){
    return html;
  }
  String postCallBack(String postValue,String postDatavalue){run();return editOutput->javascript;};
  String test(String str){};

};

// ########################################
//  Command Wait for
// ########################################



