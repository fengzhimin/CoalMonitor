using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using BSCode.Tool;
using BSCode.DataBaseEntity;
using BSCode.DataBaseOperation;

namespace BSCode.Code
{
    public partial class Login_Page : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            OperateAgreementAndCarCardInfo _temp = new OperateAgreementAndCarCardInfo();
        }

        protected void Login_Click(object sender, EventArgs e)
        {
            this.AlertLb.Text = "";
            string login_name = username.Value.ToString();
            string login_passwd = pwd.Value.ToString();
            ValidateEntity _validateEntity = new ValidateEntity();
            if (_validateEntity.LoginValidation(login_name, login_passwd) == 1)
            {
                //登录成功
                Session["loginName"] = login_name;
                Session["loginPasswd"] = login_passwd;
                Session["Power"] = 1;
                Response.Redirect("Main_Page.aspx", true);
            }
            else if (_validateEntity.LoginValidation(login_name, login_passwd) == 2)
            {
                //登录成功
                Session["loginName"] = login_name;
                Session["loginPasswd"] = login_passwd;
                Session["Power"] = 2;
                Response.Redirect("./Table_List/CarCheckInfoList_Page.aspx", true);
            }
            else if (_validateEntity.LoginValidation(login_name, login_passwd) == 3)
            {
                //登录成功
                Session["loginName"] = login_name;
                Session["loginPasswd"] = login_passwd;
                Session["Power"] = 3;
                Response.Redirect("Main_Page.aspx", true);
            }
            else
            {
                this.AlertLb.Text = "用户名或密码错误！";
            }
        }
    }
}