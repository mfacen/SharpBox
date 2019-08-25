  var config = {
      type: 'line',
      data: {
        labels: [ // Date Objects
          
        ],
        datasets: [{
          label: 'Datos',
          backgroundColor: color("blue").alpha(0.5).rgbString(),
          borderColor: color("green").alpha(1).rgbString(),
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


function addChartData() {

  for (i=1;i!=30;i++){
    datos[i]=datos[i+1];
  }
  datos[30] = randomScallingFactor();
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

 