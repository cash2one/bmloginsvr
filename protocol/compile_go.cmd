@pushd %~dp0

@set OUTPUT=..\LSControlProto\
@if not exist %OUTPUT%		(mkdir %OUTPUT%)

@set _ITEM_= 
@for %%F in (*.proto) do @(
	@set _ITEM_=%%F
	@echo .		%%F
	@protoc.exe %%F --go_out=%OUTPUT%
	@cd %~dp0
)

@pause

@popd