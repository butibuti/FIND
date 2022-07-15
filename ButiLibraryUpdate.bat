mkdir Libs
mkdir Libs\Library
mkdir Libs\lib
mkdir Libs\dll
cd Libs\Library\
git clone https://github.com/butibuti/ButiBulletWrap
git clone https://github.com/butibuti/ButiScript.git
git clone https://github.com/butibuti/ButiMemorySystem.git
git clone https://github.com/butibuti/ButiUtil.git
git clone https://github.com/butibuti/ButiMath
git clone https://github.com/butibuti/ButiEventSystem
git clone https://github.com/butibuti/ButiFont
git clone https://github.com/butibuti/ButiRendering_Dx12
git clone https://github.com/butibuti/ButiEngineHeader
cd ..\..\
cd Libs\Library\ButiBulletWrap\
git pull
cd ..\ButiScript\
git pull
cd ..\ButiMemorySystem\
git pull
cd ..\ButiUtil\
git pull
cd ..\ButiMath\
git pull
cd ..\ButiEventSystem\
git pull
cd ..\ButiFont\
git pull
cd ..\ButiRendering_Dx12\
git pull
cd ..\ButiEngineHeader\
git pull
cd ..\..\
set FILE_ID=1vkgzAuLeAu0quQkGypV-2r81FPLQkriL
curl -L "https://drive.google.com/uc?export=download&id=%FILE_ID%" -o dll.zip
set FILE_ID=1LObD7mhQ03WHranGUIsoeaCsCZL2cqzr
curl -c /tmp/cookie "https://drive.google.com/uc?export=download&id=%FILE_ID%" /dev/null
set CODE="%(awk '/_warning_/ {print %NF%}' /tmp/cookie)%"
curl -Lb /tmp/cookie "https://drive.google.com/uc?export=download&confirm=%CODE%&id=%FILE_ID%" -o lib.zip
call powershell -command "Expand-Archive -Force ./dll.zip"
call powershell -command "Expand-Archive -Force ./lib.zip"
cd ..\
xcopy /y /d /e Libs\dll\dll\ Libs\dll
xcopy /y /d /e Libs\lib\lib\ Libs\lib
rmdir /s /q Libs\dll\dll
rmdir /s /q Libs\lib\lib
del Libs\dll.zip
del Libs\lib.zip
xcopy /y /d Libs\dll\Debug\ output\UserSample\Debug\
xcopy /y /d Libs\dll\Debug_imguiEditor\ output\UserSample\Debug_imguiEditor\
xcopy /y /d Libs\dll\Release\ output\UserSample\Release\
xcopy /y /d Libs\dll\Release_imguiEditor\ output\UserSample\Release_imguiEditor\

pause