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
    /// 对tb_CarCheckInfo数据库进行增删改查
    /// </summary>
    public class OperateCarCheckInfo
    {
        private string strcon = ConfigurationManager.ConnectionStrings["sqlstr"].ConnectionString;
        public SqlConnection sqlcon;
        private tb_CarCheckInfo _tb_carCheckInfo;

        //构造函数
        public OperateCarCheckInfo()
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
        ~OperateCarCheckInfo()
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
        }

        /// <summary>
        /// 参数赋值
        /// </summary>
        public void setParametersValues(SqlCommand sqlcom, tb_CarCheckInfo _carCheckInfo)
        {
            //给参数赋值 
            sqlcom.Parameters["@agreementNo"].Value = _carCheckInfo.AgreementNo;
            sqlcom.Parameters["@carCardID"].Value = _carCheckInfo.CarCardID;
            sqlcom.Parameters["@plateNumber"].Value = _carCheckInfo.PlateNumber;
            sqlcom.Parameters["@emptyCarSize"].Value = _carCheckInfo.EmptyCarSize;
            sqlcom.Parameters["@cargoSize"].Value = _carCheckInfo.CargoSize;
            sqlcom.Parameters["@cargoCarSize"].Value = _carCheckInfo.CargoCarSize;
            sqlcom.Parameters["@emptyCarWeight"].Value = _carCheckInfo.EmptyCarWeight;
            sqlcom.Parameters["@CarWeight"].Value = _carCheckInfo.CarWeight;
            sqlcom.Parameters["@cargoWeight"].Value = _carCheckInfo.CargoWeight;
            sqlcom.Parameters["@cargoDensity"].Value = _carCheckInfo.CargoDensity;
            sqlcom.Parameters["@deductWeight"].Value = _carCheckInfo.DeductWeight;
            sqlcom.Parameters["@carPicture1"].Value = _carCheckInfo.CarPicture1;
            sqlcom.Parameters["@carPicture2"].Value = _carCheckInfo.CarPicture2;
            sqlcom.Parameters["@carPicture3"].Value = _carCheckInfo.CarPicture3;
            sqlcom.Parameters["@carPicture4"].Value = _carCheckInfo.CarPicture4;
            sqlcom.Parameters["@carPicture5"].Value = _carCheckInfo.CarPicture5;
            sqlcom.Parameters["@weighPlace"].Value = _carCheckInfo.WeighPlace;
            sqlcom.Parameters["@abnormal"].Value = _carCheckInfo.Abnormal;
            sqlcom.Parameters["@inOut"].Value = _carCheckInfo.InOut;
            sqlcom.Parameters["@recordTime"].Value = _carCheckInfo.RecordTime;
        }

        /// <summary>
        /// 增加一条记录 _carCheckInfo需要被增加的记录信息
        /// 添加成功返回TRUE，否则返回FALSE
        /// </summary>
        public Boolean Add(tb_CarCheckInfo _carCheckInfo)
        {
            string sqlstr = "select * from tb_CarCheckInfo where carCardID = '" + _carCheckInfo.CarCardID + "';";
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
                this._tb_carCheckInfo = _carCheckInfo;
                sqlstr = "insert into tb_CarCheckInfo values(@agreementNo,@carCardID,@plateNumber," +
                    "@emptyCarSize,@cargoSize,@cargoCarSize,@emptyCarWeight,@CarWeight,@cargoWeight," +
                    "@cargoDensity,@deductWeight,@carPicture1,@carPicture2,@carPicture3,@carPicture4,@carPicture5," +
                    "@weighPlace,@abnormal,@inOut,@recordTime)";
                try
                {
                    SqlCommand sqlcom = new SqlCommand(sqlstr, sqlcon);
                    this.AddParameters(sqlcom);
                    this.setParametersValues(sqlcom, this._tb_carCheckInfo);
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
        /// 通过主键ID更新一条记录 将原始数据更新为_carCheckInfo
        /// 更新成功返回TRUE 否则返回FALSE
        /// </summary>
        public Boolean UpdateByCarCardID(long _ID, tb_CarCheckInfo _carCheckInfo)
        {
            string sqlstr = "select * from tb_CarCheckInfo where ID != " + _ID + " and carCardID = '" + _carCheckInfo.CarCardID + "'";
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
                this._tb_carCheckInfo = _carCheckInfo;
                sqlstr = "update tb_CarCheckInfo set agreementNo=@agreementNo, carCardID=@carCardID," +
                    "plateNumber=@plateNumber,emptyCarSize=@emptyCarSize,cargoSize=@cargoSize,cargoCarSize=@cargoCarSize," +
                    "emptyCarWeight=@emptyCarWeight,CarWeight=@CarWeight,cargoWeight=@cargoWeight,cargoDensity=@cargoDensity," +
                    "deductWeight=@deductWeight,carPicture1=@carPicture1,carPicture2=@carPicture2,carPicture3=@carPicture3,carPicture4=@carPicture4," +
                    "carPicture5=@carPicture5, weighPlace=@weighPlace, abnormal=@abnormal, inOut=@inOut, recordTime=@recordTime where ID = " + _ID;
                try
                {
                    SqlCommand sqlcom = new SqlCommand(sqlstr, sqlcon);
                    this.AddParameters(sqlcom);
                    this.setParametersValues(sqlcom, this._tb_carCheckInfo);
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
        /// 通过车卡ID(_carCardID)删除记录
        /// </summary>
        public void DeleteByCarCardID(String _carCardID)
        {
            String sqlstr = "delete from tb_CarCheckInfo where carCardID = '" + _carCardID + "'";
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
        /// 通过车卡ID(carCardID)获取一条记录 
        /// 如果没有记录则返回NULL
        /// </summary>
        public tb_CarCheckInfo getCarCheckInfo(String _carCardID)
        {
            String sqlstr = "select * from tb_CarCheckInfo where carCardID = '" + _carCardID + "'";
            tb_CarCheckInfo _ret_carCheckInfo = new tb_CarCheckInfo();
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
                _ret_carCheckInfo = null;
            else
            {
                _ret_carCheckInfo.AgreementNo = _ds.Tables[0].Rows[0][1].ToString();
                _ret_carCheckInfo.CarCardID = _ds.Tables[0].Rows[0][2].ToString();
                _ret_carCheckInfo.PlateNumber = _ds.Tables[0].Rows[0][3].ToString();
                _ret_carCheckInfo.EmptyCarSize = _ds.Tables[0].Rows[0][4].ToString();
                _ret_carCheckInfo.CargoSize = _ds.Tables[0].Rows[0][5].ToString();
                _ret_carCheckInfo.CargoCarSize = _ds.Tables[0].Rows[0][6].ToString();
                _ret_carCheckInfo.EmptyCarWeight = Double.Parse(_ds.Tables[0].Rows[0][7].ToString());
                _ret_carCheckInfo.CarWeight = Double.Parse(_ds.Tables[0].Rows[0][8].ToString());
                _ret_carCheckInfo.CargoWeight = Double.Parse(_ds.Tables[0].Rows[0][9].ToString());
                _ret_carCheckInfo.CargoDensity = Double.Parse(_ds.Tables[0].Rows[0][10].ToString());
                _ret_carCheckInfo.DeductWeight = Double.Parse(_ds.Tables[0].Rows[0][11].ToString());
                _ret_carCheckInfo.CarPicture1 = _ds.Tables[0].Rows[0][12].ToString();
                _ret_carCheckInfo.CarPicture2 = _ds.Tables[0].Rows[0][13].ToString();
                _ret_carCheckInfo.CarPicture3 = _ds.Tables[0].Rows[0][14].ToString();
                _ret_carCheckInfo.CarPicture4 = _ds.Tables[0].Rows[0][15].ToString();
                _ret_carCheckInfo.CarPicture5 = _ds.Tables[0].Rows[0][16].ToString();
                _ret_carCheckInfo.WeighPlace = _ds.Tables[0].Rows[0][17].ToString(); 
                _ret_carCheckInfo.Abnormal = int.Parse(_ds.Tables[0].Rows[0][18].ToString());
                _ret_carCheckInfo.InOut = _ds.Tables[0].Rows[0][19].ToString();
                _ret_carCheckInfo.RecordTime = DateTime.Parse(_ds.Tables[0].Rows[0][20].ToString());
            }

            return _ret_carCheckInfo;
        }
        public tb_CarCheckInfo getCarCheckInfoByID(String id)
        {
            String sqlstr = "select * from tb_CarCheckInfo where ID = " + id;
            tb_CarCheckInfo _ret_carCheckInfo = new tb_CarCheckInfo();
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
                _ret_carCheckInfo = null;
            else
            {
                _ret_carCheckInfo.AgreementNo = _ds.Tables[0].Rows[0][1].ToString();
                _ret_carCheckInfo.CarCardID = _ds.Tables[0].Rows[0][2].ToString();
                _ret_carCheckInfo.PlateNumber = _ds.Tables[0].Rows[0][3].ToString();
                _ret_carCheckInfo.EmptyCarSize = _ds.Tables[0].Rows[0][4].ToString();
                _ret_carCheckInfo.CargoSize = _ds.Tables[0].Rows[0][5].ToString();
                _ret_carCheckInfo.CargoCarSize = _ds.Tables[0].Rows[0][6].ToString();
                _ret_carCheckInfo.EmptyCarWeight = Double.Parse(_ds.Tables[0].Rows[0][7].ToString());
                _ret_carCheckInfo.CarWeight = Double.Parse(_ds.Tables[0].Rows[0][8].ToString());
                _ret_carCheckInfo.CargoWeight = Double.Parse(_ds.Tables[0].Rows[0][9].ToString());
                _ret_carCheckInfo.CargoDensity = Double.Parse(_ds.Tables[0].Rows[0][10].ToString());
                _ret_carCheckInfo.DeductWeight = Double.Parse(_ds.Tables[0].Rows[0][11].ToString());
                _ret_carCheckInfo.CarPicture1 = _ds.Tables[0].Rows[0][12].ToString();
                _ret_carCheckInfo.CarPicture2 = _ds.Tables[0].Rows[0][13].ToString();
                _ret_carCheckInfo.CarPicture3 = _ds.Tables[0].Rows[0][14].ToString();
                _ret_carCheckInfo.CarPicture4 = _ds.Tables[0].Rows[0][15].ToString();
                _ret_carCheckInfo.CarPicture5 = _ds.Tables[0].Rows[0][16].ToString();
                _ret_carCheckInfo.WeighPlace = _ds.Tables[0].Rows[0][17].ToString();
                _ret_carCheckInfo.Abnormal = int.Parse(_ds.Tables[0].Rows[0][18].ToString());
                _ret_carCheckInfo.InOut = _ds.Tables[0].Rows[0][19].ToString();
                _ret_carCheckInfo.RecordTime = DateTime.Parse(_ds.Tables[0].Rows[0][20].ToString()); 
            }

            return _ret_carCheckInfo;
        }

        /// <summary>
        /// 获取tb_CarCheckInfo表中的所有数据
        /// </summary>
        public DataSet getAllCarCheckInfo()
        {
            String sqlstr = "select * from tb_CarCheckInfo";
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