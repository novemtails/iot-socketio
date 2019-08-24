var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);

app.get('/', function(req, res){
  res.sendFile(__dirname + '/index.html');
});


io.on('connection', function(socket){  
  console.log('a user connected');

  socket.on('disconnect', function(){
    console.log('user disconnected');
  });

  socket.on('message', function(msg){
    console.log('message: ' + msg);
    io.emit('ledOn', msg);
    console.log('On');
  });
  
  socket.on('Connection', function(msg){
    console.log('message #'  + ": " + msg);
    socket.emit('event', "Hola NodeMCU, soy el servidor");
    });
    //socket.emit('ledOn', 'dsdsdsds');
});

http.listen(process.env.PORT || 3000, function(){
  console.log('Server On *localhost:3000');
});