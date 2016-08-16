var log = "";
var curCommand = 0;
var active = true;

function process(e) {
  if (active) {
    var x = e.pageX - e.target.offsetLeft;
    var y = e.pageY - e.target.offsetTop;
    log = log + x + " " + y + "\n";
    displayLog();
  }
}

function next() {
  if (active) {
    if (curCommand == commands.length) {
      log = log + "done\n";
      document.getElementById("command").innerHTML = "Congratulations. Submit your action log.";
      active = false;
    } else {
      log = log + "next\n";
      document.getElementById("command").innerHTML = "<b>Level " + (curCommand+2) + ":</b> " + commands[curCommand] + " Then click on the <b>Next</b> button.";
      curCommand += 1;
    }
    displayLog();
  }
}

function displayLog() {
  document.getElementById('log').value = log;
}
