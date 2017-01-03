using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using BSCode.DataBaseEntity;
using BSCode.DataBaseOperation;
using System.Data.SqlClient;
using System.Data;
namespace BSCode.Code.Table_Add
{
    public partial class AgreementInfoAdd_Page : System.Web.UI.Page
    { 
        tb_AgreementInfo m_tb_AgreementInfo = new tb_AgreementInfo();
        OperateAgreementInfo m_OperateAgreementInfo = new OperateAgreementInfo();
        protected void Page_Load(object sender, EventArgs e)
        {   
            if (!IsPostBack)
            {
                 string MerchantCardID = null;
                 MerchantCardID = Request.QueryString["MerchantCardID"].ToString();
                 _merchantCardID.Value = MerchantCardID;
                 _agreementNo.Value = m_OperateAgreementInfo.GetMaxAgreementNoByMerchantCardID(MerchantCardID);
                 add_button.Attributes.Add("OnClick", "return AgreementInfoAddVerify()");             
            }
        }

        protected void add_button_Click(object sender, EventArgs e)
        {
            m_tb_AgreementInfo.AgreementNo = _agreementNo.Value.ToString();
            m_tb_AgreementInfo.MerchantCardID = _merchantCardID.Value.ToString();
            m_tb_AgreementInfo.SignDate = DateTime.Parse(_signDate.Value.ToString());
            m_tb_AgreementInfo.ValidatyPeriod = DateTime.Parse(_validatyPeriod.Value.ToString());
            m_tb_AgreementInfo.CoalTon = double.Parse(_coalTon.Value.ToString());
            m_tb_AgreementInfo.CarCardCount = int.Parse(_carCardCount.Value.ToString());
            m_tb_AgreementInfo.UseCarCardCount = int.Parse(_useCarCardCount.Value.ToString());
            m_tb_AgreementInfo.ActualCoalTon = double.Parse(_actualCoalTon.Value.ToString());
            m_tb_AgreementInfo.ReturnCarCardCount = int.Parse(_returnCarCardCount.Value.ToString());
            if (int.Parse(_agreementStatus.Value.ToString()) == 1) 
                m_tb_AgreementInfo.AgreementStatus = "完成";
            else
                m_tb_AgreementInfo.AgreementStatus = "未完成";
            if (m_OperateAgreementInfo.Add(m_tb_AgreementInfo))
                Response.Redirect("../Table_List/AgreementInfoList_Page.aspx", true);
            else
            {
                Response.Write("<script>alert('你输入的合同编号这个记录已经存在');</script>");
                _agreementNo.Value = "";
            }
        }
    }
}