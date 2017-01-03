using BSCode.DataBaseEntity;
using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Data.SqlClient;
using System.Linq;
using System.Web;

namespace BSCode.DataBaseOperation
{
    /// <summary>
    /// 对tb_MerchantInfo数据库进行增删改查
    /// </summary>
    public class OperateMerchantInfo
    {
        private string strcon = ConfigurationManager.ConnectionStrings["sqlstr"].ConnectionString;
        public SqlConnection sqlcon;
        private tb_MerchantInfo _tb_MerchantInfo;

        //构造函数
        public OperateMerchantInfo()
        {
            this.sqlcon = new SqlConnection(this.strcon);
            // 打开连接
            if (this.sqlcon.State == ConnectionState.Closed)
            {
                try
                {
                    this.sqlcon.Open();
                }
                catch(System.Data.SqlClient.SqlException e)
                {
                    throw new Exception(e.Message);
                }
            }
        }

        //析构函数
        ~OperateMerchantInfo()
        {
            // 关闭连接
            if (this.sqlcon.State == ConnectionState.Open)
            {
                try
                {
                    this.sqlcon.Close();
                }
                catch (System.Data.SqlClient.SqlException e)
                {
                    throw new Exception(e.Message);
                }
            }
        }

        /// <summary>
        /// 添加参数
        /// </summary>
        public void AddParameters(SqlCommand sqlcom)
        {
            //添加参数 
            sqlcom.Parameters.Add(new SqlParameter("@loginName", SqlDbType.NVarChar, 50));
            sqlcom.Parameters.Add(new SqlParameter("@passwd", SqlDbType.NVarChar, 50));
            sqlcom.Parameters.Add(new SqlParameter("@company", SqlDbType.NVarChar, 50));
            sqlcom.Parameters.Add(new SqlParameter("@merchantCardID", SqlDbType.NVarChar, 50));
            sqlcom.Parameters.Add(new SqlParameter("@registerDate", SqlDbType.DateTime));
            sqlcom.Parameters.Add(new SqlParameter("@registerName", SqlDbType.NVarChar, 50));
            sqlcom.Parameters.Add(new SqlParameter("@registerID", SqlDbType.NVarChar, 50));
            sqlcom.Parameters.Add(new SqlParameter("@registerPhone", SqlDbType.NVarChar, 50));
            sqlcom.Parameters.Add(new SqlParameter("@registerAddress", SqlDbType.NVarChar, 50));
            sqlcom.Parameters.Add(new SqlParameter("@loginNumber", SqlDbType.Int));
            sqlcom.Parameters.Add(new SqlParameter("@lastLoginTime", SqlDbType.DateTime));
        }

        /// <summary>
        /// 参数赋值
        /// </summary>
        public void setParametersValues(SqlCommand sqlcom, tb_MerchantInfo _merchantInfo)
        {
            //给参数赋值 
            sqlcom.Parameters["@loginName"].Value = _merchantInfo.LoginName;
            sqlcom.Parameters["@passwd"].Value = _merchantInfo.Passwd;
            sqlcom.Parameters["@company"].Value = _merchantInfo.Company;
            sqlcom.Parameters["@merchantCardID"].Value = _merchantInfo.MerchantCardID;
            sqlcom.Parameters["@registerDate"].Value = _merchantInfo.RegisterDate;
            sqlcom.Parameters["@registerName"].Value = _merchantInfo.RegisterName;
            sqlcom.Parameters["@registerID"].Value = _merchantInfo.RegisterID;
            sqlcom.Parameters["@registerPhone"].Value = _merchantInfo.RegisterPhone;
            sqlcom.Parameters["@registerAddress"].Value = _merchantInfo.RegisterAddress;
            sqlcom.Parameters["@loginNumber"].Value = _merchantInfo.LoginNumber;
            sqlcom.Parameters["@lastLoginTime"].Value = _merchantInfo.LastLoginTime;
        }

        /// <summary>
        /// 增加一条记录 _merchantInfo需要被增加的记录信息
        /// 添加成功返回TRUE，否则返回FALSE
        /// </summary>
        public Boolean Add(string _LoginName,tb_MerchantInfo _merchantInfo)
        {
            this._tb_MerchantInfo = _merchantInfo;
            string sqlstr = "insert into tb_MerchantInfo values(@loginName,@passwd,@company," +
                "@merchantCardID,@registerDate,@registerName,@registerID,@registerPhone,@registerAddress,@loginNumber,@lastLoginTime)";
            try
            {
                SqlCommand sqlcom = new SqlCommand(sqlstr, sqlcon);
                this.AddParameters(sqlcom);
                this.setParametersValues(sqlcom, this._tb_MerchantInfo);
                //执行添加语句 
                sqlcom.ExecuteNonQuery();
            }
            catch (Exception e)
            {
                throw new Exception(e.Message);
            }
            return true;
        }


        /// <summary>
        /// 是否商户登录
        /// 是返回true 不是返回false
        /// </summary>
        public Boolean JudgeMerchantInfo(string _loginName, string _loginPasswd)
        {
            string sqlstr = "select * from tb_MerchantInfo where (LoginName = '" + _loginName + " ' and  Passwd = '" + _loginPasswd + " ')";
            SqlCommand comm = new SqlCommand(sqlstr, sqlcon);
            SqlDataReader dr = comm.ExecuteReader();
            bool temp = dr.HasRows;
            if (temp)
            {
                dr.Close();
                return true;   //已经记录了
            }
            else
            {
                dr.Close();
                return false;   //已经没有记录了
            }
        }


        public Boolean UpdateByRegisterName(tb_MerchantInfo _merchantInfo)
        {
            string sqlstr = "select * from tb_MerchantInfo where loginName = '" + _merchantInfo.LoginName + "'";
            SqlCommand comm = new SqlCommand(sqlstr, sqlcon);
            SqlDataReader dr = comm.ExecuteReader();
            bool temp = dr.HasRows;
            dr.Close();
            if (temp)
            {
                return false;   //已经记录了
            }
            else
            {
                this._tb_MerchantInfo = _merchantInfo;
                sqlstr = "update tb_MerchantInfo set loginName=@loginName,passwd=@passwd,company=@company," +
                   "merchantCardID=@merchantCardID,registerDate=@registerDate,registerName=@registerName,registerID=@registerID," +
                   "registerPhone=@registerPhone,registerAddress=@registerAddress where loginName = '" + _merchantInfo.LoginName + "'";
                try
                {
                    SqlCommand sqlcom = new SqlCommand(sqlstr, sqlcon);
                    this.AddParameters(sqlcom);
                    this.setParametersValues(sqlcom, this._tb_MerchantInfo);
                    //执行添加语句 
                    sqlcom.ExecuteNonQuery();
                }
                catch (Exception e)
                {
                    throw new Exception(e.Message);
                }
                return true;
            }
        }


        // <summary>
        /// 通过主键ID更新一条记录 将原始数据更新为_merchantInfo
        /// </summary>
        public Boolean UpdateByAgreementNo(long _ID, tb_MerchantInfo _merchantInfo)
        {
            string sqlstr = "select * from tb_MerchantInfo where (ID != " + _ID + " and merchantCardID = '" + _merchantInfo.MerchantCardID + "') or ( ID != " + _ID + " and  loginName = '" + _merchantInfo.LoginName + "')";
            SqlCommand comm = new SqlCommand(sqlstr, sqlcon);
            SqlDataReader dr = comm.ExecuteReader();
            bool temp = dr.HasRows;
            dr.Close();
            if (temp)
            {
                return false;   //已经记录了
            }

            else
            {
                this._tb_MerchantInfo = _merchantInfo;
                sqlstr = "update tb_MerchantInfo set loginName=@loginName,passwd=@passwd,company=@company," +
                   "merchantCardID=@merchantCardID,registerDate=@registerDate,registerName=@registerName,registerID=@registerID," +
                   "registerPhone=@registerPhone,registerAddress=@registerAddress where ID = " + _ID;
                try
                {
                    SqlCommand sqlcom = new SqlCommand(sqlstr, sqlcon);
                    this.AddParameters(sqlcom);
                    this.setParametersValues(sqlcom, this._tb_MerchantInfo);
                    //执行添加语句 
                    sqlcom.ExecuteNonQuery();
                }
                catch (Exception e)
                {
                    throw new Exception(e.Message);
                }
                return true;
            }
        }

        /// <summary>
        /// 通商户卡ID(_merchantCardID)过删除一条记录
        /// </summary>
        public void DeleteByMerchantCardID(String _merchantCardID)
        {
            String sqlstr = "delete from tb_MerchantInfo where merchantCardID = '" + _merchantCardID + "'";
            try
            {
                SqlCommand sqlcom = new SqlCommand(sqlstr, sqlcon);
                sqlcom.ExecuteNonQuery();
            }
            catch (Exception e)
            {
                throw new Exception(e.Message);
            }
        }

        /// <summary>
        /// 得到商户卡ID(_merchantCardID)
        /// </summary>
        public String getMaxMerchant()
        {
            String sqlstr = "select merchantCardID from tb_MerchantInfo where ID in (select max(ID) as ID from tb_MerchantInfo)";
            SqlCommand comm = new SqlCommand(sqlstr, sqlcon);
            SqlDataReader dr = comm.ExecuteReader();
            bool temp = dr.HasRows;

            if (temp)
            {
                dr.Read();
                String _maxMerchant = dr["merchantCardID"].ToString();
                dr.Close();
                return this.SelfAdd(_maxMerchant, 9);
            }
            else
            {
                dr.Close();
                return "000000001";
            }
        }

        public String SelfAdd(String _number, int _lengh)
        {
            //char _temp;
            char[] m_number = _number.ToArray();
            for (int i = _lengh - 1; i >= 0; i--)
            {
                if (m_number[i] < '9')
                {
                    m_number[i] = (char)((int)m_number[i] + 1);
                    break;
                }
                else if (m_number[i] == '9')
                {
                    m_number[i] = 'a';
                    break;
                }
                else if (m_number[i] < 'f' || m_number[i] < 'F')
                {
                    m_number[i] = (char)((int)m_number[i] + 1);
                    break;
                }
                else
                    m_number[i] = '0';
            }

            _number = new string(m_number);
            return _number;
        }

        /// <summary>
        /// 通商户卡ID(_merchantCardID)获取一条记录 
        /// 如果没有记录则返回NULL
        /// </summary>
        public tb_MerchantInfo getMerchantInfo(String _merchantCardID)
        {
            String sqlstr = "select * from tb_MerchantInfo where merchantCardID = '" + _merchantCardID + "'";
            tb_MerchantInfo _ret_merchantInfo = new tb_MerchantInfo();
            DataSet _ds = new DataSet();
            try
            {
                SqlDataAdapter _da = new SqlDataAdapter(sqlstr, sqlcon);
                _da.Fill(_ds);
            }
            catch (Exception e)
            {
                throw new Exception(e.Message);
            }

            //判断是否有该记录
            if (_ds.Tables.Count > 0 && _ds.Tables[0].Rows.Count == 0)
                _ret_merchantInfo = null;
            else
            {
                _ret_merchantInfo.LoginName = _ds.Tables[0].Rows[0][1].ToString();
                _ret_merchantInfo.Passwd = _ds.Tables[0].Rows[0][2].ToString();
                _ret_merchantInfo.Company = _ds.Tables[0].Rows[0][3].ToString();
                _ret_merchantInfo.MerchantCardID = _ds.Tables[0].Rows[0][4].ToString();
                _ret_merchantInfo.RegisterDate = DateTime.Parse(_ds.Tables[0].Rows[0][5].ToString());
                _ret_merchantInfo.RegisterName = _ds.Tables[0].Rows[0][6].ToString();
                _ret_merchantInfo.RegisterID = _ds.Tables[0].Rows[0][7].ToString();
                _ret_merchantInfo.RegisterPhone = _ds.Tables[0].Rows[0][8].ToString();
                _ret_merchantInfo.RegisterAddress = _ds.Tables[0].Rows[0][9].ToString();
                _ret_merchantInfo.LoginNumber = int.Parse(_ds.Tables[0].Rows[0][10].ToString());
                _ret_merchantInfo.LastLoginTime = DateTime.Parse(_ds.Tables[0].Rows[0][11].ToString());
                }

            return _ret_merchantInfo;
        }


        /// <summary>
        /// 通过商户注册名字(RegisterName)获取一条记录 
        /// </summary>
        public tb_MerchantInfo getMerchantInfoByRegisterName(String _loginName)
        {
            String sqlstr = "select * from tb_MerchantInfo where loginName = '" + _loginName+"'";
            tb_MerchantInfo _ret_merchantInfo = new tb_MerchantInfo();
            DataSet _ds = new DataSet();
            try
            {
                SqlDataAdapter _da = new SqlDataAdapter(sqlstr, sqlcon);
                _da.Fill(_ds);
            }
            catch (Exception e)
            {
                throw new Exception(e.Message);
            }
            _ret_merchantInfo.LoginName = _ds.Tables[0].Rows[0][1].ToString();
            _ret_merchantInfo.Passwd = _ds.Tables[0].Rows[0][2].ToString();
            _ret_merchantInfo.Company = _ds.Tables[0].Rows[0][3].ToString();
            _ret_merchantInfo.MerchantCardID = _ds.Tables[0].Rows[0][4].ToString();
            _ret_merchantInfo.RegisterDate = DateTime.Parse(_ds.Tables[0].Rows[0][5].ToString());
            _ret_merchantInfo.RegisterName = _ds.Tables[0].Rows[0][6].ToString();
            _ret_merchantInfo.RegisterID = _ds.Tables[0].Rows[0][7].ToString();
            _ret_merchantInfo.RegisterPhone = _ds.Tables[0].Rows[0][8].ToString();
            _ret_merchantInfo.RegisterAddress = _ds.Tables[0].Rows[0][9].ToString();
            _ret_merchantInfo.LoginNumber = int.Parse(_ds.Tables[0].Rows[0][10].ToString());
            _ret_merchantInfo.LastLoginTime = DateTime.Parse(_ds.Tables[0].Rows[0][11].ToString());
            
            return _ret_merchantInfo;
        }


        /// <summary>
        /// 通过商户ID(ID)获取一条记录 
        /// 如果没有记录则返回NULL
        /// </summary>
        public tb_MerchantInfo getMerchantInfoById(String _id)
        {
            String sqlstr = "select * from tb_MerchantInfo where ID = " + _id;
            tb_MerchantInfo _ret_merchantInfo = new tb_MerchantInfo();
            DataSet _ds = new DataSet();
            try
            {
                SqlDataAdapter _da = new SqlDataAdapter(sqlstr, sqlcon);
                _da.Fill(_ds);
            }
            catch (Exception e)
            {
                throw new Exception(e.Message);
            }

            //判断是否有该记录
            if (_ds.Tables.Count > 0 && _ds.Tables[0].Rows.Count == 0)
                _ret_merchantInfo = null;
            else
            {
                _ret_merchantInfo.LoginName = _ds.Tables[0].Rows[0][1].ToString();
                _ret_merchantInfo.Passwd = _ds.Tables[0].Rows[0][2].ToString();
                _ret_merchantInfo.Company = _ds.Tables[0].Rows[0][3].ToString();
                _ret_merchantInfo.MerchantCardID = _ds.Tables[0].Rows[0][4].ToString();
                _ret_merchantInfo.RegisterDate = DateTime.Parse(_ds.Tables[0].Rows[0][5].ToString());
                _ret_merchantInfo.RegisterName = _ds.Tables[0].Rows[0][6].ToString();
                _ret_merchantInfo.RegisterID = _ds.Tables[0].Rows[0][7].ToString();
                _ret_merchantInfo.RegisterPhone = _ds.Tables[0].Rows[0][8].ToString();
                _ret_merchantInfo.RegisterAddress = _ds.Tables[0].Rows[0][9].ToString();
                _ret_merchantInfo.LoginNumber = int.Parse(_ds.Tables[0].Rows[0][10].ToString());
                _ret_merchantInfo.LastLoginTime = DateTime.Parse(_ds.Tables[0].Rows[0][11].ToString());
            }

            return _ret_merchantInfo;
        }

        /// <summary>
        /// 获取tb_MerchantInfo表中的所有数据
        /// </summary>
        public DataSet getAllMerchantInfo()
        {
           
            String sqlstr = "select * from tb_MerchantInfo";
            DataSet _ds = new DataSet();
            try
            {
                SqlDataAdapter _da = new SqlDataAdapter(sqlstr, sqlcon);
                _da.Fill(_ds);
            }
            catch (Exception e)
            {
                throw new Exception(e.Message);
            }

            return _ds;
        }
    }
}