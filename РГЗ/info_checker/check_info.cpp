#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>

typedef void(*info_getter)(int&, bool&);
 
TCHAR screen_info[256];	// Строка, содержащая информацию об окне (максимальная ширина)
TCHAR MMX_info[256];	// Строка, содержащая информацию о наличии технологии MMX

// Функция, выполняющаяся в отдельном потоке
DWORD WINAPI ThreadFunc(void*)
{
	info_getter dll_get_system_info;

	// Загружаем библиотеку
	HINSTANCE load = LoadLibrary(TEXT("info.dll"));

	// Если загрузилась
	if (load)
	{
		// Получаем адрес функции
		dll_get_system_info = (info_getter)GetProcAddress(load, "get_system_info");

		// Если функция найдена
		if (dll_get_system_info) {
			int max_width;
			bool MMX_support;
			
			// Получаем результат
			dll_get_system_info(max_width, MMX_support);

			// И формируем строки с полученными результатами
			wsprintf(screen_info, TEXT("Максимальная ширина окна: %d px"), max_width);

			if (MMX_support) 
				wsprintf(MMX_info, TEXT("Технология MMX поддерживается"));
			else 
				wsprintf(MMX_info, TEXT("Технология MMX не поддерживается"));
		}
		// Иначе формируем сообщение об ошибке (записываем туда, где должна
		// быть информация о ширине окна)
		else 
			wsprintf(screen_info, TEXT("Не удалось найти функцию :("));

		// Освобождаем библиотеку
		FreeLibrary(load);
	}
	// Иначе формируем сообщение об ошибке (записываем туда, где должна
	// быть информация о ширине окна)
	else 
		wsprintf(screen_info, TEXT("Не удалось загрузить библиотеку :("));

	return 0;
}

// Обработка сообщений
LRESULT CALLBACK WindowFunc(
	HWND hWnd, 
	UINT msg, 
	WPARAM wParam, 
	LPARAM lParam
)
{
	PAINTSTRUCT ps;
	HDC hDC;

	// Создание шрифтра
	HFONT hFont = CreateFont(
		20, 0, 0, 0, 
		FW_THIN, 
		FALSE, 
		FALSE, 
		FALSE, 
		DEFAULT_CHARSET, 
		OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, 
		CLEARTYPE_NATURAL_QUALITY, 
		VARIABLE_PITCH, 
		TEXT("Times New Roman")
	);

	switch (msg)
	{
	// Создаем поток
	case WM_CREATE:	
		HANDLE hThread;
		DWORD IDThread;
		hThread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, &IDThread);
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
		break;

	// Закрываем окно
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	// Инициализируем контекст устройства
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		SelectObject(hDC, hFont);
		TextOut(hDC, 15, 10, screen_info, 256);
		TextOut(hDC, 15, 30, MMX_info, 256);
		EndPaint(hWnd, &ps);
		break;

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}

int WINAPI WinMain(
	HINSTANCE hThisInst,
	HINSTANCE hPrevInst,
	LPSTR str,
	int nWinMode
)
{
	MSG msg;
	WNDCLASS wcl;
	HWND hWnd;

	// Создаем класс окна
	wcl.hInstance = hThisInst;
	wcl.lpszClassName = TEXT("MyClass");
	wcl.lpfnWndProc = WindowFunc;
	wcl.style = CS_HREDRAW | CS_VREDRAW;
	wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcl.lpszMenuName = NULL;
	wcl.cbClsExtra = 0;
	wcl.cbWndExtra = 0;
	wcl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	// Регистрируем класс окна
	RegisterClass(&wcl);

	hWnd = CreateWindow(TEXT("MyClass"), TEXT("РГЗ. Управление ресурсами"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN |
		WS_CLIPSIBLINGS,
		100, 50, 700, 120, HWND_DESKTOP,
		NULL, hThisInst, NULL);

	// Отображаем окно
	ShowWindow(hWnd, nWinMode);
	UpdateWindow(hWnd);

	// Цикл обработки сообщений
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}