var w = 0;
var a = 0;
var s = 0;
var d = 0;
var shift = 0;
var space = 0;
var roll;
var pitch;
var throttle = 1000;
var quit = 0;
$(document).keydown(function(e) {
  if (e.which == 87) {
    if (w == 0) {
      w = 1;
    }
  } else if (e.which == 83) {
    if (s == 0) {
      s = 1;
    }
  } else if (e.which == 65) {
    if (a == 0) {
      a = 1;
    }
  } else if (e.which == 68) {
    if (d == 0) {
      d = 1;
    }
  } else if (e.which == 16) {
    if (shift == 0) {
      shift = 1;
    }
  } else if (e.which == 32) {
    if (space == 0) {
      space = 1;
    }
  } else if (e.which == 81) {
    quit = 1;
  } else {
    console.log(e.which + "up");
  }

});
$(document).keyup(function(e) {
  if (e.which == 87) {
    w = 0;
  } else if (e.which == 83) {
    s = 0;
  } else if (e.which == 65) {
    a = 0;
  } else if (e.which == 68) {
    d = 0;
  } else if (e.which == 16) {
    shift = 0;
  } else if (e.which == 32) {
    space = 0;
  } else {
    console.log(e.which + "down");
  }
});

setInterval(function() {
  if (!(s & w)) {
    roll = 0;
    if (s) {
      roll = -5;
    }
    if (w) {
      roll = 5;
    }

  }
  if (!(a & d)) {
    pitch = 0;
    if (a) {
      pitch = -5;
    }
    if (d) {
      pitch = 5;
    }
  }

  if (!(shift & space)) {
    if (shift) {
      throttle -= 5;
    }
    if (space) {
      throttle += 5;
    }
  }
  $("#roll").text("roll " + roll.toString());
  $("#pitch").text("pitch " + pitch.toString());
  $("#throttle").text("throttle " + throttle.toString());
  $("#quit").text("quit = " + quit);
  sendData(roll, pitch, throttle);
}, 50);

function sendData(roll, pitch, throttle) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (xhttp.readyState == XMLHttpRequest.DONE) {
      if (xhttp.responseText.indexOf("true") > 0) {
        var send;
        if (confirm("Initialize?") == true) {
          send = "true";
        } else {
          send = "false";
        }
        xhttp.open("POST", "http://127.0.0.1:3000/init", true);
        xhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
        xhttp.send("init=" + send);
        return;
      }
    }
  }

  xhttp.open("POST", "http://127.0.0.1:3000/toServer", true);
  xhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
  if (quit) {
    xhttp.send("quit=1");
    quit = 0;
  } else {
    xhttp.send("roll=" + roll + "&pitch=" + pitch + "&throttle=" + throttle);
  }

}
