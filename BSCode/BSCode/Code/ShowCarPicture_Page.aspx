<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="ShowCarPicture_Page.aspx.cs" Inherits="BSCode.Code.picture" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>图片显示</title>
    <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  
    <link href="../CSS/authority/basic_layout.css" rel="stylesheet" type="text/css" />
    <link rel="stylesheet" type="text/css" href="../CSS/picture/normalize.css" />
    <link rel="stylesheet" type="text/css" href="../CSS/picture/default.css" />
    <link rel="stylesheet" type="text/css" href="../CSS/picture/style.css" />
    <script type="text/javascript" src="../Scripts/picture/jquery-1.11.0.min.js"></script>
    <script type="text/javascript" src="../Scripts/picture/vmc.slider.full.min.js"></script>
    <script language="javascript">
        function GetRequest() {
            var url = location.search; //获取url中"?"符后的字串 
            var theRequest = new Object();
            if (url.indexOf("?") != -1) {
                var str = url.substr(1);
                strs = str.split("&");
                for (var i = 0; i < strs.length; i++) {
                    theRequest[strs[i].split("=")[0]] = unescape(strs[i].split("=")[1]);
                }
            }
            return theRequest;
        }
        window.onload = function () {
            var Request = new Object();
            Request = GetRequest();
            var m_carCardID, m_weighPlace, m_inOut, m_path;
            var m_root;
            m_carCardID = Request["carCardID"];
            m_weighPlace = Request["weighPlace"];
            m_inOut = Request["inOut"];
            m_path = "../CarImages/";
            if (m_inOut == "进") m_inOut = "1";
            else m_inOut = "0";
            m_root = m_path + m_carCardID + "-" + m_weighPlace + "-" + m_inOut;
            document.getElementById("img0").src = m_root +"-0.jpg";
            document.getElementById("img1").src = m_root + "-1.jpg";
            document.getElementById("img2").src = m_root + "-2.jpg";
            document.getElementById("img3").src = m_root + "-3.jpg";
            document.getElementById("img4").src = m_root + "-4.jpg";
        }
    </script>
</head>
<body>
   <div id="nav_links">
        当前位置：合同车卡编号表&nbsp;>&nbsp;<span style="color: #1A5CC6;">照片查看</span>
        <div id="page_close">
            <a href="javascript:parent.$.fancybox.close();">
                <img src="../Images/common/page_close.png" width="20" height="20" style="vertical-align: text-top;" />
            </a>
        </div>
    </div>
    <form id="Form1" runat="server">
        <article class="htmleaf-container">
            <div class="content" style="width: 100%; height: 100%; margin: 30px auto; overflow: hidden;">
                <div id="slider">
                    <a href="#">
                        <img id="img0" src="" width="100%" height="100%"/></a>
                    <a href="#">
                        <img id="img1" src="" width="100%" height="100%"/></a>
                    <a href="#">
                        <img id="img2" src="" width="100%" height="100%"/></a>
                    <a href="#">
                        <img id="img3" src="" width="100%" height="100%"/></a>
                    <a href="#">
                        <img id="img4" src="" width="100%" height="100%"/></a>
                </div>
            </div>
        </article>
    </form>

    <script type="text/javascript">
        $(function () {
            $('#slider').vmcSlider({
                width: 800,
                height: 450,
                gridCol: 10,
                gridRow: 5,
                gridVertical: 20,
                gridHorizontal: 10,
                autoPlay: true,
                ascending: true,
                effects: [
					'fade', 'fadeLeft', 'fadeRight', 'fadeTop', 'fadeBottom', 'fadeTopLeft', 'fadeBottomRight',
					'blindsLeft', 'blindsRight', 'blindsTop', 'blindsBottom', 'blindsTopLeft', 'blindsBottomRight',
					'curtainLeft', 'curtainRight', 'interlaceLeft', 'interlaceRight', 'mosaic', 'bomb', 'fumes'
                ],
                ie6Tidy: false,
                random: false,
                duration: 2000,
                speed: 1500
            });
        });
    </script>

</body>
</html>
