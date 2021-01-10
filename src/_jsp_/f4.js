var map = null;
var mapvLayer = null;
var dis = 0;
var dataSet = null;
var options = null;


function callback(rs){

	var ary = [];
	var geoCoord;
	for (var i = 0; i < rs.data.length; i++) {
        geoCoord = rs.data[i];
        ary.push({
            geometry: { type:'Point', coordinates: geoCoord}
            });
    }
    dataSet = new mapv.DataSet(ary);
    mapvLayer = new mapv.baiduMapLayer(map, dataSet, options);
    sendMessage("showed");
    
}



function loadScript(msg){
	if(map != null && mapvLayer != null){
    mapvLayer.hide();
		mapvLayer.destroy();
    mapvLayer = null;
		mapvLayer = null;
    dataSet  = null;
	}

	if(document.getElementById('tempFile') != null){
		document.body.removeChild(document.getElementById('tempFile'));
	}

  var z = msg.indexOf('&');
  var url = msg.slice(0, z);
  dis = parseInt(msg.slice(z+1));

  options = {
    fillStyle: 'rgba(55, 50, 250, 0.8)',
    shadowColor: 'rgba(255, 250, 50, 1)',
    shadowBlur: 20,
    unit: 'm',
    size: dis,
    globalAlpha: 0.5,
    label: {
        show: true,
        fillStyle: 'white',
        shadowBlur: 10,
    },
    gradient: { 0.25: "rgb(0,0,255)", 0.55: "rgb(0,255,0)", 0.85: "yellow", 1.0: "rgb(255,0,0)"},
    draw: 'grid'
  }

  //alert(url + ',' + num);


    var script = document.createElement('script');
  	script.type = "text/javascript";
  	script.src = url;
  	script.id = 'tempFile';
  	document.body.appendChild(script);
}