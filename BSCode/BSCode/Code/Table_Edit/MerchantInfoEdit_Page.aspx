<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="MerchantInfoEdit_Page.aspx.cs" Inherits="BSCode.Code.Table_Edit.MerchantInfoAdd_Page" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head id="Head1" runat="server">
<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <title>商户数据表增加界面</title>
 
    <link href="../../CSS/authority/common_style.css" rel="stylesheet" type="text/css" />
    <link href="../../CSS/authority/basic_layout.css" rel="stylesheet" type="text/css" />

    <%--弹出淡然--%>
    <script src="../../Scripts/jquery-1.8.2.min.js" type="text/javascript"></script>
    <script type="text/javascript" src="../../Scripts/jquery-ui.min.js"></script>
    <script src="../../Scripts/jquery-ui-timepicker-addon.js" type="text/javascript"></script>

      <%--编辑样式--%>
    <script type="text/javascript" src="../../Scripts/bootstrap.min.js"></script>
    <link rel="stylesheet" type="text/css" href="../../CSS/edit/bootstrap.min.css" media="screen"/>

       <%--编辑弹出提示--%>
    <link rel="stylesheet" href="../../CSS/form-field-tooltip.css" type="text/css"/>
	<script type="text/javascript" src="../../Scripts/trip/rounded-corners.js" charset="gb2312"></script>
	<script type="text/javascript" src="../../Scripts/trip/form-field-tooltip.js" charset="gb2312"></script>
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

        function MerchantInfoAddVerify() {
            var loginName = document.getElementById("_loginName").value;
            var password = document.getElementById("_passwd").value;
            var company = document.getElementById("_company").value;
            var merchantCardID = document.getElementById("_merchantCardID").value;
            var registerName = document.getElementById("_registerName").value;
            var registerID = document.getElementById("_registerID").value;
            var registerPhone = document.getElementById("_registerPhone");
            var registerAddress = document.getElementById("_registerAddress");
            var identityReg = /(^\d{15}$)|(^\d{18}$)|(^\d{17}(\d|X|x)$)/;
            var mobileReg = /1[3-8]+\d{9}/;

            if (loginName == null || loginName == "") {
                alert("商户名不能为空");
                return false;
            }
            else if (password == null||password == "" ) {
                alert("商户密码不能为空");
                return false;
            }
            else if (company == null || password == "") {
                alert("商户所属公司不能为空");
                return false;
            }
            else if (registerID == null || registerID == "") {
                alert("请输入合法的身份证号码");
                return false;
            }
            else if (registerPhone == null || registerPhone == "") {
                alert("请输入合法的手机号码");
                return false;
            }
            else if (merchantCardID == null || merchantCardID == "" ) {
                alert("商户卡号不能为空");
                return false;
            }
            else if (registerName == null || registerName== "") {
                alert("注册人姓名不能为空");
                return false;
            }
            else if (registerAddress == null || registerAddress == "") {
                alert("注册人地址不能为空");
                return false;
            }
            return true;
        }
        function MerchantInfoResetVerify()
        {
            _loginName.value = "";
            _passwd.value = "";
            _company.value = "";
            _registerName.value = "";
            _registerID.value = "";
            _registerPhone.value = "";
            _registerAddress.value = "";
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
            当前位置：商户数据表&nbsp;>&nbsp;<span style="color: #1A5CC6;">商户数据表编辑</span>
            <div id="page_close">
                <a href="javascript:parent.$.fancybox.close();">
                    <img src="../../Images/common/page_close.png" width="20" height="20" style="vertical-align: text-top;" />
                </a>
            </div>
        </div>
        <br />

        <div style="margin: 0 30px" class="input-group">
            <span style="text-align: center;" class="input-group-addon">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;商户名</span>
            <input type="text" class="form-control" style="margin: 0 70px 0 0; width: 200px; height: 30px;"
                 name="_loginName" runat="server" id="_loginName" placeholder="商户名" />
            <span style="text-align: center;" class="input-group-addon">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;商户密码</span>
            <input type="text" class="form-control" style="width: 200px; height: 30px;"
                 name="_passwd" runat="server" id="_passwd" placeholder="商户密码" />
        </div>
        <br />

        <div style="margin: 0 30px" class="input-group">
            <span style="text-align: center;" class="input-group-addon">商户所属公司</span>
            <input type="text" class="form-control" style="margin: 0 70px 0 0; width: 200px; height: 30px;"
                 tooltipText="公司所在地详细地址" name="_company" runat="server" id="_company" placeholder="商户所属公司" />
            <span style="text-align: center;" class="input-group-addon">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;商户卡号</span>
            <input type="text" class="form-control" style="width: 200px; height: 30px;"
                 name="_merchantCardID" runat="server" id="_merchantCardID" placeholder="商户卡号" readonly="true" />
        </div>
        <br />

        <div style="margin: 0 30px" class="input-group">
            <span style="text-align: center;" class="input-group-addon">&nbsp;&nbsp;&nbsp;&nbsp;注册人姓名</span>
            <input type="text" class="form-control" style="margin: 0 70px 0 0; width: 200px; height: 30px;"
               name="_registerName" runat="server" id="_registerName" placeholder="注册人姓名" />
            <span style="text-align: center;" class="input-group-addon">注册人证件号</span>
            <input type="text" class="form-control" style="width: 200px; height: 30px;" onkeyup="checkRate(event,this)"
                tooltipText="注册人身份证号" name="_registerID" runat="server" id="_registerID" placeholder="注册人证件号" />
        </div>
        <br />

        <div style="margin: 0 30px" class="input-group">
            <span style="text-align: center;" class="input-group-addon">&nbsp;&nbsp;&nbsp;&nbsp;注册人电话</span>
            <input type="text" class="form-control" style="margin: 0 70px 0 0; width: 200px; height: 30px;" name="_registerPhone"
                onkeyup="this.value=this.value.replace(/\D/g,'')" onafterpaste="this.value=this.value.replace(/\D/g,'')" id="_registerPhone" runat="server" placeholder="注册人电话" />
            <span style="text-align: center;" class="input-group-addon">&nbsp;&nbsp;&nbsp;&nbsp;注册人地址</span>
            <input type="text" class="form-control" style="width: 200px; height: 30px;"
                tooltipText="注册人所在地地址" name="_registerAddress" runat="server" id="_registerAddress" placeholder="注册人地址" />
        </div>
        <br />
        <br />
        <div>
            <asp:Button ID="update_button" runat="server" Text="更新" Style="margin-left: 30%" OnClick="update_button_Click" CssClass="ui_input_btn01"></asp:Button>
            <input class="ui_input_btn01" value="重置" runat="server" style="margin-right: 30%;" onclick="MerchantInfoResetVerify()" />
        </div>
    </form>
     <script type="text/javascript" charset="gb2312">
         var tooltipObj = new DHTMLgoodies_formTooltip();
         tooltipObj.setTooltipPosition('below');
         tooltipObj.setTooltipCornerSize(10);
         tooltipObj.initFormFieldTooltip();
     </script>

</body>
</html>
