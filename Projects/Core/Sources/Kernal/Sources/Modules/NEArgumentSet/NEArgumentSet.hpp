#pragma once

#include "../../Commons/Templates/NEArrayTemplate/NEArrayTemplate.hpp"
#include "../../Commons/Units/NEArgumentBase/NEArgumentBase.hpp"

namespace NE
{
	template
	class NE_DLL NEArrayTemplate<NEArgumentBase*, true>;
	
	class NEArgumentInterfaceBase;
	
	class NE_DLL NEArgumentSet : public NEArrayTemplate<NEArgumentBase*, true>
	{
	public:
		typedef NEArgumentSet ThisClass;
		typedef NEArrayTemplate<NEArgumentBase*, true> SuperClass;

	public:
		friend class NEModule;	//	NEModule �������� serialize�� ����ϵ��� �ϱ� ����.

	public:
		NEArgumentSet(NEModule* owner);
		NEArgumentSet(NEModule* owner,type_count size);
		NEArgumentSet(NEModule* owner, const ThisClass& rhs);

	public:
		ThisClass& operator=(const ThisClass& rhs);

	public:
		NEModule& getOwner();
		const NEModule& getOwner() const;

	public:
		bool isBinded() const;
		type_result bind();

	private:	//	!Important private�ӿ� �����϶�. �ڼ��� ������ serialize(NEBinaryFileSaver& saver) const�� ������ �ּ��� ������ ��
		virtual NEBinaryFileSaver& serialize(NEBinaryFileSaver& saver) const;	//	virtual�� ����ؾ� �ϹǷ� private�ӿ��� �տ� �����(_)�� ������ �ʴ´�
		virtual NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader);

	private:
		NEModule* _owner;
	};
}