// TaskRedactionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TaskGenerator.h"
#include "TaskGeneratorDlg.h"
#include "TaskRedactionDlg.h"
#include "afxdialogex.h"

#define MaxLength 2000 // Максимальный размер задачи в символах

BOOL stopThread;

// CTaskRedactionDlg dialog

IMPLEMENT_DYNAMIC(CTaskRedactionDlg, CDialogEx)

CTaskRedactionDlg::CTaskRedactionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTaskRedactionDlg::IDD, pParent)
{
	m_pThread = NULL;
}

CTaskRedactionDlg::~CTaskRedactionDlg()
{
}

void CTaskRedactionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TASK_TEXT, ed_text);
	DDX_Control(pDX, IDOK, bt_ok);
	DDX_Control(pDX, IDC_REFRESH, bt_refresh);
	DDX_Control(pDX, IDC_ADDTASK, bt_add);
	DDX_Control(pDX, IDC_DELTASK, bt_del);
	DDX_Control(pDX, IDC_CHANGETASK, bt_change);
	DDX_Control(pDX, IDC_MESSAGE, st_mes);
	DDX_Control(pDX, IDC_STLEN, st_len);
	DDX_Control(pDX, IDC_MESLEN, st_mesLen);
	DDX_Control(pDX, IDC_TASKBOX, cb_task);
}


BEGIN_MESSAGE_MAP(CTaskRedactionDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTaskRedactionDlg::OnOK)
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_CBN_SELENDOK(IDC_TASKBOX, &CTaskRedactionDlg::OnTaskChanged)
	ON_BN_CLICKED(IDC_REFRESH, &CTaskRedactionDlg::OnBnClickedRefresh)
	ON_BN_CLICKED(IDC_ADDTASK, &CTaskRedactionDlg::OnBnClickedAddtask)
	ON_BN_CLICKED(IDC_DELTASK, &CTaskRedactionDlg::OnBnClickedDeltask)
	ON_BN_CLICKED(IDC_CHANGETASK, &CTaskRedactionDlg::OnBnClickedChangetask)
END_MESSAGE_MAP()


BOOL CTaskRedactionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	st_font.CreatePointFont(90, _T(""));
	st_mes.SetFont(&st_font);
	st_len.SetFont(&st_font);
	st_mesLen.SetFont(&st_font);
	ed_font.CreatePointFont(110, _T(""));
	ed_text.SetFont(&ed_font);
	bt_font.CreatePointFont(85, _T(""));
	bt_ok.SetFont(&bt_font);
	btWork_font.CreatePointFont(100, _T(""));
	bt_refresh.SetFont(&btWork_font);
	bt_add.SetFont(&btWork_font);
	bt_del.SetFont(&btWork_font);
	bt_change.SetFont(&btWork_font);
	this->cb_task.SetWindowText("Выбор задачи...");
	GetTasks();
	for(UINT i=1; i<taskFiles.GetCount(); i+=2) {
		this->cb_task.AddString(taskFiles[i]);
	}

	m_wndTipContrl.Create(this);
	m_wndTipContrl.AddTool(GetDlgItem(IDC_ADDTASK), ("Добавить введённую задачу"));
	m_wndTipContrl.AddTool(GetDlgItem(IDC_DELTASK), ("Удалить выбранную задачу"));
	m_wndTipContrl.AddTool(GetDlgItem(IDC_CHANGETASK), ("Сохранить изменённый текст задачи"));
	m_wndTipContrl.AddTool(GetDlgItem(IDC_REFRESH), ("Обновить список задач и очистить поля"));
	m_wndTipContrl.SetDelayTime(1000);                // Время, через которое появится подсказка
	m_wndTipContrl.SetTipTextColor(RGB(0, 0, 0));     // Цвет шрифта
	m_wndTipContrl.SetTipBkColor(RGB(255, 255, 255)); // Цвет фона
	m_wndTipContrl.Activate(TRUE);

	stopThread = false;
	m_pThread = AfxBeginThread(StreamFunction, this); // Запуск потока
	m_pThread->m_bAutoDelete = FALSE;
    m_pThread->SetThreadPriority(THREAD_PRIORITY_LOWEST);
	
	return TRUE;
}

BOOL CTaskRedactionDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_LBUTTONDOWN ||
		pMsg->message == WM_LBUTTONUP ||
		pMsg->message == WM_MOUSEMOVE)
	{
		m_wndTipContrl.RelayEvent(pMsg);
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CTaskRedactionDlg::OnClose() 
{
	stopThread = true;	
	WaitForSingleObject(m_pThread->m_hThread, 2000);
    delete m_pThread;
    m_pThread = NULL;
	EndDialog(0);
}

HBRUSH CTaskRedactionDlg::OnCtlColor(CDC* pDC, CWnd *pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID() == IDC_MESSAGE)
		pDC->SetTextColor(RGB(255, 0, 0));
	
	return hbr;
}

void CTaskRedactionDlg::OnTaskChanged() {
	CString txt;
	UINT num = this->cb_task.GetCurSel();
	this->cb_task.GetLBText(num, txt);
	this->ed_text.SetWindowText(txt);
	//this->st_mes.SetWindowText("Задача " + taskFiles[num*2]); // Достаём название файла задачи
	RefreshMes();
}

void CTaskRedactionDlg::OnBnClickedRefresh()
{
	this->ed_text.SetWindowText("");
	this->cb_task.ResetContent();
	this->cb_task.SetWindowText("Выбор задачи...");
	GetTasks();
	for(UINT i=1; i<taskFiles.GetCount(); i+=2) {
		this->cb_task.AddString(taskFiles[i]);
	}
	this->st_mes.SetWindowText("Список задач обновлён.");
	RefreshMes();
}

void CTaskRedactionDlg::OnBnClickedAddtask()
{
	this->bt_add.EnableWindow(FALSE);
	CString taskTxt;
	this->ed_text.GetWindowText(taskTxt);
	if(taskTxt != "") {
		CString taskName;
		if(taskFiles.GetCount()) {
			taskName.Format("%d", TaskNumber(taskFiles[0])+1);
			taskName = "Task" + taskName + ".txt";
		} else {
			taskName = "Task1.txt";
		}
		CFile taskFile;
		CFileException fileException;
		taskFile.Open("Tasks\\"+taskName, CFile::modeCreate | CFile::modeWrite, &fileException);
		taskFile.Write(taskTxt, taskTxt.GetLength());
		taskFile.Close();
		this->st_mes.SetWindowText("Задача успешно добавлена.");
		this->ed_text.SetWindowText("");
		// Быстрое обновление списка задач
		taskFiles.InsertAt(0, taskTxt);
		taskFiles.InsertAt(0, taskName);
		this->cb_task.InsertString(0, taskTxt);
		this->cb_task.SetWindowText("Выбор задачи...");
	} else {
		this->st_mes.SetWindowText("Нет текста задачи.");
	}
	this->bt_add.EnableWindow(TRUE);
	RefreshMes();
}

void CTaskRedactionDlg::OnBnClickedDeltask()
{
	this->bt_del.EnableWindow(FALSE);
	UINT num = this->cb_task.GetCurSel();
	if(num!=CB_ERR) {
		CString taskName = taskFiles[num*2];
		try {
			CFile::Remove("Tasks\\" + taskName);
		} catch (CFileException* pEx) {
			TRACE(_T("File %20s cannot be removed\n"), taskName);
			pEx->Delete();
		}
		this->st_mes.SetWindowText("Задача удалена.");
		this->ed_text.SetWindowText("");
		// Быстрое обновление списка задач
		taskFiles.RemoveAt(num*2, 2);
		this->cb_task.DeleteString(num);
		this->cb_task.SetWindowText("Выбор задачи...");
	} else {
		this->st_mes.SetWindowText("Задача не выбрана.");
	}
	this->bt_del.EnableWindow(TRUE);
	RefreshMes();
}

void CTaskRedactionDlg::OnBnClickedChangetask()
{
	this->bt_change.EnableWindow(FALSE);
	UINT num = this->cb_task.GetCurSel();
	if(num!=CB_ERR) {
		CString taskName = taskFiles[num*2];
		CString taskTxt;
		this->ed_text.GetWindowText(taskTxt);
		CFile taskFile;
		CFileException fileException;
		taskFile.Open("Tasks\\"+taskName, CFile::modeWrite, &fileException);
		taskFile.SetLength(0);
		taskFile.Write(taskTxt, taskTxt.GetLength());
		taskFile.Close();
		this->st_mes.SetWindowText("Задача успешно изменена.");
		this->ed_text.SetWindowText("");
		// Быстрое обновление списка задач
		this->cb_task.DeleteString(num);
		this->cb_task.InsertString(num, taskTxt);
		this->cb_task.SetWindowText("Выбор задачи...");

	} else {
		this->st_mes.SetWindowText("Задача не выбрана.");
	}
	this->bt_change.EnableWindow(TRUE);
	RefreshMes();
}


void CTaskRedactionDlg::GetTasks() {
	this->bt_refresh.EnableWindow(FALSE);
	CTaskGeneratorDlg generatorDlg;
	CList <UINT> fileList;
	HANDLE hSearch = NULL;
	WIN32_FIND_DATA wfd;
	CString mask = "Tasks\\Task*.txt";
	UINT taskN;
	hSearch = FindFirstFile(mask, &wfd);
	if (hSearch != INVALID_HANDLE_VALUE) {
		// Вытаскивание всех подходящих файлов с задачами
		taskN = TaskNumber(wfd.cFileName);
		if (taskN)
			fileList.AddTail(taskN);
		while (FindNextFile(hSearch, &wfd)) {
			if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				continue;
			taskN = TaskNumber(wfd.cFileName);
			if (taskN)
				fileList.AddTail(taskN);
		}
		FindClose(hSearch);
		ListSort(fileList);
		CFile taskFile;
		CFileException fileException;
		CString fileName = "";
		POSITION pos = fileList.GetHeadPosition();
		char taskTxt [MaxLength];
		if(taskFiles.GetCount() != 0) {
			taskFiles.RemoveAll();
		}
		while(pos) {
			fileName.Format("%d", fileList.GetNext(pos));
			fileName = "Task" + fileName + ".txt";
			taskFile.Open("Tasks\\"+fileName, CFile::modeRead, &fileException);
			memset(taskTxt, 0, sizeof(taskTxt));
			taskFile.Read(taskTxt, sizeof(taskTxt));
			taskFiles.Add(fileName); taskFiles.Add(taskTxt);
			taskFile.Close();
		}
	} else {
		this->st_mes.SetWindowText("Задачи не найдены.");
	}
	this->bt_refresh.EnableWindow(TRUE);
}

// Функция для вытаскивания номера задачи из названия файла
UINT CTaskRedactionDlg::TaskNumber(CString file) {
	file.Replace(_T("Task"),_T(""));
	file.Replace(_T(".txt"),_T(""));
	UINT num = atoi(file);
	if (num)
		return num;
	else 
		return 0;
}

// Процедура для сортировки списка целочисленных элементов
void CTaskRedactionDlg::ListSort(CList <UINT>& oldList) {
	if(oldList.GetSize()) {
		CList <UINT> newList;
		POSITION newPos = newList.GetHeadPosition();
		newList.AddTail(oldList.RemoveHead());
		UINT oldElem, newElem;
		for(UINT oldSize = oldList.GetSize(); oldSize; oldSize--) {
			oldElem = oldList.RemoveHead();
			newPos = newList.GetHeadPosition();
			while(newPos) {
				newElem = newList.GetAt(newPos);
				if(oldElem>newElem) {
					newList.InsertBefore(newPos, oldElem);
					break;
				}
				newList.GetNext(newPos);
			}
			if(!newPos)
				newList.InsertAfter(newPos, oldElem);
		}
		oldList.RemoveAll();
		for(newPos = newList.GetHeadPosition(); newPos; ) {
			newElem = newList.GetNext(newPos);
			oldList.AddTail(newElem);
		}
		newList.RemoveAll();
	}
}

void CTaskRedactionDlg::OnOK()
{
	// Окно не закрывается, если нажат Enter
	if(!GetAsyncKeyState(VK_RETURN)) {
		stopThread = true;
		WaitForSingleObject(m_pThread->m_hThread, 2000);
        delete m_pThread;
        m_pThread = NULL;
		EndDialog(0);
	}
}

void CTaskRedactionDlg::RefreshMes() {
	CRect rect;
	st_mes.GetWindowRect(&rect);
	ScreenToClient(&rect);
	InvalidateRect(&rect);
	UpdateWindow();
}

// Потоковая функция
UINT StreamFunction(LPVOID pParam) {
	CTaskRedactionDlg* taskRedaction = (CTaskRedactionDlg*)pParam;
	UINT taskLen = MaxLength;
	UINT taskLenNew;
	CString taskTxt;
	CRect rectL, rectM;
	BOOL maxLimit = FALSE;
	BOOL taskSelected = TRUE;
	while(!stopThread) {
		// Работа с текстом
		taskRedaction->ed_text.GetWindowText(taskTxt);
		taskLenNew = taskTxt.GetLength();
		if(taskLenNew!=taskLen) {
			taskRedaction->st_len.GetWindowRect(&rectL);
			taskRedaction->st_mes.GetWindowRect(&rectM);
			taskTxt.Format("%d", (MaxLength - taskLenNew));
			taskRedaction->st_len.SetWindowText(taskTxt);
			if(taskLenNew > MaxLength) {
				taskRedaction->ed_text.GetWindowText(taskTxt);
				taskRedaction->ed_text.SetWindowText(taskTxt.Left(MaxLength));
				taskRedaction->ed_text.SetSel(-1);
				maxLimit = TRUE;
				taskRedaction->st_mes.SetWindowText("Достигнут лимит символов.");
			} else if(taskLenNew < MaxLength && maxLimit == TRUE) {
				maxLimit = FALSE;
				taskRedaction->st_mes.SetWindowText("");
			}
			taskRedaction->ScreenToClient(&rectL);
			taskRedaction->InvalidateRect(&rectL);
			taskRedaction->ScreenToClient(&rectM);
			taskRedaction->InvalidateRect(&rectM);
			taskRedaction->UpdateWindow();
		}
		taskLen = taskLenNew;
		// -----
		Sleep(10);
	}
	return 0;
}