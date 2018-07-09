const char MAIN_page[] PROGMEM = R"=====(


<!DOCTYPE html>
<head>
   <meta content="text/html;charset=utf-8" http-equiv="Content-Type">
   <meta content="utf-8" http-equiv="encoding">
   <script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
   <link rel="stylesheet" type="text/css" href="style.css">
   <script>var temperature = 20.50;
      var humidity = 0.00;
   </script>
</head>
<body>
   Time: <span id="TIME"></span><br>
   Temperature:<span id="TEMPERATURE"></span><br> 
   <button type="button" onclick="sendData(1,1)">RELAY 1 ON</button> 
   <button type="button" onclick="sendData(1,0)">RELAY 1 OFF</button><br> 
   <button type="button" onclick="sendData(2,1)">RELAY 2 ON</button> 
   <button type="button" onclick="sendData(2,0)">RELAY 2 OFF</button><br>
       RELAY 1 State is : <span id="RELAYState1">NA</span>
           RELAY 2 State is : <span id="RELAYState2">NA</span> 
              Timer : <span id="TIMER">0</span><br>
   <form>tempSetpoint: <input type="number" min="0" max="100" step="1" value="0.00" name="tempSetpoint"><br>
   <button type = "submit" formaction="btnAction" >
   btn Action</button><input type="submit"formaction="set"><br>
   </form>
   Seconds= 54
      <div>
            <h2>Outlet 1</h2>
            <form>
            Countdown Timer On <input type="checkbox" name="chkTimer">
            minutes <input type="number" name = "countdownMinutes"><br>
            Temperature Control <input type="checkbox" name="chkTempControl">
            grados <input type="number" name = "tempSetting">
            Active <select name="upOrDown"><option value="UP">Heat</option><option value="DOWN">Cool</option></select><br>
                        <button type = "button" onclick="startTimer()" >Start</button>

            </form>

        </div>
        
   <script>
         function sendData(relay , state) {
                var xhttp = new XMLHttpRequest();
                xhttp.onreadystatechange = function() {
                       if (this.readyState == 4 && this.status == 200) { 
                               document.getElementById("RELAYState").innerHTML = this.responseText;
                        }     
               }; 
          xhttp.open("GET", "set?Relay="+relay+"&State="+state, true);
          xhttp.send();
          } 
          
          setInterval(function() {getData();}, 60000);
            
          function getData() {
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
                      document.getElementById("TIMER").innerHTML =      this.responseText;
                } 
             };
           xhttp.open("GET", "readTimer", true);  xhttp.send();}
           }
           
           function startTimer(){
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
                      document.getElementById("TIMER").innerHTML =      this.responseText;
                } 
             };
           xhttp.open("GET", "start", true);  xhttp.send();}}
   </script> 
</body>
</html>


)=====";
