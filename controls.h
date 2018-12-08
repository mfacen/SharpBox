
// HTM Decorators

class Wrapper: public ElementsHtml {
  public:
    ElementsHtml *element;
  Wrapper ( String wrapBefore, String wrapAfter, ElementsHtml* e) {
    addAfter = wrapAfter;
    addBefore = wrapBefore;
    element = e;
  }
    void update(){element->update();}
    String postCallBack(ElementsHtml* e,String postValue){return element->postCallBack( e, postValue);}; // es virtual, lo tienen que implementar los hijos       ATENCION CUANDO DICE VTABLE ES QUE HE DEJADO UNA FUNCION SIN DEFINIR
   String getHtml(){return addBefore+ element->getHtml()+ addAfter;};
  
  private:
  String addBefore;
  String addAfter;
};



class SimpleHtml1: public ElementsHtml {
  public:
  SimpleHtml1(String s){str=s;}
  String getHtml(){return str;};
  private:
  String str;
};


