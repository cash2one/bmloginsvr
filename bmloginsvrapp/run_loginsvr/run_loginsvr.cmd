@PUSHD	%~dp0

@set	OLD_DIR=%CD%
@set	OLD_GOPATH=%GOPATH%
@if	"%GOPATH%"==""	(	cd	..\..\..\)
@if	"%GOPATH%"==""	(	set	GOPATH=%CD%)
@cd	%OLD_DIR%

@title	BMLoginSvr
@if not exist .\bmloginsvrapp.exe (go build -o .\bmloginsvrapp.exe ..\)
@if exist .\bmloginsvrapp.exe (bmloginsvrapp.exe -lsaddr=0.0.0.0:8200 -lsgsaddr=0.0.0.0:8201)
@if	not	%ERRORLEVEL% == 0	@goto ERROR

@goto	END
:ERROR

@PAUSE

:END

@pause
@POPD