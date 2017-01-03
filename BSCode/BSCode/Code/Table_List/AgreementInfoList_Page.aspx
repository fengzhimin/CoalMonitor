<%@ Page Language="C#" EnableEventValidation = "false" AutoEventWireup="true" CodeBehind="AgreementInfoList_Page.aspx.cs" Inherits="BSCode.Code.Table_List.AgreementInfoList_Page" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>合同数据表的操作</title>

    <script type="text/javascript" src="../../Scripts/jquery-1.8.3.min.js"></script>
    <link rel="Stylesheet" type="text/css" href="../../CSS/loginDialog.css" />

    <link type="text/css" href="../../CSS/jquery-ui.css" rel="stylesheet" />
    <link href="../../CSS/jquery-ui-timepicker-addon.css" type="text/css" rel="stylesheet" />
    <link href="../../CSS/demos.css" type="text/css" rel="stylesheet" />
    <link href="../../CSS/authority/common_style.css" rel="stylesheet" type="text/css" />
    <script src="../../Scripts/jquery-1.8.2.min.js" type="text/javascript"></script>
    <script type="text/javascript" src="../../Scripts/jquery-ui.min.js"></script>
    <script src="../../Scripts/jquery-ui-timepicker-addon.js" type="text/javascript"></script>

    <link rel="stylesheet" href="../../CSS/cxcalendar.css" />
    <%--弹出淡入窗口--%>
    <script type="text/javascript" src="../../Scripts/jquery/jquery-1.7.1.js"></script>
    <script type="text/javascript" src="../../Scripts/fancybox/jquery.fancybox-1.3.4.js"></script>
    <link rel="stylesheet" type="text/css" href="../../CSS/authority/jquery.fancybox-1.3.4.css" media="screen" />
    <%--时间--%>
    <script type="text/javascript" src="../../Scripts/date/laydate.js"></script>
   
    <script type="text/javascript">
        $(document).ready(function () {
            /**编辑   **/
            $("a#edit_link").fancybox({
                'width': 768,
                'height': 500,
                'type': 'iframe',
                'hideOnOverlayClick': false,
                'showCloseButton': false,
                'onClosed': function () {
                    window.location.href = '../Table_List/AgreementInfoList_Page.aspx';
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
    <form id="form1" method="get" runat="server">
        <div style="overflow-y: scroll; height: 90%; width: 100%" id="dvBody">

            <asp:Label ID="Label1" runat="server" Text="搜索类型"></asp:Label>

            <asp:DropDownList ID="search_DropDownList" class="ui_select01" runat="server">
                <asp:ListItem>合同编号</asp:ListItem>
                <asp:ListItem>商户卡号</asp:ListItem>
            </asp:DropDownList>

            <asp:TextBox ID="search_text" runat="server" class="ui_input_txt03" width="20%" placeholder="输入搜索内容"></asp:TextBox>
            <asp:TextBox ID="start_time" runat="server" class="inline laydate-icon" placeholder="输入合同签订日期"></asp:TextBox>
            <asp:TextBox ID="end_time" runat="server" class="inline laydate-icon" placeholder="输入合同终止时间"></asp:TextBox>
            <asp:Button ID="search_button" runat="server" class="ui_input_btn01" Text="搜索" OnClick="search_button_Click" />
            <asp:Label ID="Label2" runat="server" Text="模糊搜索"></asp:Label>
            <input type="checkbox" id="search_option" runat="server" class="search_option_checkBox" /><label for="search_option"></label>

            <asp:GridView ID="GridViewID" runat="server" Width="100%" AllowPaging="true" PageSize="10" AutoGenerateColumns="False" CellPadding="4"
                ForeColor="#333333" GridLines="both" OnRowDeleting="GridViewID_RowDeleting"
                OnRowDataBound="GridViewID_RowDataBound" OnPageIndexChanging="GridView1_PageIndexChanging" OnSelectedIndexChanged="GridViewID_SelectedIndexChanged">
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
                    <asp:TemplateField HeaderText="合同编号">
                        <ItemTemplate>
                           <asp:Label ID="Label_agreementNo" runat="server" Text='<%# Eval("agreementNo") %>' CssClass="Middlelength" title='<%# Eval("agreementNo")%>'></asp:Label> 
                            </ItemTemplate>
                     </asp:TemplateField>
                    <%--<asp:BoundField DataField="merchantCardID" HeaderText="商户卡号" />--%>
                    <asp:TemplateField HeaderText="商户卡号">
                        <ItemTemplate>
                           <asp:Label ID="Label_merchantCardID" runat="server" Text='<%# Eval("merchantCardID") %>' CssClass="Middlelength" title='<%# Eval("merchantCardID")%>'></asp:Label> 
                            </ItemTemplate>
                     </asp:TemplateField>
                    <asp:BoundField DataField="signDate" DataFormatString="{0:yyyy年M月dd日}" HtmlEncode="false" HeaderText="合同签约日期" />
                    <asp:BoundField DataField="validatyPeriod" DataFormatString="{0:yyyy年M月dd日}" HtmlEncode="false" HeaderText="合同有效期" />

                    <asp:TemplateField HeaderText="计划运煤吨数">
                        <ItemTemplate>
                           <asp:Label ID="Label_coalTon" runat="server" Text='<%# Eval("coalTon") %>' CssClass="Maxlength" title='<%# Eval("coalTon")%>'></asp:Label> 
                            </ItemTemplate>
                     </asp:TemplateField>
                    <asp:BoundField DataField="carCardCount" HeaderText="车卡数量" />
                    <asp:BoundField DataField="useCarCardCount" HeaderText="已领取车卡数" />
                     <asp:TemplateField HeaderText="实际运煤吨数">
                        <ItemTemplate>
                           <asp:Label ID="Label_actualCoalTon" runat="server" Text='<%# Eval("actualCoalTon") %>' CssClass="Maxlength" title='<%# Eval("actualCoalTon")%>'></asp:Label> 
                            </ItemTemplate>
                     </asp:TemplateField>
                    <asp:BoundField DataField="returnCarCardCount" HeaderText="退卡数量" />
                    <asp:BoundField DataField="agreementStatus" HeaderText="合同状态" />

                    <asp:TemplateField HeaderText="编辑">
                       
                        <ItemTemplate>
                            <a href="../Table_Edit/AgreementInfoEdit_Page.aspx?ID=<%#DataBinder.Eval(Container.DataItem, "ID")%>"id="edit_link">编辑</a>
                        </ItemTemplate>
                    </asp:TemplateField>

                    <asp:TemplateField HeaderText="删除">
                        <ItemTemplate>
                            <asp:LinkButton ID="LinkButton1" runat="server" CausesValidation="False"
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
                    <asp:TextBox ID="txtNewPageIndex" runat="server" Width="20px" Text='<%# ((GridView)Container.Parent.Parent).PageIndex + 1 %>' />页 
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
                    <asp:Button ID="Button1" class="ui_input_btn01" runat="server" Font-Size="9pt" Text="取消" OnClick="Button_Cancel" />
                    <asp:Button ID="Button2" class="ui_input_btn01" runat="server" Font-Size="9pt" Text="删除" OnClick="Button_Mass" />
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
