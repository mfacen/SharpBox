

#define DEBUG

//     ATENCION NO USAR SERIAL AQUI< TODAVIA NO EXISTE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 



// ########################################
// CLASES
// ########################################

#include <vector>
#include <map>
#include <FixedString.h>

 // El problema es que solo estan registrados en Page los parientes de los componentes entonces no los encuentra cuando el PostRequest busca el id, no figura porque esta solo
 // el ID del pariente en page.listOf Elements[]
 const char docIdStr[] = "document.getElementById('";
 class JavaQueue {
  public:
    JavaQueue(){};
    String queue="";
    String onLoad="";
    void add (String q){queue+=q;queue+="\n";}
    void addOnLoad(String s){onLoad+=s;onLoad+="\n";}
    String get(){String temp=queue;queue="";return temp;}
    String getOnLoad(){String s="<script>";s+=onLoad;s+="</script>";return s;}
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
  float value=0;
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
    void setVisible(bool v) { v? javaQueue.add(docIdStr + this->id + "').style.display='inline';") : javaQueue.add(docIdStr + id + "').style.display='none';");}   //{visible=v;}
    void setDisabled (bool v){ v? javaQueue.add("var a=document.getElementById('" + id + "').setAttribute('disabled','disabled');"):javaQueue.add("var a=document.getElementById('" + id + "').removeAttribute('disabled');"); 
    }
    void clearHtml(){javaQueue.add("var a=document.getElementById('" + id + "').innerHTML=''");}

    virtual   String postCallBack(ElementsHtml* e,String postValue){ if(parent) return parent->postCallBack(this,postValue);else return ""; }
    virtual String getHtml(){};
    virtual void update(){};
    static  JavaQueue javaQueue;
    static String getJavaQueue(){return javaQueue.get();}
 
    static std::vector <ElementsHtml*> allHTMLElements;
    static void pushElement(ElementsHtml* e){allHTMLElements.push_back(e); }
    static void deleteElement(ElementsHtml* e){}   //  Todavia no he implementado esto
  // static char html [1000];
  // static void htmlAdd(const char* s){ strncat(html,s,1000);} // Copy char* s at the end of html, max lenght 2000.
  // static char* htmlGet(){ return html;} // Copy char* s at the end of html, max lenght 2000.
  //static String s;
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
//char ElementsHtml::html[1000];
//String ElementsHtml::s="                                                                                                                                                                                              ";
#include "output.h"

#include "input.h"
#include "controls.h"
#include "commands.h"



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
          SPIFFS.remove("/index.html");
      File htmlFile = SPIFFS.open( "/index.html" , "w");
      htmlFile.seek(0, SeekSet);

    //htmlStr.reserve(5000);        /////    Reserva espacio en la memoria para evitar fragmentacion.
    htmlStr+="<html><head> <link rel='stylesheet' type='text/css' href='style1.css'></head><body>\n";
          htmlFile.write((uint8_t *)htmlStr.c_str(), htmlStr.length());

        htmlStr=getJavaScript();
      htmlFile.write((uint8_t *)htmlStr.c_str(), htmlStr.length());


    htmlStr="<h1>";htmlStr+=title+"</h1><h3>";htmlStr+=subTitle;
          htmlFile.write((uint8_t *)htmlStr.c_str(), htmlStr.length());
htmlStr=F("</h3><nav><a href='edit.html'>Upload </a><a href='dataLog.csv'>dataLog</a><a href='delete?file=/dataLog.csv'>delete</a>"
            "<a href='settings'>Preferencias </a><a href='list?dir=/'>Directory</a></nav>\n");
      htmlFile.write((uint8_t *)htmlStr.c_str(), htmlStr.length());
//
    for ( int i=0; i<elementCount; i++) {htmlStr="";  if (strings[i]) {htmlStr=strings[i];} htmlStr+=listOfElements[i]->getHtml();htmlStr+="\n";
      //Serial.println(listOfElements[i]->name+" : HtmlStringSize: "+ String (  htmlStr.length() ) );
          htmlFile.write((uint8_t *)htmlStr.c_str(), htmlStr.length());
}
     // if (strings[elementCount]) htmlStr+=strings[elementCount];

      
htmlStr=F("<br><span id='errorLabel'></span><br>\n<button type = 'button' dataValue='primus,2323' onclick='btnClick(this)' id='switchToStation'>Connectar WIFI</button><br>");
      htmlFile.write((uint8_t *)htmlStr.c_str(), htmlStr.length());

  FSInfo fs_info;
  SPIFFS.info(fs_info);
  float fileTotalKB = (float)fs_info.totalBytes / 1024.0;
  float fileUsedKB = (float)fs_info.usedBytes / 1024.0;
htmlStr="Total KB: ";htmlStr+=String(fileTotalKB);htmlStr+="Kb / Used: ";htmlStr+=String(fileUsedKB);htmlStr+=" Page Size: ";htmlStr+=String(sizeof(this));
        htmlFile.write((uint8_t *)htmlStr.c_str(), htmlStr.length());

       htmlStr=ElementsHtml::javaQueue.getOnLoad();
      htmlFile.write((uint8_t *)htmlStr.c_str(), htmlStr.length());

htmlStr= "</body></html>";  
      htmlFile.write((uint8_t *)htmlStr.c_str(), htmlStr.length());

  //Serial.println(htmlStr);  // Atencion no usar Serial en Constructor !!!
//  ElementsHtml::htmlAdd(htmlStr.c_str());
     // htmlFile.write((uint8_t *)htmlStr.c_str(), htmlStr.length());
      htmlFile.close();
    return "";//htmlStr;

  }
  String getJavaScript(){return "<script src='javascript.js'></script>"; }

  private:
  String title;
  String subTitle;
};


