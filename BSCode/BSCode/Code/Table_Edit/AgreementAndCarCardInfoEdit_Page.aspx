<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="AgreementAndCarCardInfoEdit_Page.aspx.cs" Inherits="BSCode.Code.Table_Edit.AgreementAndCarCardInfoAdd_Page" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <title>合同车卡编号表编辑页面</title>
    <link href="../../CSS/authority/common_style.css" rel="stylesheet" type="text/css" />
    <link href="../../CSS/authority/basic_layout.css" rel="stylesheet" type="text/css" />
      <%--编辑弹出提示--%>
    <link rel="stylesheet" href="../../CSS/form-field-tooltip.css" type="text/css"/>
	<script type="text/javascript" src="../../Scripts/trip/rounded-corners.js" charset="gb2312"></script>
	<script type="text/javascript" src="../../Scripts/trip/form-field-tooltip.js" charset="gb2312"></script>

    <%--时间--%>
    <script type="text/javascript" src="../../Scripts/laydate.js"></script>
    <link rel="stylesheet" type="text/css" href="../../CSS/laydate.css" media="screen"/>

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

         function AgreementInfoAddVerify() {
             var m_agreementNo = document.getElementById("_agreementNo").value;
             var m_carCardID = document.getElementById("_carCardID").value;
             var m_emptyCarSize = document.getElementById("_emptyCarSize").value;
             var m_cargoSize = document.getElementById("_cargoSize").value;
             var m_cargoCarSize = document.getElementById("_cargoCarSize").value;
             var m_carShapeInfo = document.getElementById("_carShapeInfo").value;
             var m_emptyCarWeight = document.getElementById("_emptyCarWeight").value;
             var m_CarWeight = document.getElementById("_CarWeight").value;
             var m_cargoWeight = document.getElementById("_cargoWeight").value;
             var m_cargoDensity = document.getElementById("_cargoDensity").value;

             var m_deductWeight = document.getElementById("_deductWeight").value;
             var m_weighPlace = document.getElementById("_weighPlace").value;
             var m_abnormal = document.getElementById("_abnormal").value;
             var m_recordTime = document.getElementById("recordTime").value;

             if (m_agreementNo == "" || m_agreementNo == null) {
                 alert("请输入合同编号");
                 return false;
             }
             else if (m_carCardID == null || m_carCardID == "") {
                 alert("请输入车卡编号");
                 return false;
             }
             else if (m_emptyCarSize == null || m_emptyCarSize == "") {
                 alert("请输入空车体积");
                 return false;
             }
             else if (m_cargoSize == null || m_cargoSize == "") {
                 alert("请输入煤炭体积");
                 return false;
             }
             else if (m_cargoCarSize == null || m_cargoCarSize == "") {
                 alert("请输入煤车体积");
                 return false;
             }
             else if (m_carShapeInfo == null || m_carShapeInfo == "") {
                 alert("请输入车辆形状");
                 return false;
             }
             else if (m_emptyCarWeight == null || m_emptyCarWeight == "") {
                 alert("请输入空车重量");
                 return false;
             }
             else if (m_CarWeight == null || m_CarWeight == "") {
                 alert("请输入车标重");
                 return false;
             }
             else if (m_cargoWeight == null || m_cargoWeight == "") {
                 alert("请输入煤炭重量");
                 return false;
             }
             else if (m_cargoDensity == null || m_cargoDensity == "") {
                 alert("请输入煤炭密度");
                 return false;
             }
             else if (m_deductWeight == null || m_deductWeight == "") {
                 alert("请输入扣重");
                 return false;
             }
             else if (m_weighPlace == null || m_weighPlace == "") {
                 alert("请输入拉货地点");
                 return false;
             }
             else if (m_abnormal == null || m_abnormal == "") {
                 alert("请输入是否违规");
                 return false;
             }
             else if (m_recordTime == null || m_recordTime == "") {
                 alert("请输入记录时间");
                 return false;
             }
             return true;
         }

         function AgreementInfoResetVerify() {
             _agreementNo.value = "";
             _carCardID.value = "";
             _emptyCarSize.value = "";
             _cargoSize.value = "";
             _cargoCarSize.value = "";
             _carShapeInfo.value = "";
             _emptyCarWeight.value = "";
             _CarWeight.value = "";
             _cargoWeight.value = "";
             _cargoDensity.value = "";
             _deductWeight.value = "";
             _weighPlace.value = "";
             _abnormal.value = "";
             recordTime.value = "";
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
    <form id="Form1" runat="server">
        <div id="nav_links">
            当前位置：合同车卡编号表&nbsp;>&nbsp;<span style="color: #1A5CC6;">合同车卡编号表编辑</span>
            <div id="page_close">
                <a href="javascript:parent.$.fancybox.close();">
                    <img src="../../Images/common/page_close.png" width="20" height="20" style="vertical-align: text-top;" />
                </a>
            </div>
        </div>
        <br />

        <div style="margin: 0 30px" class="input-group">
            <span style="text-align: center;" class="input-group-addon">合同编号</span>
            <input type="text" class="form-control" style="margin: 0 70px 0 0; width: 200px; height: 30px;"
                name="_agreementNo" runat="server" id="_agreementNo" placeholder="合同编号" />
            <span style="text-align: center;" class="input-group-addon">车卡编号</span>
            <input type="text" class="form-control" style="width: 200px; height: 30px;"
                name="_carCardID" runat="server" id="_carCardID" placeholder="车卡编号" />
        </div>
        <br />

        <div style="margin: 0 30px" class="input-group">
            <span style="text-align: center;" class="input-group-addon">空车体积</span>
            <input type="text" class="form-control" style="width: 200px; height: 30px;"
                onkeyup="clearNoNum(event,this)" onblur="checkNum(this)" name="_emptyCarSize" runat="server" id="_emptyCarSize" placeholder="空车体积" />
        </div>
        <br />

        <div style="margin: 0 30px" class="input-group">
            <span style="text-align: center;" class="input-group-addon">煤炭体积</span>
            <input type="text" class="form-control" style="margin: 0 70px 0 0; width: 200px; height: 30px;"
                onkeyup="clearNoNum(event,this)" onblur="checkNum(this)" name="_cargoSize" runat="server" id="_cargoSize" placeholder="煤炭体积" />
            <span style="text-align: center;" class="input-group-addon">煤车体积</span>
            <input type="text" class="form-control" style="width: 200px; height: 30px;"
                onkeyup="clearNoNum(event,this)" onblur="checkNum(this)" name="_cargoCarSize" runat="server" id="_cargoCarSize" placeholder="煤车体积" />
        </div>
        <br />

        <div style="margin: 0 30px" class="input-group">
            <span style="text-align: center;" class="input-group-addon">车辆形状</span>
            <input type="text" class="form-control" style="margin: 0 70px 0 0; width: 200px; height: 30px;"
                name="_carShapeInfo" runat="server" id="_carShapeInfo" placeholder="车辆形状" />
            <span style="text-align: center;" class="input-group-addon">空车重量</span>
            <input type="text" class="form-control" style="width: 200px; height: 30px;"
                onkeyup="clearNoNum(event,this)" onblur="checkNum(this)" name="_emptyCarWeight" runat="server" id="_emptyCarWeight" placeholder="空车重量" />
        </div>
        <br />

        <div style="margin: 0 30px" class="input-group">
            <span style="text-align: center;" class="input-group-addon">卡车标重</span>
            <input type="text" class="form-control" style="margin: 0 70px 0 0; width: 200px; height: 30px;"
                onkeyup="clearNoNum(event,this)" onblur="checkNum(this)" name="_CarWeight" runat="server" id="_CarWeight" placeholder="卡车标重" />
            <span style="text-align: center;" class="input-group-addon">煤炭重量</span>
            <input type="text" class="form-control" style="width: 200px; height: 30px;"
                onkeyup="clearNoNum(event,this)" onblur="checkNum(this)" name="_cargoWeight" runat="server" id="_cargoWeight" placeholder="煤炭重量" />
        </div>
        <br />


        <div style="margin: 0 30px" class="input-group">
            <span style="text-align: center;" class="input-group-addon">煤炭密度</span>
            <input type="text" class="form-control" style="margin: 0 70px 0 0; width: 200px; height: 30px;"
                onkeyup="clearNoNum(event,this)" onblur="checkNum(this)" name="_cargoDensity" runat="server" id="_cargoDensity" placeholder="煤炭密度" />
            <span style="text-align: center;" class="input-group-addon">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;扣重</span>
            <input type="text" class="form-control" style="width: 200px; height: 30px;"
                onkeyup="clearNoNum(event,this)" onblur="checkNum(this)" name="_deductWeight" runat="server" id="_deductWeight" placeholder="扣重" />
        </div>
        <br />

        <div style="margin: 0 30px" class="input-group">
            <span style="text-align: center;" class="input-group-addon">拉货地点</span>
            <input type="text" class="form-control" style="margin: 0 70px 0 0; width: 200px; height: 30px;"
                tooltipText="请输入具体的拉货位置" name="_weighPlace" runat="server" id="_weighPlace" placeholder="拉货地点" />
            <span style="text-align: center;" class="input-group-addon">是否违规</span>
            <input type="text" class="form-control" style="width: 200px; height: 30px;" 
                tooltipText="0-未违规,1-可能违规,2-已违规" name="_abnormal" runat="server" id="_abnormal" placeholder="是否违规" />
        </div>
        <br />


        <div style="margin: 0 30px" class="input-group">
            <span style="text-align: center;" class="input-group-addon">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;进/出</span>
            <asp:DropDownList ID="_inOut" style="margin: 0 70px 0 0; width: 200px; height: 30px;" runat="server">
                <asp:ListItem>进</asp:ListItem>
                <asp:ListItem>出</asp:ListItem>
            </asp:DropDownList>
            <span style="text-align: center;" class="input-group-addon">记录时间</span> 
            <input type="text" class="inline laydate-icon form-control" style="width: 200px; height: 30px;" name="recordTime" runat="server" id="recordTime" placeholder="记录时间" />
        </div>
        <br />
        <br />

        <div>
            <asp:Button ID="update_button" runat="server" Text="更新" Style="margin-left: 30%" OnClick="update_button_Click" CssClass="ui_input_btn01"></asp:Button>
            <input id="Text1" class="ui_input_btn01" value="重置" runat="server" style="margin-right: 30%;" onclick="AgreementInfoResetVerify()" />
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
         var _signDate = {
             elem: '#_signDate',
             format: 'YYYY-MM-DD hh:mm:ss',
             min: '2015-7-8', //设定最小日期为当前日期
             max: '2099-06-16', //最大日期
             istime: true,
             istoday: false,
             choose: function (datas) {
                 _validatyPeriod.min = datas; //开始日选好后，重置结束日的最小日期
                 _validatyPeriod.start = datas //将结束日的初始值设定为开始日
             }
         };

         var _validatyPeriod = {
             elem: '#_validatyPeriod',
             format: 'YYYY-MM-DD hh:mm:ss',
             min: '2015-7-8',
             max: '2099-06-16',
             istime: true,
             istoday: false,
             choose: function (datas) {
                 _signDate.max = datas; //结束日选好后，充值开始日的最大日期
             }
         };
         laydate(_signDate);
         laydate(_validatyPeriod);
    </script>

</body>
</html>
