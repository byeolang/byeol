#pragma once

#ifdef UNICODE
#    define tchar wchar_t
#    define tmain wmain
#    define tcslen wcslen
#    define tcscat wcscat
#    define tcscpy wcscpy
#    define tcsncpy wcsncpy
#    define tcscmp wcscmp
#    define tcsncmp wcsncmp
#    define tprintf wprintf
#    define tscanf wscanf
#    define fgetts fgetws
#    define fputts fputws
#else
#    define tchar char
#    define tcslen strlen
#    define tcscat strcat
#    define tcscpy strcpy
#    define tcsncpy strncpy
#    define tcscmp strcmp
#    define tcsncmp strncmp
#    define tprintf printf
#    define tscanf scanf
#    define fgetts fgets
#    define fputts fputs
#endif
