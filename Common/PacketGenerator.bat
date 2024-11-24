rem ���� �������� .cs���ϵ� ����
del /S /Q "*.cs"
del /S /Q "*.h"
del /S /Q "*.cpp"
del /S /Q "..\Server\Server\Protocol_generated.h"
del /S /Q "..\MapleStory\Assets\Scripts\FlatBuffer\*.cs"

timeout /t 2 /nobreak >nul
set "fileList="

rem ���� ���丮�� ��� .fbs ������ ����
for %%f in (*.fbs) do (
    set "fileList= %%f"
)

rem flatc ���� (������ ���� ����Ʈ�� �� ���� ����)
START ./flatc.exe --csharp %fileList%
START ./flatc.exe --cpp %fileList%

START ../Server/PacketGenerator/bin/Debug/net8.0/PacketGenerator.exe "./Protocol.fbs"

rem 2�� ��ٸ� �� ���� ����
timeout /t 2 /nobreak >nul
for %%f in (*.cs) do (
    copy "%%f" "../MapleStory/Assets/Scripts/FlatBuffer"
)

XCOPY /Y ".\Protocol_generated.h" "..\Server\Server"
XCOPY /Y ".\PacketManager.h" "..\Server\Server"
XCOPY /Y ".\PacketManager.cpp" "..\Server\Server"
XCOPY /Y ".\Client\PacketManager.cs" "..\MapleStory\Assets\Scripts\Packet"

rem ���� ������ �� pause
if %ERRORLEVEL% neq 0 pause