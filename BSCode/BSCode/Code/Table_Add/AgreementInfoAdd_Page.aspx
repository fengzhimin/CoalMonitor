<%@ Page Language="C#"  AutoEventWireup="true" CodeBehind="AgreementInfoAdd_Page.aspx.cs" Inherits="BSCode.Code.Table_Add.AgreementInfoAdd_Page" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <title>合同信息增加界面</title>
      <%--编辑弹出提示--%>
    <link rel="stylesheet" href="../../CSS/form-field-tooltip.css" type="text/css"/>
	<script type="text/javascript" src="../../Scripts/trip/rounded-corners.js" charset="gb2312"></script>
	<script type="text/javascript" src="../../Scripts/trip/form-field-tooltip.js" charset="gb2312"></script>

    <link href="../../CSS/authority/common_style.css" rel="stylesheet" type="text/css" />
    <link href="../../CSS/authority/basic_layout.css" rel="stylesheet" type="text/css" />

    <link href="../../CSS/from.css" rel="stylesheet" type="text/css"/>
    <script type="text/javascript" src="../../Scripts/jquery-1.8.3.min.js"></script>

    <link type="text/css" href="../../CSS/jquery-ui.css" rel="stylesheet" />
    <link href="../../CSS/jquery-ui-timepicker-addon.css" type="text/css" rel="stylesheet" />
    <link href="../../CSS/demos.css" type="text/css" rel="stylesheet"/>

    <script src="../../Scripts/jquery-1.8.2.min.js" type="text/javascript"></script>
    <script type="text/javascript" src="../../Scripts/jquery-ui.min.js"></script>
    <script src="../../Scripts/jquery-ui-timepicker-addon.js" type="text/javascript"></script>

     <%--时间--%>
    <script type="text/javascript" src="../../Scripts/date/laydate.js"></script>
    
    <script type="text/javascript">
     
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

        function checkStatue(event, obj) {
            //响应鼠标事件，允许左右方向键移动 
            event = window.event || event;
            if (event.keyCode == 37 | event.keyCode == 39) {
                return;
            }
            //先把非数字的都替换掉，除了数字 
            obj.value = obj.value.replace(/[^(0|1)]/g, "");
        }

    </script>
</head>

<body>
        <div id="top">
            <h1>增加合同信息</h1>
        </div>

        <div class="form-bak">
            <form method="post" runat="server" action="#" onsubmit="adaptValue();">
                <div class="left">
                    <div class="line" id="line_agreementNo">
                        <div class="info">
                            <strong>合同编号</strong>                      
                        </div>
                        <div class="input">                   
                             <input type="text" name="_agreementNo" runat="server" 
                                 readonly="true" id="_agreementNo" placeholder="合同编号"/>             
                            <div class="none"><span></span></div>
                        </div>
                    </div>

                    <div class="line" id="line_merchantCardID">
                        <div class="info">
                            <strong>商户卡号</strong>
                        </div>
                        <div class="input">
                            <input type="text" name="_merchantCardID" runat="server" 
                                id="_merchantCardID" readonly="true" placeholder="商户卡号"/>   
                            <div class="none"><span></span></div>
                        </div>
                    </div>

                    <div class="line" id="line_signDate">
                        <div class="info">
                            <strong>合同签约日期</strong>
                        </div>
                        <div class="input">
                       
                             <input type="text" class="inline laydate-icon" name="_signDate" runat="server" 
                                 id="_signDate"  placeholder="合同签约日期"/> 
                                               
                            <div class="none"><span></span></div>
                        </div>
                    </div>

                    <div class="line" id="line_validatyPeriod">
                        <div class="info">
                            <strong>合同有效期</strong>
                        </div>
                        <div class="input">
                         
                             <input type="text" class="inline laydate-icon" name="_validatyPeriod" 
                                runat ="server" id="_validatyPeriod" placeholder="合同有效期"/>                    
                             <div class="none"><span></span></div>
                        </div>
                    </div>

                    <div class="line" id="line_coalTon">
                        <div class="info">
                            <strong>计划运煤吨数</strong>
                        </div>
                        <div class="input">
                            <input type="text" name="_coalTon" onkeyup="clearNoNum(event,this)" onblur="checkNum(this)"
                                id="_coalTon" runat="server" tooltipText="输入数字 可为小数" placeholder="计划运煤吨数"/>                    
                            <div class="none"><span></span></div>
                        </div>
                    </div>
                </div>

                <div class="left">
                    <div class="line" id="line_carCardCount">
                        <div class="info">
                            <strong>车卡数量</strong>          
                        </div>
                        <div class="input">
                        
                            <input type="text" name="_carCardCount" 
                              runat="server" id="_carCardCount" onkeyup="checkRate(event,this)"
                                tooltipText="输入数字 只能为整数" placeholder="车卡数量"/>                    
                            <div class="none"><span></span></div>
                        </div>
                    </div>

                    <div class="line" id="line_useCarCardCount">
                        <div class="info">
                            <strong>已领取车卡数</strong>
                        </div>
                        <div class="input">
                          
                            <input type="text" name="_useCarCardCount" runat="server" 
                                id="_useCarCardCount" onkeyup="checkRate(event,this)"
                                tooltipText="输入数字 只能为整数" placeholder="已领取车卡数"/>                    
                            <div class="none"><span></span></div>
                        </div>
                    </div>

                    <div class="line" id="line_actualCoalTon">
                        <div class="info">
                            <strong>实际运煤吨数</strong>
                        </div>
                        <div class="input">
                            <input type="text" name="_actualCoalTon" runat="server" id="_actualCoalTon" 
                                onkeyup="clearNoNum(event,this)" onblur="checkNum(this)" tooltipText="输入数字 可为小数" placeholder="实际运煤吨数"/>                    
                            <div class="none"><span></span></div>
                        </div>
                    </div>
                    <div class="line" id="line_returnCarCardCount">
                        <div class="info">
                            <strong>退卡数量</strong>
                            <span class="tips">输入数字</span>
                        </div>
                        <div class="input">     
                            <input type="text" name="_returnCarCardCount" runat="server" 
                                onkeyup="checkRate(event,this)" tooltipText="输入数字 只能为整数" id="_returnCarCardCount"  placeholder="退卡数量"/>                                  
                            <div class="none"><span></span></div>
                        </div>
                    </div>
                    <div class="line" id="line_agreementStatus">
                        <div class="info">
                            <strong>合同状态</strong>
                        </div>
                        <div class="input">
                            <input type="text" maxlength="1" name="_agreementStatus" runat="server" onkeyup="checkStatue(event,this)" tooltipText="只能输入0或者1，0代表未完成，1代表完成"
                                 id="_agreementStatus"   placeholder="合同状态"/>                             
                            <div class="none"><span></span></div>
                        </div>
                    </div>
                </div>
                <div >     
                    <asp:button  style="margin-left:40%;" id="add_button"  class="ui_input_btn01" runat="server" Text="添加" onclick="add_button_Click"></asp:button>
                    <input class="ui_input_btn01" value="重置" style="margin-right:40%;" runat="server" onclick="AgreementInfoResetVerify()"></input>&nbsp;
                </div>
            </form>
        </div>

      <script type="text/javascript" charset="gb2312">
          var tooltipObj = new DHTMLgoodies_formTooltip();
          tooltipObj.setTooltipPosition('right');
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
