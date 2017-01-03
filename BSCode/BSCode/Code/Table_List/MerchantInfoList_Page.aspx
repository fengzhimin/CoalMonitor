<%@ Page Language="C#" EnableEventValidation = "false" AutoEventWireup="true" CodeBehind="MerchantInfoList_Page.aspx.cs" Inherits="BSCode.Code.Table_List.MerchantInfoList_Page" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <title>商户数据表操作</title>
    <!--时间-->
    <link type="text/css" href="../../CSS/jquery-ui.css" rel="stylesheet" />
    <link href="../../CSS/jquery-ui-timepicker-addon.css" type="text/css" rel="stylesheet" />
    <link href="../../CSS/demos.css" type="text/css" rel="stylesheet"/>
    <link href="../../CSS/authority/common_style.css" rel="stylesheet" type="text/css" />
    <script src="../../Scripts/jquery-1.8.2.min.js" type="text/javascript"></script>
    <script type="text/javascript" src="../../Scripts/jquery-ui.min.js"></script>
    <script src="../../Scripts/jquery-ui-timepicker-addon.js" type="text/javascript"></script>
      <%--弹出淡入窗口--%>
    <script type="text/javascript" src="../../Scripts/jquery/jquery-1.7.1.js"></script>
    <script type="text/javascript" src="../../Scripts/fancybox/jquery.fancybox-1.3.4.js"></script>
    <link rel="stylesheet" type="text/css" href="../../CSS/authority/jquery.fancybox-1.3.4.css" media="screen"/>

     <%--时间--%>
    <script type="text/javascript" src="../../Scripts/date/laydate.js"></script>
    
    <script type="text/javascript">
        $(document).ready(function () {
            /**编辑   **/
            $("a#edit_link").fancybox({
                'width': 768,
                'height': 360,
                'type': 'iframe',
                'hideOnOverlayClick': false,
                'showCloseButton': false,
                'onClosed': function () {
                    window.location.href = '../Table_List/MerchantInfoList_Page.aspx';
                }
            });
        });
    </script>
    <style type="text/css">
        .Maxlength {
            display: block;
            width: 100px;
            overflow: hidden;
            white-space: nowrap;
            -o-text-overflow: ellipsis;
            text-overflow: ellipsis;
        }
        .Middlelength {
            display: block;
            width: 70px;
            overflow: hidden;
            white-space: nowrap;
            -o-text-overflow: ellipsis;
            text-overflow: ellipsis;
        }
        .Minlength {
            display: block;
            width: 50px;
            overflow: hidden;
            white-space: nowrap;
            -o-text-overflow: ellipsis;
            text-overflow: ellipsis;
        }
    </style>


</head>

<body>
    <form id="form1" runat="server">

        <asp:Label ID="Label1" runat="server" Text="搜索类型"></asp:Label>
        <asp:DropDownList ID="search_DropDownList" class="ui_select01" runat="server">
            <asp:ListItem>商户名</asp:ListItem>
            <asp:ListItem>商户卡号</asp:ListItem>
        </asp:DropDownList>
        <asp:TextBox ID="search_text" runat="server" width="20%" class="ui_input_txt03" placeholder="输入搜索内容"></asp:TextBox>
        <asp:TextBox ID="start_time" runat="server" class="inline laydate-icon" placeholder="输入注册起始日期"></asp:TextBox>
         <asp:TextBox ID="end_time" runat="server" class="inline laydate-icon" placeholder="输入注册终止日期"></asp:TextBox>
        <asp:Button ID="search_button" runat="server" class="ui_input_btn01" Text="搜索" OnClick="search_button_Click" />
        <asp:Label ID="Label2" runat="server" Text="模糊搜索"></asp:Label>
        <input type="checkbox" id="search_option" runat="server" class="search_option_checkBox" /><label for="search_option"></label>

        <div style="overflow-y: scroll; height: 90%; width: 100%" id="dvBody">
            <asp:GridView ID="GridViewID" width="100%" runat="server" AllowPaging="true" PageSize="10" AutoGenerateColumns="False" CellPadding="4"
                ForeColor="#333333" GridLines="both" OnRowDeleting="GridViewID_RowDeleting"  
                OnRowDataBound="GridViewID_RowDataBound" OnPageIndexChanging="GridView1_PageIndexChanging">
                <RowStyle BackColor="#FFFBD6" ForeColor="#333333" />
                <FooterStyle BackColor="#990000" Font-Bold="True" ForeColor="White" />
                <RowStyle HorizontalAlign="Center" VerticalAlign="Middle" />
                <PagerStyle CssClass="gvPagerCss" BackColor="#FFCC66" ForeColor="#333333" HorizontalAlign="Center" />

                <Columns>
                    <asp:TemplateField>
                        <ItemTemplate>
                            <asp:CheckBox ID="CheckBox_Select" runat="server" />
                        </ItemTemplate>
                    </asp:TemplateField>


                    <asp:TemplateField HeaderText="商户名">
                        <ItemTemplate>
                           <asp:Label ID="Labe_name" runat="server" Text='<%# Eval("loginName") %>' CssClass="Minlength" title='<%# Eval("loginName")%>'></asp:Label> 
                            </ItemTemplate>
                     </asp:TemplateField>
                    <asp:TemplateField HeaderText="商户密码">
                        <ItemTemplate>
                           <asp:Label ID="Label_PWD" runat="server" Text='<%# Eval("passwd") %>' CssClass="Middlelength" title='<%# Eval("passwd")%>'></asp:Label> 
                            </ItemTemplate>
                     </asp:TemplateField>
                    <asp:TemplateField HeaderText="商户所属公司">
                        <ItemTemplate>
                           <asp:Label ID="Label_company" runat="server" Text='<%# Eval("company") %>' CssClass="Middlelength" title='<%# Eval("company")%>'></asp:Label> 
                            </ItemTemplate>
                     </asp:TemplateField>
                     <asp:TemplateField HeaderText="商户卡号">
                        <ItemTemplate>
                           <asp:Label ID="Label_merchantCardID" runat="server" Text='<%# Eval("merchantCardID") %>' CssClass="Middlelength" title='<%# Eval("merchantCardID")%>'></asp:Label> 
                            </ItemTemplate>
                     </asp:TemplateField>
                    <asp:TemplateField HeaderText="注册日期">
                        <ItemTemplate>
                           <asp:Label ID="Label_registerDate" runat="server" Text='<%# Eval("registerDate") %>' CssClass="Middlelength" title='<%# Eval("registerDate")%>'></asp:Label> 
                            </ItemTemplate>
                     </asp:TemplateField>
                    <asp:BoundField DataField="registerName" HeaderText="注册人姓名" />
                    <asp:TemplateField HeaderText="注册人证件号">
                        <ItemTemplate>
                           <asp:Label ID="Label_registerID" runat="server" Text='<%# Eval("registerID") %>'  CssClass="Maxlength" title='<%# Eval("registerID")%>'></asp:Label> 
                            </ItemTemplate>
                     </asp:TemplateField>
                     <asp:TemplateField HeaderText="注册人电话">
                        <ItemTemplate>
                           <asp:Label ID="Label_registerPhone" runat="server" Text='<%# Eval("registerPhone") %>' CssClass="Middlelength" title='<%# Eval("registerPhone")%>'></asp:Label> 
                            </ItemTemplate>
                     </asp:TemplateField>
                     <asp:TemplateField HeaderText="注册人地址">
                        <ItemTemplate>
                           <asp:Label ID="Label_registerAddress" runat="server" Text='<%# Eval("registerAddress") %>' CssClass="Middlelength" title='<%# Eval("registerAddress")%>'></asp:Label> 
                            </ItemTemplate>
                     </asp:TemplateField>
                    <asp:BoundField DataField="loginNumber" HeaderText="登录次数" />
                    <asp:TemplateField HeaderText="最近登录时间">
                        <ItemTemplate>
                           <asp:Label ID="Label_lastLoginTime" runat="server" Text='<%# Eval("lastLoginTime") %>' CssClass="Middlelength" title='<%# Eval("lastLoginTime")%>'></asp:Label> 
                            </ItemTemplate>
                     </asp:TemplateField>
               
                    <asp:TemplateField HeaderText="添加合同">
                        <ItemTemplate>
                            <a href="../Table_Add/AgreementInfoAdd_Page.aspx?MerchantCardID=<%#DataBinder.Eval(Container.DataItem, "MerchantCardID")%>">添加合同</a>
                        </ItemTemplate>
                    </asp:TemplateField>

                    <asp:TemplateField HeaderText="编辑">
                        <ItemTemplate>
                            <a href="../Table_Edit/MerchantInfoEdit_Page.aspx?ID=<%#DataBinder.Eval(Container.DataItem, "ID")%>" id="edit_link">编辑</a>
                        </ItemTemplate>
                    </asp:TemplateField>
                    <asp:TemplateField  HeaderText="删除">
                        <ItemTemplate>
                            <asp:LinkButton ID="LinkButton_delete" runat="server" CausesValidation="False"
                                CommandName="Delete" Text="删除" OnClientClick="javascript:return confirm('真的要删除吗？');"></asp:LinkButton>
                        </ItemTemplate>
                    </asp:TemplateField>
                </Columns>

                <PagerTemplate> 
                    当前第: 
                    <asp:Label ID="LabelCurrentPage" runat="server" Text="<%# ((GridView)Container.NamingContainer).PageIndex + 1 %>"></asp:Label>
                    页/共: 
                    <asp:Label ID="LabelPageCount" runat="server" Text="<%# ((GridView)Container.NamingContainer).PageCount %>"></asp:Label>
                    页
                    <asp:Button ID="LinkButtonFirstPage" class="ui_input_btn01" Text="首页" runat="server" CommandArgument="First" CommandName="Page"
                        Visible='<%#((GridView)Container.NamingContainer).PageIndex != 0 %>'></asp:Button>
                    <asp:Button ID="LinkButtonPreviousPage" class="ui_input_btn01" Text="上一页" runat="server" CommandArgument="Prev"
                        CommandName="Page" Visible='<%# ((GridView)Container.NamingContainer).PageIndex != 0 %>'></asp:Button>
                    <asp:Button ID="LinkButtonNextPage" runat="server" class="ui_input_btn01" Text="下一页" CommandArgument="Next" CommandName="Page"
                        Visible='<%# ((GridView)Container.NamingContainer).PageIndex != ((GridView)Container.NamingContainer).PageCount - 1 %>'></asp:Button>
                    <asp:Button ID="LinkButtonLastPage" runat="server" class="ui_input_btn01" Text="尾页" CommandArgument="Last" CommandName="Page"
                        Visible='<%# ((GridView)Container.NamingContainer).PageIndex != ((GridView)Container.NamingContainer).PageCount - 1 %>'></asp:Button>
                    转到第 
                    <asp:TextBox ID="txtNewPageIndex" runat="server" class="ui_input_txt01" Width="20px" Text='<%# ((GridView)Container.Parent.Parent).PageIndex + 1 %>' />页 
                    <asp:Button ID="btnGo" runat="server" class="ui_input_btn01" CausesValidation="False" CommandArgument="-2"
                        CommandName="Page" Text="跳转" />
                </PagerTemplate>

                <EditRowStyle HorizontalAlign="Center" VerticalAlign="Middle" />
                <RowStyle ForeColor="#000066" />
                <SelectedRowStyle BackColor="#669999" Font-Bold="True" ForeColor="White" />
                <PagerStyle BackColor="White" ForeColor="#000066" HorizontalAlign="Left" />
                <HeaderStyle BackColor="#006699" Font-Bold="True" ForeColor="White" CssClass="Freezing" />
            </asp:GridView>
            <asp:CheckBox ID="CheckBox_Select" runat="server" AutoPostBack="True" Font-Size="9pt" OnCheckedChanged="CheckBox_Selectall"
                        Text="全选" />
                    <asp:Button ID="Button1" runat="server" class="ui_input_btn01" Text="取消" OnClick="Button_Cancel" />
                    <asp:Button ID="Button2" runat="server" class="ui_input_btn01" Text="删除" OnClick="Button_Mass" />

        <asp:Button ID="Export_Excel" runat="server" class="ui_input_btn01" OnClick="Export_Excel_Click" Text="导出" />

        </div>

    </form>
    <script type="text/javascript">
                !function () {
                    laydate.skin('molv');//切换皮肤，请查看skins下面皮肤库
                    laydate({ elem: '#demo' });//绑定元素
                }();

                //日期范围限制
                var start_time = {
                    elem: '#start_time',
                    format: 'YYYY-MM-DD hh:mm:ss',
                    min: '2015-7-8', //设定最小日期为当前日期
                    max: '2099-06-16', //最大日期
                    istime: true,
                    istoday: false,
                    choose: function (datas) {
                        end_time.min = datas; //开始日选好后，重置结束日的最小日期
                        end_time.start = datas //将结束日的初始值设定为开始日
                    }
                };

                var end_time = {
                    elem: '#end_time',
                    format: 'YYYY-MM-DD hh:mm:ss',
                    min: '2015-7-8',
                    max: '2099-06-16',
                    istime: true,
                    istoday: false,
                    choose: function (datas) {
                        start_time.max = datas; //结束日选好后，充值开始日的最大日期
                    }
                };
                laydate(start_time);
                laydate(end_time);
    </script>


</body>
</html>
