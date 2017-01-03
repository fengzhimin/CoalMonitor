using BSCode.DataBaseEntity;
using BSCode.DataBaseOperation;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Data.SqlClient;
using System.Data;

namespace BSCode.Code.Table_Add
{
    public partial class MerchantInfoAdd_Page : System.Web.UI.Page
    {
        tb_MerchantInfo m_tb_MerchantInfo = new tb_MerchantInfo();
        OperateMerchantInfo m_OperateMerchantInfo = new OperateMerchantInfo();
        //int temp = 0;

        protected void Page_Load(object sender, EventArgs e)
        {
            if (!IsPostBack)
            {
                //给button1添加客户端事件  
                add_button.Attributes.Add("OnClick", "return MerchantInfoAddVerify()");
                _merchantCardID.Text = m_OperateMerchantInfo.getMaxMerchant();
            }
        }

        protected void add_button_Click(object sender, EventArgs e)
        {
            m_tb_MerchantInfo.LoginName = _loginName.Text.ToString();
            m_tb_MerchantInfo.Passwd = _passwd.Text.ToString();
            m_tb_MerchantInfo.RegisterName = _registerName.Text.ToString();
            m_tb_MerchantInfo.RegisterID = _registerID.Text.ToString();
            m_tb_MerchantInfo.MerchantCardID = _merchantCardID.Text.ToString();
            m_tb_MerchantInfo.Company = _company.Text.ToString();
            m_tb_MerchantInfo.RegisterPhone = _registerPhone.Text.ToString();
            m_tb_MerchantInfo.RegisterAddress = _registerAddress.Text.ToString();

            if (m_OperateMerchantInfo.Add(_loginName.Text.ToString(), m_tb_MerchantInfo))
                Response.Redirect("../Table_List/MerchantInfoList_Page.aspx", true);
            else
            {
                Response.Write("<script>alert('你输入的商户名已经存在');</script>");
                _loginName.Text = "";
            }
        }
    }
}