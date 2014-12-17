@PUSHD	%~dp0

@call update_inc_files.bat
@call update_c_code.bat

@set	OLD_DIR=%CD%
@set	OLD_GOPATH=%GOPATH%
@cd	..\..\
@set	GOPATH=%CD%
@cd	%OLD_DIR%

@go install
@if not %ERRORLEVEL% == 0	@goto ERROR

@goto	END
:ERROR
@echo	!!!
@echo	!!!	ERROR in:	%_ITEM_%
@echo	!!!
PAUSE

:END
@SET	_ITEM_=

@POPD