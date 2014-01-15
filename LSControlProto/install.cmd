@pushd %~dp0

@set WORK_DIR=%CD%
@cd ..\..\
@set GOPATH=%CD%
@cd %WORK_DIR%
@go install

@popd