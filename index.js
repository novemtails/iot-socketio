var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);

app.get('/', function(req, res){
  res.sendFile(__dirname + '/index.html');
});


io.on('connection', function(socket){  
  
    socket.on('Connection', function(msg){
        console.log('message #'  + ": " + msg);
        socket.emit('event', "Hola NodeMCU, soy el servidor");
      });

     //socket.emit('ledOff', socket.id);
});

http.listen(process.env.PORT || 3000, function(){
  console.log('Server On *localhost:3000');
});