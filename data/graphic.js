var dataArray = [];


window.onLoad = function() {
var ctx = document.getElementById('canvasGraphic').getContext('2d');
var myChart = new Chart(ctx, {
    type: 'bar',
    data: {
        labels: ['Red', 'Blue', 'Yellow', 'Green', 'Purple', 'Orange'],
        datasets: [{
            label: '# of Votes',
            data: [12, 19, 3, 5, 2, 3],
            backgroundColor: [
                'rgba(255, 99, 132, 0.2)',
                'rgba(54, 162, 235, 0.2)',
                'rgba(255, 206, 86, 0.2)',
                'rgba(75, 192, 192, 0.2)',
                'rgba(153, 102, 255, 0.2)',
                'rgba(255, 159, 64, 0.2)'
            ],
            borderColor: [
                'rgba(255, 99, 132, 1)',
                'rgba(54, 162, 235, 1)',
                'rgba(255, 206, 86, 1)',
                'rgba(75, 192, 192, 1)',
                'rgba(153, 102, 255, 1)',
                'rgba(255, 159, 64, 1)'
            ],
            borderWidth: 1
        }]
    },
    options: {
        scales: {
            yAxes: [{
                ticks: {
                    beginAtZero: true
                }
            }]
        }
    }
});


} 


//loadCSV(); // Download the CSV data, load Google Charts, parse the data, and draw the chart


/*
Structure:

    loadCSV
        callback:
        parseCSV
        load Google Charts (anonymous)
            callback:
            updateViewport
                displayDate
                drawChart
*/

/*
               |                    CHART                    |
               |                  VIEW PORT                  |
invisible      |                   visible                   |      invisible
---------------|---------------------------------------------|--------------->  time
       viewportStartTime                              viewportEndTime

               |______________viewportWidthTime______________|

viewportWidthTime = 1 day * 2^zoomLevel = viewportEndTime - viewportStartTime
*/

// function loadCSV() {
//     var xmlhttp = new XMLHttpRequest();
//     xmlhttp.onreadystatechange = function() {
//         if (this.readyState == 4 && this.status == 200) {
//             dataArray = parseCSV(this.responseText);
//             google.charts.load('current', { 'packages': ['line', 'corechart'] });
//                   google.charts.load('current', {'packages':['gauge']});

//             google.charts.setOnLoadCallback(updateViewport);
//             //radialTemperature.setValueAnimated(lastTemperature);

//         }
//     };
//     xmlhttp.open("GET", "dataLog.csv", true);
//     xmlhttp.send();
//     var loadingdiv = document.getElementById("loading");
//     //loadingdiv.style.visibility = "visible";
// }


// function parseCSV(string) {
//     var array = [];
//     var lines = string.split("\n");
//     for (var i = 0; i < lines.length; i++) {
//         var data = lines[i].split(",", 5);
//         //console.log( lastValue + " - " + data[0]);
//         if ( lastValue < parseInt(data[0]) ) {
            
//             lastValue = parseFloat (data[0]);

//             data[0] = new Date(parseInt(data[0]) * 1000);
//             data[1] = parseFloat(data[1]);
//             data[2] = parseFloat(data[2]);
//             data[3] = parseInt(data[3]);
//             data[4] = parseFloat(data[4])/3;
//             array.push(data);
//         }
//     }
//     return array;
// }


