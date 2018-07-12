

class ActiveControl:public ElementsHTML {
  public:
  String name;
  Input* inputLeft;
  Input* inputRight;
  Output* output;
  String op = ">";
  ActiveControl(String n,Input* inL, Input* inR, Output* out):inputLeft{inL},inputRight{inR},output{out},name{n}{
    html="<span>"+name+" Input="+ inputLeft->getHtml() +"  "+ op + inputRight->getHtml()+ "   Output=" + output->getHtml() + " then </span>";
    javascript="";
    postRequest = "";
    };
    
  String getHtml(){return html;}

  void update(){
    
  }
  
};

