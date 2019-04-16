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


function btnClickText(elementName,elementValue){var xhttp = new XMLHttpRequest();
   xhttp.onreadystatechange = function() { if (this.readyState == 4 && this.status == 200) {eval(this.responseText);}
    }; xhttp.open('GET', 'btnClick?button='+ elementName + '&value='+ elementValue +'', true);xhttp.send();
}


var connection = new WebSocket('ws://'+location.host+':81');
connection.onopen = function(){ console.log('Connection open!');}
connection.onclose = function(){console.log('Connection closed');}
connection.onerror = function(error){   console.log('Error detected: ' + error);}
connection.onmessage = function(e){   var server_message = e.data;  eval(server_message); }


function makeTable ( csv ) {

    var rows = csv.split('\n'),
    table = document.createElement('table'),
    tr = null, td = null,
    tds = null;
    var columns = new Array ( 'Fecha' , 'ID Granja' , 'Estanque' , 'Temperatura' , 'Oxigeno Disuelto','Oxygeno %','Salinidad');
// Create an empty <thead> element and add it to the table:
var header = table.createTHead();
var row = header.insertRow(0);
for ( var i=0; i<columns.length; i++ ) {
    var cell = row.insertCell(i);
    cell.innerHTML = columns[i];
}

                //table.append(columns);
                for ( var i = 0; i < rows.length-1; i++ ) {
                    tr = document.createElement('tr');
                    tds = rows[i].split(',');
                    for ( var j = 0; j < tds.length; j++ ) {
                        td = document.createElement('td');
                        td.innerHTML = tds[j];
                        if (j==0) td.innerHTML = new Date( parseInt (tds[j])*1000).toLocaleString();
                        //if (i==0)     {var cell = row.insertCell(j);
                            //      cell.innerHTML = columns[j];

                            tr.appendChild(td);
                        }
                        table.appendChild(tr);
                    }

                    //table.style.border = "solid darkgray";
                    table.style.display = "block";
                    table.style.padding = "1em";
                    table.id = "tableData";
                    table.style["overflow-y"] = "auto";
                    table.style["font-size"] = "0.3 em";
                    //table.border = "solid darkgray";
                    table.style.height = "200px";
                //table.style.width = "500px";
                return table;
            }
            
