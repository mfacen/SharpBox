 function btnClick(btn){

    elementName=btn.id;elementValue=btn.value;

    
    if ( elementName == "switchToStation") {
        var credentials = prompt("Por favor ingrese el SSID y contraseña separados por una coma.", "Primus-pfg,19771018");
        elementValue = credentials;
    }


      var xhttp = new XMLHttpRequest(); 
   xhttp.onreadystatechange = function()
   { if (this.readyState == 4 && this.status == 200) {console.log(this.responseText);eval(this.responseText)}
}; xhttp.open('GET', 'btnClick?button='+ elementName + '&value=' + elementValue , true);xhttp.send();};


function btnClickText(elementName,elementValue){
	var xhttp = new XMLHttpRequest();
   	xhttp.onreadystatechange = function() {
   		 if (this.readyState == 4 && this.status == 200) {
   		 	eval(this.responseText);
   		 }
	}
	xhttp.open('GET', 'btnClick?button='+ elementName + '&value='+ elementValue +'', true);xhttp.send();
}


var connection = new WebSocket('ws://'+location.host+':81');
connection.onopen = function(){ console.log('Connection open!');}
connection.onclose = function(){console.log('Connection closed');}
connection.onerror = function(error){   console.log('Error detected: ' + error);}
connection.onmessage = function(e){ 
	 var server_message = e.data;
	  console.log("WebSocket EVAL: \n"+e.data); 		//													HERE TURN ON AND OFF LOOGING OF EVAL
	  eval(server_message); }


window.onload = function(){
    // ....
    var now = new Date();
    btnClickText("Time",now.getTime()/1000);

// var target = document.getElementById("Prueba"); // your canvas element
//  window.ODgauge = new Gauge(target).setOptions(opts); //   si le pongo windows se convierte en global.
// window.ODgauge.setMaxValue(30); // set max gauge value
// window.ODgauge.setMinValue(0);  // Prefer setter over gauge.minValue = 0
// //ODgauge.staticLabels = true;
// ////ODgauge.set(0); // set actual value

// console.log("Window Load, ODgauge set(0);");
 };


function btn(id,text){
   var s = "<button type='button' width='40' id='";s+=id; s+= "' value ='"; s+= text; s+= "' onclick=\"btnClickText('"; s+= id;s+= "',this.value)\" >";
                      s+=text; s+= "</button>\n";
  console.log("Btn()sub id:"+id+" :  text: "+text);

  document.getElementById(id).outerHtml=s;
}
