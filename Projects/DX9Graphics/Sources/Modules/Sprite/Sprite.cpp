#include "Sprite.hpp"
#include "../DX9/DX9.hpp"
#include "../Model/Model.hpp"
#include "../Texture/Texture.hpp"
#include "../SpriteTexter/SpriteTexter.hpp"

// A structure for our custom vertex type
struct CUSTOMVERTEX
{
	FLOAT x, y, z;      // The untransformed, 3D position for the vertex
	DWORD color;        // The vertex color
};

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)

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
		// Initialize three vertices for rendering a triangle
		CUSTOMVERTEX g_Vertices[] =
		{
			{ -1.0f,-1.0f, 0.0f, 0xffff0000, },
			{  -1.0f,1.0f, 0.0f, 0xff0000ff, },
			{  1.0f, -1.0f, 0.0f, 0xffffffff, },
			{  1.0f, 1.0f, 0.0f, 0xffffffff, },
		};

		// Create the vertex buffer.
		if( FAILED( device->CreateVertexBuffer( 4 * sizeof( CUSTOMVERTEX ),
			0, D3DFVF_CUSTOMVERTEX,
			D3DPOOL_DEFAULT, &_sprite, NULL ) ) )
		{
			return E_FAIL;
		}

		// Fill the vertex buffer.
		void* gpu_vertex = 0;
		if( FAILED( _sprite->Lock( 0, sizeof( g_Vertices ), ( void** )&gpu_vertex, 0 ) ) )
			return RESULT_TYPE_WARNING;
		memcpy( gpu_vertex, g_Vertices, sizeof( g_Vertices ) );
		_sprite->Unlock();

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

		static type_bool is_adj_initialized = false;
		if( ! is_adj_initialized)
		{
			D3DXMATRIX adjr, adjs;
			D3DXMatrixRotationX(&adj, D3DX_PI);
			//D3DXMatrixScaling(&adjs, 0.02f, 0.02f, 0.02f);
			//adj = adjs * adjr;
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
			
			//	�����׽�Ʈ ����:
			//		������ ���ؼ��� �����׽�Ʈ�� ����Ǿ�� �ȵȴ�. ��� ���� �׽�Ʈ�� ����.
			device->SetRenderState(D3DRS_ZENABLE, FALSE);	
			//		���� �غ�:
			model.adjustBlendingOption();
			device->SetTransform(D3DTS_WORLD, &model.getWorldMatrix());
			device->SetTexture(0, &model.getTexture().getTexture());
			//		������:
			device->BeginScene();
			device->SetStreamSource(0, _sprite, 0, sizeof(CUSTOMVERTEX));
			device->SetFVF(D3DFVF_CUSTOMVERTEX);
			device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
			device->EndScene();
			
			//	���� �׽�Ʈ ����:
			device->SetRenderState(D3DRS_ZENABLE, TRUE);

			/*
				�浹 Volume Sphere�� Scale�� ����� ���� ����ϸ�, ���������� ������� �ʴ´�.
			*/
			D3DXMATRIX sphere_scale;
			type_float avg_scale = (model.arg_scale_x.getValue() + model.arg_scale_y.getValue()) / 2.0f;
			D3DXMatrixScaling(&sphere_scale, avg_scale, avg_scale, avg_scale);

			sphere_scale = sphere_scale * model.getWorldMatrix();

			//_renderSphere(dx9.getDevice(), sphere_scale);
		}

		return RESULT_SUCCESS;		
	}
}