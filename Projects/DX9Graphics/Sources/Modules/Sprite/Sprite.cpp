#include "Sprite.hpp"
#include "../DX9/DX9.hpp"
#include "../Model/Model.hpp"
#include "../Texture/Texture.hpp"
#include "../SpriteTexter/SpriteTexter.hpp"

namespace DX9Graphics
{
	D3DXMATRIX Sprite::adj;
	D3DXMATRIX Sprite::adj_for_font;

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

	type_result Sprite::_initializeSprite()
	{
		//	pre:
		ThisClass::releaseResource();

		DX9& dx9 = DX9::getInstancedDX();
		if( ! &dx9)
			return ALERT_WARNING("����DX9��Bind �����������մϴ�.");

		LPDIRECT3DDEVICE9 device = dx9.getDevice();


		//	post:
		if(FAILED(D3DXCreateSprite(device, &_sprite)))
			return ALERT_WARNING("��������Ʈ ������ �����߽��ϴ�.");

		return RESULT_SUCCESS;
	}

	type_result Sprite::_renderSprite(Model& model, Texture& texture, LPDIRECT3DDEVICE9 device)
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
		//					���ο� Sprite�� ��� = ������� * �־��� World ���
		//
		//				��������� �տ��� ���ؾ� �Ѵ�. �ڿ��� ���� ���, ��������� �θ�World��� ��ġ�� 
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
		//						=	World������� * Font������� * �־���World���
		//			
		//			������:
		//				World��������� �տ��� ������� ����� �����ϰ� �ȴ�.
		//				�ڿ��� ���������� �������� ��Ƣ��Ǵ� ������ �������, ��Ʈ�� ����� 
		//				����� �Ǵ� �� ����. X���� ���� ����� ȸ�����⵵ ����� �����ϴ� �ɷ� ���δ�.

		static type_bool is_adj_initialized = false;
		if( ! is_adj_initialized)
		{
			D3DXMATRIX adjr, adjs;
			D3DXMatrixRotationX(&adjr, D3DX_PI);
			D3DXMatrixScaling(&adjs, 0.02f, 0.02f, 0.02f);
			adj = adjs * adjr;
			D3DXMatrixTranslation(&adj_for_font, 0, 0, -0.5f);
			is_adj_initialized = true;
		}
		if( ! _sprite)
			_initializeSprite();


		DX9& dx9 = DX9::getInstancedDX();

		//	main:
		if(	&_sprite	&& 
			&dx9		)
		{
			D3DXMATRIX e;
			D3DXMatrixIdentity(&e);
			dx9.getDevice()->SetTransform(D3DTS_WORLD, &e);
			_sprite->Begin(D3DXSPRITE_DONOTMODIFY_RENDERSTATE | D3DXSPRITE_OBJECTSPACE);
			//	����ε� ����� ã�� ���ؼ� �̷������� �����Ͽ���.
			D3DXMATRIX world = /*adj **/ model.getWorldMatrix();
			
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
			D3DXVECTOR3 center_of_texture(width / 2.0f, height / 2.0f, 1.0f),
						world_translation(world._41, world._42, world._43);			

			_sprite->Draw(&texture.getTexture(), &source_rect, &center_of_texture, &world_translation, model.createColor());
			if(model.arg_texter_binder.isEnable())
			{
				SpriteTexter& texter = model.getTexter();
				Texture& texture = model.getTexture();
				if (!&texter ||
					!&texture)
				{
					ALERT_WARNING(" : Texter�� Texture ���� �ϳ��� ���ε��� �ȵǾ� �ֽ��ϴ�.");
					goto POST;	//	same as that break if blockstatements.
				}
					

				world *= adj_for_font;
				_sprite->SetTransform(&world);	//	Texter�� ���ؼ� �̸� ��Ʈ��������� Set�Ѵ�.

				texter.render(_sprite, texture);
			}

		POST:
			_sprite->End();

			/*
				�浹 Volume Sphere�� Scale�� ����� ���� ����ϸ�, ���������� ������� �ʴ´�.
			*/
			D3DXMATRIX sphere_scale;
			type_float avg_scale = (model.arg_scale_x.getValue() + model.arg_scale_y.getValue()) / 2.0f;
			D3DXMatrixScaling(&sphere_scale, avg_scale, avg_scale, avg_scale);

			sphere_scale = sphere_scale * model.getWorldMatrix();

			_renderSphere(dx9.getDevice(), sphere_scale);
		}

		return RESULT_SUCCESS;		
	}
}