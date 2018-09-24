


//     ATENCION NO USAR SERIAL AQUI< TODAVIA NO EXISTE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 



// ########################################
// CLASES
// ########################################

#include <vector>

 // El problema es que solo estan registrados en Page los parientes de los componentes entonces no los encuentra cuando el PostRequest busca el id, no figura porque esta solo
 // el ID del pariente en page.listOf Elements[]
 
 class JavaQueue {
  public:
    JavaQueue(){};
    String queue="";
    void add (String q){queue+=q;}
    String get(){String temp=queue;queue="";return temp;}
};

class ElementsHTML{
  public:
  ~ElementsHTML(){};      // destructor...     todas las clases abstractas deben tener un destructor definido.
 // static Page page;
  String name;
  String id;
  String descriptor;
  String html;
  String datavalue;
  ElementsHTML* parent = 0;
 static  JavaQueue javaQueue;

 virtual void update(){};
  virtual   String postCallBack(String postValue, String postDataValue){}; // es virtual, lo tienen que implementar los hijos       ATENCION CUANDO DICE VTABLE ES QUE HE DEJADO UNA FUNCION SIN DEFINIR
  virtual String getHtml(){};
  ElementsHTML* getIdOwner (String idd){if (this->id=idd) return this;else return nullptr;}
  static String getJavaQueue(){return javaQueue.get();}
  static std::vector <ElementsHTML*> allHTMLElements;
  static void pushElement(ElementsHTML* e){allHTMLElements.push_back(e); }
  static void deleteElement(ElementsHTML* e){}   //  Todavia no he implementado esto
};

class CompositeHTML: public ElementsHTML {
  public:
  
    std::vector <ElementsHTML*> childs;

  void addChild (ElementsHTML *ch){
  childs.push_back(ch);
  }

};




JavaQueue ElementsHTML::javaQueue;                  // Los miembros STATIC necesitan esta inicializacion, si no no funcionan... ???
std::vector <ElementsHTML*> ElementsHTML::allHTMLElements;


#include "output.h"

#include "input.h"
#include "controls.h"
#include "commands.h"



// ########################################
//  Program
// ########################################

class Program: public ElementsHTML {
public:
String name;
  Program ( String n ){
    name=n;
    id = n;
    label = new Label(name+"lbl","0");
    } // MALDITA SEA NO HACER ESTO !!!!!!!!!!!     Serial.println("Created Program "+name);}
  Commands* listOfCommands[50];
   int commandCount = 0;
   int runIndex =  0;
   Label* label;
  void addCommand (Commands* com){
    listOfCommands[commandCount] = com;
    commandCount++;
  }
  void deleteCommand(int index){
    for ( int i=index; i<49; i++)  listOfCommands[i]=listOfCommands[i+1];
    commandCount--;
  }
  void run() {
    if (  listOfCommands[runIndex]->run() ) runIndex++;
    if (runIndex>=commandCount) runIndex=0;
    javaQueue.add("document.getElementById('" + label->id + "').innerHTML='" + String(runIndex) +"  current: "+listOfCommands[runIndex]->name+ "';");
  }
  String getHtml(){
    String html;
    html+="<div id='"+id+"'><h4>"+name+"</h4> CommandCount = " + label->getHtml()+"<br>";
    for ( int i=0; i<commandCount; i++) {   html+=listOfCommands[i]->getHtml()+"\n"; }
    html+= "</div>";
    return html;
  }
};


// ########################################
//  Page
// ########################################

class Page {
public:

   ElementsHTML* listOfElements[50];
   int elementCount = 0;
String javaScript;
  void addElement (ElementsHTML* el){
    listOfElements[elementCount] = el;
    elementCount++;
  }
  void deleteElement(int index){
    for ( int i=index; i<19; i++)
    listOfElements[i]=listOfElements[i+1];
    elementCount--;
  }
  void updateElements(){
    for ( int i=0; i<elementCount; i++) {  listOfElements[i]->update();}
  }
   String getJavaQueue(){
        String reply;   //  for ( int i=0; i<elementCount; i++) {   reply+=listOfElements[i]->getJavaQueue(); }
        reply = ElementsHTML::getJavaQueue();
       return reply;
   }
   String getHtml(){
    String html;
    html+="<html><head> <link rel='stylesheet' type='text/css' href='style1.css'> </head><body>\n";
    for ( int i=0; i<elementCount; i++) {   html+=listOfElements[i]->getHtml()+"\n"; }
    html+=getJavaScript();
      
html+="<br><span id='errorLabel'></span><br>\n";

  FSInfo fs_info;
  SPIFFS.info(fs_info);
  float fileTotalKB = (float)fs_info.totalBytes / 1024.0;
  float fileUsedKB = (float)fs_info.usedBytes / 1024.0;
html+="Total KB: "+String(fileTotalKB)+"Kb / Used: "+String(fileUsedKB);
  
html+= "</body></html>";    
    return html;
  }
  String getJavaScript(){
      String str;
    str += "<script> \n function btnClick(btn){ \t var xhttp = new XMLHttpRequest(); \n";
    str += " \t elementName=btn.id;elementdataValue=btn.getAttribute('data-value');elementValue=btn.value;xhttp.onreadystatechange = function()\n";
    str += " { if (this.readyState == 4 && this.status == 200) {console.log(this.responseText);eval(this.responseText)}\n";    //eval(this.responseText); 
  str += "}; xhttp.open('GET', 'btnClick?button='+ elementName + '&value=' + elementValue +'&datavalue=' + elementdataValue, true);xhttp.send();};\n";

      str += "function btnClickText(elementName,elementValue,elementdataValue){var xhttp = new XMLHttpRequest();\n";
    str += " xhttp.onreadystatechange = function()";
    str += " { if (this.readyState == 4 && this.status == 200) {eval(this.responseText);}\n";     
  str += "}; xhttp.open('GET', 'btnClick?button='+ elementName + '&value=' + elementValue +'&datavalue=' + elementdataValue, true);xhttp.send();}\n";
  str+="setInterval(function() {getData();}, 2000); function getData(){var  xhttp = new XMLHttpRequest(); xhttp.onreadystatechange = function() {if (this.readyState == 4 && this.status == 200) { \n";
  str+="   eval(this.responseText)}};   xhttp.open('GET', 'getData', true); xhttp.send();};\n";



  str += "</script>";
  return str;
  }
  
};


