// LoaderDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Loader.h"
#include "LoaderDlg.h"
#include <Tlhelp32.h>
#include "process_thread_func.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WORDMAXLEN 0x5000
HWND my_hwnd = NULL;


// CLoaderDlg 对话框

#pragma comment(lib,"taskmgr_hook.lib")
_declspec(dllimport) DWORD starthook(DWORD hthread);


CLoaderDlg::CLoaderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoaderDlg::IDD, pParent)
    , text_words(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLoaderDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_WORDS, edit_words);
    DDX_Text(pDX, IDC_WORDS, text_words);
}

BEGIN_MESSAGE_MAP(CLoaderDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CLoaderDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLoaderDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &CLoaderDlg::OnBnClickedButton1)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CLoaderDlg 消息处理程序

BOOL CLoaderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
    my_hwnd = m_hWnd;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CLoaderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CLoaderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CLoaderDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//OnOK();
}

void CLoaderDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//OnCancel();
}

//////////////////////////////////////////////////////////////////////////
//void appendWords(char * Astr)
//{
//    char * s = new char [WORDMAXLEN];
//    memset(s, 0, WORDMAXLEN);
//    GetDlgItemTextA(my_hwnd, IDC_WORDS, s, WORDMAXLEN);
//    if (0 != s[WORDMAXLEN - 1])
//    {
//        memset(s, 0, WORDMAXLEN);
//    }
//    strcat_s(s, WORDMAXLEN - strlen(s), Astr);
//    strcat_s(s, WORDMAXLEN - strlen(s), "\r\n");
//    ::SetDlgItemTextA(my_hwnd,IDC_WORDS, s);
//}
int CLoaderDlg::appendWords(char * Astr)
{
    UpdateData(TRUE);
    if (WORDMAXLEN <= text_words.GetLength() + strlen(Astr))
    {
        text_words = Astr;
        text_words += "\r\n";
    }
    else
    {
        text_words += Astr;
        text_words += "\r\n";
    }

    UpdateData(FALSE);
    edit_words.LineScroll(edit_words.GetLineCount());
    return 0;
}


//////////////////////////////////////////////////////////////////////////

void CLoaderDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
    DWORD pid = 0, tid = 0;
    DWORD ok = 0;

    tid = GetWindowThreadByClass("Notepad");
    if ( 0 == tid )
    {
        pid = GetProcessIDByName("taskmgr.exe");
        if (0 == pid)
        {
            appendWords("There's no such process!");
            return;
        }
        else 
        {
            if (pid < 0)
            {
                appendWords("There's more than one process!");
                return;
            }
            else
            {
                tid = GetProcessWindowThread(pid);
                if ( !tid )
                {
                    appendWords("Get window thread error!");
                    return;
                }
            }
        }
    }

    if ( !tid )
    {
        appendWords("Window thread not found!");
    }

	while(!ok)
	{
		ok = starthook((DWORD)tid);
        Sleep(500);
        if (!ok)
        {
            appendWords("hook failed!");
        }
	}
    if (ok)
    {
        appendWords("hooked!");
    }
}

void CLoaderDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	//CDialog::OnClose();
	OnCancel();
}


