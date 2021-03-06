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
var logDelay = 1000;

//Control Elements
var driveInput;
var dutyInput;

//Current Elements
var currentElements = [];

//Load Elements
var loadElements = [];

//Velocity Elements
var xVel, yVel;

//Image Canvas
var ctx,
    canvas,
    depthCtx,
    depthCanvas;

//Topics List
var cmdVel,
    image_rawCompressed,
    currentSensors,
    loadSensors,
    encoderSensors,
    opticalFlowSensors,
    serverPing,
    image_depthCompressed;

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

    //Load Display
    loadElements[0] = document.getElementById('RFS');
    loadElements[1] = document.getElementById('RMS');
    loadElements[2] = document.getElementById('RRS');
    loadElements[3] = document.getElementById('LRS');
    loadElements[4] = document.getElementById('LMS');
    loadElements[5] = document.getElementById('LFS');

    //Velocity Display
    xVel = document.getElementById('xVel');
    yVel = document.getElementById('yVel');

    //Canvas Display
    canvas = document.querySelector('#canvas');
    ctx = canvas.getContext('2d');
    ctx.fillRect(0,0,canvas.width, canvas.height);
    depthCanvas = document.querySelector('#depthCanvas');
    depthCtx = depthCanvas.getContext('2d');
    depthCtx.fillRect(0,0,depthCanvas.width, depthCanvas.height);

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

//Log data every second
(function() {
	var func = function() {
		$("#dataLog").append("\n");
		for (var i=0; i<currentElements.length; i++) {
			$("#dataLog").append(currentElements[i].innerHTML + ",");
		}
		for (var i=0; i<loadElements.length; i++) {
			$("#dataLog").append(loadElements[i].innerHTML + ",");
		}
		$("#dataLog").append(xVel.innerHTML + ",");
		$("#dataLog").append(yVel.innerHTML + ",");
		$("#dataLog").append(Date.now()/1000);

		setTimeout(func, logDelay);
	}
	setTimeout(func, logDelay)
})();

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

    image_depthCompressed = new ROSLIB.Topic({
        ros : ros,
        name : '/depth_cam/compressed',
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

    opticalFlowSensors = new ROSLIB.Topic({
	ros: ros,
	name: '/opticalFlowSensors',
	messageType: 'std_msgs/String'
    });

   serverPing.subscribe(function(message) {
	//Reset timer
	timeCounter = 0;
    });

    currentSensors.subscribe(function(message) {
	//PARSE DATA
	var sensors = message.data.split(',');
	for (var i=0; i<currentElements.length; i++) {
	    currentElements[i].innerHTML = sensors[i];
        }
    });
    
    loadSensors.subscribe(function(message) {
	//PARSE DATA
	var sensors = message.data.split(',');
	for (var i=0; i<loadElements.length; i++) {
	    loadElements[i].innerHTML = sensors[i];
        }
    });

    opticalFlowSensors.subscribe(function(message) {
	var sensors = message.data.split(',');
	xVel.innerHTML = sensors[0];
	yVel.innerHTML = sensors[1];
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

    image_depthCompressed.subscribe(function(message) {
	console.log('hit');
	//PAINT JPEG TO CANVAS
	var imgObj = new Image();
	imgObj.onload = function() {
		depthCtx.drawImage(imgObj, 0, 0);
	};
	imgObj.src = 'data:image/jpg;base64,'+message.data;
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
    image_depthCompressed.unsubscribe();
    connectionStatus.style.color = 'Red';
    ctx.fillRect(0,0,canvas.width, canvas.height);
    depthCtx.fillRect(0,0,canvas.width, canvas.height);
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

function clearLog() {
    var r = confirm("Clear log?");
    if(r==true) {
	$("#dataLog").html("Current LF,Current LM, Current LR,Current RF,Current RM, Current RR,Force LF,Force LM, Force LR,Force RF,Force RM, Force RR,Velocity X,Velocity Y,Time");
    }
}

function saveLog() {
    var content = $("#dataLog").val();
    var contentType = 'application/octet-stream';
    var filename = 'dataLog';
    var fileExtension = ".csv";

    if(!contentType) contentType = 'application/octet-stream';
    var a = document.createElement('a');
    var blob = new Blob([content], {'type':contentType});
    a.href = window.URL.createObjectURL(blob);
    a.download = filename+fileExtension;
    a.click();
}

function changeLogDelay() {
    logDelay = $("#logDelay").val();
}





