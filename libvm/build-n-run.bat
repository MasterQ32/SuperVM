
as.exe -o %1.bin %1.asm 
IF %ERRORLEVEL% NEQ 0 (
  exit
)

svmln -o %1.exp -c %1.bin
IF %ERRORLEVEL% NEQ 0 (
	exit
)

rem emulator -d %1.exp