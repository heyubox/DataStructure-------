/***********************************************/
/*      文件名：msgutils.js                     */
/*      修改：Legend·Lai                        */
/*      日期：30th, March,2020                  */
/*      作用：建立js和qt的通信通道                */
/*      存放位置：qwebchannel.js后,layerOpt.js前 */
/************************************************/


//js中交互对象
var context;

//获取qt对象
function init() {
      if (typeof qt != 'undefined' ) {
          new QWebChannel( qt.webChannelTransport, function(channel) {
                                           context = channel.objects.context;})
      }else{
          alert("qt对象获取失败！");
      }
}

//发送消息给qt
function sendMessage (msg) {
      if (typeof context != 'undefined'){
          context.onMsg(msg);
      }else {
           alert("js对象获取失败！");
      }
}

//供qt调用
function recvMessage(msg, code){
    if (code == 'f1sl'){
        loadScript(msg);
    }else if(code == 'f3sl'){
        loadScript(msg);
    }else if(code == 'f3ced'){
        switchMode(msg);
    }else if(code == 'f5ced'){
    	  switchMode(msg);
    }else if(code == 'f4sg'){
        loadScript(msg);
    }else if(code == 'f8sp'){
    	loadScript(msg);
    }else if(code == 'f8sw'){
        switchMode();
    }
}

init();


//向qt发送网页元素加载完毕消息，enable qt功能
window.onload = function(){
    sendMessage('ready');
    alert("加载完成");
}