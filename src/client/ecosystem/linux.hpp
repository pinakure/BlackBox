#pragma once
#include <filesystem>
#define vsprintf_s(buffer_ptr, buffer_size, format, a_pointer)					vsnprintf(buffer_ptr, buffer_size, format, a_pointer)
#define _itoa_s(number, buffer, buffer_size, base)	 							snprintf(buffer, sizeof(buffer_size), "%d", number)
#define fopen_s(file_pointer, filename, open_mode)								(*(file_pointer)) = fopen(filename, open_mode)
#define strcpy_s(dest, size, src)                                               snprintf(dest, size, "%s", src)