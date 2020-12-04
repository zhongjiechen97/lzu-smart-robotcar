//<script type="text/javascript">
setInterval(goWay,500);
setInterval(showThings,500);
//setInterval(showThings,500);
var map = new BMap.Map("allmap");
<?php
//取出GPSdata
include "conn.php";
$sql="select * from `GPSdata`";
$result = $conn->query($sql);
$data = $result->fetch_array();
?>
var startLong0 = <?php echo $data['longitude']; ?>;
var startLat0 = <?php echo $data['latitude']; ?>;
var ggPoint = new BMap.Point(startLong0, startLat0);

var startLong;
var startLat;
var endLong;
var endLat;

//坐标转换完之后的回调函数
translateCallback = function (data){
    if(data.status === 0) {
        map.centerAndZoom(data.points[0], 15);
        startLong=data.points[0].lng;
        startLat=data.points[0].lat;
        endLong=startLong;
        endLat=startLat;
    }
}

setTimeout(function(){
    var convertor = new BMap.Convertor();
    var pointArr = [];
    pointArr.push(ggPoint);
    convertor.translate(pointArr, 1, 5, translateCallback)
}, 0);
// 添加带有定位的导航控件
var navigationControl = new BMap.NavigationControl({
    // 靠左上角位置
    anchor: BMAP_ANCHOR_TOP_LEFT,
    // LARGE类型
    type: BMAP_NAVIGATION_CONTROL_LARGE,
    // 启用显示定位
    enableGeolocation: true
});
map.addControl(navigationControl);
// 添加定位控件
var geolocationControl = new BMap.GeolocationControl();
geolocationControl.addEventListener("locationSuccess", function(e){
    // 定位成功事件
    var address = '';
    address += e.addressComponent.province;
    address += e.addressComponent.city;
    address += e.addressComponent.district;
    address += e.addressComponent.street;
    address += e.addressComponent.streetNumber;
    alert("当前定位地址为：" + address);
});
geolocationControl.addEventListener("locationError",function(e){
    // 定位失败事件
    alert(e.message);
});
map.addControl(geolocationControl);
function addClickHandler(content,marker){
    marker.addEventListener("click",function(e){
        openInfo(content,e)}
    );
}
function openInfo(content,e){
    var p = e.target;
    var point = new BMap.Point(p.getPosition().lng, p.getPosition().lat);
    var infoWindow = new BMap.InfoWindow(content);  // 创建信息窗口对象
    map.openInfoWindow(infoWindow,point); //开启信息窗口
}
var carMk;
var myIcon = new BMap.Icon("img/car.png", new BMap.Size(37,25), {imageOffset: new BMap.Size(0, 0)});//卡车
//draw map
function goWay(){
    //新GPS数据
    $.ajax({
        type:"GET",
        url:"/smartcar/getGPSdata.php",
        dataType:"json",
        success:function(data){
            //console.log(parseFloat(data.longitude),parseFloat(data.latitude));
            endLong1 = data.longitude;
            endLat1 = data.latitude;
            //console.log(endLong1,endLat);

        }
    })

    var ggPoint1 = new BMap.Point(endLong1, endLat1);
    //坐标转换完之后的回调函数
    translateCallback1 = function (data){
        if(data.status === 0) {
            endLong=data.points[0].lng;
            endLat=data.points[0].lat;
        }
    }

    setTimeout(function(){
        var convertor1 = new BMap.Convertor();
        var pointArr1 = [];
        pointArr1.push(ggPoint1);
        convertor1.translate(pointArr1, 1, 5, translateCallback1)
    }, 0);
    drawIcon(startLong,startLat,endLong,endLat);
    startLong=endLong;
    startLat=endLat;
}
//画轨迹图
function drawGreenLine(startLong,startLat,endLong,endLat){
    var polyline = new BMap.Polyline([
        new BMap.Point(startLong,startLat),//起始点的经纬度
        new BMap.Point(endLong,endLat)//终止点的经纬度
    ], {
        strokeColor:"green",//设置颜色
        strokeWeight:5, //宽度
        strokeOpacity:1//透明度
    });
    map.addOverlay(polyline);
}


function drawIcon(startLong,startLat,endLong,endLat){
    if(carMk){
        map.removeOverlay(carMk);
    }
    carMk = new BMap.Marker(
        new BMap.Point(endLong,endLat),//起始点的经纬度
        {icon:myIcon}
    );
    map.addOverlay(carMk);
    drawGreenLine(startLong,startLat,endLong,endLat);
}
//遗失物品标记
<?php
/*$sql="select * from `things`";
$result = $conn->query($sql);
if(!empty($result))
    while ($row = $result->fetch_array()) {
        */?>/*
        var sContent =
        "<h4 style='margin:0 0 5px 0;padding:0.2em 0'><?php /*echo $row['name']; */?></h4>" +
        "<img style='float:right;margin:4px' id='imgDemo' src='<?php /*echo $row['imgsrc']; */?>' width='139' height='104' title='<?php /*echo $row['name']; */?>'/>" + "</div>";
        var longtitude = <?php /*echo $row['longtitude']; */?>;
        var latitude = <?php /*echo $row['latitude']; */?>;
        var point = new BMap.Point(longtitude, latitude);
        var marker = new BMap.Marker(point);
        map.addOverlay(marker);
        addClickHandler(sContent,marker);
        */<?php
/*    }
*/?>
function showThings() {

    $.ajax({
        type:"GET",
        url:"/smartcar/getThings.php",
        dataType:"json",
        success:function(data){
            //things = data;
            time = data.time;
            longtitude1 = data.longtitude;
            latitude1 = data.latitude;
            imgsrc = data.imgsrc;
            imgPath = imgsrc.substr(imgsrc.indexOf("img"),imgsrc.length);
            name = data.name;
            //alert(imgPath );

        }
    })

    var sContent =
        "<h4 style='margin:0 0 5px 0;padding:0.2em 0'>"+ name +"</h4>" +
        "<img style='float:right;margin:4px' id='imgDemo' src='"+ imgPath +"' width='139' height='104' title='"+ name +"'/></div>";
    var longtitude = longtitude1;
    var latitude = latitude1;
    var point = new BMap.Point(longtitude, latitude);
    var marker = new BMap.Marker(point);
    map.addOverlay(marker);
    addClickHandler(sContent,marker);

}

//</script>
