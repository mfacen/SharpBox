

class ActiveControl:public ElementsHTML {
  public:
  String name;
  Input* inputLeft;
  Input* inputRight;
  Output* output;
  Input* outputEdit;
  String op = ">";
  ActiveControl(String n,Input* inL, Input* inR, Output* out,Input* outEdit):inputLeft{inL},inputRight{inR},output{out},outputEdit{outEdit},name{n}{
    html="<div>"+name+" Input="+ inputLeft->getHtml() +"  "+ op + inputRight->getHtml()+ "  Then   Output=" + output->getHtml() + " =  "+outputEdit->getHtml()+"</div>";
   id=name;
    javascript="";
        postRequest=id;
        
    };
    
  String getHtml(){return html;}

  void update(){
    if (op=">") {
      ( inputLeft->value > inputRight->value )? output->update(  outputEdit->value): output->update ( !outputEdit->value );
    }
  }
  
};

