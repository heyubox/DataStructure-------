var map = null;
var editMode = 51;
var rect1 = null;
var rect2 = null;


function switchMode(msg){
	if(msg == "close"){
		editMode = 0;
	}
	else if(msg == "f5a1"){
		editMode = 51;
		alert("Function 5 area1 editing...");
	}else if(msg == "f5a2"){
		editMode = 52;
		alert("Function 5 area2 editing...");
	}else if(msg == 'f6a1'){
		editMode = 6;
		if(rect2 != null){map.removeOverlay(rect2); rect2 = null;}
		alert("Function 6 area editing...");
	}
}

