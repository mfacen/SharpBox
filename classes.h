

// ########################################
// CLASES
// ########################################

class ElementsHTML{
  public:
  String name;
  String id;
    String descriptor;
  String html;
  String javascript;
  String postRequest;
virtual   String postCallBack(String postValue, String postDataValue){}; // es virtual, lo tienen que implementar los hijos       ATENCION CUANDO DICE VTABLE ES QUE HE DEJADO UNA FUNCION SIN DEFINIR
//virtual   String getHtml(){}; // es virtual, lo tienen que implementar los hijos                                  por ej String getHtml()   ---->>>>  falta poner {}
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
    if (runIndex>0){
    if (  listOfCommands[runIndex]->run() ) runIndex++;
    }
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

};

