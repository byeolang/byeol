#pragma once

#include "../Particle/Particle.hpp"
#include "../ShaderProgram/ShaderProgram.hpp"

namespace DX9Graphics
{
	using namespace NE;

	class NE_DLL Camera : public Particle
	{
	public:
		typedef Particle SuperClass;
		typedef Camera ThisClass;
		typedef NETVector<ShaderProgram*, true> ShaderProgramSet;

	public:
		NETArgument<NENodeSelector>		arg_targets;		
		NETArgument<NEUIntKey>			arg_viewport_x;
		NETArgument<NEUIntKey>			arg_viewport_y;
		NETArgument<NEUIntKey>			arg_viewport_width;
		NETArgument<NEUIntKey>			arg_viewport_height;
		NETArgument<NEFloatKey>			arg_viewport_minimum_z;
		NETArgument<NEFloatKey>			arg_viewport_maximum_z;
		NETArgument<NEModuleCodeSetKey>	arg_shaders;

	public:
		Camera()
			: SuperClass()
		{
			_release();
		}
		Camera(const Camera& source)
			: SuperClass(source)
		{
			_assign(source);
		}

	public:
		const ThisClass& operator=(const ThisClass& source)
		{
			if(this == &source) return *this;

			NEModule::operator=(source);

			return _assign(source);
		}
		bool operator==(const ThisClass& source) const
		{
			if(	SuperClass::operator!=(source)		||
				_look		!= source._look			||
				_projection	!= source._projection	||
				_view		!= source._view			)
				return false;

			return true;			
		}
		bool operator!=(const ThisClass& source) const
		{
			return ! operator==(source);
		}

	public:
		D3DXMATRIX& getViewMatrix() { return _view; }
		const D3DXMATRIX& getViewMatrix() const { return _view; }
		D3DXMATRIX& getProjectionMatrix() { return _projection; }
		const D3DXMATRIX& getProjectionMatrix() const { return _projection; }

	protected:
		virtual type_result _onFetchModule()
		{
			SuperClass::_onFetchModule();

			arg_viewport_x.setPurposeLimitation(NEArgumentBase::READ_BY);
			arg_viewport_x.setEnable(false);
			arg_viewport_y.setPurposeLimitation(NEArgumentBase::READ_BY);
			arg_viewport_width.setPurposeLimitation(NEArgumentBase::READ_BY);
			arg_viewport_height.setPurposeLimitation(NEArgumentBase::READ_BY);
			arg_viewport_minimum_z.setValue(1.0f);
			arg_viewport_minimum_z.setPurposeLimitation(NEArgumentBase::READ_BY);
			arg_viewport_maximum_z.setValue(100.0f);
			arg_viewport_maximum_z.setPurposeLimitation(NEArgumentBase::READ_BY);

			return RESULT_SUCCESS;
		}
		virtual type_result _onFetchArguments(NEArgumentList& tray)
		{
			tray.push(arg_targets);

			SuperClass::_onFetchArguments(tray);

			tray.push(arg_viewport_x);			
			tray.push(arg_viewport_y);			
			tray.push(arg_viewport_width);		
			tray.push(arg_viewport_height);		
			tray.push(arg_viewport_minimum_z);	
			tray.push(arg_viewport_maximum_z);	
			tray.push(arg_shaders);	

			return RESULT_SUCCESS;
		}

	public:
		const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _header;

			if(_header.getArgumentsComments().getLength() <= 0)
			{
				_header.getComment() = 
					"�������� ������(������ ȭ�鿡 ����) ������ ī�޶� �����մϴ�.\n"
					"ī�޶�� �ڽ��� ��ġ��, ���� �ִ� ����, ������ ����, ���� ���, ������ ����� �׷��� ������ ȿ������ ������ �� �ֽ��ϴ�.";
				NETStringSet& args = _header.getArgumentsComments();
				const NETStringSet& org = SuperClass::getHeader().getArgumentsComments();
				args.resize(org.getLength() + 8);
				args.push("Targets\nī�޶� �Կ��� Particle Module�� ������ �ִ� ���");

				args.push(org);

				args.push("ViewportX\nȭ�鿡 �׷����Ե� ������ X ��ǥ\nDisabled��, �ڵ����� �������� ũ�⸸ŭ �������ݴϴ�.");
				args.push("ViewportY\nȭ�鿡 �׷����Ե� ������ Y ��ǥ");
				args.push("ViewportWidth\nȭ�鿡 �׷����Ե� ������ ��");
				args.push("ViewportHeight\nȭ�鿡 �׷����Ե� ������ ����");
				args.push("ViewportMinimumZ\nȭ�鿡 �׷����Ե� ������ Z near");
				args.push("ViewportMaximumZ\nȭ�鿡 �׷����Ե� ������ Z far");
				args.push("ShaderPrgrams\n������ Shader��");
			}

			return _header;
		}
		virtual type_result render();
		virtual type_result _onExecute()
		{
			if( ! isEnable()) return RESULT_SUCCESS | RESULT_ABORT_ACTION;

			//	main:
			_updateLookVector();
			_updateViewMatrix();
			return _updateProjectionMatrix();
		}
		virtual NEBinaryFileSaver& serialize(NEBinaryFileSaver& saver) const
		{
			SuperClass::serialize(saver);

			saver.execute((char*) &_view, sizeof(D3DXMATRIX));
			saver.execute((char*) &_projection, sizeof(D3DXMATRIX));
			saver.execute((char*) &_look, sizeof(D3DXVECTOR3));

			return saver;
		}
		virtual NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader)
		{
			SuperClass::serialize(loader);

			loader.execute((char*) &_view, sizeof(D3DXMATRIX));
			loader.execute((char*) &_projection, sizeof(D3DXMATRIX));
			loader.execute((char*) &_look, sizeof(D3DXVECTOR3));

			return loader;
		}
		virtual void release()
		{
			SuperClass::release();

			_release();
		}

	public:

		D3DVIEWPORT9 createViewPort() const
		{
			if( ! arg_viewport_x.isEnable()) return createMaximizedViewPort();

			D3DVIEWPORT9 viewport = {
				arg_viewport_x.getValue(), arg_viewport_y.getValue(), 
				arg_viewport_width.getValue(), arg_viewport_height.getValue(),
				arg_viewport_minimum_z.getValue(), arg_viewport_maximum_z.getValue()
			};			

			return viewport;
		}
		D3DVIEWPORT9 createMaximizedViewPort() const
		{
			D3DVIEWPORT9 to_return;
			const DX9& dx9 = DX9::getInstancedDX();
			if( ! &dx9)
			{
				ALERT_ERROR(" : ���ε� ����");
				to_return.X = 0;
				to_return.Y = 0;
				to_return.Width = 0;
				to_return.Height = 0;
				to_return.MinZ = 0.0f;
				to_return.MaxZ = 1.0f;
			}
			else
			{
				const DX9::PresentParameters& parameters = dx9.getPresentParameters();
				to_return.X = 0;
				to_return.Y = 0;
				to_return.Width = parameters.BackBufferWidth;
				to_return.Height = parameters.BackBufferHeight;
				to_return.MinZ = 0.0f;
				to_return.MaxZ = 1.0f;
			}

			return to_return;		
		}

	private:	
		type_result _dockTransforms()
		{
			DX9& dx9 = DX9::getInstancedDX();
			if( ! &dx9)
				return ALERT_ERROR("DX9�� ���ε� ���� �ʾҽ��ϴ�.");

			LPDIRECT3DDEVICE9 device = dx9.getDevice();

			device->SetTransform(D3DTS_VIEW, &_view);
			device->SetTransform(D3DTS_PROJECTION, &_projection);
			return RESULT_SUCCESS;
		}
		virtual type_result _updateProjectionMatrix() = 0;
		type_result _updateViewMatrix()
		{
			//	Ÿ����:				
			D3DXVECTOR3	pos		= createTranslationVector(),
				look_at = pos + _look,
				up		= createTransformedVectorByYawPitchRoll(D3DXVECTOR3(0, 1, 0));


			//	main:			
			D3DXMatrixLookAtLH(&_view, &pos, &look_at, &up);
			return RESULT_SUCCESS;
		}	
		type_result _updateLookVector()
		{
			_look = createTransformedVectorByYawPitchRoll(D3DXVECTOR3(0, 0, 1));

			return RESULT_SUCCESS;
		}
		void _release()
		{
			D3DXMATRIX i;
			D3DXMatrixIdentity(&i);

			_view = _projection = i;
			_look = D3DXVECTOR3();
		}
		ThisClass& _assign(const ThisClass& source)
		{
			if(this == &source) return *this;

			_view = source._view;
			_projection = source._projection;
			_look = source._look;
			arg_targets = source.arg_targets;
			arg_viewport_x = source.arg_viewport_x;
			arg_viewport_y = source.arg_viewport_y;
			arg_viewport_height = source.arg_viewport_height;
			arg_viewport_width = source.arg_viewport_width;
			arg_viewport_minimum_z = source.arg_viewport_minimum_z;
			arg_viewport_maximum_z = source.arg_viewport_maximum_z;
			arg_shaders = source.arg_shaders;

			return *this;
		}
		type_index _getLastProgramIndex(NEModuleCodeSet& within);

	private:
		D3DXMATRIX _view;
		D3DXMATRIX _projection;
		D3DXVECTOR3 _look;

	public:
		static const NECodeSet& getModuleScriptCodes();
	};
}