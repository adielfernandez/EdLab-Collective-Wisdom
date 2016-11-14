//Initialize socket and get data
var socket = io.connect('http://' + window.location.hostname + ':8080');

// Listen for the initial connection event with the server
// Normally  you can give a message any name you want, but this is a special case.
// 'connect' is a reserved message name for SocketIO.
socket.on('connect', function() {
  console.log('connected!')
});

socket.on('confirm', function(data) {
  
  console.log(data);


});

// Send 'hello' to server
function sendMessage() {

  // Send the server a message
  //retrieve data from text inputs
  var messageText = document.getElementById('messageBox').value;
  var nameText = document.getElementById('nameBox').value;

  socket.emit('new-message', { name: nameText, message: messageText });

  console.log("Message Sent: \n" + nameText + ", " + messageText);

  //clear out the input boxes
  document.getElementById('messageBox').value = "";
  document.getElementById('nameBox').value = "";

}

//Button event
var messageButton = document.querySelector('#sendButton');

messageButton.addEventListener('click', function(e) {
  e.preventDefault();

  console.log("Mouse Pressed: " + e);

  // On button click, send our 'hello' message.
  sendMessage();
});



//listen for heartbeats to check connection
var lastHeartbeatTime;
var connected = false;

socket.on('heartbeat', function(data) {
  


  var d = new Date();
  lastHeartbeatTime = d.getTime();

  console.log('Heartbeat received at: ' + lastHeartbeatTime);

});

var interval = setInterval(checkConnection, 600);

function checkConnection(){

  // console.log("Checking connection...");

  var d = new Date();

  //if its been too long (just a bit over 500 ms, OF's heartbeat rate)
  //since we've received a message then we're disconnected
  if(d.getTime() - lastHeartbeatTime > 600){
    connected = false;
  } else {
    connected = true;
  }

  if(connected){
    document.getElementById('connected_inputs').style.display = "initial";
    document.getElementById('not_connected_error').style.display = "none";
    // console.log("Connected!");

  } else {
    document.getElementById('connected_inputs').style.display = "none";
    document.getElementById('not_connected_error').style.display = "initial";
    // console.log("NOT Connected!");
  }
  






}










