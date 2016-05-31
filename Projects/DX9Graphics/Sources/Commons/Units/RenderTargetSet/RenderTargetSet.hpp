#pragma once 

#include "../RenderTarget/RenderTarget.hpp"

namespace DX9Graphics
{
	/*
	��	RenderTarget*, true�� �����ϴ°�:
	RenderTarget�����ڸ� ���� �� �� �ִ�. ShaderProgram*�� ������ �ϴ�, ������
	������ ����ϹǷ� �⺻�����ڷδ� ������ �� ���� �����̴�.
	*/
	template
	class NE_DLL NETArray<RenderTarget*, true>;

	class NE_DLL RenderTargetSet : public NETArray<RenderTarget*, true>
	{
	public:
		typedef NETArray<RenderTarget*, true> SuperClass;
		typedef RenderTargetSet ThisClass;
		typedef RenderTarget Trait;

	public:
		RenderTargetSet()
			: SuperClass()
		{
			_filled_target_index = 0;
		}

	public:
		type_result notifyTargetFilled()
		{
			_filled_target_index++;
			_filled_target_index %= 2;

			return RESULT_SUCCESS;
		}
		Trait& getFilledTarget()
		{
			return getElement(_filled_target_index);
		}
		const Trait& getFilledTarget() const
		{
			return getElement(_filled_target_index);
		}
		Trait& getEmptyTarget()
		{
			return getElement(getEmptyTargetIndex());
		}
		const Trait& getEmptyTarget() const
		{
			return getElement(getEmptyTargetIndex());
		}
		type_int getFilledTargetIndex() const { return _filled_target_index; }
		type_int getEmptyTargetIndex() const
		{
			return (_filled_target_index + 1) % getSize();
		}

	public:
		virtual void release()
		{
			SuperClass::release();

			_filled_target_index = 0;
		}
		virtual NEObject& clone() const
		{
			return *(new ThisClass(*this));
		}

	public:
		type_result initialize(DX9& dx9);

	private:
		type_int _filled_target_index;
	};
}