
// TaskGeneratorDlg.h : ���� ���������
//

#pragma once


// ���������� ���� CTaskGeneratorDlg
class CTaskGeneratorDlg : public CDialogEx
{
// ��������
public:
	CTaskGeneratorDlg(CWnd* pParent = NULL);	// ����������� �����������

// ������ ����������� ����
	enum { IDD = IDD_TASKGENERATOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// ��������� DDX/DDV

// ����������
protected:
	HICON m_hIcon;

	// ��������� ������� ����� ���������
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CStringArray taskTexts; // ���������� ���������� � �������� �����
	CEdit ed_text;
	CStatic st_mes;
	CButton bt_gtask;
	CButton bt_ok;
	CButton bt_redact;
	CFont st_font; // ����� ��� Static
	CFont ed_font; // ����� ��� Edit
	CFont bt_font; // ����� ��� Button
	HBRUSH OnCtlColor(CDC*, CWnd *, UINT);
	BOOL FileNameCheck(CString);
	void GetTasks();
	void RefreshMes();
	afx_msg void OnBnClickedGtask();
	afx_msg void OnBnClickedTaskredact();
};
