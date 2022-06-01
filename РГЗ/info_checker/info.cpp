#include <windows.h>

extern "C" __declspec(dllexport) void get_system_info(int& max_width, bool& is_MMX_supported) {
	// Получаем максимальную ширину экрана
	max_width = GetSystemMetrics(SM_CXFULLSCREEN);

	// Проверяем наличие технологии MMX
	int MMX_support;

	__asm {
		MOV EAX, 1				// Делаем запрос флагов функции CPUID
		CPUID					
		AND EDX, 0x800000		// Проверяем 23 бит регистра EDX
		MOV MMX_support, EDX	// Сохраняем его значение
	}

	// Если значение регистра EDX не равно 0 -> MMX поддерживается
	is_MMX_supported = MMX_support != 0 ? true : false;

	return;
}