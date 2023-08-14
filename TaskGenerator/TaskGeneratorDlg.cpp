
// TaskGeneratorDlg.cpp : файл реализации
//

#include "stdafx.h"
#include "TaskGenerator.h"
#include "TaskGeneratorDlg.h"
#include "TaskRedactionDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MaxLength 2000 // Максимальный размер задачи в символах

// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
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


// диалоговое окно CTaskGeneratorDlg



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


// обработчики сообщений CTaskGeneratorDlg

BOOL CTaskGeneratorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
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

	// Задает значок для этого диалогового окна. Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок
 
	// Задание размеры шрифтов
	st_font.CreatePointFont(90, _T(""));
	st_mes.SetFont(&st_font);
	ed_font.CreatePointFont(110, _T(""));
	ed_text.SetFont(&ed_font);
	bt_font.CreatePointFont(85, _T(""));
	bt_gtask.SetFont(&bt_font);
	bt_ok.SetFont(&bt_font);
	bt_redact.SetFont(&bt_font);

	// TODO: добавьте дополнительную инициализацию

	// Проверка, есть ли папка с заданиями, если нет, то создать её
	if (GetFileAttributes("Tasks") == INVALID_FILE_ATTRIBUTES)
		CreateDirectory("Tasks", NULL);

	GetTasks();

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
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

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок. Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CTaskGeneratorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CTaskGeneratorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HBRUSH CTaskGeneratorDlg::OnCtlColor(CDC* pDC, CWnd *pWnd, UINT nCtlColor)
{
    switch (nCtlColor)
    {
		case CTLCOLOR_STATIC: // Изменение статических элементов
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
		this->st_mes.SetWindowText("Задачи не найдены.");
	}
	RefreshMes();
}

// Обработка названия файла, чтоб он соответствовал шаблону
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
	taskRedactor.DoModal(); // Создаётся модальное окно, которое нужно закрыть прежде чем продолжить работу
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
		// Вытаскивание всех подходящих файлов с задачами
		if (FileNameCheck(wfd.cFileName)) {
			file = wfd.cFileName;
			if(!taskFile.Open("Tasks\\"+file, CFile::modeRead, &fileException)) {
				this->st_mes.SetWindowText("Не удалось открыть файл с задачей: "+file);
			} else {
				char taskTxt [MaxLength]; // Максимальный размер задачи в символах
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
					this->st_mes.SetWindowText("Не удалось открыть файл с задачей: "+file);
				} else {
					char taskTxt [MaxLength]; // Максимальный размер задачи в символах
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
		this->st_mes.SetWindowText("Задачи не найдены.");
	}
	RefreshMes();
}

// Чтобы текст в статическом элементе не накапливался
void CTaskGeneratorDlg::RefreshMes() {
	CRect rect;
	st_mes.GetWindowRect(&rect);
	ScreenToClient(&rect);
	InvalidateRect(&rect);
	UpdateWindow();
}