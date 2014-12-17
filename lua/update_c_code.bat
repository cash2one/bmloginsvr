@PUSHD	%~dp0

@if	exist	.\$.*		@DEL	/Q	.\$.*

@for %%F in (./luasrc/lua/src/*.c) do @(
	@echo	#include "./luasrc/lua/src/%%F" > $.%%F
)

@DEL	/Q	.\$.lua.c
@DEL	/Q	.\$.luac.c

@PAUSE

@POPD