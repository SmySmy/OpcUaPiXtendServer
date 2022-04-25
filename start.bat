
@echo OFF
REM
REM added the following string to the Path environment variable
REM
REM Computer -> Systemeigenschaften -> Erweiterte Systemeinstallung -> Umgebungsvariablen
REM
REM Name:       Path
REM String:     C:\ASNeG\usr\bin;C:\ASNeG\usr\lib
REM Console:    set PATH=%PATH%;C:\ASNeG\usr\bin;C:\ASNeG\usr\lib
REM

OpcUaServer4 CONSOLE C:\ASNeG\etc\OpcUaStack\OpcUaPiXtendServer\OpcUaServer.xml 

pause
