const path = require('path')
const express = require('express')
const app = express()
const exphbs = require('express-handlebars')
var bodyParser = require('body-parser')
var util = require('util')
var roll
var pitch
var throttle
var init = false;
var initRes = null;
var quit = 0;

app.all('*', function(req, res, next) {
  res.header('Access-Control-Allow-Origin', '*');
  res.header('Access-Control-Allow-Methods', 'PUT, GET, POST, DELETE, OPTIONS');
  res.header('Access-Control-Allow-Headers', 'accept, content-type, x-parse-application-id, x-parse-rest-api-key, x-parse-session-token');
  // intercept OPTIONS method
  if ('OPTIONS' == req.method) {
    res.send(200);
  } else {
    next();
  }
});

app.use("/public", express.static(__dirname + "/public"));

app.use(bodyParser.urlencoded({
  extended: false
}));
app.use(bodyParser.json());


app.engine('.hbs', exphbs({
  defaultLayout: 'main',
  extname: '.hbs',
  layoutsDir: path.join(__dirname, 'views/layouts')
}))
app.set('view engine', '.hbs')
app.set('views', path.join(__dirname, 'views'))


app.get('/', (request, response) => {
  response.render('home', {
    name: 'John'
  })
})

app.post('/init', (req, res) => {

  res.writeHead(200, {
    'Content-Type': 'text/plain'
  });
  req.on('data', function(chunk) {

  });
  //res.send('init' + init)
  res.end('callback(\'\"msg\": \"OK\"}\')');
  init = false;
  initRes = req.body.init;
  console.log(req.body.init);
})


app.post('/toServer', (req, res) => {
  //console.log('Request received: ');
  //util.log(util.inspect(req)) // this line helps you inspect the request so you can see whether the data is in the url (GET) or the req body (POST)
  //util.log('Request recieved: \nmethod: ' + req.method + '\nurl: ' + req.url) // this line logs just the method and url

  res.writeHead(200, {
    'Content-Type': 'text/plain'
  });
  req.on('data', function(chunk) {

  });
  //res.send('init' + init)
  res.end('callback(\'{init: ' + init + ' \"msg\": \"OK\"}\')');

  if (req.body.quit == '1') {
    quit = 1;
    console.log("quit");

  } else {
    quit = 0;
    roll = req.body.roll
    pitch = req.body.pitch
    throttle = req.body.throttle
    console.log(req.body.roll);
    console.log(req.body.pitch);
    console.log(req.body.throttle);
  }


})

app.get('/pinit', (request, res) => {
  //console.log("here");

  if (initRes == "true") {
    res.send('@t')
    initRes = null;
    console.log("money");
  } else if (initRes == "false") {
    res.send('@f')
    initRes = null;
  } else {
    res.send('@n');
    init = true;
  }
})



app.get('/data', (request, res) => {
  if (quit == 1) {
    res.send('@q')
    // console.log("here");
  } else {
    res.send('@' + throttle + '_' + roll + '_' + pitch + '_0_')
  }

})

app.listen(3000, '0.0.0.0', function() {
  console.log('Listening to port:  ' + 3000);
});
