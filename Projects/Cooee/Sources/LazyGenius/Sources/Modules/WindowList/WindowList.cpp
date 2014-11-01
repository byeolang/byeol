#include "WindowList.hpp"

namespace LG
{
	void WindowList::collectGarbages()
	{
		NEListTemplate<type_index> indexes;

		int n=0;
		for(Iterator* itr=getIterator(0); itr ;itr=itr->getNext())
		{
			if(itr->getValue().delete_me) {
				indexes.pushFront(n);
				itr->getValue().delete_me = false;	//	Ȥ�ö� �� �����찡 ��Ƴ��ƾ��� ��, ��� delete_me�� ���� �ʵ��� flag�� ���ش�.
			}
			n++;
		}

		for(NEListTemplate<type_index>::Iterator* itr=indexes.getIterator(0); itr ;itr=itr->getNext()) {
			type_index index = itr->getValue();
			getElement(index).onTerminate();
			SuperClass::remove(index);
		}

		if(indexes.getLength() > 0)
			_focusNewWindow();
	}

	void WindowList::draw()
	{
		NEArrayTemplate<Window*, false> stack(getLength());
		for(Iterator* itr=getIterator(0); itr ;itr=itr->getNext())
			stack.push(&(itr->getValue()));

		for(int n=stack.getLengthLastIndex(); n >= 0 ;n--)
			stack[n].onDraw();
	}
}