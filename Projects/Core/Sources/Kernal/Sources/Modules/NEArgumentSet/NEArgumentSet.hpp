#pragma once

#include "../NETArray/NETArray.hpp"
#include "../NEArgumentBase/NEArgumentBase.hpp"

namespace NE
{
	template
	class NE_DLL NETArray<NEArgumentBase*, false>;

	class NE_DLL NEArgumentSet : public NETArray<NEArgumentBase*, false>
	{
	public:
		typedef NEArgumentSet ThisClass;
		typedef NETArray<NEArgumentBase*, false> SuperClass;

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