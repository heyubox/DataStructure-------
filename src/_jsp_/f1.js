/***********************************************/
/*			文件名：layerOpt.js				   */
/*			作者：Legend·Lai					   */
/*			日期：30th, March,2020			   */
/*			作用：对地图点图层进行操作			   */
/*			存放位置：msgutils.js后			   */
/***********************************************/

var view = null, pointLayer = null;	//图层管理器，点图层
var added = null;					//加载图层标志


//回调函数, 读取本地json加载点图层
function callback(rs){
	var dataSet = [];

    for (var i = 0; i < rs.length; i++) {
        var geoCoord = rs[i].cod;
        dataSet.push({
            geometry: { type:'Point', coordinates: geoCoord}
            });
    }

    pointLayer = new mapvgl.PointLayer({
         blend: 'lighter',
         shape: 'circle',
         color: 'rgba(255, 0, 0, 0.5)',
         data: dataSet,
         size: 4
    });
    view.addLayer(pointLayer);
    sendMessage('showed');
}



//加载点图层，采用DOM上加节点的方式以及jsonp的使用
function loadScript(url){
	if(added != null){
		cleanLayer();
		deleteScript(added);
		added = null;
	}
	var beg = url.indexOf('/') + 1;
	var end = url.lastIndexOf('.');
  	var script = document.createElement('script');
  	script.type = "text/javascript";
  	script.src = url;
  	script.id = url.substring(beg, end);
  	added = script.id;
  	document.body.appendChild(script);
}



//清除点图层
function cleanLayer(){
	if(added != null)
		view.removeLayer(pointLayer);
}


//点大小随着缩放层级改变
function sizeFix(){
	if(pointLayer != null){
		var zoomLevel = map.getZoom();
      	if(zoomLevel > 17 && pointLayer.options.size != 10){
        	cleanLayer();
          	pointLayer.options.size = 10;
          	view.addLayer(pointLayer);
      	}else if(zoomLevel <= 17 && pointLayer.options.size == 10){
      		cleanLayer();
          	pointLayer.options.size = 4;
          	view.addLayer(pointLayer);
      	} 
	}
}


function findScript(id){
	 if(document.getElementById(id) != null)
      alert('found');
    else
      alert('unfound');
}


function deleteScript(id){
	document.body.removeChild(document.getElementById(id));
}