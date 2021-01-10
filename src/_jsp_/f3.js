var editModeOpenFlag = false;
var map = null;
var plane_overlay = null;
var point_overlay = null;

function switchMode(msg){

	if(msg == "open"){
		editModeOpenFlag = true;
		alert("editMode open");
	}
	else {
		editModeOpenFlag = false;
		if(map != null && plane_overlay != null){
			map.removeOverlay(plane_overlay);
		 	plane_overlay = null;
		}
		alert("editMode exit");
	}
}


function callback(rs){

	var points = [];// 添加海量点数据
    for (var i = 0; i < rs.data.length; i++) {
    	points.push(new BMap.Point(rs.data[i][0], rs.data[i][1]));
    }

    var options = {
        size: BMAP_POINT_SIZE_SMALL,
        shape: BMAP_POINT_SHAPE_STAR,
        color: 'red'
    }

    point_overlay = new BMap.PointCollection(points, options);
    map.addOverlay(point_overlay);
    alert("show");
}


function loadScript(msg){

	if(point_overlay != null){
		map.removeOverlay(point_overlay);
		point_overlay = null;
	}

	if(msg == 'false'){
		alert("too many points unable to show!");
		return ;
	}

	if(document.getElementById('tempFile') != null){
		document.body.removeChild(document.getElementById('tempFile'));
	}

  	var script = document.createElement('script');
  	script.type = "text/javascript";
  	script.src = msg;
  	script.id = 'tempFile';
  	document.body.appendChild(script);
}