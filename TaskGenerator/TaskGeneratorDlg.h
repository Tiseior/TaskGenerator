
// TaskGeneratorDlg.h : файл заголовка
//

#pragma once


// диалоговое окно CTaskGeneratorDlg
class CTaskGeneratorDlg : public CDialogEx
{
// Создание
public:
	CTaskGeneratorDlg(CWnd* pParent = NULL);	// стандартный конструктор

// Данные диалогового окна
	enum { IDD = IDD_TASKGENERATOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV

// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CStringArray taskTexts; // Глобальная переменная с текстами задач
	CEdit ed_text;
	CStatic st_mes;
	CButton bt_gtask;
	CButton bt_ok;
	CButton bt_redact;
	CFont st_font; // Шрифт для Static
	CFont ed_font; // Шрифт для Edit
	CFont bt_font; // Шрифт для Button
	HBRUSH OnCtlColor(CDC*, CWnd *, UINT);
	BOOL FileNameCheck(CString);
	void GetTasks();
	void RefreshMes();
	afx_msg void OnBnClickedGtask();
	afx_msg void OnBnClickedTaskredact();
};
