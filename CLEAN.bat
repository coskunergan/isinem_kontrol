settings@echo off

echo -----------------------------------
echo clean project/coide_gen/rvmdk/ewarm
@for /r . %%a in (.) do (
    @if exist "%%a\coide_gen" rd /s /q "%%a\coide_gen"
    @if exist "%%a\Debug_Flash" rd /s /q "%%a\Debug_Flash"
    @if exist "%%a\Debug_Ram" rd /s /q "%%a\Debug_Ram"
	//@if exist "%%a\Output" rd /s /q "%%a\Output"
	//@if exist "%%a\Flash" rd /s /q "%%a\Flash"
	@if exist "%%a\Debug" rd /s /q "%%a\Debug"
	@if exist "%%a\settings" rd /s /q "%%a\settings"
    @if exist "%%a\ewarm\settings" rd /s /q "%%a\ewarm\settings"
    @if exist "%%a\Release" rd /s /q "%%a\Release"
    //@if exist "%%a\*.dep" del /q "%%a\*.dep"
    @if exist "%%a\obj" rd /s /q "%%a\obj"  
	@if exist "%%a\List" rd /s /q "%%a\List"  	
    @if exist "%%a\lst" rd /s /q "%%a\lst"
    @if exist "%%a\*.uvgui.*" del /q "%%a\*.uvgui.*"
    @if exist "%%a\*.bak" del /q "%%a\*.bak" 
    @if exist "%%a\*.crf" del /q "%%a\*.crf" 
    @if exist "%%a\*.map" del /q "%%a\*.map" 	
	@if exist "%%a\*.plg" del /q "%%a\*.plg" 	
	@if exist "%%a\*.axf" del /q "%%a\*.axf" 	
    @if exist "%%a\*.lnp" del /q "%%a\*.lnp" 		
	@if exist "%%a\*.tra" del /q "%%a\*.tra" 	
	@if exist "%%a\*.htm" del /q "%%a\*.htm" 	
    @if exist "%%a\*.o" del /q "%%a\*.o" 
    @if exist "%%a\*.d" del /q "%%a\*.d"  

	@if exist "%%a\*.lst" del /q "%%a\*.lst"	
	@if exist "%%a\*.__i" del /q "%%a\*.__i"		
	@if exist "%%a\*.iex" del /q "%%a\*.iex"		
	@if exist "%%a\*.l1p" del /q "%%a\*.l1p"		
	@if exist "%%a\*.sct" del /q "%%a\*.sct"	
    @if exist "%%a\*.o" del /q "%%a\*.o" 
    @if exist "%%a\*.out" del /q "%%a\*.out"  
    @if exist "%%a\*.bin" del /q "%%a\*.bin" 	
	@if exist "%%a\Project" rd /s /q "%%a\Project"
	
)
@echo completed
echo -----------------------------------



