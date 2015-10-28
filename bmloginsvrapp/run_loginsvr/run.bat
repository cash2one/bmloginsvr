@start .\bmloginsvrapp.exe -lsaddr=0.0.0.0:8200 -lsgsaddr=0.0.0.0:8201 -httpaddr=:8181
@rem @start .\gamesvr.exe listenip=0.0.0.0:8400 loginsvr=127.0.0.1:8201 outerip=121.40.197.47:8400
@start .\bmregsvrapp.exe -listenaddress=0.0.0.0:8081 -lsaddress=127.0.0.1:8201 -ulsaddress=121.40.197.47:8400 -lshttpaddr=http://localhost:8181 -usinghttpmode=1