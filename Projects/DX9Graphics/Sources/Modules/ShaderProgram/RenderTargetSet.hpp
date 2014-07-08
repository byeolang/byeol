/*
	��	RenderTarget*, true�� �����ϴ°�:
		RenderTarget�����ڸ� ���� �� �� �ִ�. ShaderProgram*�� ������ �ϴ�, ������
		������ ����ϹǷ� �⺻�����ڷδ� ������ �� ���� �����̴�.
*/
template
class NE_DLL NEArrayTemplate<ShaderProgram::RenderTarget*, true>;

class NE_DLL RenderTargetSet : public NEArrayTemplate<ShaderProgram::RenderTarget*, true>
{
public:
	typedef NEArrayTemplate<ShaderProgram::RenderTarget*, true> SuperClass;
	typedef RenderTargetSet ThisClass;
	typedef ShaderProgram::RenderTarget Trait;

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
		return (_filled_target_index+1) % getSize(); 
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
	type_result initialize(DX9& dx9)
	{		
		for(int n=0; n < getSize() ;n++)
		{
			RenderTarget& target = getElement(n);
			if( ! target.isInitialized())
				target.initialize(dx9);
		}

		return RESULT_SUCCESS;
	}

private:
	type_int _filled_target_index;
};