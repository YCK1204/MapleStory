START ./flatc.exe --csharp "./Protocol.fbs"
START ../PacketGenerator/bin/Debug/net8.0/PacketGenerator.exe "./Protocol.fbs"

for %%f in (*.cs) do (
    copy "%%f" "../DummyClient/FlatBuffer"
    copy "%%f" "../MapleStoryServer/FlatBuffer"
)

XCOPY /Y ".\Server\PacketManager.cs" "..\MapleStoryServer\Packet"
XCOPY /Y ".\Client\PacketManager.cs" "..\DummyClient\Packet"

if %ERRORLEVEL% neq 0 pause