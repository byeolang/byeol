#pragma once

#include "../../Commons/Templates/NEArrayTemplate/NEArrayTemplate.hpp"
#include "../../Commons/Units/NEArgumentBase/NEArgumentBase.hpp"

namespace NE
{
	template
	class NE_DLL NEArrayTemplate<NEArgumentBase*, false>;

	class NE_DLL NEArgumentSet : public NEArrayTemplate<NEArgumentBase*, false>
	{
	public:
		typedef NEArgumentSet ThisClass;
		typedef NEArrayTemplate<NEArgumentBase*, false> SuperClass;

	public:
		NEArgumentSet();
		NEArgumentSet(type_count size);
		NEArgumentSet(const ThisClass& rhs);

	public:
		bool isBinded() const;
		type_result bind();	

	public:
		virtual NEBinaryFileSaver& serialize(NEBinaryFileSaver& saver) const;	//	virtual�� ����ؾ� �ϹǷ� private�ӿ��� �տ� �����(_)�� ������ �ʴ´�
		virtual NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader);
	};
}