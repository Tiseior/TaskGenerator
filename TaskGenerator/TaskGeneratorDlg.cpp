
// TaskGeneratorDlg.cpp : ���� ����������
//

#include "stdafx.h"
#include "TaskGenerator.h"
#include "TaskGeneratorDlg.h"
#include "TaskRedactionDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MaxLength 2000 // ������������ ������ ������ � ��������

// ���������� ���� CAboutDlg ������������ ��� �������� �������� � ����������

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ������ ����������� ����
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV

// ����������
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// ���������� ���� CTaskGeneratorDlg



CTaskGeneratorDlg::CTaskGeneratorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTaskGeneratorDlg::IDD, pParent)
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hIcon = AfxGetApp()->LoadIcon(IDI_TG);
}

void CTaskGeneratorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TASK_TEXT, ed_text);
	DDX_Control(pDX, IDC_MESSAGE, st_mes);
	DDX_Control(pDX, IDC_GTASK, bt_gtask);
	DDX_Control(pDX, IDOK, bt_ok);
	DDX_Control(pDX, IDC_TASKREDACT, bt_redact);
}

BEGIN_MESSAGE_MAP(CTaskGeneratorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_GTASK, &CTaskGeneratorDlg::OnBnClickedGtask)
	ON_BN_CLICKED(IDC_TASKREDACT, &CTaskGeneratorDlg::OnBnClickedTaskredact)
END_MESSAGE_MAP()


// ����������� ��������� CTaskGeneratorDlg

BOOL CTaskGeneratorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���������� ������ "� ���������..." � ��������� ����.

	// IDM_ABOUTBOX ������ ���� � �������� ��������� �������.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ������ ������ ��� ����� ����������� ����. ����� ������ ��� �������������,
	//  ���� ������� ���� ���������� �� �������� ����������
	SetIcon(m_hIcon, TRUE);			// ������� ������
	SetIcon(m_hIcon, FALSE);		// ������ ������
 
	// ������� ������� �������
	st_font.CreatePointFont(90, _T(""));
	st_mes.SetFont(&st_font);
	ed_font.CreatePointFont(110, _T(""));
	ed_text.SetFont(&ed_font);
	bt_font.CreatePointFont(85, _T(""));
	bt_gtask.SetFont(&bt_font);
	bt_ok.SetFont(&bt_font);
	bt_redact.SetFont(&bt_font);

	// TODO: �������� �������������� �������������

	// ��������, ���� �� ����� � ���������, ���� ���, �� ������� �
	if (GetFileAttributes("Tasks") == INVALID_FILE_ATTRIBUTES)
		CreateDirectory("Tasks", NULL);

	GetTasks();

	return TRUE;  // ������� �������� TRUE, ���� ����� �� ������� �������� ����������
}

void CTaskGeneratorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��� ���������� ������ ����������� � ���������� ���� ����� ��������������� ����������� ���� �����,
//  ����� ���������� ������. ��� ���������� MFC, ������������ ������ ���������� ��� �������������,
//  ��� ������������� ����������� ������� ��������.

void CTaskGeneratorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �������� ���������� ��� ���������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ������������ ������ �� ������ ����������� ��������������
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ��������� ������
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ������� �������� ��� ������� ��� ��������� ����������� ������� ��� �����������
//  ���������� ����.
HCURSOR CTaskGeneratorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HBRUSH CTaskGeneratorDlg::OnCtlColor(CDC* pDC, CWnd *pWnd, UINT nCtlColor)
{
    switch (nCtlColor)
    {
		case CTLCOLOR_STATIC: // ��������� ����������� ���������
			pDC->SetTextColor(RGB(255, 0, 0));
			pDC->SetBkMode(TRANSPARENT);
			return (HBRUSH)GetStockObject(NULL_BRUSH);
		default:
			return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    }
}

void CTaskGeneratorDlg::OnBnClickedGtask() {
	this->st_mes.SetWindowText("");
	UINT num = taskTexts.GetCount();
	if(num != 0) {
		this->ed_text.SetWindowText(taskTexts[rand()%num]);
	} else {
		this->st_mes.SetWindowText("������ �� �������.");
	}
	RefreshMes();
}

// ��������� �������� �����, ���� �� �������������� �������
BOOL CTaskGeneratorDlg::FileNameCheck(CString file) {
	file.Replace(_T("Task"),_T(""));
	file.Replace(_T(".txt"),_T(""));
	if (atoi(file))
		return 1;
	else 
		return 0;
}


void CTaskGeneratorDlg::OnBnClickedTaskredact()
{
	CTaskRedactionDlg taskRedactor;
	this->ed_text.SetWindowText("");
	taskRedactor.DoModal(); // �������� ��������� ����, ������� ����� ������� ������ ��� ���������� ������
	GetTasks();
}

void CTaskGeneratorDlg::GetTasks() {
	if(taskTexts.GetCount() != 0) {
			taskTexts.RemoveAll();
	}
	HANDLE hSearch = NULL;
	WIN32_FIND_DATA wfd;
	CFile taskFile;
	CFileException fileException;
	CString mask = "Tasks\\Task*.txt";
	CString file = "";
	hSearch = FindFirstFile(mask, &wfd);
	if (hSearch != INVALID_HANDLE_VALUE) {
		// ������������ ���� ���������� ������ � ��������
		if (FileNameCheck(wfd.cFileName)) {
			file = wfd.cFileName;
			if(!taskFile.Open("Tasks\\"+file, CFile::modeRead, &fileException)) {
				this->st_mes.SetWindowText("�� ������� ������� ���� � �������: "+file);
			} else {
				char taskTxt [MaxLength]; // ������������ ������ ������ � ��������
				memset(taskTxt, 0, sizeof(taskTxt));
				taskFile.Read(taskTxt, sizeof(taskTxt));
				taskTexts.Add(taskTxt);
				taskFile.Close();
			}
		}
		while (FindNextFile(hSearch, &wfd)) {
			if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				continue;
			if (FileNameCheck(wfd.cFileName)) {
				file = wfd.cFileName;
				if(!taskFile.Open("Tasks\\"+file, CFile::modeRead, &fileException)) {
					this->st_mes.SetWindowText("�� ������� ������� ���� � �������: "+file);
				} else {
					char taskTxt [MaxLength]; // ������������ ������ ������ � ��������
					memset(taskTxt, 0, sizeof(taskTxt));
					taskFile.Read(taskTxt, sizeof(taskTxt));
					taskTexts.Add(taskTxt);
					taskFile.Close();
				}
			}
		}
		FindClose(hSearch);
		this->st_mes.SetWindowText("");
	}
	else {
		this->st_mes.SetWindowText("������ �� �������.");
	}
	RefreshMes();
}

// ����� ����� � ����������� �������� �� ������������
void CTaskGeneratorDlg::RefreshMes() {
	CRect rect;
	st_mes.GetWindowRect(&rect);
	ScreenToClient(&rect);
	InvalidateRect(&rect);
	UpdateWindow();
}