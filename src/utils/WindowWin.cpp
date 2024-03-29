/*
** Easystream EIP, 2021
** easystream_main_plugin
** File description:
** obs
*/

#include "Window.hpp"

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winuser.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <locale>
#include <codecvt>

#define MAX_SEARCH 1000

bool GetWindowTitle(HWND window, std::string &title)
{
	size_t len = (size_t)GetWindowTextLengthW(window);
	std::wstring wtitle;

	wtitle.resize(len);
	if (!GetWindowTextW(window, &wtitle[0], (int)len + 1)) {
		return false;
	}

	len = os_wcs_to_utf8(wtitle.c_str(), 0, nullptr, 0);
	title.resize(len);
	os_wcs_to_utf8(wtitle.c_str(), 0, &title[0], len + 1);
	return true;
}

bool WindowValid(HWND window)
{
	LONG_PTR styles;
	DWORD id;

	if (!IsWindowVisible(window)) {
		return false;
	}
	GetWindowThreadProcessId(window, &id);
	if (id == GetCurrentProcessId()) {
		return false;
	}

	styles = GetWindowLongPtr(window, GWL_STYLE);

	if (styles & WS_CHILD) {
		return false;
	}

	return true;
}

BOOL CALLBACK GetTitleCB(HWND hwnd, LPARAM lParam)
{
	if (!WindowValid(hwnd)) {
		return TRUE;
	}

	std::string title;
	GetWindowTitle(hwnd, title);
	if (title.empty()) {
		return TRUE;
	}

	std::vector<std::string> &titles =
		*reinterpret_cast<std::vector<std::string> *>(lParam);
	titles.push_back(title);

	return TRUE;
}

VOID EnumWindowsWithMetro(__in WNDENUMPROC lpEnumFunc, __in LPARAM lParam)
{
	HWND childWindow = NULL;
	int i = 0;

	while (i < MAX_SEARCH &&
	       (childWindow = FindWindowEx(NULL, childWindow, NULL, NULL))) {
		if (!lpEnumFunc(childWindow, lParam)) {
			return;
		}
		i++;
	}
}

std::vector<std::string> es::utils::window::GetWindowList()
{
	std::vector<std::string> windows;
	windows.resize(0);
	EnumWindowsWithMetro(GetTitleCB, reinterpret_cast<LPARAM>(&windows));
	return windows;
}

std::string es::utils::window::GetCurrentWindowTitle()
{
	std::string title;
	HWND window = GetForegroundWindow();
	DWORD pid;
	DWORD thid;
	thid = GetWindowThreadProcessId(window, &pid);
	// GetWindowText will freeze if the control it is reading was created in another thread.
	// It does not directly read the control.
	// Instead it waits for the thread that created the control to process a WM_GETTEXT message.
	// So if that thread is frozen in a WaitFor... call you have a deadlock.
	if (GetCurrentProcessId() == pid) {
		title = "OBS";
		return title;
	}
	GetWindowTitle(window, title);
	return title;
}

std::pair<int, int> es::utils::window::GetCursorPos()
{
	std::pair<int, int> pos(0, 0);
	POINT cursorPos;
	if (GetPhysicalCursorPos(&cursorPos)) {
		pos.first = cursorPos.x;
		pos.second = cursorPos.y;
	}
	return pos;
}

HWND getHWNDfromTitle(std::string title)
{
	HWND hwnd = NULL;


	const char *lp = title.c_str();

	hwnd = FindWindowEx(NULL, NULL, NULL, lp);
	return hwnd;
}

bool es::utils::window::IsMaximized(const std::string &title)
{
	RECT appBounds;
	MONITORINFO monitorInfo = {0};
	HWND hwnd = NULL;

	hwnd = getHWNDfromTitle(title);
	if (!hwnd) {
		return false;
	}

	monitorInfo.cbSize = sizeof(MONITORINFO);
	GetMonitorInfo(MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST),
		       &monitorInfo);

	if (hwnd && hwnd != GetDesktopWindow() && hwnd != GetShellWindow()) {
		if (IsZoomed(hwnd)) {
			return true;
		}
		GetWindowRect(hwnd, &appBounds);
		if (monitorInfo.rcMonitor.bottom == appBounds.bottom &&
		    monitorInfo.rcMonitor.top == appBounds.top &&
		    monitorInfo.rcMonitor.left == appBounds.left &&
		    monitorInfo.rcMonitor.right == appBounds.right) {
			return true;
		}
	}
	return false;
}

bool es::utils::window::IsFullscreen(const std::string &title)
{
	RECT appBounds;
	MONITORINFO monitorInfo = {0};

	HWND hwnd = NULL;
	hwnd = getHWNDfromTitle(title);

	if (!hwnd) {
		return false;
	}

	monitorInfo.cbSize = sizeof(MONITORINFO);
	GetMonitorInfo(MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST),
		       &monitorInfo);

	if (hwnd && hwnd != GetDesktopWindow() && hwnd != GetShellWindow()) {
		GetWindowRect(hwnd, &appBounds);
		if (monitorInfo.rcMonitor.bottom == appBounds.bottom &&
		    monitorInfo.rcMonitor.top == appBounds.top &&
		    monitorInfo.rcMonitor.left == appBounds.left &&
		    monitorInfo.rcMonitor.right == appBounds.right) {
			return true;
		}
	}

	return false;
}

void GetProcessList(QStringList &processes)
{

	HANDLE procSnapshot;
	PROCESSENTRY32 procEntry;

	procSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (procSnapshot == INVALID_HANDLE_VALUE) {
		return;
	}

	procEntry.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(procSnapshot, &procEntry)) {
		CloseHandle(procSnapshot);
		return;
	}

	do {
		std::string str(procEntry.szExeFile);
		QString tempexe = QString::fromStdString(str);
		if (tempexe == "System") {
			continue;
		}
		if (tempexe == "[System Process]") {
			continue;
		}
		if (processes.contains(tempexe)) {
			continue;
		}
		processes.append(tempexe);
	} while (Process32Next(procSnapshot, &procEntry));

	CloseHandle(procSnapshot);
}

bool es::utils::window::IsInFocus(const QString &executable)
{
	// only checks if the current foreground window is from the same executable,
	// may return true for any window from a program
	HWND foregroundWindow = GetForegroundWindow();
	DWORD processId = 0;
	GetWindowThreadProcessId(foregroundWindow, &processId);

	HANDLE process = OpenProcess(
		PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
	if (process == NULL) {
		return false;
	}

	WCHAR executablePath[600];
	GetModuleFileNameExW(process, 0, executablePath, 600);
	CloseHandle(process);

	QString file = QString::fromWCharArray(executablePath)
			       .split(QRegularExpression("(/|\\\\)"))
			       .back();

	// True if executable switch equals current window
	bool equals = (executable == file);
	// True if executable switch matches current window
	bool matches = file.contains(QRegularExpression(executable));

	return (equals || matches);
}

#endif