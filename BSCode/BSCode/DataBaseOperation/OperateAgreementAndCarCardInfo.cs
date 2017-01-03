using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using BSCode.DataBaseEntity;
using System.Configuration;
using System.Data.SqlClient;
using System.Data;
using System.Text;

namespace BSCode.DataBaseOperation
{
    /// <summary>
    /// 对tb_AgreementAndCarCardInfo数据库进行增删改查
    /// </summary>
    public class OperateAgreementAndCarCardInfo
    {
        private string strcon = ConfigurationManager.ConnectionStrings["sqlstr"].ConnectionString;
        public SqlConnection sqlcon;
        private tb_AgreementAndCarCardInfo _tb_AgreementAndCarCardInfo;

        //构造函数
        public OperateAgreementAndCarCardInfo()
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
        ~OperateAgreementAndCarCardInfo()
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
            sqlcom.Parameters.Add(new SqlParameter("@carCardID", SqlDbType.NVarChar, 50));
            sqlcom.Parameters.Add(new SqlParameter("@plateNumber", SqlDbType.NVarChar, 50));
            sqlcom.Parameters.Add(new SqlParameter("@emptyCarSize", SqlDbType.NVarChar, 50));
            sqlcom.Parameters.Add(new SqlParameter("@cargoSize", SqlDbType.NVarChar, 50));
            sqlcom.Parameters.Add(new SqlParameter("@cargoCarSize", SqlDbType.NVarChar, 50));
            sqlcom.Parameters.Add(new SqlParameter("@carShapeInfo", SqlDbType.NVarChar, 50));
            sqlcom.Parameters.Add(new SqlParameter("@emptyCarWeight", SqlDbType.Float));
            sqlcom.Parameters.Add(new SqlParameter("@CarWeight", SqlDbType.Float));
            sqlcom.Parameters.Add(new SqlParameter("@cargoWeight", SqlDbType.Float));
            sqlcom.Parameters.Add(new SqlParameter("@cargoDensity", SqlDbType.Float));
            sqlcom.Parameters.Add(new SqlParameter("@deductWeight", SqlDbType.Float));
            sqlcom.Parameters.Add(new SqlParameter("@carPicture1", SqlDbType.NVarChar, 50));
            sqlcom.Parameters.Add(new SqlParameter("@carPicture2", SqlDbType.NVarChar, 50));
            sqlcom.Parameters.Add(new SqlParameter("@carPicture3", SqlDbType.NVarChar, 50));
            sqlcom.Parameters.Add(new SqlParameter("@carPicture4", SqlDbType.NVarChar, 50));
            sqlcom.Parameters.Add(new SqlParameter("@carPicture5", SqlDbType.NVarChar, 50));
            sqlcom.Parameters.Add(new SqlParameter("@weighPlace", SqlDbType.NVarChar, 50));
            sqlcom.Parameters.Add(new SqlParameter("@abnormal", SqlDbType.Int));
            sqlcom.Parameters.Add(new SqlParameter("@inOut", SqlDbType.NVarChar, 1));
            sqlcom.Parameters.Add(new SqlParameter("@recordTime", SqlDbType.DateTime));
            sqlcom.Parameters.Add(new SqlParameter("@firstTime", SqlDbType.DateTime));
            sqlcom.Parameters.Add(new SqlParameter("@lastTime", SqlDbType.DateTime));
        }

        /// <summary>
        /// 参数赋值
        /// </summary>
        public void setParametersValues(SqlCommand sqlcom, tb_AgreementAndCarCardInfo _agreementAndCarCardInfo)
        {
            //给参数赋值 
            sqlcom.Parameters["@agreementNo"].Value = _agreementAndCarCardInfo.AgreementNo;
            sqlcom.Parameters["@carCardID"].Value = _agreementAndCarCardInfo.CarCardID;
            sqlcom.Parameters["@plateNumber"].Value = _agreementAndCarCardInfo.PlateNumber;
            sqlcom.Parameters["@emptyCarSize"].Value = _agreementAndCarCardInfo.EmptyCarSize;
            sqlcom.Parameters["@cargoSize"].Value = _agreementAndCarCardInfo.CargoSize;
            sqlcom.Parameters["@cargoCarSize"].Value = _agreementAndCarCardInfo.CargoCarSize;
            sqlcom.Parameters["@carShapeInfo"].Value = _agreementAndCarCardInfo.CarShapeInfo;
            sqlcom.Parameters["@emptyCarWeight"].Value = _agreementAndCarCardInfo.EmptyCarWeight;
            sqlcom.Parameters["@CarWeight"].Value = _agreementAndCarCardInfo.CarWeight;
            sqlcom.Parameters["@cargoWeight"].Value = _agreementAndCarCardInfo.CargoWeight;
            sqlcom.Parameters["@cargoDensity"].Value = _agreementAndCarCardInfo.CargoDensity;
            sqlcom.Parameters["@deductWeight"].Value = _agreementAndCarCardInfo.DeductWeight;
            sqlcom.Parameters["@carPicture1"].Value = _agreementAndCarCardInfo.CarPicture1;
            sqlcom.Parameters["@carPicture2"].Value = _agreementAndCarCardInfo.CarPicture2;
            sqlcom.Parameters["@carPicture3"].Value = _agreementAndCarCardInfo.CarPicture3;
            sqlcom.Parameters["@carPicture4"].Value = _agreementAndCarCardInfo.CarPicture4;
            sqlcom.Parameters["@carPicture5"].Value = _agreementAndCarCardInfo.CarPicture5;
            sqlcom.Parameters["@weighPlace"].Value = _agreementAndCarCardInfo.WeighPlace;
            sqlcom.Parameters["@abnormal"].Value = _agreementAndCarCardInfo.Abnormal;
            sqlcom.Parameters["@inOut"].Value = _agreementAndCarCardInfo.InOut;
            sqlcom.Parameters["@recordTime"].Value = _agreementAndCarCardInfo.RecordTime;
            sqlcom.Parameters["@firstTime"].Value = _agreementAndCarCardInfo.FirstTime;
            sqlcom.Parameters["@lastTime"].Value = _agreementAndCarCardInfo.LastTime;
        }

        /// <summary>
        /// 增加一条记录 _agreementAndCarCardInfo需要被增加的记录信息
        /// 添加成功返回TRUE，否则返回FALSE
        /// </summary>
        public Boolean Add(tb_AgreementAndCarCardInfo _agreementAndCarCardInfo)
        {
            string sqlstr = "select * from tb_AgreementAndCarCardInfo where agreementNo = '" + _agreementAndCarCardInfo.AgreementNo + "' and carCardID = '" + _agreementAndCarCardInfo.CarCardID + "';";
            SqlCommand comm = new SqlCommand(sqlstr, sqlcon);
            SqlDataReader dr = comm.ExecuteReader();
            bool temp = dr.HasRows;
            dr.Close();
            if (temp)
            {
                return false;   //已经存放对应和同和车卡的记录了
            }
            else
            {
                this._tb_AgreementAndCarCardInfo = _agreementAndCarCardInfo;
                sqlstr = "insert into tb_AgreementAndCarCardInfo values(@agreementNo,@carCardID,@plateNumber," +
                    "@emptyCarSize,@cargoSize,@cargoCarSize,@carShapeInfo,@emptyCarWeight,@CarWeight,@cargoWeight," +
                    "@cargoDensity,@deductWeight,@carPicture1,@carPicture2,@carPicture3,@carPicture4,@carPicture5," +
                    "@weighPlace,@abnormal,@inOut,@recordTime,@firstTime,@lastTime)";
                try
                {
                    SqlCommand sqlcom = new SqlCommand(sqlstr, sqlcon);
                    this.AddParameters(sqlcom);
                    this.setParametersValues(sqlcom, this._tb_AgreementAndCarCardInfo);
                    //执行添加语句 
                    sqlcom.ExecuteNonQuery();
                }
                catch (Exception e)
                {
                    throw new Exception(e.Message);
                }
            }

            return true;
        }

        /// <summary>
        /// 通过主键ID更新一条记录 将原始数据更新为_agreementAndCarCardInfo
        /// 更新成功返回TRUE 否则返回FALSE
        /// </summary>
        public Boolean UpdateByAgreementNoAndCarCardID(long _ID, tb_AgreementAndCarCardInfo _agreementAndCarCardInfo)
        {
            string sqlstr = "select * from tb_AgreementAndCarCardInfo where (ID != " + _ID + " and agreementNo = '" + _agreementAndCarCardInfo.AgreementNo + "' ) or ( ID != " + _ID + " and  carCardID = '" + _agreementAndCarCardInfo.CarCardID + "')";
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
                this._tb_AgreementAndCarCardInfo = _agreementAndCarCardInfo;
                sqlstr = "update tb_AgreementAndCarCardInfo set agreementNo=@agreementNo,carCardID=@carCardID,plateNumber=@plateNumber," +
                    "emptyCarSize=@emptyCarSize,cargoSize=@cargoSize,cargoCarSize=@cargoCarSize,carShapeInfo=@carShapeInfo," +
                    "emptyCarWeight=@emptyCarWeight,CarWeight=@CarWeight,cargoWeight=@cargoWeight,cargoDensity=@cargoDensity," +
                    "deductWeight=@deductWeight,carPicture1=@carPicture1,carPicture2=@carPicture2,carPicture3=@carPicture3,carPicture4=@carPicture4," +
                    "carPicture5=@carPicture5, weighPlace=@weighPlace, abnormal=@abnormal, inOut=@inOut, recordTime=@recordTime, " +
                    "firstTime=@firstTime, lastTime=@lastTime where ID = " + _ID;
                try
                {
                    SqlCommand sqlcom = new SqlCommand(sqlstr, sqlcon);
                    this.AddParameters(sqlcom);
                    this.setParametersValues(sqlcom, this._tb_AgreementAndCarCardInfo);
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
        /// 通过合同编号(AgreementNo)删除该合同的所有记录
        /// </summary>
        public void DeleteByAgreementNo(String _agreementNo)
        {
            String sqlstr = "delete from tb_AgreementAndCarCardInfo where agreementNo = '" + _agreementNo + "'";
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
        /// 通过合同编号(_agreementNo)和车卡ID(carCardID)删除指定记录
        /// </summary>
        public void DeleteByAgreementNoAndCarCardID(String _agreementNo, String _carCardID)
        {
            String sqlstr = "delete from tb_AgreementAndCarCardInfo where agreementNo = '" + _agreementNo + "' and carCardID = '" + _carCardID + "'";
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
        /// 通过合同编号(_agreementNo)和车卡ID(carCardID)获取一条记录 
        /// 如果没有记录则返回NULL
        /// </summary>
        public tb_AgreementAndCarCardInfo getAgreementAndCarCardInfo(String _agreementNo, String _carCardID)
        {
            String sqlstr = "select * from tb_AgreementAndCarCardInfo where agreementNo = '" + _agreementNo + "' and carCardID = '" + _carCardID + "'";
            tb_AgreementAndCarCardInfo _ret_agreementAndCarCardInfo = new tb_AgreementAndCarCardInfo();
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
                _ret_agreementAndCarCardInfo = null;
            else
            {
                _ret_agreementAndCarCardInfo.AgreementNo = _ds.Tables[0].Rows[0][1].ToString();
                _ret_agreementAndCarCardInfo.CarCardID = _ds.Tables[0].Rows[0][2].ToString();
                _ret_agreementAndCarCardInfo.PlateNumber = _ds.Tables[0].Rows[0][3].ToString();
                _ret_agreementAndCarCardInfo.EmptyCarSize = _ds.Tables[0].Rows[0][4].ToString();
                _ret_agreementAndCarCardInfo.CargoSize = _ds.Tables[0].Rows[0][5].ToString();
                _ret_agreementAndCarCardInfo.CargoCarSize = _ds.Tables[0].Rows[0][6].ToString();
                _ret_agreementAndCarCardInfo.CarShapeInfo = _ds.Tables[0].Rows[0][7].ToString();
                _ret_agreementAndCarCardInfo.EmptyCarWeight = Double.Parse(_ds.Tables[0].Rows[0][8].ToString());
                _ret_agreementAndCarCardInfo.CarWeight = Double.Parse(_ds.Tables[0].Rows[0][9].ToString());
                _ret_agreementAndCarCardInfo.CargoWeight = Double.Parse(_ds.Tables[0].Rows[0][10].ToString());
                _ret_agreementAndCarCardInfo.CargoDensity = Double.Parse(_ds.Tables[0].Rows[0][11].ToString());
                _ret_agreementAndCarCardInfo.DeductWeight = Double.Parse(_ds.Tables[0].Rows[0][12].ToString());
                _ret_agreementAndCarCardInfo.CarPicture1 = _ds.Tables[0].Rows[0][13].ToString();
                _ret_agreementAndCarCardInfo.CarPicture2 = _ds.Tables[0].Rows[0][14].ToString();
                _ret_agreementAndCarCardInfo.CarPicture3 = _ds.Tables[0].Rows[0][15].ToString();
                _ret_agreementAndCarCardInfo.CarPicture4 = _ds.Tables[0].Rows[0][16].ToString();
                _ret_agreementAndCarCardInfo.CarPicture5 = _ds.Tables[0].Rows[0][17].ToString();
                _ret_agreementAndCarCardInfo.WeighPlace = _ds.Tables[0].Rows[0][18].ToString(); 
                _ret_agreementAndCarCardInfo.Abnormal = int.Parse(_ds.Tables[0].Rows[0][19].ToString());
                _ret_agreementAndCarCardInfo.InOut = _ds.Tables[0].Rows[0][20].ToString();
                _ret_agreementAndCarCardInfo.RecordTime = DateTime.Parse(_ds.Tables[0].Rows[0][21].ToString());
                _ret_agreementAndCarCardInfo.FirstTime = DateTime.Parse(_ds.Tables[0].Rows[0][22].ToString());
                _ret_agreementAndCarCardInfo.LastTime = DateTime.Parse(_ds.Tables[0].Rows[0][23].ToString());
            }

            return _ret_agreementAndCarCardInfo;
        }

        /// <summary>
        /// 通过ID(_agreementNo)和车卡ID(carCardID)获取一条记录 
        /// 如果没有记录则返回NULL
        /// </summary>
        public tb_AgreementAndCarCardInfo getAgreementAndCarCardInfoById(String _id)
        {
            String sqlstr = "select * from tb_AgreementAndCarCardInfo where ID = " + _id;
            tb_AgreementAndCarCardInfo _ret_agreementAndCarCardInfo = new tb_AgreementAndCarCardInfo();
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
                _ret_agreementAndCarCardInfo = null;
            else
            {
                _ret_agreementAndCarCardInfo.AgreementNo = _ds.Tables[0].Rows[0][1].ToString();
                _ret_agreementAndCarCardInfo.CarCardID = _ds.Tables[0].Rows[0][2].ToString();
                _ret_agreementAndCarCardInfo.PlateNumber = _ds.Tables[0].Rows[0][3].ToString();
                _ret_agreementAndCarCardInfo.EmptyCarSize = _ds.Tables[0].Rows[0][4].ToString();
                _ret_agreementAndCarCardInfo.CargoSize = _ds.Tables[0].Rows[0][5].ToString();
                _ret_agreementAndCarCardInfo.CargoCarSize = _ds.Tables[0].Rows[0][6].ToString();
                _ret_agreementAndCarCardInfo.CarShapeInfo = _ds.Tables[0].Rows[0][7].ToString();
                _ret_agreementAndCarCardInfo.EmptyCarWeight = Double.Parse(_ds.Tables[0].Rows[0][8].ToString());
                _ret_agreementAndCarCardInfo.CarWeight = Double.Parse(_ds.Tables[0].Rows[0][9].ToString());
                _ret_agreementAndCarCardInfo.CargoWeight = Double.Parse(_ds.Tables[0].Rows[0][10].ToString());
                _ret_agreementAndCarCardInfo.CargoDensity = Double.Parse(_ds.Tables[0].Rows[0][11].ToString());
                _ret_agreementAndCarCardInfo.DeductWeight = Double.Parse(_ds.Tables[0].Rows[0][12].ToString());
                _ret_agreementAndCarCardInfo.CarPicture1 = _ds.Tables[0].Rows[0][13].ToString();
                _ret_agreementAndCarCardInfo.CarPicture2 = _ds.Tables[0].Rows[0][14].ToString();
                _ret_agreementAndCarCardInfo.CarPicture3 = _ds.Tables[0].Rows[0][15].ToString();
                _ret_agreementAndCarCardInfo.CarPicture4 = _ds.Tables[0].Rows[0][16].ToString();
                _ret_agreementAndCarCardInfo.CarPicture5 = _ds.Tables[0].Rows[0][17].ToString();
                _ret_agreementAndCarCardInfo.WeighPlace = _ds.Tables[0].Rows[0][18].ToString();
                _ret_agreementAndCarCardInfo.Abnormal = int.Parse(_ds.Tables[0].Rows[0][19].ToString());
                _ret_agreementAndCarCardInfo.InOut = _ds.Tables[0].Rows[0][20].ToString();
                _ret_agreementAndCarCardInfo.RecordTime = DateTime.Parse(_ds.Tables[0].Rows[0][21].ToString());
                _ret_agreementAndCarCardInfo.FirstTime = DateTime.Parse(_ds.Tables[0].Rows[0][22].ToString());
                _ret_agreementAndCarCardInfo.LastTime = DateTime.Parse(_ds.Tables[0].Rows[0][23].ToString());
            }

            return _ret_agreementAndCarCardInfo;
        }

        /// <summary>
        /// 获取tb_AgreementAndCarCardInfo表中的所有数据
        /// </summary>
        public DataSet getAllAgreementAndCarCardInfo()
        {
            String sqlstr = "select * from tb_AgreementAndCarCardInfo";
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

        /// <summary>
        /// 通过合同编号来获取相应的合同信息
        /// </summary>
        public DataSet getAgreementAndCarCardInfoByAgreementNo(string m_agreementNo)
        {
            String sqlstr = "select * from tb_AgreementAndCarCardInfo where agreementNo='" + m_agreementNo + "'";
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