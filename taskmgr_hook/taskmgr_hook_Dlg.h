#pragma once


// taskmgr_hook_Dlg 对话框

class taskmgr_hook_Dlg : public CDialog
{
	DECLARE_DYNAMIC(taskmgr_hook_Dlg)

public:
	taskmgr_hook_Dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~taskmgr_hook_Dlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnClose();
    afx_msg void OnBnClickedButton1();
    CString text_text;
    virtual BOOL OnInitDialog();
};
