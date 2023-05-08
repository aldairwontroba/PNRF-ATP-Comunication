echo off
:: -------------------------------------------
:: Arquivo responsavel por executar TPBIG.exe
:: -------------------------------------------
:: Seta o diretorio do TPBIG.exe
SET GNUDIR=C:\ATP\atpmingw\
:: Executa o mytpbig.exe
%GNUDIR%mytpbig.exe BOTH %~nx1 s -R
PAUSE
:: Deleta arquivos auxiliares e temporarios
DEL dum*.bin
DEL *.tmp