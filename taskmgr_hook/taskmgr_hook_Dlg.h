#pragma once


// taskmgr_hook_Dlg �Ի���

class taskmgr_hook_Dlg : public CDialog
{
	DECLARE_DYNAMIC(taskmgr_hook_Dlg)

public:
	taskmgr_hook_Dlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~taskmgr_hook_Dlg();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnClose();
    afx_msg void OnBnClickedButton1();
    CString text_text;
    virtual BOOL OnInitDialog();
};
