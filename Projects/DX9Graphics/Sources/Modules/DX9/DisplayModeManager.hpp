class DisplayModeManager : public NETList<DisplayMode>
{
	friend class DX9;

public:
	DisplayModeManager();

public:
	type_index findCorrespondingDisplayWithoutFormat(const DisplayMode& source);
	type_result makeDisplayModeAvailable(DisplayMode& target, LPDIRECT3D9 direct);

public:
	type_result initialize(LPDIRECT3D9 direct);

private:
	//	�׷��� �ϵ��� �����ϴ� ����� ������ ���Ѵ�
	type_result _pushAvailableDisplayMode(LPDIRECT3D9 direct);
	type_result __enumerateOneDisplayMode(LPDIRECT3D9 direct, D3DFORMAT format);
};