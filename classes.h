

#define DEBUG

//     ATENCION NO USAR SERIAL AQUI< TODAVIA NO EXISTE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 



// ########################################
// CLASES
// ########################################

#include <vector>
#include <map>

 // El problema es que solo estan registrados en Page los parientes de los componentes entonces no los encuentra cuando el PostRequest busca el id, no figura porque esta solo
 // el ID del pariente en page.listOf Elements[]
 
 class JavaQueue {
  public:
    JavaQueue(){};
    String queue="";
    void add (String q){queue+=q+"\n";}
    String get(){String temp=queue;queue="";return temp;}
};

class AverageModule {
  public:
    void addValue(float v) { values[index] = v; index++; if (index==10)index=0;  }
    float getAverage(){ float partial=0; for (int i=0; i<10;i++){ partial+=values[i]; } return partial/10;}
  private:
float values[10] = {0}  ;
int index = 0;

};

class ElementsHtml{
  public:
  ~ElementsHtml(){};      // destructor...     todas las clases abstractas deben tener un destructor definido.
 // static Page page;
  String name;
  String id;
  String descriptor;
  String style="";
  bool visible=true;
  //String html;
  ElementsHtml* parent = 0;
  //Wrapper* wrapper = nullptr;
      String getName(){return name;}
    String getId(){return id;}
    void setId(String i){id=i;}
    void setName(String n){name=n;}
    void setStyle(String s){style=s;}
 static  JavaQueue javaQueue;
 
 //void setWrapper(String before, String after){wrapper = new Wrapper(before,after);}
 virtual void update(){};
  virtual   String postCallBack(ElementsHtml* e,String postValue){}; // es virtual, lo tienen que implementar los hijos       ATENCION CUANDO DICE VTABLE ES QUE HE DEJADO UNA FUNCION SIN DEFINIR
  virtual String getHtml(){};
  static String getJavaQueue(){return javaQueue.get();}
  void setVisible(bool v) { v? javaQueue.add("document.getElementById('" + this->id + "').style.display='inline';") : javaQueue.add("document.getElementById('" + id + "').style.display='none';");}   //{visible=v;}
  void setDisabled (bool v){ v? javaQueue.add("var a=document.getElementById('" + id + "').setAttribute('disabled','disabled');"):javaQueue.add("var a=document.getElementById('" + id + "').removeAttribute('disabled');"); 
    }
  void clearHtml(){javaQueue.add("var a=document.getElementById('" + id + "').innerHTML=''");}
  static std::vector <ElementsHtml*> allHTMLElements;
  static void pushElement(ElementsHtml* e){allHTMLElements.push_back(e); }
  static void deleteElement(ElementsHtml* e){}   //  Todavia no he implementado esto
  static char html [5000];
  static void htmlAdd(const char* s){ strncat(html,s,2000);} // Copy char* s at the end of html, max lenght 2000.
  static String htmlGet(){ return html;} // Copy char* s at the end of html, max lenght 2000.
};

class CompositeHtml: public ElementsHtml {
  public:
    void addChild(ElementsHtml* e) { childs.push_back(e);}
    String getHtml() {String s;for (int i = 0; i<childs.size(); i++) { s+=childs[i]->getHtml(); }; return s; }  // iterate through the childs
  private:
    std::vector <ElementsHtml*> childs;
};



JavaQueue ElementsHtml::javaQueue;                  // Los miembros STATIC necesitan esta inicializacion, si no no funcionan... ???
std::vector <ElementsHtml*> ElementsHtml::allHTMLElements;
char ElementsHtml::html[5000];

#include "output.h"

#include "input.h"
#include "controls.h"
#include "commands.h"



// ########################################
//  Program
// ########################################

class Program: public ElementsHtml {
public:
String name;
  Program ( String n ){
    name=n;
    id = n;
    label = new Label(name+"lbl","0");
    } // MALDITA SEA NO HACER ESTO !!!!!!!!!!!     Serial.println("Created Program "+name);}
  Commands* listOfCommands[20];
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
  void update(){}//{    for ( int i=0; i<commandCount; i++) {  listOfCommands[i]->update(); } }

    
  
};

//class CompositeProgram: public Program {
//  public:
//    void addCommand(Commands* e) { childs.push_back(e);}
//    String getHtml() {for (int i = 0; i<childs.size(); i++) { }  }  // iterate through the childs
//  private:
//    std::vector <Commands*> childs;
//};

// ########################################
//  Page
// ########################################

class Page {
public:

   ElementsHtml* listOfElements[50];
   String strings[50];
   int elementCount = 0;

   Page(String stitle, String sSubTitle){title = stitle; subTitle= sSubTitle;}
  void addElement (ElementsHtml* el){
    listOfElements[elementCount] = el;
    elementCount++;
  }
  void addString( String s ) { strings[elementCount] = s;}
  void deleteElement(int index){
    for ( int i=index; i<49; i++)
      listOfElements[i]=listOfElements[i+1];
    elementCount--;
  }
  void update(){
   // for ( int i=0; i<elementCount; i++) {  listOfElements[i]->update();}
  }
   String getJavaQueue(){
        String reply;   //  for ( int i=0; i<elementCount; i++) {   reply+=listOfElements[i]->getJavaQueue(); }
        reply = ElementsHtml::getJavaQueue();
       return reply;
   }
   String getHtml(){
    String htmlStr;
    htmlStr+="<html><head> <link rel='stylesheet' type='text/css' href='style1.css'></head><body>\n";
        htmlStr+=getJavaScript();

    htmlStr+="<h1>"+title+"</h1><h3>"+subTitle+"</h3><nav><a href='edit.html'>Upload </a><a href='dataLog.csv'>dataLog</a><a href='delete?file=/dataLog.csv'>delete</a>"
            "<a href='settings'>Preferencias </a><a href='list?dir=/'>Directory</a></nav>\n";
    for ( int i=0; i<elementCount; i++) {  if (strings[i]) {htmlStr+=strings[i];} htmlStr+=listOfElements[i]->getHtml()+"\n";}
      if (strings[elementCount]) htmlStr+=strings[elementCount];

      
htmlStr+="<br><span id='errorLabel'></span><br>\n<button type = 'button' dataValue='primus,2323' onclick='btnClick(this)' id='switchToStation'>Connectar WIFI</button><br>";

  FSInfo fs_info;
  SPIFFS.info(fs_info);
  float fileTotalKB = (float)fs_info.totalBytes / 1024.0;
  float fileUsedKB = (float)fs_info.usedBytes / 1024.0;
htmlStr+="Total KB: "+String(fileTotalKB)+"Kb / Used: "+String(fileUsedKB);
  
htmlStr+= "</body></html>";    
  Serial.println(htmlStr);  // Atencion no usar Serial en Constructor !!!

    return htmlStr;

  }
  String getJavaScript(){return "<script src='javascript.js'></script>"; }

  private:
  String title;
  String subTitle;
};


