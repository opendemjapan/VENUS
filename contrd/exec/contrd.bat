@echo off
if not "%2"=="" cd %2
if not exist c04d goto err
if "%1"=="n" goto :n
if "%1"=="s" goto :s
if "%1"=="w" goto :w
if "%1"=="u" goto :u
if "%1"=="g" goto :g
if "%1"=="h" goto :h
if "%1"=="i" goto :i
if "%1"=="j" goto :j
echo contrd [n/s/w/u/g/h/i/j] (filename)
goto quit
:n
%dvdir%\object\ncontrd.exe
goto end
:s
%dvdir%\object\scontrd.exe
goto end
:w
%dvdir%\object\wcontrd.exe
goto end
:u
%dvdir%\object\ucontrd.exe
goto end
:g
%dvdir%\object\gcontrd.exe
goto end
:h
%dvdir%\object\hcontrd.exe
goto end
:i
%dvdir%\object\icontrd.exe
goto end
:j
%dvdir%\object\jcontrd.exe
goto end
:err
echo c04 not exist
goto quit
:end
del c06
:quit
