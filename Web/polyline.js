//<script type="text/javascript">
//绘制全年温湿度变化折线图
function getPolyLine(){

    // 路径配置
    require.config({
        paths: {
        echarts: 'http://echarts.baidu.com/build/dist'
        }
    });
    // 使用
    require(
        [
        'echarts',
        'echarts/chart/line',//需要折线图则加载line模块
        'echarts/chart/bar' // 使用柱状图就加载bar模块，按需加载
        ],
        function (ec) {
            // 基于准备好的dom，初始化echarts图表
            var myChart = ec.init(document.getElementById('chart'));
            option = {
                title:{
                  text:'年平均温湿度变化',
                  subtext:'榆中县',
                  x:'center'
                },
                tooltip: {
                    trigger: 'axis',//坐标轴触发
                    //trigger: 'item'数据项图形触发
                },
                legend: {
                    orient: 'horizontal',//设置布局方式，默认水平布局
                    x: 'left',//设置水平安放位置，默认全图居中
                    y: 'top',//设置垂直安放位置，默认全图顶端
                    data: ['温度°C', '湿度%']
                },
                //工具框，可选
                toolbox: {
                    show: true,
                    feature: {
                        mark: {show: true},
                        dataView: {show: true, readOnly: false},
                        magicType: {show: true, type: ['line', 'bar', 'stack', 'tiled']},
                        restore: {show: true},
                        saveAsImage: {show: true}//下载工具
                    }
                },
                calculable: true,
                xAxis: [
                {
                    type: 'category',
                    boundaryGap: false,// boundaryGap值为false的时候，折线第一个点在y轴上
                    data: ['一月', '二月', '三月', '四月', '五月', '六月', '七月','八月','九月', '十月', '十一月','十二月']
                }
                ],
                yAxis: [
                {
                    //type: 'value',
                    //splitNumber:10,// 设置y轴刻度间隔个数
                    //interval:10,设置X轴数据间隔几个显示一个，为0表示都显示
                    min: -20,// 设置y轴刻度的最小值
                    max: 80,// 设置y轴刻度的最大值
                    // 设置y轴数据旋转倾斜
                    /*axisLabel:{
                        formatter: {value} °C;
                    }*/
                }
                ],
                series: [
                {
                    name: '平均温度',
                    type: 'line',
                    symbolSize:10,//设置折线上圆点大小
                    symbol:'circle',//设置拐点为实心圆
                    //stack: '总量',
                    itemStyle: {
                        normal: {
                            areaStyle: {type: 'default'},
                            label:{
                                show:true//在拐点上显示数值
                            }
                        }
                    },
                    data: [(0-13)/2,(5-8)/2, (12-2)/2, (21+4)/2, (20+7)/2, (24+11)/2, (26+13)/2,(26+11)/2, (21+8)/2, (14+1)/2, (8-5)/2,(3-10)/2]
                },
                {
                    name: '平均湿度',
                    type: 'line',
                    //stack: '总量',
                    itemStyle: {
                        normal: {
                            areaStyle: {type: 'default'},
                            label:{
                                show:true
                            }
                        }
                    },
                    data: [40, 36, 39, 34, 45, 56, 63, 51, 53, 64, 53, 48,49]
                }
                ]
            };

                myChart.setOption(option);// 为echarts对象加载数据
        }
    );
}
//绘制当前时间段温湿度数据
function getRecentData(){
    //var dataArray;
    $.ajax({
        type: "GET",
        url: "/smartcar/getData.php",
        dataType: "json",
        success: function (data) {
            dataArray = data;
            //console.log(dataArray[0]["time"]);

        }
    })
    // 路径配置
    require.config({
        paths: {
            echarts: 'http://echarts.baidu.com/build/dist'
        }
    });
    // 使用
    require(
        [
            'echarts',
            'echarts/chart/line',//需要折线图则加载line模块
            'echarts/chart/bar' // 使用柱状图就加载bar模块，按需加载
        ],
        function (ec) {
            // 基于准备好的dom，初始化echarts图表
            var myChart = ec.init(document.getElementById('chart'));
            var time = new Array(20);
            option = {
                title:{
                    text:'实时数据',
                    subtext:'榆中县',
                    x:'center'
                },
                tooltip: {
                    trigger: 'axis',//坐标轴触发
                    //trigger: 'item'数据项图形触发
                },
                legend: {
                    orient: 'horizontal',//设置布局方式，默认水平布局
                    x: 'left',//设置水平安放位置，默认全图居中
                    y: 'top',//设置垂直安放位置，默认全图顶端
                    data: ['温度°C', '湿度%']
                },
                //工具框，可选
                toolbox: {
                    show: true,
                    feature: {
                        mark: {show: true},
                        dataView: {show: true, readOnly: false},
                        magicType: {show: true, type: ['line', 'bar', 'stack', 'tiled']},
                        restore: {show: true},
                        saveAsImage: {show: true}//下载工具
                    }
                },
                calculable: true,
                xAxis: [
                    {
                        type: 'category',
                        interval:0,//设置X轴数据间隔几个显示一个，为0表示都显示
                        boundaryGap: false,// boundaryGap值为false的时候，折线第一个点在y轴上
                        data: [dataArray[0]["time"].substr(11),dataArray[1]["time"].substr(11),dataArray[2]["time"].substr(11),dataArray[3]["time"].substr(11),dataArray[4]["time"].substr(11), dataArray[5]["time"].substr(11), dataArray[6]["time"].substr(11),dataArray[7]["time"].substr(11),dataArray[8]["time"].substr(11), dataArray[9]["time"].substr(11), dataArray[10]["time"].substr(11),dataArray[11]["time"].substr(11),dataArray[12]["time"].substr(11),dataArray[13]["time"].substr(11)]
                    }
                ],
                yAxis: [
                    {
                        type: 'value',
                        splitNumber:10,// 设置y轴刻度间隔个数
                        //interval:10,//设置X轴数据间隔几个显示一个，为0表示都显示
                        min: -20,// 设置y轴刻度的最小值
                        max: 80,// 设置y轴刻度的最大值
                        // 设置y轴数据旋转倾斜
                        /*axisLabel:{
                            formatter: {value} °C;
                        }*/
                    }
                ],
                series: [
                    {
                        name: '温度',
                        type: 'line',
                        symbolSize:10,//设置折线上圆点大小
                        symbol:'circle',//设置拐点为实心圆
                        //stack: '总量',
                        itemStyle: {
                            normal: {
                                areaStyle: {type: 'default'},
                                label:{
                                    show:true//在拐点上显示数值
                                }
                            }
                        },
                        data: [dataArray[0]["temperature"],dataArray[1]["temperature"],dataArray[2]["temperature"],dataArray[3]["temperature"],dataArray[4]["temperature"], dataArray[5]["temperature"], dataArray[6]["temperature"],dataArray[7]["temperature"],dataArray[8]["temperature"], dataArray[9]["temperature"], dataArray[10]["temperature"],dataArray[11]["temperature"],dataArray[12]["temperature"],dataArray[13]["temperature"]]
                    },
                    {
                        name: '湿度',
                        type: 'line',
                        //stack: '总量',
                        itemStyle: {
                            normal: {
                                areaStyle: {type: 'default'},
                                label:{
                                    show:true
                                }
                            }
                        },
                        data: [dataArray[0]["humidity"],dataArray[1]["humidity"],dataArray[2]["humidity"],dataArray[3]["humidity"],dataArray[4]["humidity"], dataArray[5]["humidity"], dataArray[6]["humidity"],dataArray[7]["humidity"],dataArray[8]["humidity"], dataArray[9]["humidity"], dataArray[10]["humidity"],dataArray[11]["humidity"],dataArray[12]["humidity"],dataArray[13]["humidity"]]
                    }
                ]
            };

            myChart.setOption(option);// 为echarts对象加载数据
        }
    );
}
//</script>