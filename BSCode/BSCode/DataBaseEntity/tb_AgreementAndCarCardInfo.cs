using System;

namespace BSCode.DataBaseEntity
{
    /// <summary>
    /// tb_AgreementAndCarCardInfo:实体类
    /// </summary>
    public class tb_AgreementAndCarCardInfo
    {
        public tb_AgreementAndCarCardInfo()
        {
            //初始化数据
            this.RecordTime = System.DateTime.Now;
            this.FirstTime = this.LastTime = System.DateTime.Now;
            this.PlateNumber = this.EmptyCarSize = this.CargoSize = this.CargoCarSize = this.CarShapeInfo = "";
            this.CarPicture1 = this.CarPicture2 = this.CarPicture3 = this.CarPicture4 = this.CarPicture5 = "";
            this.WeighPlace = this.InOut = "";
        }
        #region Model
        private long _ID;
        private string _agreementNo;
        private string _carCardID;
        private string _plateNumber;
        private string _emptyCarSize;
        private string _cargoSize;
        private string _cargoCarSize;
        private string _carShapeInfo;
        private double _emptyCarWeight;
        private double _CarWeight;
        private double _cargoWeight;
        private double _cargoDensity;
        private double _deductWeight;
        private string _carPicture1;
        private string _carPicture2;
        private string _carPicture3;
        private string _carPicture4;
        private string _carPicture5;
        private string _weighPlace;
        private int _abnormal;
        private string _inOut;
        private DateTime _recordTime;
        private DateTime _firstTime;
        private DateTime _lastTime;

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
        public string CarCardID
        {
            get { return _carCardID; }
            set { _carCardID = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public string PlateNumber
        {
            get { return _plateNumber; }
            set { _plateNumber = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public string EmptyCarSize
        {
            get { return _emptyCarSize; }
            set { _emptyCarSize = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public string CargoSize
        {
            get { return _cargoSize; }
            set { _cargoSize = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public string CargoCarSize
        {
            get { return _cargoCarSize; }
            set { _cargoCarSize = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public string CarShapeInfo
        {
            get { return _carShapeInfo; }
            set { _carShapeInfo = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public double EmptyCarWeight
        {
            get { return _emptyCarWeight; }
            set { _emptyCarWeight = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public double CarWeight
        {
            get { return _CarWeight; }
            set { _CarWeight = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public double CargoWeight
        {
            get { return _cargoWeight; }
            set { _cargoWeight = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public double CargoDensity
        {
            get { return _cargoDensity; }
            set { _cargoDensity = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public double DeductWeight
        {
            get { return _deductWeight; }
            set { _deductWeight = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public string CarPicture1
        {
            get { return _carPicture1; }
            set { _carPicture1 = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public string CarPicture2
        {
            get { return _carPicture2; }
            set { _carPicture2 = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public string CarPicture3
        {
            get { return _carPicture3; }
            set { _carPicture3 = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public string CarPicture4
        {
            get { return _carPicture4; }
            set { _carPicture4 = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public string CarPicture5
        {
            get { return _carPicture5; }
            set { _carPicture5 = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public string WeighPlace
        {
            get { return _weighPlace; }
            set { _weighPlace = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public int Abnormal
        {
            get { return _abnormal; }
            set { _abnormal = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public string InOut
        {
            get { return _inOut; }
            set { _inOut = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public DateTime RecordTime
        {
            get { return _recordTime; }
            set { _recordTime = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public DateTime FirstTime
        {
            get { return _firstTime; }
            set { _firstTime = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public DateTime LastTime
        {
            get { return _lastTime; }
            set { _lastTime = value; }
        }  
        #endregion Model
    }
}