@echo off
z:\Projects\Progressing\Node\Projects\Cooee\Solutions\VisualStudio2008\Cooee\..\..\..\..\..\Resources\Applications\AfterBuild\AfterBuild.exe z:\Projects\Progressing\Node\Projects\Cooee\Solutions\VisualStudio2008\Cooee\VersionNo.ini z:\Projects\Progressing\Node\Projects\Cooee\Binaries\Cooee\..\Cooee z:\Projects\Progressing\Node\Projects\Cooee\Solutions\VisualStudio2008\Cooee\..\..\..\..\..\SDK\Libraries\
if errorlevel 1 goto VCReportError
goto VCEnd
:VCReportError
echo Project : error PRJ0019: �������� ���� �ڵ带 ��ȯ�߽��ϴ�. ��ġ: "���� �� �̺�Ʈ�� �����ϰ� �ֽ��ϴ�..."
exit 1
:VCEnd