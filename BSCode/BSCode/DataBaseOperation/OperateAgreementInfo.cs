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
    /// 对tb_AgreementInfo数据库进行增删改查
    /// </summary>
    public class OperateAgreementInfo
    {
        public string strcon = ConfigurationManager.ConnectionStrings["sqlstr"].ConnectionString;
        public SqlConnection sqlcon;
        public tb_AgreementInfo _tb_AgreementInfo;

        //构造函数
        public OperateAgreementInfo()
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
        ~OperateAgreementInfo()
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
            sqlcom.Parameters.Add(new SqlParameter("@agreementNo", SqlDbType.NVarChar, 50));
            sqlcom.Parameters.Add(new SqlParameter("@merchantCardID", SqlDbType.NVarChar, 50));
            sqlcom.Parameters.Add(new SqlParameter("@signDate", SqlDbType.DateTime));
            sqlcom.Parameters.Add(new SqlParameter("@validatyPeriod", SqlDbType.DateTime));
            sqlcom.Parameters.Add(new SqlParameter("@coalTon", SqlDbType.Float));
            sqlcom.Parameters.Add(new SqlParameter("@carCardCount", SqlDbType.Int));
            sqlcom.Parameters.Add(new SqlParameter("@useCarCardCount", SqlDbType.Int));
            sqlcom.Parameters.Add(new SqlParameter("@actualCoalTon", SqlDbType.Float));
            sqlcom.Parameters.Add(new SqlParameter("@returnCarCardCount", SqlDbType.Int));
            sqlcom.Parameters.Add(new SqlParameter("@agreementStatus", SqlDbType.NVarChar, 5));
        }

        /// <summary>
        /// 参数赋值
        /// </summary>
        public void setParametersValues(SqlCommand sqlcom, tb_AgreementInfo _agreementInfo)
        {
            //给参数赋值 
            sqlcom.Parameters["@agreementNo"].Value = _agreementInfo.AgreementNo;
            sqlcom.Parameters["@merchantCardID"].Value = _agreementInfo.MerchantCardID;
            sqlcom.Parameters["@signDate"].Value = _agreementInfo.SignDate;
            sqlcom.Parameters["@validatyPeriod"].Value = _agreementInfo.ValidatyPeriod;
            sqlcom.Parameters["@coalTon"].Value = _agreementInfo.CoalTon;
            sqlcom.Parameters["@carCardCount"].Value = _agreementInfo.CarCardCount;
            sqlcom.Parameters["@useCarCardCount"].Value = _agreementInfo.UseCarCardCount;
            sqlcom.Parameters["@actualCoalTon"].Value = _agreementInfo.ActualCoalTon;
            sqlcom.Parameters["@returnCarCardCount"].Value = _agreementInfo.ReturnCarCardCount;
            sqlcom.Parameters["@agreementStatus"].Value = _agreementInfo.AgreementStatus;
        }

        /// <summary>
        /// 增加一条记录 _agreementInfo需要被增加的记录信息
        /// 添加成功返回TRUE，否则返回FALSE
        /// </summary>
        public Boolean Add(tb_AgreementInfo _agreementInfo)
        {
            this._tb_AgreementInfo = _agreementInfo;
            string sqlstr = "insert into tb_AgreementInfo values(@agreementNo,@merchantCardID,@signDate," +
                "@validatyPeriod,@coalTon,@carCardCount,@useCarCardCount,@actualCoalTon,@returnCarCardCount,@agreementStatus)";
            try
            {
                SqlCommand sqlcom = new SqlCommand(sqlstr, sqlcon);
                this.AddParameters(sqlcom);
                this.setParametersValues(sqlcom, this._tb_AgreementInfo);
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
        /// 给最大合同编号增加1
        /// </summary>

        public String SelfAdd(String _number, int _lengh)
        {
            //char _temp;
            char[] m_number = _number.ToArray();
            for (int i = _lengh - 1; i >= 9; i--)
            {
                if (m_number[i] < '9')
                {
                    m_number[i] = (char)((int)m_number[i] + 1);
                    break;
                }
                else if (m_number[i] == '9')
                {
                    m_number[i] = 'A';
                    break;
                }
                else if (m_number[i] < 'F')
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
        /// 通合商户号查找该商户的最大合同号
        /// 返回最大合同号
        /// </summary>
        public string GetMaxAgreementNoByMerchantCardID(string m_MerchantCardID)
        {
            string sqlstr = "select * from tb_AgreementInfo where ID in (select max(ID) from tb_AgreementInfo where MerchantCardID = '" + m_MerchantCardID + "')";
            SqlCommand comm = new SqlCommand(sqlstr, sqlcon);
            SqlDataReader dr = comm.ExecuteReader();
            bool temp = dr.HasRows;
            if (temp)
            {
                dr.Read();
                String _maxagreementNo = dr["agreementNo"].ToString();
                dr.Close();
                return this.SelfAdd(_maxagreementNo, 18);
            }
            else
            {
                dr.Close();
                return m_MerchantCardID+"000000001";
            }
        }

        /// <summary>
        /// 通合主键ID更新一条记录 将原始数据更新为_agreementInfo
        /// </summary>
        public Boolean UpdateByAgreementNo(long _ID,tb_AgreementInfo _agreementInfo)
        {
            string sqlstr = "select * from tb_AgreementInfo where (ID != " + _ID + " and agreementNo = '" + _agreementInfo.AgreementNo + "') ";
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
                this._tb_AgreementInfo = _agreementInfo;
                sqlstr = "update tb_AgreementInfo set agreementNo=@agreementNo,merchantCardID=@merchantCardID,signDate=@signDate," +
                    "validatyPeriod=@validatyPeriod,coalTon=@coalTon,carCardCount=@carCardCount,useCarCardCount=@useCarCardCount," +
                    "actualCoalTon=@actualCoalTon,returnCarCardCount=@returnCarCardCount,agreementStatus=@agreementStatus where ID = " + _ID;
                try
                {
                    SqlCommand sqlcom = new SqlCommand(sqlstr, sqlcon);
                    this.AddParameters(sqlcom);
                    this.setParametersValues(sqlcom, this._tb_AgreementInfo);
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
        /// 通合同编号(AgreementNo)过删除一条记录
        /// </summary>
        public void DeleteByAgreementNo(String _agreementNo)
        {
            Console.Write(_agreementNo);
            String sqlstr = "delete from tb_AgreementInfo where agreementNo = '" + _agreementNo + "'";
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
        /// 通过合同编号(_agreementNo)获取一条记录 
        /// 如果没有记录则返回NULL
        /// </summary>
        public tb_AgreementInfo getAgreementInfo(String _agreementNo)
        {
            String sqlstr = "select * from tb_AgreementInfo where agreementNo = '" + _agreementNo + "'";
            tb_AgreementInfo _ret_agreementInfo = new tb_AgreementInfo();
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
                _ret_agreementInfo = null;
            else
            {
                _ret_agreementInfo.AgreementNo = _ds.Tables[0].Rows[0][1].ToString();
                _ret_agreementInfo.MerchantCardID = _ds.Tables[0].Rows[0][2].ToString();
                _ret_agreementInfo.SignDate = DateTime.Parse(_ds.Tables[0].Rows[0][3].ToString());
                _ret_agreementInfo.ValidatyPeriod = DateTime.Parse(_ds.Tables[0].Rows[0][4].ToString());
                _ret_agreementInfo.CoalTon = Double.Parse(_ds.Tables[0].Rows[0][5].ToString());
                _ret_agreementInfo.CarCardCount = int.Parse(_ds.Tables[0].Rows[0][6].ToString());
                _ret_agreementInfo.UseCarCardCount = int.Parse(_ds.Tables[0].Rows[0][7].ToString());
                _ret_agreementInfo.ActualCoalTon = Double.Parse(_ds.Tables[0].Rows[0][8].ToString());
                _ret_agreementInfo.ReturnCarCardCount = int.Parse(_ds.Tables[0].Rows[0][9].ToString());
                _ret_agreementInfo.AgreementStatus = _ds.Tables[0].Rows[0][10].ToString();
                }

            return _ret_agreementInfo;
        }

        /// <summary>
        /// 通过ID号(_agreementNo)获取一条记录 
        /// 如果没有记录则返回NULL
        /// </summary>
        public tb_AgreementInfo getAgreementInfoById(String _id)
        {
            String sqlstr = "select * from tb_AgreementInfo where ID = " + _id;
            tb_AgreementInfo _ret_agreementInfo = new tb_AgreementInfo();
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
                _ret_agreementInfo = null;
            else
            {
                _ret_agreementInfo.AgreementNo = _ds.Tables[0].Rows[0][1].ToString();
                _ret_agreementInfo.MerchantCardID = _ds.Tables[0].Rows[0][2].ToString();
                _ret_agreementInfo.SignDate = DateTime.Parse(_ds.Tables[0].Rows[0][3].ToString());
                _ret_agreementInfo.ValidatyPeriod = DateTime.Parse(_ds.Tables[0].Rows[0][4].ToString());
                _ret_agreementInfo.CoalTon = Double.Parse(_ds.Tables[0].Rows[0][5].ToString());
                _ret_agreementInfo.CarCardCount = int.Parse(_ds.Tables[0].Rows[0][6].ToString());
                _ret_agreementInfo.UseCarCardCount = int.Parse(_ds.Tables[0].Rows[0][7].ToString());
                _ret_agreementInfo.ActualCoalTon = Double.Parse(_ds.Tables[0].Rows[0][8].ToString());
                _ret_agreementInfo.ReturnCarCardCount = int.Parse(_ds.Tables[0].Rows[0][9].ToString());
                _ret_agreementInfo.AgreementStatus = _ds.Tables[0].Rows[0][10].ToString();
            }

            return _ret_agreementInfo;
        } 
        /// <summary>
        /// 获取tb_AgreementInfo表中的所有数据
        /// </summary>
        public DataSet getAllAgreementInfo()
        {
            String sqlstr = "select * from tb_AgreementInfo ";
            DataSet _ds = new DataSet();
            try
            {
                SqlDataAdapter _da = new SqlDataAdapter(sqlstr, sqlcon);
                _da.Fill(_ds,"tb_AgreementInfo");
            }
            catch (Exception e)
            {
                throw new Exception(e.Message);
            }

            return _ds;
        }

        /// <summary>
        /// 通过商户名字在合同表中得到相应的合同信息
        /// </summary>
        public DataSet getMerchantInfoAgreementInfo(string m_loginName)
        {
            String sqlstr = "select * from tb_AgreementInfo where merchantCardID in(select merchantCardID from tb_MerchantInfo where loginName = '" + m_loginName + "')";
            DataSet _ds = new DataSet();
            try
            {
                SqlDataAdapter _da = new SqlDataAdapter(sqlstr, sqlcon);
                _da.Fill(_ds, "tb_AgreementInfo");
            }
            catch (Exception e)
            {
                throw new Exception(e.Message);
            }

            return _ds;
        }
    }
}