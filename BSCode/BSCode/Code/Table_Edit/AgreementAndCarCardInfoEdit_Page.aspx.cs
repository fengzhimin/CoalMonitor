using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using BSCode.DataBaseEntity;
using BSCode.DataBaseOperation;
namespace BSCode.Code.Table_Edit
{
    public partial class AgreementAndCarCardInfoAdd_Page : System.Web.UI.Page
    {
        tb_AgreementAndCarCardInfo m_tb_AgreementAndCarCardInfo = new tb_AgreementAndCarCardInfo();
        OperateAgreementAndCarCardInfo m_OperateAgreementAndCarCardInfo = new OperateAgreementAndCarCardInfo();
        protected void Page_Load(object sender, EventArgs e)
        {
            if (!IsPostBack)
            {
                update_button.Attributes.Add("OnClick", "return AgreementInfoAddVerify()");
                string id = null;
                id = Request.QueryString["ID"].ToString();
                m_tb_AgreementAndCarCardInfo = m_OperateAgreementAndCarCardInfo.getAgreementAndCarCardInfoById(id);
                _agreementNo.Value = m_tb_AgreementAndCarCardInfo.AgreementNo;
                _carCardID.Value = m_tb_AgreementAndCarCardInfo.CarCardID;
                _emptyCarSize.Value = m_tb_AgreementAndCarCardInfo.EmptyCarSize;
                _cargoSize.Value = m_tb_AgreementAndCarCardInfo.CargoSize;
                _cargoCarSize.Value = m_tb_AgreementAndCarCardInfo.CargoCarSize;
                _carShapeInfo.Value = m_tb_AgreementAndCarCardInfo.CarShapeInfo;
                _emptyCarWeight.Value = m_tb_AgreementAndCarCardInfo.EmptyCarWeight.ToString();
                _CarWeight.Value = m_tb_AgreementAndCarCardInfo.CarWeight.ToString();
                _cargoWeight.Value = m_tb_AgreementAndCarCardInfo.CargoWeight.ToString();

                _cargoDensity.Value = m_tb_AgreementAndCarCardInfo.CargoDensity.ToString();
                _deductWeight.Value = m_tb_AgreementAndCarCardInfo.DeductWeight.ToString();
                _weighPlace.Value = m_tb_AgreementAndCarCardInfo.WeighPlace;
                _abnormal.Value = m_tb_AgreementAndCarCardInfo.Abnormal.ToString();
                _inOut.SelectedValue = m_tb_AgreementAndCarCardInfo.InOut.ToString();
                recordTime.Value = m_tb_AgreementAndCarCardInfo.RecordTime.ToString();
            }
        }

        protected void update_button_Click(object sender, EventArgs e)
        {
            string id = null;
            id = Request.QueryString["ID"].ToString();
            m_tb_AgreementAndCarCardInfo.AgreementNo = _agreementNo.Value.ToString();
            m_tb_AgreementAndCarCardInfo.CarCardID = _carCardID.Value.ToString();
            m_tb_AgreementAndCarCardInfo.EmptyCarSize = _emptyCarSize.Value.ToString();
            m_tb_AgreementAndCarCardInfo.CargoSize = _cargoSize.Value.ToString();
            m_tb_AgreementAndCarCardInfo.CargoCarSize = _cargoCarSize.Value.ToString();
            m_tb_AgreementAndCarCardInfo.CarShapeInfo = _carShapeInfo.Value.ToString();
            m_tb_AgreementAndCarCardInfo.EmptyCarWeight = double.Parse(_emptyCarWeight.Value.ToString());
            m_tb_AgreementAndCarCardInfo.CarWeight = double.Parse(_CarWeight.Value.ToString());
            m_tb_AgreementAndCarCardInfo.CargoWeight = double.Parse(_cargoWeight.Value.ToString());

            m_tb_AgreementAndCarCardInfo.CargoDensity = double.Parse(_cargoDensity.Value.ToString());
            m_tb_AgreementAndCarCardInfo.DeductWeight = double.Parse(_deductWeight.Value.ToString());
            m_tb_AgreementAndCarCardInfo.WeighPlace = _weighPlace.Value.ToString();
            m_tb_AgreementAndCarCardInfo.Abnormal = int.Parse(_abnormal.Value.ToString());
            m_tb_AgreementAndCarCardInfo.InOut = _inOut.SelectedValue.ToString();
            m_tb_AgreementAndCarCardInfo.RecordTime = DateTime.Parse(recordTime.Value.ToString());

            if (!m_OperateAgreementAndCarCardInfo.UpdateByAgreementNoAndCarCardID(long.Parse(id), m_tb_AgreementAndCarCardInfo))
            {
                Response.Write("<script>alert('你输入的合同编号或车卡ID已经存在');</script>");
            }
        }


    }
}