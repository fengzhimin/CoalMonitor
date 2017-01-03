using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using BSCode.DataBaseOperation;
using System.Data.SqlClient;
using System.Data;
using BSCode.DataBaseEntity;
using System.Windows;
using System.IO;

namespace BSCode.Code.Table_List
{
    public partial class MerchantInfoList_Page : System.Web.UI.Page
    {
        OperateMerchantInfo m_OperateMerchantInfo = new OperateMerchantInfo();
        DataSet m_allRecord = new DataSet();
        DataSet m_searchRecord = new DataSet();
        protected void Page_Load(object sender, EventArgs e)
        {
            if (!IsPostBack)
            {
                string Power = Session["Power"].ToString();
                if (Power == "3")
                {
                    this.GridViewID.Columns[12].Visible = false;
                }
                bind();
            } 
        }


        //删除
        protected void GridViewID_RowDeleting(object sender, GridViewDeleteEventArgs e)
        {
            OperateMerchantInfo _OperateMerchantInfo = new OperateMerchantInfo();
            _OperateMerchantInfo.DeleteByMerchantCardID(GridViewID.DataKeys[e.RowIndex]["merchantCardID"].ToString());
            bind();
        }



        //着色
        protected void GridViewID_RowDataBound(object sender, GridViewRowEventArgs e)
        {
            int i;
            //执行循环，保证每条数据都可以更新
            for (i = -1; i < GridViewID.Rows.Count; i++)
            {
                //首先判断是否是数据行
                if (e.Row.RowType == DataControlRowType.DataRow)
                {
                    //当鼠标停留时更改背景色
                    e.Row.Attributes.Add("onmouseover", "c=this.style.backgroundColor;this.style.backgroundColor='#00A9FF'");
                    //for (int n = 0; n < e.Row.Cells.Count; n++)
                    //{            //定义一个string类型变量用来存放每个单元格的内容            
                    //    var temp = e.Row.Cells[i].Text;
                    //    //设置title为GridView的HeadText            
                    //    e.Row.Cells[i].Attributes.Add("title", temp);//未截取长度                //判定temp的长度,            
                    //    if (temp.Length > 10)
                    //    {              //截取字符串              
                    //        temp = temp.Substring(0, 9) + "...";
                    //    }
                    //}  
                    //当鼠标移开时还原背景色
                    e.Row.Attributes.Add("onmouseout", "this.style.backgroundColor=c");
                }
            }
           

        }
        protected void GridView1_PageIndexChanging(object sender, GridViewPageEventArgs e)
        {
            CheckBox_Select.Checked = false;
            // 得到该控件 
            GridView theGrid = sender as GridView;
            int newPageIndex = 0;
            if (e.NewPageIndex == -3)
            {
                //点击了Go按钮 
                TextBox txtNewPageIndex = null;
                //GridView较DataGrid提供了更多的API，获取分页块可以使用BottomPagerRow 或者TopPagerRow，当然还增加了HeaderRow和FooterRow 
                GridViewRow pagerRow = theGrid.BottomPagerRow;
                if (pagerRow != null)
                {
                    //得到text控件 
                    txtNewPageIndex = pagerRow.FindControl("txtNewPageIndex") as TextBox;
                }
                if (txtNewPageIndex != null)
                {
                    //得到索引 
                    newPageIndex = int.Parse(txtNewPageIndex.Text) - 1;
                }
            }
            else
            {
                //点击了其他的按钮 
                newPageIndex = e.NewPageIndex;
            }
            //防止新索引溢出 
            newPageIndex = newPageIndex < 0 ? 0 : newPageIndex;
            newPageIndex = newPageIndex >= theGrid.PageCount ? theGrid.PageCount - 1 : newPageIndex;
            //得到新的值 
            theGrid.PageIndex = newPageIndex;
            //重新绑定 
            bind();
            //GridViewID.PageIndex = e.NewPageIndex;
            //GridViewID.DataBind(); 
        }
        protected void CheckBox_Selectall(object sender, EventArgs e)
        {
            int count = GridViewID.Rows.Count;
            CheckBox cbox;
            if (CheckBox_Select.Checked == true)
            {
                for (int i = 0; i < count; i++)
                {
                    cbox = (CheckBox)GridViewID.Rows[i].FindControl("CheckBox_Select");
                    cbox.Checked = true;
                }
            }
            else
            {
                for (int i = 0; i < count; i++)
                {
                    cbox = (CheckBox)GridViewID.Rows[i].FindControl("CheckBox_Select");
                    cbox.Checked = false;
                }
            }
            
        }
        protected void Button_Cancel(object sender, EventArgs e)
        {
            CheckBox_Select.Checked = false;
            for (int i = 0; i <= GridViewID.Rows.Count - 1; i++)
            {
                CheckBox cbox = (CheckBox)GridViewID.Rows[i].FindControl("CheckBox_Select");
                cbox.Checked = false;
            }
        }
        protected void Button_Mass(object sender, EventArgs e)
        {
            OperateMerchantInfo _OperateMerchantInfo = new OperateMerchantInfo();
            for (int i = 0; i <= GridViewID.Rows.Count - 1; i++)
            {
                CheckBox cbox = (CheckBox)GridViewID.Rows[i].FindControl("CheckBox_Select");
                if (cbox.Checked == true)
                {

                    _OperateMerchantInfo.DeleteByMerchantCardID(GridViewID.DataKeys[i]["merchantCardID"].ToString());
                }
            }
            bind();
        }


        protected void Export_Excel_Click(object sender, EventArgs e)
        {
            GridViewID.AllowPaging = false; //清除分页   
            //将编辑和删除按钮隐藏
            GridViewID.Columns[0].Visible = false;
            GridViewID.Columns[13].Visible = false;
            GridViewID.Columns[12].Visible = false;


            bind();  //你绑定gridview1数据源的那个函数。
            GridViewToExcel(GridViewID, "application/ms-excel", "tb_AgreementInfo.xls");
            GridViewID.AllowSorting = true; //恢复分页  
            GridViewID.Columns[0].Visible = true;
            GridViewID.Columns[13].Visible = true;
            GridViewID.Columns[12].Visible = true;

            bind(); //再次绑定
        }


        public static void GridViewToExcel(Control ctrl, string FileType, string FileName)
        {
            HttpContext.Current.Response.Charset = "GB2312";
            HttpContext.Current.Response.ContentEncoding = System.Text.Encoding.UTF8;//注意编码
            HttpContext.Current.Response.AppendHeader("Content-Disposition",
                "attachment;filename=" + HttpUtility.UrlEncode(FileName, System.Text.Encoding.UTF8).ToString());
            HttpContext.Current.Response.ContentType = FileType;//image/JPEG;text/HTML;image/GIF;vnd.ms-excel/msword 
            ctrl.Page.EnableViewState = false;
            StringWriter tw = new StringWriter();
            HtmlTextWriter hw = new HtmlTextWriter(tw);
            ctrl.RenderControl(hw);
            HttpContext.Current.Response.Write(tw.ToString());
            HttpContext.Current.Response.End();
        }

        public override void VerifyRenderingInServerForm(Control control)
        {

        }

        protected void search_button_Click(object sender, EventArgs e)
        {
            bind();
        }


        //绑定数据
        public void bind()
        {
            
            m_allRecord = m_OperateMerchantInfo.getAllMerchantInfo();
            string _search_text = search_text.Text.ToString();
            string _start_time = start_time.Text.ToString();
            string _end_time = end_time.Text.ToString();
            string _search_DropDownList = search_DropDownList.SelectedValue;
            string _SQL = "select * from tb_MerchantInfo";

            if (_search_text == "" && _start_time == "")
            {
                GridViewID.DataSource = m_allRecord;
            }
            else
            {
                if (_search_text != "")
                {
                    if (_search_DropDownList == "商户名")
                        _SQL += " where loginName";
                    else
                        _SQL += " where merchantCardID";
                    if (search_option.Checked)
                        _SQL += " like '%" + _search_text + "%'";
                    else
                        _SQL += " = '" + _search_text + "'";

                    if (_start_time != "")
                    {
                        _SQL += " and registerDate >= '" + _start_time + "'";
                        if (_end_time != "")
                            _SQL += " and registerDate<= '" + _end_time + "'";
                    }
                }
                else
                {
                    if (_start_time != "")
                    {
                        _SQL += " where registerDate >= '" + _start_time + "'";
                        if (_end_time != "")
                            _SQL += " and registerDate <= '" + _end_time + "'";
                    }
                }

                try
                {
                    SqlDataAdapter _da = new SqlDataAdapter(_SQL, m_OperateMerchantInfo.sqlcon);
                    _da.Fill(m_searchRecord);
                }
                catch (Exception ee)
                {
                    throw new Exception(ee.Message);
                }

                GridViewID.DataSource = m_searchRecord;
            }
            GridViewID.DataKeyNames = new string[] { "ID", "merchantCardID" };//主键
            GridViewID.DataBind();
        }
    }
}