//TOGGLES
var togglelistener = true;
var togglerosout = true;

//ROS Elements
var ws;
var ros;

//Connection Elements
var connected = false;
var attemptReconnect = false;
var connectionStatus;
var connectionAddress = '129.100.231.84';
var connectionIP;
var inputIP;
var timeCounter = 0;
var acceptedTimeDiff = 5;
var frameTimeoutInput;

//Control Elements
var driveInput;
var dutyInput;

//Current Elements
var currentElements = [];

//Velocity Elements
var xVel, yVel, zVel;

//TextAreas
var rosoutIn;
var listenerIn;
var serialOut;

//Image Canvas
var ctx;
var canvas;

//Topics List
var cmdVel;
var rosout;
var listener;
var image_rawCompressed;
var driveCommands;
var dutyCommands;
var currentSensors;
var loadSensors;
var encoderSensors;
var serverPing;
var VO_Velocity;

window.onload=function() {
    //FIND ELEMENTS
    frameTimeoutInput = document.getElementById('frameTimeoutInput');
    rosoutIn = document.getElementById('rosoutIn');
    listenerIn = document.getElementById('listenerIn');
    driveInput = document.getElementById('driveInput');
    dutyInput = document.getElementById('dutyInput');
    connectionStatus = document.getElementById('connectionStatus');
    connectionIP = document.getElementById('connectionIP');
    inputIP = document.getElementById('inputIP');

    //Current Display
    currentElements[0] = document.getElementById('RFC');
    currentElements[1] = document.getElementById('RMC');
    currentElements[2] = document.getElementById('RRC');
    currentElements[3] = document.getElementById('LRC');
    currentElements[4] = document.getElementById('LMC');
    currentElements[5] = document.getElementById('LFC');

    //Velocity Display
    xVel = document.getElementById('xVel');
    yVel = document.getElementById('yVel');
    zVel = document.getElementById('zVel');

    //Canvas Display
    canvas = document.querySelector("#canvas");
    ctx = canvas.getContext("2d");
    ctx.fillRect(0,0,canvas.width, canvas.height);

    //Set Connection Address
    connectionIP.innerHTML = connectionAddress;
    inputIP.value = connectionAddress;
    console.log('Attempt connection at '+connectionAddress);

    //Initialize Connection
    ros = new ROSLIB.Ros({
        url : 'ws://'+connectionAddress+':9090'
    });
    initTopics();
};

//WATCHDOG TO ENSURE CONNECTION
window.setInterval(function() {
	if(attemptReconnect) {
		reconnectRos();
	}else {
	timeCounter++;
		if(timeCounter>acceptedTimeDiff) {
			closeSockets();
			timeCounter = 0;
			attemptReconnect = true;
		}	
	}
}, 200);

function initTopics() {
    //DEFINE CONNECTION
    ros.on('connection', function() {
        connected = true;
        console.log('Connected to websocket server.');
        connectionStatus.innerHTML = 'Connected';
        connectionStatus.style.color = 'Green';
        connectionIP.innerHTML = connectionAddress;
    });

    ros.on('error', function(error) {
        console.log('Error connecting to websocket server: ', error);
        connectionStatus.style.color = 'Red';
    });

    ros.on('close', function() {
        connected = false;
        console.log('Connection to websocket server closed.');
        connectionStatus.style.color = 'Red';
    });

    //INITIALIZE TOPICS
    listener = new ROSLIB.Topic({
        ros : ros,
        name : '/listener',
        messageType : 'std_msgs/String'
    });

    cmdVel = new ROSLIB.Topic({
        ros : ros,
        name : '/husky/cmd_vel',
        messageType : 'geometry_msgs/Twist'
    });

    rosout = new ROSLIB.Topic({
        ros : ros,
        name : '/rosout',
        messageType : 'rosgraph_msgs/Log'
    });

    image_rawCompressed = new ROSLIB.Topic({
        ros : ros,
        name : '/image_raw/compressed',
        messageType : 'sensor_msgs/CompressedImage'
    });

    serverPing = new ROSLIB.Topic({
        ros : ros,
        name : '/serverPing',
        messageType : 'std_msgs/String'
    });

    currentSensors = new ROSLIB.Topic({
	ros: ros,
	name: '/currentSensors',
	messageType: 'std_msgs/String'
    });

    loadSensors = new ROSLIB.Topic({
	ros: ros,
	name: '/loadSensors',
	messageType: 'std_msgs/String'
    });

    encoderSensors = new ROSLIB.Topic({
	ros: ros,
	name: '/encoderSensors',
	messageType: 'std_msgs/String'
    });

    driveCommands = new ROSLIB.Topic({
	ros: ros,
	name: '/driveCommands',
	messageType: 'std_msgs/String'
    });

    dutyCommands = new ROSLIB.Topic({
	ros: ros,
	name: '/dutyCommands',
	messageType: 'std_msgs/String'
    });

    VO_Velocity = new ROSLIB.Topic({
	ros: ros,
	name: '/VO_Velocity',
	messageType: 'geometry_msgs/Twist'
    });

    listener.subscribe(function(message){
        listenerIn.value += listener.name + ': ' + message.data + '\n\n';
    });

    rosout.subscribe(function(message) {
        rosoutIn.value += message.name +': '+ message.msg + '\n\n';;
    });

    currentSensors.subscribe(function(message) {
	console.log(currentSensors.name+ ': '+ message.data + '\n');
	//PARSE DATA
	var sensors = message.data.split(',');
	for (var i=0; i<currentElements.length; i++) {
	    currentElements[i].innerHTML = sensors[i];
        }
    });
    
    loadSensors.subscribe(function(message) {
	console.log(loadSensors.name+ ': '+ message.data + '\n');
    });

    VO_Velocity.subscribe(function(message) {
	xVel.innerHTML = message.linear.x;
	yVel.innerHTML = message.linear.y;
	zVel.innerHTML = message.linear.z;
    });

    encoderSensors.subscribe(function(message) {
	console.log(encoderSensors.name+ ': '+ message.data + '\n');
    });

    image_rawCompressed.subscribe(function(message) {
	//PAINT JPEG TO CANVAS
	var imgObj = new Image();
	imgObj.onload = function() {
		ctx.drawImage(imgObj, 0, 0);
	};
	imgObj.src = 'data:image/jpg;base64,'+message.data;
    });

    serverPing.subscribe(function(message) {
	//Reset timer
	timeCounter = 0;
    });
}

function closeSockets() {
    //CLEAN UP SUBSCRIPTIONS & CLOSE SOCKET
    listener.unsubscribe();
    rosout.unsubscribe();
    image_rawCompressed.unsubscribe();
    currentSensors.unsubscribe();
    loadSensors.unsubscribe();
    encoderSensors.unsubscribe();
    serverPing.unsubscribe();
    VO_Velocity.unsubscribe();
    ctx.fillRect(0,0,canvas.width, canvas.height); //clear canvas to know video feed has stopped
    ros.close();
};

function reconnectRos() {
    attemptReconnect = false;
    if(connected && (inputIP.value == connectionAddress))
        return;
    closeSockets();
    connectionAddress = inputIP.value;
    console.log('Attempt connection at '+connectionAddress);
    ros = new ROSLIB.Ros({
        url : 'ws://'+connectionAddress+':9090'
    });
    initTopics();
};

function toggleRosout() {
    var button = document.getElementById('showhiderosout');
    var element = document.getElementById('rosoutDiv');
    if(togglerosout) {
        $(element).hide();
        togglerosout = false;
        button.innerHTML = 'show';
    }
    else {
        $(element).show();
        togglerosout = true;
        button.innerHTML = 'hide';
    }
};

function publishRosout() {
    var input = document.getElementById('sendRosoutText');
    var message = input.value;
    input.value = '';
    var mess = new ROSLIB.Message({
        name: '/WebApp',
        msg: message
    });
    rosout.publish(mess);
};

function toggleListener() {
    var button = document.getElementById('showhidelistener');
    var element = document.getElementById('listenerDiv');
    if(togglelistener) {
        $(element).hide();
        togglelistener = false;
        button.innerHTML = 'show';
    }
    else {
        $(element).show();
        togglelistener = true;
        button.innerHTML = 'hide';
    }
};

function publishListener() {
    var input = document.getElementById('sendListenerText');
    var message = input.value;
    input.value = '';
    var mess = new ROSLIB.Message({
        data: message
    });
    listener.publish(mess);
};

function pingServer() {
	$.ajax({
            type: "POST",
            url: 'http://'+connectionAddress+':3000/ping',
            success: function(msg){
		alert('Server is still active');
            },
            error: function(error) {
                closeSocket();
		console.log('Server has failed.');
		alert('Server has failed');
            }
        });
};

function changeFrameTimeout() {
    if(frameTimeoutInput.value >= 0)
        acceptedTimeDiff = frameTimeoutInput.value;
};

function publishDriveCommand() {
	var input = driveInput.value;
	driveInput.value = '';
	var mess = new ROSLIB.Message({
            data: input
        });
	driveCommands.publish(mess);

};

function publishDutyCommand() {
	var input = dutyInput.value;
	dutyInput.value = '';
	var mess = new ROSLIB.Message({
            data: input
        });
	dutyCommands.publish(mess);
};












