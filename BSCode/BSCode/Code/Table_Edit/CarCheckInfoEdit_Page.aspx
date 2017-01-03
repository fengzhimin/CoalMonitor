<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="CarCheckInfoEdit_Page.aspx.cs" Inherits="BSCode.Code.Table_Edit.CarCheckInfoAdd_Page" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <title>车证数据表编辑界面</title>
    <link href="../../CSS/authority/common_style.css" rel="stylesheet" type="text/css" />
    <link href="../../CSS/authority/basic_layout.css" rel="stylesheet" type="text/css" />

     <%--编辑弹出提示--%>
    <link rel="stylesheet" href="../../CSS/form-field-tooltip.css" type="text/css"/>
	<script type="text/javascript" src="../../Scripts/trip/rounded-corners.js" charset="gb2312"></script>
	<script type="text/javascript" src="../../Scripts/trip/form-field-tooltip.js" charset="gb2312"></script>

    <%--时间--%>
    <script type="text/javascript" src="../../Scripts/date/laydate.js"></script>

    <%--编辑样式--%>
    <script type="text/javascript" src="../../Scripts/bootstrap.min.js"></script>
    <link rel="stylesheet" type="text/css" href="../../CSS/edit/bootstrap.min.css" media="screen"/>

    <%--弹出淡然--%>
    <script src="../../Scripts/jquery-1.8.2.min.js" type="text/javascript"></script>
    <script type="text/javascript" src="../../Scripts/jquery-ui.min.js"></script>

    <script type="text/javascript">
 
        $(document).ready(function () {
            /*
             * 更新
             */
            $("#update_button").click(function () {
                /**  关闭弹出iframe  **/
                window.parent.$.fancybox.close();
            });
        });
        function CarCheckInfoAddVerify() {
            var carCardID = document.getElementById("_carCardID").value;
            var emptyCarSize = document.getElementById("_emptyCarSize").value;
            var cargoSize = document.getElementById("_cargoSize").value;
            var cargoCarSize = document.getElementById("_cargoCarSize").value;
            var emptyCarWeight = document.getElementById("_emptyCarWeight").value;
            var CarWeight = document.getElementById("_CarWeight").value;
            var cargoWeight = document.getElementById("_cargoWeight").value;
            var cargoDensity = document.getElementById("_cargoDensity").value;
            var deductWeight = document.getElementById("_deductWeight").value;
            var weighPlace = document.getElementById("_weighPlace").value;
            var abnormal = document.getElementById("_abnormal").value;
            var inOut = document.getElementById("_inOut").value;
            var recordTime = document.getElementById("_recordTime").value;
            var identityReg = /(^\d{15}$)|(^\d{18}$)|(^\d{17}(\d|X|x)$)/;
            var mobileReg = /1[3-8]+\d{9}/;

            if (carCardID == null || carCardID == "")
            {
                alert("车卡ID不能为空");
                return false;

            }
            if (emptyCarSize == null || emptyCarSize == "") {
                alert("空车体积不能为空");
                return false;

            }
            if (cargoSize == null || cargoSize == "") {
                alert("煤炭体积不能为空");
                return false;

            }
            if (cargoCarSize == null || cargoCarSize == "") {
                alert("煤车体积不能为空");
                return false;

            }
            if (emptyCarWeight == null || emptyCarWeight == "") {
                alert("空车重量不能为空");
                return false;

            }
            if (CarWeight == null || CarWeight == "") {
                alert("车不标重能为空");
                return false;

            }
            if (cargoWeight == null || cargoWeight == "") {
                alert("煤炭重量不能为空");
                return false;

            }
            if (cargoDensity == null || cargoDensity == "") {
                alert("煤炭密度不能为空");
                return false;

            }
            if (deductWeight == null || deductWeight == "") {
                alert("扣重能为空");
                return false;

            }
            if (weighPlace == null || weighPlace == "") {
                alert("拉货地点不能为空");
                return false;

            }
            if (abnormal == null || abnormal == "") {
                alert("是否违规不能为空");
                return false;

            }
            if (inOut == null || inOut == "") {
                alert("进出不能为空");
                return false;

            }
            if (recordTime == null || recordTime == "") {
                alert("刷卡时间不能为空");
                return false;

            }
            return true;

        }
        function CarCheckInfoResetVerify()
        {
            _recordTime.value = "";
            _emptyCarSize.value = "";
            _cargoSize.value = "";
            _cargoCarSize.value = "";
            _emptyCarWeight.value = "";
            _CarWeight.value = "";
            _cargoWeight.value = "";
            _cargoDensity.value = "";
            _deductWeight.value = "";
            _weighPlace.value = "";
            _abnormal.value = "";
            _inOut.SelectedValue = "";
            _recordTime = "";
        }


        function clearNoNum(event, obj) {
            //响应鼠标事件，允许左右方向键移动 
            event = window.event || event;
            if (event.keyCode == 37 | event.keyCode == 39) {
                return;
            }
            //先把非数字的都替换掉，除了数字和. 
            obj.value = obj.value.replace(/[^\d.]/g, "");
            //必须保证第一个为数字而不是. 
            obj.value = obj.value.replace(/^\./g, "");
            //保证只有出现一个.而没有多个. 
            obj.value = obj.value.replace(/\.{2,}/g, ".");
            //保证.只出现一次，而不能出现两次以上 
            obj.value = obj.value.replace(".", "$#$").replace(/\./g, "").replace("$#$", ".");
        }
        function checkNum(obj) {
            //为了去除最后一个. 
            obj.value = obj.value.replace(/\.$/g, "");
        }

        function checkRate(event, obj) {
            //响应鼠标事件，允许左右方向键移动 
            event = window.event || event;
            if (event.keyCode == 37 | event.keyCode == 39) {
                return;
            }
            //先把非数字的都替换掉，除了数字 
            obj.value = obj.value.replace(/[^\d]/g, "");
        }

     </script>
</head>

<body>
    <form id="form1" runat="server">
        <div id="nav_links">
            当前位置：车证数据表&nbsp;>&nbsp;<span style="color: #1A5CC6;">车证数据表编辑</span>
            <div id="page_close">
                <a href="javascript:parent.$.fancybox.close();">
                    <img src="../../Images/common/page_close.png" width="20" height="20" style="vertical-align: text-top;" />
                </a>
            </div>
        </div>
        <br />
        <br />
        <div style="margin: 0 30px" class="input-group">
            <span style="text-align: center;" class="input-group-addon">&nbsp;&nbsp;&nbsp;&nbsp;合同ID</span>
            <input type="text" class="form-control" style="margin: 0 70px 0 0; width: 200px; height: 30px;"
                name="_agreementNo" runat="server" id="_agreementNo" readonly="true" placeholder="合同ID" />
            <span style="text-align: center;" class="input-group-addon">&nbsp;&nbsp;&nbsp;&nbsp;车卡ID</span>
            <input type="text" class="form-control" style="width: 200px; height: 30px;"
                onkeyup="checkRate(event,this)" name="_carCardID" runat="server" id="_carCardID" placeholder="车卡ID" />
        </div>
        <br />
        <div style="margin: 0 30px" class="input-group">
            <span style="text-align: center;" class="input-group-addon">空车体积</span>
            <input type="text" class="form-control" style="margin: 0 70px 0 0; width: 200px; height: 30px;"
                onkeyup="clearNoNum(event,this)" onblur="checkNum(this)" name="_emptyCarSize" runat="server" id="_emptyCarSize" placeholder="空车体积" />
            <span style="text-align: center;" class="input-group-addon">煤炭体积</span>
            <input type="text" class="form-control" style="width: 200px; height: 30px;"
                name="_cargoSize" runat="server" id="_cargoSize" placeholder="煤炭体积" />
        </div>
        <br />
        <div style="margin: 0 30px" class="input-group">
            <span style="text-align: center;" class="input-group-addon">煤车体积</span>
            <input type="text" class="form-control"
                onkeyup="clearNoNum(event,this)" onblur="checkNum(this)" style="margin: 0 70px 0 0; width: 200px; height: 30px;" 
                name="_cargoCarSize" id="_cargoCarSize" runat="server" placeholder="煤车体积" />
            <span style="text-align: center;" class="input-group-addon">空车重量</span>
            <input type="text" class="form-control"
                onkeyup="clearNoNum(event,this)" onblur="checkNum(this)" style="width: 200px; height: 30px;" 
                name="_emptyCarWeight" runat="server" id="_emptyCarWeight" placeholder="空车重量" />
        </div>

        <br />
        <div style="margin: 0 30px" class="input-group">
            <span style="text-align: center;" class="input-group-addon">&nbsp;&nbsp;&nbsp;&nbsp;车标重</span>
            <input type="text" class="form-control"
                onkeyup="clearNoNum(event,this)" onblur="checkNum(this)" style="margin: 0 70px 0 0; width: 200px; height: 30px;" 
                name="_CarWeight" runat="server" id="_CarWeight" placeholder="车标重" />
            <span style="text-align: center;" class="input-group-addon">煤炭重量</span>
            <input type="text" class="form-control"
                onkeyup="clearNoNum(event,this)" onblur="checkNum(this)" style="width: 200px; height: 30px;" 
                name="_cargoWeight" runat="server" id="_cargoWeight" placeholder="煤炭重量" />
        </div>


        <br />
        <div style="margin: 0 30px" class="input-group">
            <span style="text-align: center;" class="input-group-addon">煤炭密度</span>
            <input type="text" class="form-control"
                onkeyup="clearNoNum(event,this)" onblur="checkNum(this)" style="margin: 0 70px 0 0; width: 200px; height: 30px;" name="_cargoDensity" runat="server" id="_cargoDensity" placeholder="煤炭密度" />
            <span style="text-align: center;" class="input-group-addon">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;扣重</span>
            <input type="text" class="form-control"
                onkeyup="clearNoNum(event,this)" onblur="checkNum(this)" style="width: 200px; height: 30px;" name="_deductWeight" runat="server" id="_deductWeight" placeholder="扣重" />
        </div>
         <br />
        <div style="margin: 0 30px" class="input-group">
            <span style="text-align: center;" class="input-group-addon">拉货地点</span>
            <input type="text" class="form-control"
                tooltipText="请输入具体的拉货位置" style="margin: 0 70px 0 0; width: 200px; height: 30px;" name="_weighPlace" runat="server" id="_weighPlace" placeholder="拉货地点" />
            <span style="text-align: center;" class="input-group-addon">是否违规</span>
            <input type="text" class="form-control" tooltipText="0-未违规,1-可能违规,2-已违规"
                style="width: 200px; height: 30px;" name="_abnormal" runat="server" id="_abnormal" placeholder="是否违规" />
        </div>


        <br />
        <div style="margin: 0 30px" class="input-group">
            <span style="text-align: center;" class="input-group-addon">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;进/出</span>
            <asp:DropDownList ID="_inOut" style="margin: 0 70px 0 0; width: 200px; height: 30px;" runat="server">
                <asp:ListItem>进</asp:ListItem>
                <asp:ListItem>出</asp:ListItem>
            </asp:DropDownList>

            <span style="text-align: center;" class="input-group-addon">刷卡时间</span>
            <input type="text" class="inline laydate-icon form-control"
                style="width: 200px; height: 30px;" name="_recordTime" runat="server" id="_recordTime" placeholder="刷卡时间" />
        </div>
        <br />
        <br />
        <div>
            <asp:Button ID="update_button" runat="server" Text="更新" style="margin-left:35%" OnClick="update_button_Click" CssClass="ui_input_btn01"></asp:Button>
            <input class="ui_input_btn01" value="重置" runat="server" style="margin-right:35%;" onclick="CarCheckInfoResetVerify()"/>
        </div>

    </form>

     <script type="text/javascript" charset="gb2312">
         var tooltipObj = new DHTMLgoodies_formTooltip();
         tooltipObj.setTooltipPosition('below');
         tooltipObj.setPageBgColor('#808080');
         tooltipObj.setTooltipCornerSize(15);
         tooltipObj.initFormFieldTooltip();
     </script>

     <script type="text/javascript">
         !function () {
             laydate.skin('molv');//切换皮肤，请查看skins下面皮肤库
             laydate({ elem: '#demo' });//绑定元素
         }();

         //日期范围限制
         var _recordTime = {
             elem: '#_recordTime',
             format: 'YYYY-MM-DD hh:mm:ss',
             min: '2015-7-8', //设定最小日期为当前日期
             max: '2099-06-16', //最大日期
             istime: true,
             istoday: false,
             choose: function (datas) {
                 _recordTime.min = datas; //开始日选好后，重置结束日的最小日期
                 _recordTime.start = datas //将结束日的初始值设定为开始日
             }
         };
         laydate(_recordTime);
         </script>
</body>
</html>
