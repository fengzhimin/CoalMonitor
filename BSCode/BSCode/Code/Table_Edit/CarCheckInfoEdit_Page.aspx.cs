using BSCode.DataBaseEntity;
using BSCode.DataBaseOperation;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace BSCode.Code.Table_Edit
{
    public partial class CarCheckInfoAdd_Page : System.Web.UI.Page
    {
        tb_CarCheckInfo m_tb_carCheckInfo = new tb_CarCheckInfo();
        OperateCarCheckInfo m_OperateCarCheckInfo = new OperateCarCheckInfo();
        protected void Page_Load(object sender, EventArgs e)
        {
            if (!IsPostBack)
            {
                string m_Power = Session["Power"].ToString();         
                update_button.Attributes.Add("OnClick", "return CarCheckInfoAddVerify()");
                string id = null;
                id = Request.QueryString["ID"].ToString();
                m_tb_carCheckInfo = m_OperateCarCheckInfo.getCarCheckInfoByID(id);
                _agreementNo.Value = m_tb_carCheckInfo.AgreementNo;
                _carCardID.Value = m_tb_carCheckInfo.CarCardID;
                _CarWeight.Value = m_tb_carCheckInfo.CarWeight.ToString();
                _cargoSize.Value = m_tb_carCheckInfo.CargoSize;
                _emptyCarSize.Value = m_tb_carCheckInfo.EmptyCarSize;
                _emptyCarWeight.Value = m_tb_carCheckInfo.EmptyCarWeight.ToString();
                _inOut.SelectedValue = m_tb_carCheckInfo.InOut;
                _recordTime.Value = m_tb_carCheckInfo.RecordTime.ToString();
                _deductWeight.Value = m_tb_carCheckInfo.DeductWeight.ToString();
                _cargoDensity.Value = m_tb_carCheckInfo.CargoDensity.ToString();
                _abnormal.Value = m_tb_carCheckInfo.Abnormal.ToString();
                _weighPlace.Value = m_tb_carCheckInfo.WeighPlace;
                _cargoWeight.Value = m_tb_carCheckInfo.CargoWeight.ToString();
                _cargoCarSize.Value = m_tb_carCheckInfo.CargoCarSize;
                if (m_Power == "2")
                {
                    _agreementNo.Attributes.Add("readonly", "true");
                    _carCardID.Attributes.Add("readonly", "true");
                    _weighPlace.Attributes.Add("readonly", "true");
                    _cargoWeight.Attributes.Add("readonly", "true");
                    _cargoCarSize.Attributes.Add("readonly", "true");
                    _carCardID.Attributes.Add("readonly", "true");
                    _CarWeight.Attributes.Add("readonly", "true");
                    _cargoSize.Attributes.Add("readonly", "true");
                    _emptyCarSize.Attributes.Add("readonly", "true");
                    _emptyCarWeight.Attributes.Add("readonly", "true");
                    _inOut.Attributes.Add("readonly", "true");
                    _recordTime.Attributes.Add("readonly", "true");
                    _deductWeight.Attributes.Add("readonly", "true");
                    _cargoDensity.Attributes.Add("readonly", "true");
                }
            
            
            }
             

        }
        protected void update_button_Click(object sender, EventArgs e)
        {
            m_tb_carCheckInfo.AgreementNo = _agreementNo.Value.ToString();
            m_tb_carCheckInfo.CarCardID = _carCardID.Value.ToString();
//          m_tb_carCheckInfo.CarNumber = _carNumber.Value.ToString();
            m_tb_carCheckInfo.CarWeight = Double.Parse(_CarWeight.Value.ToString());
            m_tb_carCheckInfo.CargoSize = _cargoSize.Value.ToString();
            m_tb_carCheckInfo.EmptyCarSize = _emptyCarSize.Value.ToString();
            _emptyCarWeight.Value = m_tb_carCheckInfo.EmptyCarWeight.ToString();
            m_tb_carCheckInfo.InOut = _inOut.SelectedValue.ToString();
            m_tb_carCheckInfo.RecordTime =DateTime.Parse( _recordTime.Value.ToString());
            _deductWeight.Value = m_tb_carCheckInfo.DeductWeight.ToString();
            m_tb_carCheckInfo.DeductWeight =Double.Parse(_cargoDensity.Value.ToString());
            m_tb_carCheckInfo.Abnormal =int.Parse(_abnormal.Value.ToString());
            m_tb_carCheckInfo.WeighPlace= _weighPlace.Value.ToString();
            m_tb_carCheckInfo.CargoWeight =double.Parse(_cargoWeight.Value.ToString());
            m_tb_carCheckInfo.CargoCarSize = _cargoCarSize.Value.ToString();
            string id = null;
            id = Request.QueryString["ID"].ToString();
            if (!m_OperateCarCheckInfo.UpdateByCarCardID(long.Parse(id),m_tb_carCheckInfo))
            {
                Response.Write("<script>alert('你输入的车卡号已经存在');</script>");                    
            }
        }
    }
}