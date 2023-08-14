
// TaskGenerator.h : главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CTaskGeneratorApp:
// О реализации данного класса см. TaskGenerator.cpp
//

class CTaskGeneratorApp : public CWinApp
{
public:
	CTaskGeneratorApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CTaskGeneratorApp theApp;