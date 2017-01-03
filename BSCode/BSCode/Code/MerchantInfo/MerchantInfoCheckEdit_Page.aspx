<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="MerchantInfoCheckEdit_Page.aspx.cs" Inherits="BSCode.Code.MerchantInfo.MerchantInfoCheckEdit_Page" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head id="Head1" runat="server">
<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <title>商户数据表增加界面</title>
    <link href="../../CSS/from.css" rel="stylesheet" type="text/css"/>
    <script type="text/javascript" src="../../Scripts/jquery-1.8.3.min.js"></script>
    <link href="../../CSS/authority/common_style.css" rel="stylesheet" type="text/css" />

       <%--编辑弹出提示--%>
    <link rel="stylesheet" href="../../CSS/form-field-tooltip.css" type="text/css"/>
	<script type="text/javascript" src="../../Scripts/trip/rounded-corners.js" charset="gb2312"></script>
	<script type="text/javascript" src="../../Scripts/trip/form-field-tooltip.js" charset="gb2312"></script>

    <script type="text/javascript">

        jQuery(function () {
            // 时间设置
            jQuery('#_signDate').datetimepicker({
                timeFormat: "HH:mm:ss",
                dateFormat: "yy-mm-dd"
            });
        });
        jQuery(function () {
            // 时间设置
            jQuery('#_validatyPeriod').datetimepicker({
                timeFormat: "HH:mm:ss",
                dateFormat: "yy-mm-dd"
            });
        });
        function MerchantInfoCheckEditVerify() {
            var loginName = document.getElementById("_loginName").value;
            var password = document.getElementById("_passwd").value;
            var company = document.getElementById("_company").value;
            var merchantCardID = document.getElementById("_merchantCardID").value;
            var registerName = document.getElementById("_registerName").value;
            var registerID = document.getElementById("_registerID").value;
            var registerPhone = document.getElementById("_registerPhone");
            var registerAddress = document.getElementById("_registerAddress");

            var mobileReg = /1[3-8]+\d{9}/;

            if (loginName == null || loginName == "") {
                alert("商户名不能为空");
                return false;
            }
            else if (password == null || password == "") {
                alert("商户密码不能为空");
                return false;
            }
            else if (company == null || password == "") {
                alert("商户所属公司不能为空");
                return false;
            }
            else if (registerID == null || registerID == "" || !/^\d{17}(\d|x)$/i.test(sId)) {
                alert("请输入合法的身份证号码");
                return false;
            }
            else if (registerPhone == null || registerPhone == "" || mobileReg.test(registerPhone) == false) {
                alert("请输入合法的手机号码");
                return false;
            }
            else if (merchantCardID == null || merchantCardID == "") {
                alert("商户卡号不能为空");
                return false;
            }
            else if (registerName == null || registerName == "") {
                alert("注册人姓名不能为空");
                return false;
            }
            else if (registerAddress == null || registerAddress == "") {
                alert("注册人地址不能为空");
                return false;
            }
            return true;
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
    
        <div id="top">
            <h1>个人信息</h1>
        </div>

        <div class="form-bak">
            <form id="Form1" method="post" runat="server" action="#" onsubmit="adaptValue();">
                <div class="left">
                    <div class="line" id="line_loginName">
                        <div class="info">
                            <strong>商户名</strong>
                        </div>
                        <div class="input">
                              <asp:TextBox type="text" ID="_loginName" runat="server" 
                                  placeholder="商户名"></asp:TextBox>             
                            <div class="none"><span></span></div>
                        </div>
                    </div>

                    <div class="line" id="line_passwd">
                        <div class="info">
                            <strong>商户密码</strong>
                        </div>
                        <div class="input">
                            <asp:TextBox type="text" ID="_passwd" runat="server" 
                                placeholder="商户密码"></asp:TextBox>                     
                            <div class="none"><span></span></div>
                        </div>
                    </div>

                    <div class="line" id="line_company">
                        <div class="info">
                            <strong>商户所属公司</strong>
                        </div>
                        <div class="input">
                            <asp:TextBox ID="_company" runat="server" 
                                tooltipText="公司所在地详细地址" placeholder="商户所属公司"></asp:TextBox>                     
                            <div class="none"><span></span></div>
                        </div>
                    </div>

                    <div class="line" id="line_merchantCardID">
                        <div class="info">
                            <strong>商户卡号</strong>
                            <span class="tips"></span>
                        </div>
                        <div class="input">
                             <asp:TextBox ID="_merchantCardID" runat="server" 
                                readonly="true" placeholder="商户卡号"></asp:TextBox>                     
                             <div class="none"><span></span></div>
                        </div>
                    </div>
                </div>

                <div class="left">
                    <div class="line" id="line_registerName">
                        <div class="info">
                            <strong>注册人姓名</strong>
                        </div>
                        <div class="input">
                            <asp:TextBox ID="_registerName" runat="server" 
                                 placeholder="注册人姓名"></asp:TextBox>

                            <div class="none"><span></span></div>
                        </div>
                    </div>

                    <div class="line" id="line_registerID">
                        <div class="info">
                            <strong>注册人证件号</strong>
                        </div>
                        <div class="input">
                            <asp:TextBox ID="_registerID" runat="server" onkeyup="checkRate(event,this)"
                                tooltipText="注册人身份证号" placeholder="注册人证件号"></asp:TextBox>

                            <div class="none"><span></span></div>
                        </div>
                    </div>

                    <div class="line" id="line_registerPhone">
                        <div class="info">
                            <strong>注册人电话</strong>
                        </div>
                        <div class="input">
                            <asp:TextBox onkeyup="this.value=this.value.replace(/\D/g,'')" onafterpaste="this.value=this.value.replace(/\D/g,'')" 
                                ID="_registerPhone" runat="server" placeholder="注册人电话"></asp:TextBox> 
                         
                            <div class="none"><span></span></div>
                        </div>
                    </div>
                    <div class="line" id="line_registerAddress">
                        <div class="info">
                            <strong>注册人地址</strong>
                        </div>
                        <div class="input">
                            <asp:TextBox ID="_registerAddress" runat="server" 
                                tooltipText="注册人所在地地址"  placeholder="注册人地址"></asp:TextBox>                    
                            <div class="none"><span></span></div>
                        </div>
                    </div>

                </div>

                <div>
                     <asp:Button ID="edit_button" type="button" class="ui_input_btn01" style="margin-left:20%" runat="server" Text="编辑" onclick="edit_button_Click"/>
                     <asp:Button ID="update_button" type="button" class="ui_input_btn01" style="margin-left:20%" runat="server" Text="更新" onclick="update_button_Click" />
                     <asp:Button ID="cacle_button" type="button" class="ui_input_btn01" style="margin-left:20%" runat="server" Text="取消" onclick="cacle_button_Click" />
                </div>
            </form>
        </div>

      <script type="text/javascript" charset="gb2312">
          var tooltipObj = new DHTMLgoodies_formTooltip();
          tooltipObj.setTooltipPosition('below');
          tooltipObj.setTooltipCornerSize(15);
          tooltipObj.initFormFieldTooltip();
     </script>
</body>
</html>
