<%@ Page Language="C#"  AutoEventWireup="true" CodeBehind="AgreementInfoEdit_Page.aspx.cs" Inherits="BSCode.Code.Table_Edit.AgreementInfoAdd_Page" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <title>合同信息增加界面</title>
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

        function AgreementInfoAddVerify() {
            var m_agreementNo = document.getElementById("_agreementNo").value;
            var m_merchantCardID = document.getElementById("_merchantCardID").value;
            var m_signDate = document.getElementById("_signDate").value;
            var m_validatyPeriod = document.getElementById("_validatyPeriod").value;
            var m_coalTon = document.getElementById("_coalTon").value;
            var m_carCardCount = document.getElementById("_carCardCount").value;
            var m_useCarCardCount = document.getElementById("_useCarCardCount");
            var m_actualCoalTon = document.getElementById("_actualCoalTon");
            var m_returnCarCardCount = document.getElementById("_returnCarCardCount");
            var m_agreementStatus = document.getElementById("_agreementStatus");

            var time_reg = /(\d{4}-\d{2}-\d{2}\d{2}:\d{2}:\d{2})|(\d{2}-\d{2}\d{2}:\d{2}:\d{2})|(\d{2}:\d{2}:\d{2})/;
            if (m_agreementNo == "" || m_agreementNo == null) {
                alert("请输入合同编号");
                return false;
            }
            else if (m_merchantCardID == null || m_merchantCardID == "") {
                alert("请输入商户卡号");
                return false;
            }
            else if (m_signDate == null || m_signDate == ""  || m_signDate.test(time_reg) == false) {
                alert("请输入合同签约日期");
                return false;
            }
            else if (m_validatyPeriod == null || m_validatyPeriod == "" || m_validatyPeriod.test(time_reg) == false) {
                alert("请输入合同有效期");
                return false;
            }
            else if (m_coalTon == null || m_coalTon == "" ) {
                alert("请输入计划运煤吨数");
                return false;
            }
            else if (m_carCardCount == null || m_carCardCount == "") {
                alert("请输入车卡数量");
                return false;
            }
            else if (m_useCarCardCount == null || m_useCarCardCount == "") {
                alert("请输入已领取车卡数");
                return false;
            }
            else if (m_actualCoalTon == null || m_actualCoalTon == "") {
                alert("请输入实际运煤吨数");
                return false;
            }
            else if (m_returnCarCardCount == null || m_returnCarCardCount == "") {
                alert("请输入退卡数量");
                return false;
            }
            else if (m_agreementStatus == null || m_agreementStatus == "") {
                alert("请输入合同状态");
                return false;
            }
            return true;
        }

        function AgreementInfoResetVerify() {
            _signDate.value = "";
            _validatyPeriod.value = "";
            _coalTon.value = "";
            _carCardCount.value = "";
            _useCarCardCount.value = "";
            _actualCoalTon.value = "";
            _returnCarCardCount.value = "";
            _agreementStatus.value = "";
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

        function checkRate(event, obj) {
            //响应鼠标事件，允许左右方向键移动 
            event = window.event || event;
            if (event.keyCode == 37 | event.keyCode == 39) {
                return;
            }
            //先把非数字的都替换掉，除了数字 
            obj.value = obj.value.replace(/[^\d]/g, "");
        }

        function checkNum(obj) {
            //为了去除最后一个. 
            obj.value = obj.value.replace(/\.$/g, "");
        }
    </script>

</head>

<body>
    <form runat="server">
        <div id="nav_links">
            当前位置：合同数据表&nbsp;>&nbsp;<span style="color: #1A5CC6;">合同数据表编辑</span>
            <div id="page_close">
                <a href="javascript:parent.$.fancybox.close();">
                    <img src="../../Images/common/page_close.png" width="20" height="20" style="vertical-align: text-top;" />
                </a>
            </div>
        </div>
        <br />
        <br />
        <div style="margin: 0 30px" class="input-group">
            <span style="text-align: center;" class="input-group-addon">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;合同编号</span>
            <input type="text" class="form-control" style="margin: 0 70px 0 0; width: 200px; height: 30px;"
                name="_agreementNo" runat="server" id="_agreementNo" readonly="true" placeholder="合同编号" />
            <span style="text-align: center;" class="input-group-addon">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;商户卡号</span>
            <input type="text" class="form-control" style="width: 200px; height: 30px;"
                name="_merchantCardID" runat="server" id="_merchantCardID" readonly="true" placeholder="商户卡号" />
        </div>
        <br />
        <div style="margin: 0 30px" class="input-group">
            <span style="text-align: center;" class="input-group-addon">合同签约日期</span>
            <input type="text" class="inline laydate-icon form-control"
                style="margin: 0 70px 0 0; width: 200px; height: 30px;" name="_signDate" runat="server" id="_signDate" placeholder="合同签约日期" />
            <span style="text-align: center;" class="input-group-addon">&nbsp;&nbsp;&nbsp;&nbsp;合同有效期</span>
            <input type="text" class="inline laydate-icon form-control"
                style="width: 200px; height: 30px;" name="_validatyPeriod" runat="server" id="_validatyPeriod" placeholder="合同有效期" />
        </div>
        <br />
        <div style="margin: 0 30px" class="input-group">
            <span style="text-align: center;" class="input-group-addon">计划运煤吨数</span>
            <input type="text" class="form-control"
                style="margin: 0 70px 0 0; width: 200px; height: 30px;" name="_coalTon" tooltipText="输入数字 可为小数"  
                onkeyup="clearNoNum(event,this)" onblur="checkNum(this)" id="_coalTon" runat="server" placeholder="计划运煤吨数" />
            <span style="text-align: center;" class="input-group-addon">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;车卡数量</span>
            <input type="text" class="form-control"
                style="width: 200px; height: 30px;" name="_carCardCount" 
                tooltipText="输入数字 只能为整数" onkeyup="checkRate(event,this)" runat="server" id="_carCardCount" placeholder="车卡数量" />
        </div>

        <br />
        <div style="margin: 0 30px" class="input-group">
            <span style="text-align: center;" class="input-group-addon">已领取车卡数</span>
            <input type="text" class="inline laydate-icon form-control"
                style="margin: 0 70px 0 0; width: 200px; height: 30px;" onkeyup="checkRate(event,this)"
                    tooltipText="输入数字 只能为整数" name="_useCarCardCount" runat="server" id="_useCarCardCount" placeholder="已领取车卡数" />
            <span style="text-align: center;" class="input-group-addon">实际运煤吨数</span>
            <input type="text" class="inline laydate-icon form-control"
                style="width: 200px; height: 30px;" name="_actualCoalTon" tooltipText="输入数字 可为小数" 
                onkeyup="clearNoNum(event,this)" onblur="checkNum(this)" runat="server" id="_actualCoalTon" placeholder="实际运煤吨数" />
        </div>


        <br />
        <div style="margin: 0 30px" class="input-group">
            <span style="text-align: center;" class="input-group-addon">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;退卡数量</span>
            <input type="text" class="inline laydate-icon form-control"
                style="margin: 0 70px 0 0; width: 200px; height: 30px;" onkeyup="checkRate(event,this)"
                    tooltipText="输入数字 只能为整数" name="_returnCarCardCount" runat="server" id="_returnCarCardCount" placeholder="退卡数量" />
            <span style="text-align: center;" class="input-group-addon">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;合同状态</span>
            <asp:DropDownList ID="_agreementStatus" style="width: 200px; height: 30px;" runat="server">
                <asp:ListItem>完成</asp:ListItem>
                <asp:ListItem>未完成</asp:ListItem>
            </asp:DropDownList>
        </div>
        <br />
        <br />
        <div>
            <asp:Button ID="update_button" runat="server" Text="更新" style="margin-left:35%" OnClick="update_button_Click" CssClass="ui_input_btn01"></asp:Button>
            <input class="ui_input_btn01" value="重置" runat="server" style="margin-right:35%;" onclick="AgreementInfoResetVerify()"/>
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
