#include "CFilter.hpp"
//	�⺻������
CFilter::CFilter(LPDIRECT3DDEVICE9 device, UINT render_target_width, UINT render_target_height)
: _technique(NULL), _err(NULL), _effect(NULL), _enable(false),
_render_target_width(render_target_width), _render_target_height(render_target_height)
{
	_ratio = (float) _render_target_width / _render_target_height;
}
//	�Ҹ���
CFilter::~CFilter()
{
	release();
}	
//	���� ���� (�ݹ�)
void	CFilter::release()
{
	_technique = NULL;
	if (_effect)
	{
		_effect->Release();
		_effect = NULL;
	}	
	if (_err)
	{
		_err->Release();
		_err = NULL;
	}
}	
//	���ؽ��� �������� (CEngine���� ���� �ڵ尡 ����)
void	CFilter::drawVertex(LPDIRECT3DDEVICE9 device)	
{
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
//	���̴� ����(������Ʈ����)�� �о���δ�.
void	CFilter::loadShaderFile(LPCTSTR filename, LPDIRECT3DDEVICE9 device)
{
	if (_effect)
	{
		CError e
		(	
			_T("���10:\t���̴��� �����Ϸ� ������, �̹� �����մϴ�. ���� ���̴��� �����ϰ� ����ϴ�."),
			_T("�ذ���:"),
			_T(__FUNCTION__)
		);
		_effect->Release();
		_effect = NULL;
	}
	//	shader���� �ε�
	FILE*	file_pointer;		
	_tfopen_s(&file_pointer, filename, _T("rb"));

	if (file_pointer == NULL)
		return ;

	fseek(file_pointer, 0L, SEEK_END);
	long	size = ftell(file_pointer);

	fseek(file_pointer, 0L, SEEK_SET);
	TCHAR*	source_code = new TCHAR[size];
	fread(source_code, size, sizeof(TCHAR), file_pointer);
	fclose(file_pointer);

	//	shader ����		
	if ( FAILED(D3DXCreateEffect(device, source_code, size, NULL, NULL, D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY, NULL, &_effect, &_err)) )
	{
		//	���. ����Ʈ����Ʈ�� ������� ����
		CString compile_message = _T("");
		if (_err)
			compile_message = (LPCTSTR) _err->GetBufferPointer();

		CError e
		(	
			_T("���08:\t���̴���, �ʱ�ȭ ���� ���߽��ϴ�.") + compile_message, 
			_T("�ذ���:"),
			_T(__FUNCTION__)
		);
		e.log();			
		_enable = false; // Ư�� ���̴��� ���� �ε带 ���Ѱ��, �� ���̴��� ������� ���ϵ��� �ؾ��Ѵ�
	}			
	else
		_enable = true;
}
//	���ؽ��� �Ҵ��Ѵ�.
void	CFilter::setVertex(LPDIRECT3DVERTEXBUFFER9 vertex_buffer, CArea vertex_area , DWORD color, CArea texture_area)
{	
	MYVERTEX*	vertex;
	//	���� �ᱸ��:
	vertex_buffer->Lock(	0, 4 * sizeof(MYVERTEX), (void**) &vertex, NULL);	
	//	���ؽ� ����:		
	//		����:
	vertex[0].x = vertex_area._x1;		vertex[0].y = vertex_area._y2;
	vertex[0].tu = texture_area._x1;	vertex[0].tv = texture_area._y1;
	vertex[0].z = 1.0f;					vertex[0].color = color;
	//		�»�:
	vertex[1].x = vertex_area._x1;		vertex[1].y = vertex_area._y1;
	vertex[1].tu = texture_area._x1;	vertex[1].tv = texture_area._y2;
	vertex[1].z = 1.0f;					vertex[1].color = color;
	//		����:
	vertex[2].x =vertex_area._x2;		vertex[2].y = vertex_area._y2;
	vertex[2].tu = texture_area._x2;	vertex[2].tv = texture_area._y1;
	vertex[2].z = 1.0f;					vertex[2].color = color;
	//		���:
	vertex[3].x = vertex_area._x2;		vertex[3].y = vertex_area._y1;
	vertex[3].tu = texture_area._x2;	vertex[3].tv = texture_area._y2;
	vertex[3].z = 1.0f;					vertex[3].color = color;
	//	���� Ǯ��:
	vertex_buffer->Unlock();
}
//	����������Ʈ�� �����Ѵ�.
void	CFilter::setRenderState(LPDIRECT3DDEVICE9 device, char mode)
{
	if (mode == DEFAULT_CENGINE_RENDERSTATE_LINEAR)
	{
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	}
	else if (mode == DEFAULT_CENGINE_RENDERSTATE_ALPHABLENDING)
	{
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	}
	else if (mode == DEFAULT_CENGINE_RENDERSTATE_DARKEN)
	{
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
	}
	else if (mode == DEFAULT_CENGINE_RENDERSTATE_NEGATIVE)
	{
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);
	}			 
}