




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
       