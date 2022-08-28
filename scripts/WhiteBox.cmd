@echo off

@set OutputDir=.\\output
@set StdoutLog=%OutputDir%\\stdout.log
@set StderrLog=%OutputDir%\\stderr.log

@if not exist %OutputDir% @(
	@mkdir %OutputDir%
)

@for %%t in (FrameworkTests PETests) do @(
	@for %%a in (Win32 x64) do @(
		@for %%c in (Debug Release) do @(
			@echo ====----====----==== >> %StdoutLog%
			@echo ====----====----==== >> %StderrLog%
			@echo %%a-%%c\\%%t started... >> %StdoutLog%
			@echo %%a-%%c\\%%t started... >> %StderrLog%
			..\\build\\bin\\%%t\\%%a-%%c\\%%t.exe 1>> %StdoutLog% 2>> %StderrLog%
			@echo %%a-%%c\\%%t done >> %StdoutLog%
			@echo %%a-%%c\\%%t done >> %StderrLog%
			@echo ====----====----==== >> %StdoutLog%
			@echo ====----====----==== >> %StderrLog%
			@echo . >> %StdoutLog%
			@echo . >> %StderrLog%
		)
	)
)
