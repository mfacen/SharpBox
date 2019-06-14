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


     // var ctx = document.getElementById('chart').getContext('2d');
      //window.myChart = new Chart(ctx, config);

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


// function addChartData() {

//   for (i=1;i!=30;i++){
//     datos[i]=datos[i+1];
//   }
//   datos[30] = randomScallingFactor();
//   window.myChart.update();
// }



  var config = {
      type: 'line',
      data: {
        labels: [ // Date Objects
          
        ],
        datasets: [{
          label: 'Datos',
          backgroundColor: '343434',
          //borderColor: color("green").alpha(1).rgbString(),
          fill: false,
          data: [
          ],
        }]
      },
      options: {
        title: {
          text: 'Chart.js Time Scale'
        },
        scales: {
          xAxes: [{
            type: 'linear',
            //distribution: 'linear',
            display: 'true',
            scaleLabel: {
              display: true,
              labelString: 'Time'
            },
            offset: true,
            scaleBeginAtZero: false

          }],
          yAxes: [{
            scaleLabel: {
              display: true,
              labelString: 'value'
            }
          }]
        },
      }
    };

var myChart;
var data0 = [];
var dataLabels = [];
for (i=0;i<30;i++){ dataLabels.push(i);data0.push(i/3);}

        config.data.labels=dataLabels;
  config.data.datasets[0].data = (data0);


//var datos = parseCSV("time,relay,temp,analog\n11111111,0,20,7\n11111122,0,21,7.5\n11111133,0,22.5,8\n");




function loadCSV() {
  var xmlhttp = new XMLHttpRequest();
  xmlhttp.onreadystatechange = function() {
    if (this.readyState == 4 && ( this.status == 200  || this.status == 404 ) ) {
      dataArray = parseCSV(this.responseText);
            window.myChart.update();

      return dataArray;

    }
  };
  xmlhttp.open("GET", "dataLog.csv", true);
  xmlhttp.send();
}


function addChartData(f) {
      data0[29] = f;

  for (i=0;i<29;i++){

    data0[i]=data0[i+1];
  }
  config.data.datasets[0].data = (data0);

  window.myChart.update();
}


 
 function btnClickedPlus(){config.options.scales.xAxes[0].time.min = moment(config.options.scales.xAxes[0].time.min).add( 1,"hours");
            config.options.scales.xAxes[0].time.max = moment(config.options.scales.xAxes[0].time.max).subtract( 1,"hours");
            window.myChart.update(); }
 function btnClickedMinus(){config.options.scales.xAxes[0].time.min = moment(config.options.scales.xAxes[0].time.min).subtract( 1,"hours");
                config.options.scales.xAxes[0].time.max = moment(config.options.scales.xAxes[0].time.max).add( 1,"hours");
              window.myChart.update(); }
 function btnClickedZoomIn(){config.options.scales.xAxes[0].time.min = moment(config.options.scales.xAxes[0].time.min).add( 1,"hours");
              config.options.scales.xAxes[0].time.max = moment(config.options.scales.xAxes[0].time.max).add( 1,"hours");
               window.myChart.update(); }
 function btnClickedZoomOut(){config.options.scales.xAxes[0].time.min = moment(config.options.scales.xAxes[0].time.min).subtract( 1,"hours");
              config.options.scales.xAxes[0].time.max = moment(config.options.scales.xAxes[0].time.max).subtract( 1,"hours");
               window.myChart.update(); }

 
