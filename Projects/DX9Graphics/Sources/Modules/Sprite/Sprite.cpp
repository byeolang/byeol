#include "Sprite.hpp"
#include "define.hpp"
#include "../DX9/DX9.hpp"
#include "../Model/Model.hpp"
#include "../Texture/Texture.hpp"
#include "../SpriteTexter/SpriteTexter.hpp"

namespace DX9Graphics
{
	const NEExportable::ModuleHeader& Sprite::getHeader() const
	{
		static NEExportable::ModuleHeader _header;

		if(_header.isValid() != RESULT_NOTHING)
		{
			_header.getName() = _T(_NAME);
			_header.getDeveloper() = _T(_DEVELOPER);
			_header.setRevision(_REVISION);
			_header.getComment() = _T(_COMMENT);
			_header.getVersion()  = _T(_VERSION);
			_header.getReleaseDate() = _T(_DATE);
			_header.getModuleDependencies().create(_DEPENDENCIES_COUNT);
			_header.setMaxErrorCodeCount(1);
		}

		return _header;
	}

	type_result Sprite::_renderSprite(const Model& model, Texture& texture, LPDIRECT3DDEVICE9 device)
	{
		//	pre:
		//		���� ��Ʈ����:
		//			�� �̰� ����ϴ°�?:
		//				������ �𸣰����� D3DXSPRITE�� OBJECTSPACE | NO BILLBOARD�� ����ϸ� X-AXIS FLIP
		//				�Ǿ ��µȴ�. ����ε� ������ ã�� ���ؼ� RotateX(180degree)�� ���س� �����
		//				World�� ���ϴ� ������ �����Ͽ���.
		//
		//			World�� ���ϴ� ����:
		//
		//					���ο� Sprite�� ��� = �־��� World ��� * �������
		//
		//				��������� �ڿ��� ���ؾ� �Ѵ�. �տ��� ���� ���, ��������� �θ�Model��� ��ġ�� 
		//				�������ԵǹǷ� ��������� 180�� ȸ���� �ֱ� ������ �־���World����� Z�� ��ȣ�� 
		//				�ݴ�� �ۿ��ϰ� �ȴ�.
		//
		//			Scaling ���� ��Ʈ����:
		//				D3DXSPRITE�� ���ο��� PLATE (���� 4���� ��� �𵨸�)�� ũ�⸦ �־��� Texture�� px
		//				��ŭ �ִ� �� ����. ������� Sample.bmp�� ũ�Ⱑ 50x50�̸� ȭ��󿡴� ���� �׸�������
		//				Modeling �����ͷν��� 50x50�� ����� ū ��ü�� ���ϰ� �ȴ�.
		//				�̷��� ū ��ü�� ���̰��� �پ�� �� �ۿ� ������, �������� ���̰��� �ֱ� ���ؼ��� 
		//				�����Ӵ� 500������ Z���� �������� �� ������ ������ �ɰ�������.
		//				����, Model�� Scaling�� 1�� �ƴ� ��Ұ�(���� ��� 0.2) ������ �Ҵ��ϸ� �����ϴ�.
		//
		//			�׷��� �� Scaling ���� ��Ʈ������ ������ �Ǿ��°�:
		//				������ �̿� ���� ������ ������������ ������ ���� �Ǵµ�, ���� ������ Scaling�� 0.8
		//				�� ���, ���� (1),
		//					
		//					���� �������� ũ��(0.64) = �־��� �������� ũ��(0.8) * ��������(0.8)
		//
		//				��, ������ ������� ����� ���� ���� �� ũ�Ⱑ ����ȭ �Ǳ� �����̴�.
		//				�� ������ �ذ��ϱ� ���� ����� ũ�� 3������ ����ȴ�.
		//
		//					1. Model���� Rotate, Trans Scale �� Ư�� ��Ҵ� ������������ �ݿ����� �ʵ���
		//					���� Flag�� �����Ѵ�.
		//						:	�����ϳ�, �߸��� ��������̴�. �߰������� Flag�� �ðԵǸ� ����������.
		//
		//					2. ��üX�� ���������� ������ �ֱ����� �ð��� ���°� �����ǵ���, ���������� 
		//					ũ���� ������ �̸� �����ش�. (���� ���� ���, ���������� ���ԵǴ� ���������� 
		//					ũ��(1.0) = ���� ������ ũ��(0.8) * ����������ũ���� ����(1.25). �̷��� �ϸ�,
		//					���� ����(1)�� ���Խ�, ���� ���������� ũ��� �ֱ����� ũ��(0.8)�� ������ �ȴ�.
		//						:	���� �ùٸ� ����̳�, 3���� ����� ������ ���� ��ƴ�.
		//							�̸� ����� �����Ϸ��� NENodeCodeSet�� push �Ǵ� Ÿ�̹��� �������� Module
		//							�� Catch �� �� �־�� �ϱ� �����̴�. (���� �ٸ� �ļ��� ������ �ְ�����)
		//
		//					3. Plate�� �������Ѵٴ� ���� �밳 ������ ����� Scaling ������ �޴´ٴ� �� �ǹ��Ѵ�.
		//					�׷��ٸ� �ƿ� ��� Plate�� ������ Scaling ����� ���ع����� ���� ������
		//						:	��� ������ �ƴϳ�, �����ϸ鼭�� ������ �䱸������ ���� ������Ų��.
		//				
		//				����, ���⼭�� ������� 3���� ���� Scaling ��������� ������ ���Ѵ�. (����ġ��
		//				���ϴ� ����� �ػ󵵳� �ؽ����� ũ�� ����� �ְ��� ���� õ�������̸� ������ ��Ȯ��
		//				����� ����� ����� ���Ƿ� ������ ������ ������ �����Ͽ���.
		//					
		//					1.	ī�޶�� ��ü�� �Ÿ��� �� 5 �̸�,
		//					2.	ī�޶��� ������ �� �߾��϶�, Prespective��,
		//					3.	fov�� 90, aspect 1.333 �϶��� ũ�Ⱑ ������ ����ϰ� ������ �� 0.02�� ���Ѵ�
		//
		//			��Ʈ ���� ��Ʈ����:
		//				��Ʈ�� ��� Z-Fighting�� ����ȴ�. �̸� ���� ���ؼ� ������Ʈ����(Translation(0,0,-0.5))
		//				�� ����Ͽ� Sprite���� �ణ �ø���. ��Ʈ ������Ʈ������ "World������Ʈ����" �ڿ�
		//				�������� �ǹǷ� ���� ������ ������ ����.
		//				
		//						=	�־���World��� * World������� * Font�������
		//			
		//			������:
		//				���İ� �޸� World��������� �տ��� ������� ����� �����ϰ� �ȴ�. ������ ����.
		//				�𸣰ڴ�. �ڿ��� ���������� �������� ��Ƣ��Ǵ� ������ �������, ��Ʈ�� ����� 
		//				����� �Ǵ� �� ����. X���� ���� ����� ȸ�����⵵ ����� �����ϴ� �ɷ� ���δ�.

		static D3DXMATRIX adj, adj_for_font;
		static type_bool is_adj_initialized = false;
		if( ! is_adj_initialized)
		{
			D3DXMATRIX adjr, adjs;
			D3DXMatrixRotationY(&adjr, D3DX_PI);
			D3DXMatrixScaling(&adjs, 0.02, 0.02, 0.02);
			adj = adjs * adjr;
			D3DXMatrixTranslation(&adj_for_font, 0, 0, -0.5);
			is_adj_initialized = true;
		}
		if( ! _sprite)
			_initializeSprite();


		//	main:
		if(&_sprite)
		{
			D3DXMATRIX e;
			D3DXMatrixIdentity(&e);
			getBinded().getDevice()->SetTransform(D3DTS_WORLD, &e);
			_sprite->Begin(D3DXSPRITE_DONOTMODIFY_RENDERSTATE | D3DXSPRITE_OBJECTSPACE);
			//	����ε� ����� ã�� ���ؼ� �̷������� �����Ͽ���.
			D3DXMATRIX world = adj * model.getWorldMatrix();
			
			_sprite->SetTransform(&world);
			/*
				Draw�Ǹ޼ҵ����ڼ���:
					1	g_pTexture	:	�ؽ���
					2	RECT		:	�ؽ��Ŀ��������λ���ұ���. UV��ǥ������ȭ��0~1�ǰ��̶���̰��ؽ�����ü���ǹ��Ѵٰ���ȴ�.
					3	VECTOR3		:	RECT���������ѱ���������������ؽ������߽������λ����������������Ѵ�.
					4	VECTOR3		:	SetTransform����������Translation Vector�������Ѵ�.
					5	DWORD		:	����.
			*/
			RECT source_rect = texture.createSourceRect();
			type_uint	width = texture.getWidthOfOneFrame(),
						height = texture.getHeightOfOneFrame();
			D3DXVECTOR3 center_of_texture(width / 2.0f, height / 2.0f, 0.5),
						world_translation(world._41, world._42, world._43);			
			_sprite->Draw(&texture.getTexture(), &source_rect, &center_of_texture, &world_translation, model.createColor());
			if(model.isFontEnabled())
			{
				SpriteTexter& texter = (SpriteTexter&) model.getTexterSelector().getBinder().getBinded();
				Texture& texture = (Texture&) model.getTextureSelector().getBinder().getBinded();

				world *= adj_for_font;
				_sprite->SetTransform(&world);	//	Texter�� ���ؼ� �̸� ��Ʈ��������� Set�Ѵ�.

				if( &texter && &texture)
					texter.render(_sprite, texture);
			}
			_sprite->End();
		}

		return RESULT_SUCCESS;		
	}
						
	type_result Sprite::_initializeSprite()
	{
		//	pre:
		ThisClass::releaseResource();

		DX9& dx9 = getBinded();
		if( ! &dx9)
		{
			ALERT_WARNING("����DX9��Bind �����������մϴ�.");
			return RESULT_TYPE_WARNING;
		}
		LPDIRECT3DDEVICE9 device = dx9.getDevice();


		//	post:
		if(FAILED(D3DXCreateSprite(device, &_sprite)))
		{
			ALERT_WARNING("��������Ʈ ������ �����߽��ϴ�.");
			return RESULT_TYPE_WARNING;
		}
		return RESULT_SUCCESS;
	}

	type_result Sprite::dock(Model& model)
	{
		//	pre:
		if( ! isEnable()) return RESULT_SUCCESS | RESULT_ABORT_ACTION;
		//		DX9 ���ε�:
		DX9& dx9 = getBinded();
		if( ! &dx9)
		{
			ALERT_ERROR(" : ����̽����ε����з�����");
			return RESULT_TYPE_ERROR;
		}
		if( ! &model)
		{
			ALERT_ERROR("���� �����ϴ�.");
			return RESULT_SUCCESS;
		}
		Texture& texture = static_cast<Texture&>(model.getTextureSelector().getBinder().getBinded());
		if( ! &texture)
		{
			ALERT_WARNING("�ؽ��İ� �����ϴ�.");
			return RESULT_TYPE_WARNING;
		}


		//	main:
		LPDIRECT3DDEVICE9 device = dx9.getDevice();		

		return _renderSprite(model, texture, device);
	}

	

}