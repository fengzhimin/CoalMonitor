using System;

namespace BSCode.DataBaseEntity
{
    /// <summary>
    /// tb_AgreementInfo:实体类
    /// </summary>
    public class tb_AgreementInfo
    {
        private long _ID;
        private string _agreementNo;
        private string _merchantCardID;
        private DateTime _signDate;
        private DateTime _validatyPeriod;
        private double _coalTon;
        private int _carCardCount;
        private int _useCarCardCount;
        private double _actualCoalTon;
        private int _returnCarCardCount;
        private string _agreementStatus;

        public tb_AgreementInfo()
        {
            this.SignDate = this.ValidatyPeriod = System.DateTime.Now;
            this.AgreementNo = this.MerchantCardID = "";
            this.AgreementStatus = "";
        }

        /// <summary>
        /// 
        /// </summary>
        public long ID
        {
            get { return _ID; }
            set { _ID = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public string AgreementNo
        {
            get { return _agreementNo; }
            set { _agreementNo = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public string MerchantCardID
        {
            get { return _merchantCardID; }
            set { _merchantCardID = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public DateTime SignDate
        {
            get { return _signDate; }
            set { _signDate = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public DateTime ValidatyPeriod
        {
            get { return _validatyPeriod; }
            set { _validatyPeriod = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public double CoalTon
        {
            get { return _coalTon; }
            set { _coalTon = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public int CarCardCount
        {
            get { return _carCardCount; }
            set { _carCardCount = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public int UseCarCardCount
        {
            get { return _useCarCardCount; }
            set { _useCarCardCount = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public double ActualCoalTon
        {
            get { return _actualCoalTon; }
            set { _actualCoalTon = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public int ReturnCarCardCount
        {
            get { return _returnCarCardCount; }
            set { _returnCarCardCount = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public string AgreementStatus
        {
            get { return _agreementStatus; }
            set { _agreementStatus = value; }
        }
    }
}