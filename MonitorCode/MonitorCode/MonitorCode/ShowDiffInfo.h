#pragma once

#include "OperateDataBase.h"

// ShowDiffInfo 对话框

class ShowDiffInfo : public CDialogEx
{
	DECLARE_DYNAMIC(ShowDiffInfo)

public:
	ShowDiffInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ShowDiffInfo();

// 对话框数据
	enum { IDD = IDD_SHOW_DIFFERENT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedConfireButton();
	afx_msg void OnBnClickedCancelButton();
	virtual BOOL OnInitDialog();
	CString m_style_string;
	CString m_style;
	CString m_carCard_string;
	CString m_carCard;
	CString m_agreementA_string;
	CString m_agreementA;
	CString m_agreementB_string;
	CString m_agreementB;
	CString m_cargoSize_string1;
	CString m_cargoSize1;
	CString m_cargoSize_string2;
	CString m_cargoSize2;
	CString m_cargoSize_string3;
	CString m_cargoWeight_string1;
	CString m_cargoWeight1;
	CString m_cargoWeight_string2;
	CString m_cargoWeight2;
	CString m_cargoWeight_string3;
	CString m_cargoDensity_string1;
	CString m_cargoDensity1;
	CString m_cargoDensity_string2;
	CString m_cargoDensity2;
	CString m_cargoDensity_string3;
	CString m_deductWeight_string1;
	CString m_deductWeight1;
	CString m_deductWeight_string2;
	CString m_deductWeight2;
	CString m_deductWeight_string3;
	CString m_emptyCarSize_string1;
	CString m_emptyCarSize1;
	CString m_emptyCarSize_string2;
	CString m_emptyCarSize2;
	CString m_emptyCarSize_string3;
	CString m_carCargoSize_string1;
	CString m_carCargoSize1;
	CString m_carCargoSize_string2;
	CString m_carCargoSize2;
	CString m_carCargoSize_string3;
	CString m_emptyCarWeight_string1;
	CString m_emptyCarWeight1;
	CString m_emptyCarWeight_string2;
	CString m_emptyCarWeight2;
	CString m_emptyCarWeight_string3;
	CString m_carWeight_string1;
	CString m_carWeight1;
	CString m_carWeight_string2;
	CString m_carWeight2;
	CString m_carWeight_string3;
	CFont m_font_static, m_font_edit;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
