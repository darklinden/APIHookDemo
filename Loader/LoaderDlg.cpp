// LoaderDlg.cpp : ʵ���ļ�
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


// CLoaderDlg �Ի���

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


// CLoaderDlg ��Ϣ�������

BOOL CLoaderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
    my_hwnd = m_hWnd;
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CLoaderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CLoaderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CLoaderDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//OnOK();
}

void CLoaderDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	//CDialog::OnClose();
	OnCancel();
}


