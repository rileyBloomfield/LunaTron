var express = require('express');
var auth = require('basic-auth');
var app = express();

app.use(function(req, res, next) {
	var user = auth(req);
	next();
});
app.use(express.static('public'));

app.post('/ping', function(req, res) {
	res.status(200).send('OK');
});

app.listen(3000, function() {
	console.log('Listening on port 3000');
});

