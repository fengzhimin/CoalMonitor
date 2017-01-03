<%@ Page Language="C#" EnableEventValidation="false" AutoEventWireup="true" CodeBehind="AgreementAndCarCardInfoList_Page.aspx.cs" Inherits="BSCode.Code.AgreementAndCarCardInfoList_Page" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <title></title>
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
    <link rel="stylesheet" type="text/css" href="../../CSS/authority/jquery.fancybox-1.3.4.css" media="screen" />
    <%--时间--%>
    <script type="text/javascript" src="../../Scripts/date/laydate.js"></script>
    
    <script type="text/javascript">
        $(document).ready(function () {
            /**编辑   **/
            $("a#edit_link").fancybox({
                'width': 768,
                'height': 800,
                'type': 'iframe',
                'hideOnOverlayClick': false,
                'showCloseButton': false,
                'onClosed': function () {
                    window.location.href = '../Table_List/AgreementAndCarCardInfoList_Page.aspx';
                }
            });

            /**编辑   **/
            $("a#picture_link").fancybox({
                'width': 800,
                'height': 480,
                'type': 'iframe',
                'hideOnOverlayClick': false,
                'showCloseButton': false,
                'onClosed': function () {
                    window.location.href = './AgreementAndCarCardInfoList_Page.aspx';
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
    <div style="overflow-y: scroll; height: 90%;width:100%" id="dvBody">

        <asp:Label ID="Label1" runat="server" Text="搜索类型"></asp:Label>

        <asp:DropDownList ID="search_DropDownList" class="ui_select01" runat="server">
            <asp:ListItem>合同编号</asp:ListItem>
            <asp:ListItem>商户卡编号</asp:ListItem>
        </asp:DropDownList>

        <asp:TextBox id="search_text" runat="server" width="20%" class="ui_input_txt03" placeholder="输入搜索内容"></asp:TextBox>
        <asp:TextBox id="start_time" runat="server" class="inline laydate-icon"  placeholder="输入起始时间" ></asp:TextBox>
        <asp:TextBox id="end_time" runat="server" class="inline laydate-icon" placeholder="输入终止时间"></asp:TextBox>
        <asp:Button ID="search_button" runat="server" class="ui_input_btn01" Text="搜索" onclick="search_button_Click"/>
        <asp:Label ID="Label2" runat="server" Text="模糊搜索"></asp:Label>
        <input type="checkbox" id="search_option" runat="server" class="search_option_checkBox" /><label for="search_option"></label>

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
                 <asp:TemplateField HeaderText="合同编号">
                        <ItemTemplate>
                           <asp:Label ID="Label_agreementNo" runat="server" Text='<%# Eval("agreementNo") %>' CssClass="Middlelength"  title='<%# Eval("agreementNo")%>'
></asp:Label> 

                            </ItemTemplate>
                     </asp:TemplateField>
                <asp:TemplateField HeaderText="车卡ID">
                    <ItemTemplate>
                        <asp:Label ID="Label_carCardID" runat="server" Text='<%# Eval("carCardID") %>' CssClass="Middlelength" title='<%# Eval("carCardID")%>'></asp:Label>

                    </ItemTemplate>
                </asp:TemplateField>

                <asp:TemplateField HeaderText="车牌号">
                    <ItemTemplate>
                        <asp:Label ID="Label_plateNumber" runat="server" Text='<%# Eval("plateNumber") %>' CssClass="Middlelength" title='<%# Eval("plateNumber")%>'></asp:Label>
                    </ItemTemplate>
                </asp:TemplateField>

                <asp:TemplateField HeaderText="空车体积">
                    <ItemTemplate>
                        <asp:Label ID="Label_emptyCarSize" runat="server" Text='<%# Eval("emptyCarSize") %>' CssClass="Middlelength" title='<%# Eval("emptyCarSize")%>'></asp:Label>

                    </ItemTemplate>
                     </asp:TemplateField>
                <asp:TemplateField HeaderText="煤炭体积">
                    <ItemTemplate>
                        <asp:Label ID="Label_cargoSize" runat="server" Text='<%# Eval("cargoSize") %>' CssClass="Middlelength" title='<%# Eval("cargoSize")%>'></asp:Label>

                    </ItemTemplate>
                     </asp:TemplateField>
                <asp:TemplateField HeaderText="煤车体积">
                    <ItemTemplate>
                        <asp:Label ID="Label_cargoCarSize" runat="server" Text='<%# Eval("cargoCarSize") %>' CssClass="Middlelength" title='<%# Eval("cargoCarSize")%>'></asp:Label>

                    </ItemTemplate>
                     </asp:TemplateField>
                <asp:TemplateField HeaderText="车辆形状">
                    <ItemTemplate>
                        <asp:Label ID="Label_carShapeInfo" runat="server" Text='<%# Eval("carShapeInfo") %>' CssClass="Middlelength" title='<%# Eval("carShapeInfo")%>'></asp:Label>

                    </ItemTemplate>
                     </asp:TemplateField>
                <asp:TemplateField HeaderText="空车重量">
                    <ItemTemplate>
                        <asp:Label ID="Label_emptyCarWeight" runat="server" Text='<%# Eval("emptyCarWeight") %>' CssClass="Middlelength" title='<%# Eval("emptyCarWeight")%>'></asp:Label>

                    </ItemTemplate>
                     </asp:TemplateField>
                 <asp:TemplateField HeaderText="车标重">
                    <ItemTemplate>
                        <asp:Label ID="Label_CarWeight" runat="server" Text='<%# Eval("CarWeight") %>' CssClass="Middlelength" title='<%# Eval("CarWeight")%>'></asp:Label>

                    </ItemTemplate>
                     </asp:TemplateField>
                <asp:TemplateField HeaderText="煤炭重量">
                    <ItemTemplate>
                        <asp:Label ID="Label_cargoWeight" runat="server" Text='<%# Eval("cargoWeight") %>' CssClass="Middlelength" title='<%# Eval("cargoWeight")%>'></asp:Label>

                    </ItemTemplate>
                     </asp:TemplateField>
                <asp:TemplateField HeaderText="煤炭密度">
                    <ItemTemplate>
                        <asp:Label ID="Label_cargoDensity" runat="server" Text='<%# Eval("cargoDensity") %>' CssClass="Middlelength" title='<%# Eval("cargoDensity")%>'></asp:Label>

                    </ItemTemplate>
                     </asp:TemplateField>
                <asp:TemplateField HeaderText="扣重">
                    <ItemTemplate>
                        <asp:Label ID="Label_deductWeight" runat="server" Text='<%# Eval("deductWeight") %>' title='<%# Eval("deductWeight")%>'></asp:Label>
                    </ItemTemplate>
                     </asp:TemplateField>
                <asp:TemplateField HeaderText="拉货地点">
                        <ItemTemplate>
                           <asp:Label ID="Label_weighPlace" runat="server" 
                               Text='<%# Eval("weighPlace") %>' CssClass="Middlelength" title='<%# Eval("weighPlace")%>'></asp:Label> 
                            </ItemTemplate>
                     </asp:TemplateField>
                <asp:TemplateField HeaderText="是否违规">
                        <ItemTemplate>
                           <asp:Label ID="Label_abnormal" runat="server" 
                               Text='<%# Eval("abnormal") %>' CssClass="Middlelength" title='<%# Eval("abnormal")%>'></asp:Label> 
                            </ItemTemplate>
                     </asp:TemplateField>

                <asp:TemplateField HeaderText="inOut">
                    <EditItemTemplate>
                        <asp:DropDownList ID="DropDownList_inOut" runat="server" Width="151px">
                        </asp:DropDownList>
                    </EditItemTemplate>
                    <ItemTemplate>
                        <asp:Label ID="Label_inOut" runat="server" Text='<%# Eval("inOut") %>' CssClass="Middlelength"></asp:Label>
                    </ItemTemplate>
                </asp:TemplateField>

                 <asp:TemplateField HeaderText="刷卡时间">
                    <EditItemTemplate>
                        <asp:TextBox ID="record_time" runat="server">
                        </asp:TextBox>
                    </EditItemTemplate>
                    <ItemTemplate>
                        <asp:Label ID="Label_recordTime" runat="server" Text='<%# Eval("recordTime") %>' CssClass="Middlelength" title='<%# Eval("recordTime")%>'></asp:Label>
                    </ItemTemplate>
                </asp:TemplateField>
                <asp:TemplateField HeaderText="进入时间">
                    <EditItemTemplate>
                        <asp:TextBox ID="firstTime" runat="server">
                        </asp:TextBox>
                    </EditItemTemplate>
                    <ItemTemplate>
                        <asp:Label ID="Label_firstTime" runat="server" Text='<%# Eval("firstTime") %>' CssClass="Middlelength" title='<%# Eval("firstTime")%>'></asp:Label>
                    </ItemTemplate>
                </asp:TemplateField>
                 <asp:TemplateField HeaderText="离开时间">
                    <EditItemTemplate>
                        <asp:TextBox ID="lastTime" runat="server">
                        </asp:TextBox>
                    </EditItemTemplate>
                    <ItemTemplate>
                        <asp:Label ID="Label_lastTime" runat="server" Text='<%# Eval("lastTime") %>' CssClass="Middlelength" title='<%# Eval("lastTime")%>'></asp:Label>
                    </ItemTemplate>
                </asp:TemplateField>
                 <asp:TemplateField HeaderText="查看车照">
                    <ItemTemplate>
                         <a href="../ShowCarPicture_Page.aspx?carCardID=<%#DataBinder.Eval(Container.DataItem,"carCardID")%>&weighPlace=<%#DataBinder.Eval(Container.DataItem,"weighPlace")%>&inOut=<%#DataBinder.Eval(Container.DataItem,"inOut")%>" id="picture_link">查看</a>
                    </ItemTemplate>
                </asp:TemplateField>

                <asp:TemplateField HeaderText="编辑">
                    <ItemTemplate>
                        <a href="../Table_Edit/AgreementAndCarCardInfoEdit_Page.aspx?ID=<%#DataBinder.Eval(Container.DataItem, "ID")%>" id="edit_link">编辑</a>
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
                <asp:TextBox ID="txtNewPageIndex" runat="server" class="ui_input_txt01" Width="20px" Text='<%# ((GridView)Container.Parent.Parent).PageIndex + 1 %>' />页 
                <asp:Button ID="btnGo" runat="server" class="ui_input_btn01" CausesValidation="False" CommandArgument="-2"
                    CommandName="Page" Text="跳转" />
            </PagerTemplate>

            <RowStyle ForeColor="#000066" BackColor="#FFFFCC" BorderColor="Blue" BorderWidth="3px" Width="20%" HorizontalAlign="Center" BorderStyle="Groove" />
            <SelectedRowStyle BackColor="#669999" Font-Bold="True" ForeColor="White" />
            <PagerStyle BackColor="White" ForeColor="#000066" HorizontalAlign="Left" />
            <HeaderStyle BackColor="#006699" Font-Bold="True" ForeColor="White" BorderStyle="Double" BorderWidth="3px" />
        </asp:GridView>

        <asp:CheckBox ID="CheckBox_SelectA" runat="server" AutoPostBack="True" Font-Size="9pt" OnCheckedChanged="CheckBox_Selectall"
                        Text="全选" />
                    <asp:Button ID="Button1" runat="server" class="ui_input_btn01" Font-Size="9pt" Text="取消" OnClick="Button_Cancel" />
                    <asp:Button ID="Button2" runat="server" class="ui_input_btn01" Font-Size="9pt" Text="删除" OnClick="Button_Mass" />
        <asp:CheckBox ID="Car_CheckBox" runat="server" AutoPostBack="True" Font-Size="12px"
             OnCheckedChanged="Car_CheckBox_CheckedChanged" Text="显示隐藏车辆属性" />
        <asp:CheckBox ID="Cargo_CheckBox" runat="server" AutoPostBack="True" Font-Size="12px"
             OnCheckedChanged="Cargo_CheckBox_CheckedChanged" Text="显示隐藏煤炭属性" />
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
