<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="Login_Page.aspx.cs" Inherits="BSCode.Code.Login_Page" %>

<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
        <title>SCT-后台系统</title>
        <link href="../CSS/login_css.css" rel="stylesheet" type="text/css" />
        <link href="../CSS/input_css/component.css" rel="stylesheet" type="text/css" />
        <link href="../CSS/input_css/demo.css" rel="stylesheet" type="text/css" />
        <link href="../CSS/input_css/normalize.css" rel="stylesheet" type="text/css" />
        <script type="text/javascript" src="../Scripts/jquery/jquery-1.7.1.js"></script>
    </head>

    <body>
	    <div id="login_center">
		    <div id="login_area">
			    <div id="login_box">
				    <div id="login_form">
					    <form id="submitForm" runat="server">
						    <div id="login_tip">
							    <span id="login_err" class="sty_txt2"></span>
						    </div>
                            <div class="content">
                                <span class="input input--kaede">
					                <input runat="server" class="input__field input__field--kaede" type="text" id="username" />
					                <label class="input__label input__label--kaede" for="username">
						                <span class="input__label-content input__label-content--kaede">用户名</span>
					                </label>
				                </span>
				                <span class="input input--kaede">
					                <input class="input__field input__field--kaede" runat="server" type="password" id="pwd" />
					                <label class="input__label input__label--kaede" for="pwd">
						                <span class="input__label-content input__label-content--kaede">密码</span>
					                </label>
				                </span>  
						    </div>
                            <div>
                                <asp:Label ID="AlertLb" runat="server" ForeColor="#ff0000"></asp:Label>
                            </div>
						    <div id="btn_area">
							    <input type="button" class="login_btn" id="login_sub" value="登  录" runat="server" onserverclick="Login_Click">
							    <input type="reset" class="login_btn" id="login_ret" value="重 置">
						    </div>
					    </form>
				    </div>
			    </div>
		    </div>
	    </div>
    </body>
</html>