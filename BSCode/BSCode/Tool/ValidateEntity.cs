using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Configuration;
using System.Data.SqlClient;
using System.Data;
using BSCode.DataBaseEntity;
using BSCode.DataBaseOperation;
using System.Web.UI;
using System.Web.UI.WebControls;
//author: fzm
//function: 验证实体类

namespace BSCode.Tool
{
    /// <summary>
    /// 验证方法类
    /// </summary>
    public class ValidateEntity
    {
        //author: fzm
        /// <summary> 
        /// 功能：验证用户登录信息
        /// 返回结果: 用户名和密码正确，返回true 否则false
        /// </summary>
        OperateMerchantInfo m_OperateMerchantInfo = new OperateMerchantInfo();
        public int LoginValidation(String _loginName, String _loginPasswd)
        {
            if (String.IsNullOrEmpty(_loginName) || String.IsNullOrEmpty(_loginPasswd))
                return 0;
            else if (_loginName.Equals("root") && _loginPasswd.Equals("root"))
                return 1;
            else if (_loginName.Equals("check") && _loginPasswd.Equals("check"))
                return 2;
            else if (m_OperateMerchantInfo.JudgeMerchantInfo(_loginName, _loginPasswd))
                return 3;
            else
                return 0;
        }

        //author: fzm
        /// <summary> 
        /// 功能：链接数据库代码
        /// </summary>
        public void linkDataBase()
        {
            //判断登录信息
            string strcon = ConfigurationManager.ConnectionStrings["sqlstr"].ConnectionString;
            SqlConnection sqlcnn = new SqlConnection(strcon);
            // 打开连接
            if (sqlcnn.State == ConnectionState.Closed)
            {
                try
                {
                    sqlcnn.Open();
                }
                catch
                {
                    //Response.Redirect("/404.html");
                }
            }
        }
    }
}