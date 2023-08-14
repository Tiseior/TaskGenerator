#pragma once


// CTaskRedactionDlg dialog

class CTaskRedactionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTaskRedactionDlg)

public:
	CTaskRedactionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTaskRedactionDlg();

// Dialog Data
	enum { IDD = IDD_TASKREDACTION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	CWinThread* m_pThread; // Хендл Tриады
	CToolTipCtrl m_wndTipContrl;
	BOOL PreTranslateMessage(MSG*);

	DECLARE_MESSAGE_MAP()

public:
	CStringArray taskFiles; // Глобальная переменная с названиями файлов и текстами задач
	CEdit ed_text;
	CButton bt_ok;
	CButton bt_refresh;
	CButton bt_add;
	CButton bt_del;
	CButton bt_change;
	CStatic st_mes;
	CStatic st_len;
	CStatic st_mesLen;
	CComboBox cb_task;
	CFont ed_font;     // Шрифт для Edit
	CFont bt_font;     // Шрифт для Button
	CFont btWork_font; // Шрифт для рабочих Button
	CFont st_font;     // Шрифт для Static
	void OnTaskChanged();
	void GetTasks();
	UINT TaskNumber(CString);
	void ListSort(CList <UINT>&);
	void RefreshMes();
	afx_msg HBRUSH OnCtlColor(CDC*, CWnd *, UINT);
	afx_msg void OnOK();
	afx_msg void OnBnClickedRefresh();
	afx_msg void OnBnClickedAddtask();
	afx_msg void OnBnClickedDeltask();
	afx_msg void OnBnClickedChangetask();
};

UINT StreamFunction(LPVOID);
extern BOOL stopThread; // Индикатор работы Триады
