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
namespace BSCode.Code.Table_Edit
{
    public partial class AgreementInfoAdd_Page : System.Web.UI.Page
    { 
        tb_AgreementInfo m_tb_AgreementInfo = new tb_AgreementInfo();
        OperateAgreementInfo m_OperateAgreementInfo = new OperateAgreementInfo();
        protected void Page_Load(object sender, EventArgs e)
        {   
            if (!IsPostBack)
            {              
                update_button.Attributes.Add("OnClick", "return AgreementInfoAddVerify()");
                string id = null;
                id = Request.QueryString["ID"].ToString();
                m_tb_AgreementInfo = m_OperateAgreementInfo.getAgreementInfoById(id);
                _agreementNo.Value = m_tb_AgreementInfo.AgreementNo;
                _merchantCardID.Value = m_tb_AgreementInfo.MerchantCardID;
                _signDate.Value = m_tb_AgreementInfo.SignDate.ToString();
                _validatyPeriod.Value = m_tb_AgreementInfo.ValidatyPeriod.ToString();
                _coalTon.Value = m_tb_AgreementInfo.CoalTon.ToString();
                _carCardCount.Value = m_tb_AgreementInfo.CarCardCount.ToString();
                _useCarCardCount.Value = m_tb_AgreementInfo.UseCarCardCount.ToString();
                _actualCoalTon.Value = m_tb_AgreementInfo.ActualCoalTon.ToString();
                _returnCarCardCount.Value = m_tb_AgreementInfo.ReturnCarCardCount.ToString();
                _agreementStatus.SelectedValue = m_tb_AgreementInfo.AgreementStatus.ToString();
            }
        }

        protected void update_button_Click(object sender, EventArgs e)
        {
            string id = null;
            id = Request.QueryString["ID"].ToString();
            m_tb_AgreementInfo.AgreementNo = _agreementNo.Value.ToString();
            m_tb_AgreementInfo.MerchantCardID = _merchantCardID.Value.ToString();
            m_tb_AgreementInfo.SignDate = DateTime.Parse(_signDate.Value.ToString());
            m_tb_AgreementInfo.ValidatyPeriod = DateTime.Parse(_validatyPeriod.Value.ToString());
            m_tb_AgreementInfo.CoalTon = double.Parse(_coalTon.Value.ToString());
            m_tb_AgreementInfo.CarCardCount = int.Parse(_carCardCount.Value.ToString());
            m_tb_AgreementInfo.UseCarCardCount = int.Parse(_useCarCardCount.Value.ToString());
            m_tb_AgreementInfo.ActualCoalTon = double.Parse(_actualCoalTon.Value.ToString());
            m_tb_AgreementInfo.ReturnCarCardCount = int.Parse(_returnCarCardCount.Value.ToString());
            m_tb_AgreementInfo.AgreementStatus = _agreementStatus.SelectedValue.ToString();
            if (!m_OperateAgreementInfo.UpdateByAgreementNo(long.Parse(id), m_tb_AgreementInfo))
            {
                Response.Write("<script>alert('你输入的合同编号或商户卡ID已经存在');</script>");
            }     
        }
    }
}