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

namespace BSCode.Code.Table_Edit
{
    public partial class MerchantInfoAdd_Page : System.Web.UI.Page
    {
        tb_MerchantInfo m_tb_MerchantInfo = new tb_MerchantInfo();
        OperateMerchantInfo m_OperateMerchantInfo = new OperateMerchantInfo();

        protected void Page_Load(object sender, EventArgs e)
        {
            if (!IsPostBack)
            {
                //给button1添加客户端事件  
                string Power = Session["Power"].ToString();
                update_button.Attributes.Add("OnClick", "return MerchantInfoAddVerify()");
                string id = null;
                id = Request.QueryString["ID"].ToString();
                m_tb_MerchantInfo = m_OperateMerchantInfo.getMerchantInfoById(id);
                _loginName.Value = m_tb_MerchantInfo.LoginName;
                _passwd.Value = m_tb_MerchantInfo.Passwd;
                _registerName.Value = m_tb_MerchantInfo.RegisterName;
                _registerID.Value = m_tb_MerchantInfo.RegisterID;
                _merchantCardID.Value = m_tb_MerchantInfo.MerchantCardID;
                _company.Value = m_tb_MerchantInfo.Company;
                _registerPhone.Value = m_tb_MerchantInfo.RegisterPhone;
                _registerAddress.Value = m_tb_MerchantInfo.RegisterAddress;
                if (Power=="3")
                {
                    _loginName.Attributes.Add("readonly", "true");
                    _passwd.Attributes.Add("readonly", "true");
                    _registerName.Attributes.Add("readonly", "true");
                    _registerID.Attributes.Add("readonly", "true");
                    _merchantCardID.Attributes.Add("readonly", "true");
                    _company.Attributes.Add("readonly", "true");
                    _registerPhone.Attributes.Add("readonly", "true");
                    _registerAddress.Attributes.Add("readonly", "true");
                }
            }
        }

        protected void update_button_Click(object sender, EventArgs e)
        {
            m_tb_MerchantInfo.LoginName = _loginName.Value.ToString();
            m_tb_MerchantInfo.Passwd = _passwd.Value.ToString();
            m_tb_MerchantInfo.RegisterName = _registerName.Value.ToString();
            m_tb_MerchantInfo.RegisterID = _registerID.Value.ToString();
            m_tb_MerchantInfo.MerchantCardID = _merchantCardID.Value.ToString();
            m_tb_MerchantInfo.Company = _company.Value.ToString();
            m_tb_MerchantInfo.RegisterPhone = _registerPhone.Value.ToString();
            m_tb_MerchantInfo.RegisterAddress = _registerAddress.Value.ToString();
            string id = null;
            id = Request.QueryString["ID"].ToString();

            if (!m_OperateMerchantInfo.UpdateByAgreementNo(long.Parse(id),m_tb_MerchantInfo))
            {
                Response.Write("<script>alert('你输入的商户卡ID或登录名已经存在');</script>");
            
            }
        }
    }
}