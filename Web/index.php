<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
    <title>主页</title>
    <meta name="description" content="">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="apple-touch-icon" href="apple-touch-icon.png">
    <link rel="stylesheet" href="css/bootstrap.min.css">
    <link rel="stylesheet" href="css/bootstrap-theme.min.css">
    <link rel="stylesheet" href="css/fontAwesome.css">
    <link rel="stylesheet" href="css/light-box.css">
    <link rel="stylesheet" href="css/style.css">
    <!--<link href="https://fonts.googleapis.com/css?family=Kanit:100,200,300,400,500,600,700,800,900" rel="stylesheet">-->
    <script src="js/vendor/modernizr-2.8.3-respond-1.4.2.min.js"></script>
    <script src="http://echarts.baidu.com/build/dist/echarts.js"></script>
    <script type="text/javascript" src="js/jquery-3.4.1.min.js"></script>
    <script src="js/polyline.js"></script>
    <script type="text/javascript">

        $(document).ready(function(){
            //getPolyLine();
            setInterval(getRecentData,5000);
            //getRecentData();
            $("#show_data").click(function(){

                var buttonText = document.getElementById("show_data").innerHTML;
                if (buttonText==="年平均温湿度变化"){
                    $("#show_data").text("返回");
                    //getRecentData();
                    getPolyLine();
                }
                else{
                    $("#show_data").text("年平均温湿度变化");
                    getRecentData();
                }

            });
        });
    </script>
</head>

<body>

<nav>
    <div class="logo">
        <a href="index.html">校园<em>小车</em></a>
    </div>
    <div class="menu-icon">
        <span></span>
    </div>
</nav>
<div id="video-container">
    <div class="video-overlay"></div>
    <div class="video-content">
        <div class="inner">
            <h1>Welcome to <em>SMARTCAR</em></h1>
            <div class="scroll-icon">
                <a class="scrollTo" data-scrollTo="portfolio" href="#"><img src="img/scroll-icon.png" alt=""></a>
            </div>
        </div>
    </div>
    <video autoplay loop muted>
        <source src="SmartCar.mp4" type="video/mp4"/>
    </video>
</div>
<div class="page-heading" id="portfolio">
    <div class="container">
        <div class="heading-content">
            <h1>遗失物品标记图<em>及小车轨迹 </em></h1>
        </div>
    </div>
</div>

<!--遗失物标记图-->
<div id="allmap"></div>
<!--绘制温湿度折线图-->
<div style="margin-left: 50px;margin-top: 20px; "><button id="show_data" style="horiz-align: right">年平均温湿度变化</button></div>
<!--<div id="chart" style="height:600px;width: 1100px;margin-left: 30px;"></div>-->
<div style="height:600px;width: 1100px;margin-left: 30px;">
    <div id="chart" style="height:100%;width: 90%;margin-left: 20%;align-content: center"></div>
</div>

<footer>
    <div class="container-fluid">
        <div class="col-md-12">
            <p>Copyright &copy; 校园小车项目组<a href="about.html" target="_blank" title="校园小车">关于我们</a>
            </p>
        </div>
    </div>
</footer>


<!-- Modal button -->
<div class="popup-icon">
    <button id="modBtn" class="modal-btn"><img src="img/contact-icon.png" alt=""></button>
</div>

<!-- 留言 -->
<div id="modal" class="modal">
    <!-- Modal Content -->
    <div class="modal-content">
        <!-- Modal Header -->
        <div class="modal-header">
            <h3 class="header-title">给我们 <em>留言</em></h3>
            <div class="close-btn"><img src="img/close_contact.png" alt=""></div>
        </div>
        <!-- Modal Body -->
        <div class="modal-body">
            <div class="col-md-6 col-md-offset-3">
                <form id="contact" action="messages.php" method="post">
                    <div class="row">
                        <div class="col-md-12">
                            <fieldset>
                                <input name="name" type="text" class="form-control" id="name" placeholder="姓名"
                                       required="">
                            </fieldset>
                        </div>
                        <div class="col-md-12">
                            <fieldset>
                                <input name="email" type="email" class="form-control" id="email" placeholder="邮箱"
                                       required="">
                            </fieldset>
                        </div>
                        <div class="col-md-12">
                            <fieldset>
                                <textarea name="message" rows="6" class="form-control" id="message" placeholder="留言"
                                          required></textarea>
                            </fieldset>
                        </div>
                        <div class="col-md-12">
                            <fieldset>
                                <input type="submit" value="提交" id="form-submit" class="btn"/>
                            </fieldset>
                        </div>
                    </div>
                </form>
            </div>
        </div>
    </div>
</div>


<section class="overlay-menu">
    <div class="container">
        <div class="row">
            <div class="main-menu">
                <ul>
                    <li>
                        <a href="index.php">首页</a>
                    </li>
                    <li>
                        <a href="about.html">关于我们</a>
                    </li>
                    <li>
                        <a href="blog.html">博客</a>
                    </li>
                </ul>
                <p></p>
            </div>
        </div>
    </div>
</section>
<script type="text/javascript" src="//api.map.baidu.com/api?v=2.0&ak=kuoMWw4WN83FxTOUP7tjjk0K5Q6jkhZp"></script>
<script type="text/javascript" src="map.php">
</script>
<script>window.jQuery || document.write('<script src="js/vendor/jquery-1.11.2.min.js"><\/script>')</script>
<script src="js/vendor/bootstrap.min.js"></script>
<script src="js/plugins.js"></script>
<script src="js/main.js"></script>
</body>
</html>
