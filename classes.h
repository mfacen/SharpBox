

// ########################################
// CLASES
// ########################################


class ElementsHTML{
  public:
 // static Page page;
  String name;
  String id;
    String descriptor;
  String html;
  String javascript;
  String postRequest;
  String datavalue;
virtual   String postCallBack(String postValue, String postDataValue){}; // es virtual, lo tienen que implementar los hijos       ATENCION CUANDO DICE VTABLE ES QUE HE DEJADO UNA FUNCION SIN DEFINIR
//virtual   String getHtml(){}; // es virtual, lo tienen que implementar los hijos                                  por ej String getHtml()   ---->>>>  falta poner {}
virtual String getHtml(){};

};


#include "input.h"
#include "output.h"
#include "controls.h"
#include "commands.h"



// ########################################
//  Program
// ########################################

class Program {
public:

  Commands* listOfCommands[20];
   int commandCount = 0;
   int runIndex =  0;
  void addCommand (Commands* com){
    listOfCommands[commandCount] = com;
    commandCount++;
  }
  void deleteCommand(int index){
    for ( int i=index; i<19; i++)
    listOfCommands[i]=listOfCommands[i+1];
    commandCount--;
  }
  void runProgram() {
    if (  listOfCommands[runIndex]->run() ) runIndex++;
    if (runIndex>commandCount) runIndex=0;
  }
  String getHtml(){
    String html;
    for ( int i=0; i<commandCount; i++) {   html+=listOfCommands[i]->getHtml(); }
    return html;
  }
};


// ########################################
//  Page
// ########################################


class Page {
public:

   ElementsHTML* listOfElements[20];
//   Elements listOfElements1[20];
   int elementCount = 0;

   //Page(){};
  void addElement (ElementsHTML* el){
    listOfElements[elementCount] = el;
 //   listOfElements1[elementCount] = *el;
    elementCount++;
  }
  void deleteElement(int index){
    for ( int i=index; i<19; i++)
    listOfElements[i]=listOfElements[i+1];
    elementCount--;
  }
   String getHtml(){
    String html;
    for ( int i=0; i<elementCount; i++) {   html+=listOfElements[i]->getHtml()+"<br>"; }
    return html;
  }
};

class postObserver {
  public:
  virtual String received_postRequest(String value, String datavalue) = 0;
  }; 
  
  class observer_concrete : public postObserver {
    public:
    virtual String received_postRequest(String value, String datavalue) override     { }
    };
    
    class subject {
      public:
      void register_observer(postObserver& o)     {
        observers.push_back(o);
        }
        void notify_observers()
        {
          for (postObserver& o : observers) {
            //o.received_postRequest();
            }
            }
            
            private:     std::vector<std::reference_wrapper<postObserver>> observers;
  };

