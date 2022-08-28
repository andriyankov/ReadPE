
@echo off

set ToolName=readpe
set SevenZipDir="c:\\Program Files\\7-Zip"
set SevenZipTool=%SevenZipDir%\\7z.exe
set ArchiveName=%ToolName%-x.y.z-stable.7z
set DocsDir="..\\docs\\"
set ToolDir="..\\build\\bin\\%ToolName%\\"
set PyPackageDir="..\\py-package"
set PythonPath="c:\\python34\\python.exe"
set ReleaseDir=.\\release

@if exist .\\*.out (
	del .\\*.out
)

@call %ToolDir%\\Win32-Release\\%ToolName%.exe --help 1> .\\help32.out
@call %ToolDir%\\x64-Release\\%ToolName%.exe --help 1> .\\help64.out

@fc .\\help32.out .\\help64.out
@if errorlevel 1 (
	@goto help_error
)

@call %ToolDir%\\Win32-Release\\%ToolName%.exe --donate 1> .\\donate32.out
@call %ToolDir%\\x64-Release\\%ToolName%.exe --donate 1> .\\donate64.out

@fc .\\donate32.out .\\donate64.out
@if errorlevel 1 (
	@goto donate_error
)

:ok
	@if exist *.7z (
		del *.7z
	)
	@if exist %ReleaseDir% (
		rmdir /q /s %ReleaseDir%
	)

	mkdir %ReleaseDir%

	pushd .
	cd %PyPackageDir%
	call %PythonPath% .\\setup.py sdist
	popd

	copy %ToolDir%\\Win32-Release\\readpe.exe %ReleaseDir%\\readpe32.exe
	copy %ToolDir%\\x64-Release\\readpe.exe %ReleaseDir%\\readpe64.exe
	copy %DocsDir%\\ru_ReadMe.txt %ReleaseDir%\\
	copy %DocsDir%\\ru_ChangeLog.txt %ReleaseDir%\\
	copy %PyPackageDir%\\dist\\readpe*.zip %ReleaseDir%\\

	call md5sum %ReleaseDir%\\ 1> %ReleaseDir%\\Checksum.txt

	call %SevenZipTool% a %ArchiveName% -mhe %ReleaseDir%\\*.*

	rmdir /q /s %ReleaseDir%
	
	@goto exit

:help_error
	@echo Different Help Pages

:donate_error
	@echo Different Donate Pages

:exit
	@del *.out
