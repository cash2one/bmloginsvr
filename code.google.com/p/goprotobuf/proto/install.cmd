@PUSHD %~dp0
@SET WORK_DIR= %CD%
@IF "%GOPATH%"==""	(CD ..\..\..\..\..\)
@IF "%GOPATH%"=="" (SET GOPATH=%CD%)
@CD %WORK_DIR%
@go install
@PAUSE
@POPD