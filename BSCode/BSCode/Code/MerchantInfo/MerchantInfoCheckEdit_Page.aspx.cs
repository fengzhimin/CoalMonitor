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

namespace BSCode.Code.MerchantInfo
{
    public partial class MerchantInfoCheckEdit_Page : System.Web.UI.Page
    {
        tb_MerchantInfo m_tb_MerchantInfo = new tb_MerchantInfo();
        OperateMerchantInfo m_OperateMerchantInfo = new OperateMerchantInfo();
        static string loginName, passwd, registerName, registerID, merchantCardID,
            company, registerPhone, registerAddress;
        protected void Page_Load(object sender, EventArgs e)
        {
            if (!IsPostBack)
            {
                //给button1添加客户端事件  
                _loginName.Attributes.Add("readonly", "true");
                _passwd.Attributes.Add("readonly", "true");
                _registerName.Attributes.Add("readonly", "true");
                _registerID.Attributes.Add("readonly", "true");
                _company.Attributes.Add("readonly", "true");
                _registerPhone.Attributes.Add("readonly", "true");
                _registerAddress.Attributes.Add("readonly", "true");
                update_button.Visible = false;
                cacle_button.Visible = false;
                update_button.Attributes.Add("OnClick", "return MerchantInfCheckEditVerify()");
                string m_loginName = Session["loginName"].ToString();
                m_tb_MerchantInfo = m_OperateMerchantInfo.getMerchantInfoByRegisterName(m_loginName);
                // 先保存初始的值   方便为下面取消返回所有的初始值
                loginName = m_tb_MerchantInfo.LoginName;
                passwd = m_tb_MerchantInfo.Passwd;
                registerName = m_tb_MerchantInfo.RegisterName;
                registerID = m_tb_MerchantInfo.RegisterID;
                merchantCardID = m_tb_MerchantInfo.MerchantCardID;
                company = m_tb_MerchantInfo.Company;
                registerPhone = m_tb_MerchantInfo.RegisterPhone;
                registerAddress = m_tb_MerchantInfo.RegisterAddress;

                _loginName.Text = m_tb_MerchantInfo.LoginName;
                _passwd.Text = m_tb_MerchantInfo.Passwd;
                _registerName.Text = m_tb_MerchantInfo.RegisterName;
                _registerID.Text = m_tb_MerchantInfo.RegisterID;
                _merchantCardID.Text = m_tb_MerchantInfo.MerchantCardID;
                _company.Text = m_tb_MerchantInfo.Company;
                _registerPhone.Text = m_tb_MerchantInfo.RegisterPhone;
                _registerAddress.Text = m_tb_MerchantInfo.RegisterAddress;
            }
        }

        protected void edit_button_Click(object sender, EventArgs e)
        {
            _loginName.Attributes.Remove("readonly");
            _passwd.Attributes.Remove("readonly");
            _registerName.Attributes.Remove("readonly");
            _registerID.Attributes.Remove("readonly");
            _company.Attributes.Remove("readonly");
            _registerPhone.Attributes.Remove("readonly");
            _registerAddress.Attributes.Remove("readonly");
            edit_button.Visible = false;
            update_button.Visible = true;
            cacle_button.Visible = true;
        }

        protected void cacle_button_Click(object sender, EventArgs e)
        {
            edit_button.Visible = true;
            update_button.Visible = false;
            cacle_button.Visible = false;
            _loginName.Text = loginName;
            _passwd.Text = passwd;
            _registerName.Text = registerName;
            _registerID.Text = registerID;
            _merchantCardID.Text = merchantCardID;
            _company.Text = company;
            _registerPhone.Text = registerPhone;
            _registerAddress.Text = registerAddress;
            _loginName.Attributes.Add("readonly", "true");
            _passwd.Attributes.Add("readonly", "true");
            _registerName.Attributes.Add("readonly", "true");
            _registerID.Attributes.Add("readonly", "true");
            _company.Attributes.Add("readonly", "true");
            _registerPhone.Attributes.Add("readonly", "true");
            _registerAddress.Attributes.Add("readonly", "true");
        }

        protected void update_button_Click(object sender, EventArgs e)
        {
            edit_button.Visible = true;
            update_button.Visible = false;
            cacle_button.Visible = false;
            m_tb_MerchantInfo.LoginName = _loginName.Text.ToString();
            m_tb_MerchantInfo.Passwd = _passwd.Text.ToString();
            m_tb_MerchantInfo.RegisterName = _registerName.Text.ToString();
            m_tb_MerchantInfo.RegisterID = _registerID.Text.ToString();
            m_tb_MerchantInfo.MerchantCardID = _merchantCardID.Text.ToString();
            m_tb_MerchantInfo.Company = _company.Text.ToString();
            m_tb_MerchantInfo.RegisterPhone = _registerPhone.Text.ToString();
            m_tb_MerchantInfo.RegisterAddress = _registerAddress.Text.ToString();
            _loginName.Attributes.Add("readonly", "true");
            _passwd.Attributes.Add("readonly", "true");
            _registerName.Attributes.Add("readonly", "true");
            _registerID.Attributes.Add("readonly", "true");
            _company.Attributes.Add("readonly", "true");
            _registerPhone.Attributes.Add("readonly", "true");
            _registerAddress.Attributes.Add("readonly", "true");
            if (m_tb_MerchantInfo.LoginName!=loginName&&!m_OperateMerchantInfo.UpdateByRegisterName(m_tb_MerchantInfo))
            {
                Response.Write("<script>alert('你输入的商户名已经存在');</script>");
            }
        }
    }
}

