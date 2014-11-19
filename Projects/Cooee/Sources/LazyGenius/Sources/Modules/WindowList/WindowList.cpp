#include "WindowList.hpp"
#include "../Core/Core.hpp"

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

		WORD backup = Core::getColor();
		Core::setColorLock(false);
		Core::back_buffer.setColorLock(false);
		Core::back_buffer.setColor(DARKGRAY, LIGHTGRAY);
		Core::setColor(DARKGRAY, LIGHTGRAY);
		Core::setColorLock(true);
		Core::back_buffer.setColorLock(true);

		for(int n=stack.getLengthLastIndex(); n >= 1 ;n--)
			stack[n].onDraw();

		Core::setColorLock(false);
		Core::back_buffer.setColorLock(false);
		Core::setColor(backup);

		if(stack.getLength() > 0)
			stack[0].onDraw();
	}
}