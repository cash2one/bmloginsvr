@pushd %~dp0
@copy /y ..\bmregsvrapp.exe
bmregsvrapp.exe -listenaddress=:8081 -mailaccount=backmir_admin@163.com -mailpassword=backmir_admin163 -smtpaddr=smtp.163.com:25 -lsaddress=127.0.0.1:8300 -ulsaddress=1.1.1.1:8888
@pause
@popd