var injectedID;
var injectedValue;
var ODgauge;
//var ODgauge;
var opts = {
  angle: .15, // The span of the gauge arc
  lineWidth: 0.2, // The line thickness
  radiusScale: .9, // Relative radius
  pointer: {
    length: 0.6, // // Relative to gauge radius
    strokeWidth: 0.044, // The thickness
    color: '#888888' // Fill color
},
  limitMax: true,     // If false, max value increases automatically if value > maxValue
  limitMin: true,     // If true, the min value of the gauge will be fixed
  colorStart: '#6F6EA0',   // Colors
  colorStop: '#C0C0DB',    // just experiment with them
  strokeColor: '#ff00ff',  // to see which ones work best for you
  generateGradient: true,
  highDpiSupport: true,     // High resolution support
  //percentColors: [[0.0, "#a9d70b" ], [0.50, "#f9c802"], [1.0, "#ff0000"]], // cambia color mientras sube
  staticLabels: {
  font: "10px sans-serif",  // Specifies font
  labels: [0, 5, 10, 15,20],  // Print labels at these values
  color: "beige",  // Optional: Label text color
  fractionDigits: 0  // Optional: Numerical precision. 0=round off.
},
staticZones: [
	   {strokeStyle: "#F03E3E", min: 0, max: 3}, // Red from 100 to 130
	   {strokeStyle: "#FFDD00", min: 3, max: 4}, // Yellow
	   {strokeStyle: "#30B32D", min: 4, max: 6}, // Green
	   {strokeStyle: "#FFDD00", min: 6, max: 10}, // Yellow
	   {strokeStyle: "#F03E3E", min: 10, max: 20}  // Red
	   ],

	};

var target = document.getElementById(injectedID); // your canvas element
var ODgauge = new Gauge(target).setOptions(opts); // create sexy gauge!
ODgauge.setMaxValue(20); // set max gauge value
ODgauge.setMinValue(0);  // Prefer setter over gauge.minValue = 0
//ODgauge.staticLabels = true;
ODgauge.set(0); // set actual value

function setGauge ( value ){ ODgauge.set(value)}