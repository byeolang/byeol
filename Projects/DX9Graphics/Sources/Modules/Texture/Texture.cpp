#include "Texture.hpp"
#include "../DX9/DX9.hpp"

namespace DX9Graphics
{
	type_result Texture::dock(Model& model)
	{
		if (!_texture)
			return ALERT_WARNING("�ؽ��İ� �ε���� �ʾƼ� Dock �� �� �����ϴ�.");

		DX9& dx9 = DX9::getInstancedDX();
		if (!&dx9)
			return ALERT_ERROR("DX9�� ���ε� ����");

		LPDIRECT3DDEVICE9 device = dx9.getDevice();

		device->SetTexture(0, _texture);

		return RESULT_SUCCESS;
	}
	type_result Texture::initializeResource()
	{
		DX9& dx9 = DX9::getInstancedDX();
		if (!&dx9)
			return ALERT_ERROR("DX9�� ���ε� ����");

		LPDIRECT3DDEVICE9 device = dx9.getDevice();

		ThisClass::releaseResource();
		SuperClass::initializeResource();


		//	main:
		D3DXIMAGE_INFO image_info;
		HRESULT result = D3DXCreateTextureFromFileEx(
			device, arg_path.getValue().toCharPointer(),
			D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0,
			D3DFMT_DXT1, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
			createColorKey(), &image_info, NULL, &_texture);


		//	post:
		//		�ؽ��� ũ�� �˻�:
		if (!_isTextureSizePowerOf2(image_info))
		{
			ALERT_WARNING("�ؽ��İ� 2�� �¼��� �ƴմϴ�.\n�״�� ������� ���ϴϱ� �ӽù������� NON-2-POWER Flag�� ó���մϴ�.\n��, MipMap�� 1�̹Ƿ� ������ �϶��ɲ�����:\n\t�̹����� (Width, Height) = (%d, %d)", image_info.Width, image_info.Height);

			SuperClass::initializeResource();

			result = D3DXCreateTextureFromFileEx(
				device, arg_path.getValue().toCharPointer(),
				D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0,
				D3DFMT_DXT1, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
				createColorKey(), 0, NULL, &_texture);

			if (FAILED(result))
				return ALERT_ERROR("�ؽ��� 2�� ��¼� �ε� ����\n�̱�. �ӽù������� �����߳׿�. �Ƹ��� �׷���ī�尡 ������ ������ ���ϳ����ϴ�\n���� ���� ���� �ڵ� ���ؿ�̤�\n�ٹ�~^^");
		}
		if (FAILED(result))
			return ALERT_ERROR("�ؽ��� ��ü ��������:\n2�� �¼��� �ƴմϴٸ�, ������ �����߽��ϴ�.\n\t�����ڵ� : %x", result);

		//		�ؽ����� ���� ����:
		_texture->GetLevelDesc(0, &_texture_description);
		return _setResourceRetrieved();
	}
}