
// TaskGenerator.h : ������� ���� ��������� ��� ���������� PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "resource.h"		// �������� �������


// CTaskGeneratorApp:
// � ���������� ������� ������ ��. TaskGenerator.cpp
//

class CTaskGeneratorApp : public CWinApp
{
public:
	CTaskGeneratorApp();

// ���������������
public:
	virtual BOOL InitInstance();

// ����������

	DECLARE_MESSAGE_MAP()
};

extern CTaskGeneratorApp theApp;