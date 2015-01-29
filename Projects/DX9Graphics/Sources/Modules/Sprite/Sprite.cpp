#include "Sprite.hpp"
#include "../DX9/DX9.hpp"
#include "../Model/Model.hpp"
#include "../Texture/Texture.hpp"
#include "../SpriteTexter/SpriteTexter.hpp"

namespace DX9Graphics
{
	type_result Sprite::dock(Model& model)
	{
		//	pre:
		if( ! isEnable()) return RESULT_SUCCESS | RESULT_ABORT_ACTION;
		//		DX9 ���ε�:
		DX9& dx9 = DX9::getInstancedDX();
		if( ! &dx9)
			return ALERT_ERROR(" : ����̽����ε����з�����");

		if( ! &model)
			return ALERT_ERROR("���� �����ϴ�.");

		Texture& texture = model.getTexture();
		if( ! &texture)		
			return ALERT_WARNING("�ؽ��İ� �����ϴ�.");


		//	main:
		LPDIRECT3DDEVICE9 device = dx9.getDevice();		

		return _renderSprite(model, texture, device);
	}
}