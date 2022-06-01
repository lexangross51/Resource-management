#include <windows.h>

extern "C" __declspec(dllexport) void get_system_info(int& max_width, bool& is_MMX_supported) {
	// �������� ������������ ������ ������
	max_width = GetSystemMetrics(SM_CXFULLSCREEN);

	// ��������� ������� ���������� MMX
	int MMX_support;

	__asm {
		MOV EAX, 1				// ������ ������ ������ ������� CPUID
		CPUID					
		AND EDX, 0x800000		// ��������� 23 ��� �������� EDX
		MOV MMX_support, EDX	// ��������� ��� ��������
	}

	// ���� �������� �������� EDX �� ����� 0 -> MMX ��������������
	is_MMX_supported = MMX_support != 0 ? true : false;

	return;
}