//taskmgr_hook.h : taskmgr_hook DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Ctaskmgr_hookApp
// �йش���ʵ�ֵ���Ϣ������� taskmgr_hook.cpp
//

class Ctaskmgr_hookApp : public CWinApp
{
public:
	Ctaskmgr_hookApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
    virtual int ExitInstance();
};
