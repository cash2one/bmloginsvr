@PUSHD	%~dp0

@SET	ITEMS=luaconf lua lualib lauxlib

@FOR	%%F IN (%ITEMS%)	DO	@(
	@DEL	/Q	..\..\inc\lua\%%F.h
	@COPY	/Y	.\luasrc\lua\src\%%F.h	..\..\inc\lua\
)

@pause

@POPD