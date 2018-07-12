// ########################################
//  Commands
// ########################################

class Commands: public ElementsHTML{
  public:

  virtual bool run(){};
};

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
    btn = new Button ( "btn"+name);
      html="<span>"+output->html+"   "+output->stateStr +" "+ editOutput->getHtml()+ btn->getHtml() + "</span>";
  }
  bool run () {
    output->update(value);
    return true;
  }
  String getHtml(){
    return html;
  }
  String postCallBack(String postValue){};
};


