g++ -std=c++11 -o double_buffer_demo.exe double_buffer_demo.cpp -static -luser32 -lkernel32
if not errorlevel 1 (
	.\double_buffer_demo.exe
)