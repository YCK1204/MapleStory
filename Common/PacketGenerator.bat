rem 기존 프로토콜 .cs파일들 삭제
del /S /Q "*.cs"
del /S /Q "*.h"
del /S /Q "*.cpp"
del /S /Q "..\Server\Server\Protocol_generated.h"
del /S /Q "..\MapleStory\Assets\Scripts\FlatBuffer\*.cs"

timeout /t 2 /nobreak >nul
set "fileList="

rem 현재 디렉토리의 모든 .fbs 파일을 누적
for %%f in (*.fbs) do (
    set "fileList= %%f"
)

rem flatc 실행 (누적된 파일 리스트를 한 번에 전달)
START ./flatc.exe --csharp %fileList%
START ./flatc.exe --cpp %fileList%

START ../Server/PacketGenerator/bin/Debug/net8.0/PacketGenerator.exe "./Protocol.fbs"

rem 2초 기다린 후 파일 복사
timeout /t 2 /nobreak >nul
for %%f in (*.cs) do (
    copy "%%f" "../MapleStory/Assets/Scripts/FlatBuffer"
)

XCOPY /Y ".\Protocol_generated.h" "..\Server\Server"
XCOPY /Y ".\PacketManager.h" "..\Server\Server"
XCOPY /Y ".\PacketManager.cpp" "..\Server\Server"
XCOPY /Y ".\Client\PacketManager.cs" "..\MapleStory\Assets\Scripts\Packet"

rem 문제 생겼을 시 pause
if %ERRORLEVEL% neq 0 pause