using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace BSCode.DataBaseEntity
{
    /// <summary>
    /// tb_MerchantInfo:实体类
    /// </summary>
    public class tb_MerchantInfo
    {
        private long _ID;
        private string _loginName;
        private string _passwd;
        private string _company;
        private string _merchantCardID;
        private DateTime _registerDate;
        private string _registerName;
        private string _registerID;
        private string _registerPhone;
        private string _registerAddress;
        private int _loginNumber;
        private DateTime _lastLoginTime;

        public tb_MerchantInfo()
        {
            this.RegisterDate = this.LastLoginTime = System.DateTime.Now;
            this.LoginName = this.Passwd = this.Company = this.MerchantCardID = "";
            this.RegisterName = this.RegisterID = this._registerPhone = this.RegisterAddress = "";
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
        public string LoginName
        {
            get { return _loginName; }
            set { _loginName = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public string Passwd
        {
            get { return _passwd; }
            set { _passwd = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public string Company
        {
            get { return _company; }
            set { _company = value; }
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
        public DateTime RegisterDate
        {
            get { return _registerDate; }
            set { _registerDate = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public string RegisterName
        {
            get { return _registerName; }
            set { _registerName = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public string RegisterID
        {
            get { return _registerID; }
            set { _registerID = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public string RegisterPhone
        {
            get { return _registerPhone; }
            set { _registerPhone = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public string RegisterAddress
        {
            get { return _registerAddress; }
            set { _registerAddress = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public int LoginNumber
        {
            get { return _loginNumber; }
            set { _loginNumber = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public DateTime LastLoginTime
        {
            get { return _lastLoginTime; }
            set { _lastLoginTime = value; }
        }
    }
}