#include "ScriptFilePopUpMenu.hpp"
#include "../HeaderModifier/HeaderModifier.hpp"
#include "../Core/Core.hpp"

void ScriptFilePopUpMenu::onUpdateData()
{
	list.items.create(4);
	list.items.push("���� ����");
	list.items.push("�ҷ�����");
	list.items.push("�����ϱ�");
	list.items.push("��� ����");
}

void ScriptFilePopUpMenu::onItemChoosed(type_index index, const NEString& chosen_content)
{
	NEEventHandler& handler = Editor::getInstance().getEventHandler();

	switch(index)
	{
	case 0:	//	���� ����		
		::Core::commander.command("new");
		break;

	case 1:	//	�ҷ� ����
		class LoadScript : public LG::ListWindow
		{
		public:
			NEStringSet real_paths;

			LoadScript() : LG::ListWindow("�о���� ������ �����ϼ���", 40, 10, 20, 6, BLACK, WHITE, WHITE, LIGHTMAGENTA)
			{
				updateFileList();
			}
			virtual NEObject& clone() const { return *(new LoadScript(*this)); }

			void updateFileList()
			{
				NEStringList bucket, path_bucket;

				const NETString& work_directory  = Kernal::getInstance().getSettings().getScriptDirectory(),
							  & extractor		= Kernal::getInstance().getSettings().getScriptExtractor();
				NETString path = work_directory + "*" + extractor;

				NEFileSystem fs(path.toCharPointer(), true);
				const NEFileSystem::TaskUnit* task = 0;
				fs.findFile();
				while(task = &fs.getLastElement())
				{
					bucket.push(task->getFileName());
					path_bucket.push(task->getFilePath());

					fs.findFile();
				}

				//	Bucket���� ���� �����ͷ� �̵�:
				_synchronizeListToArray(bucket, list.items);
				_synchronizeListToArray(path_bucket, real_paths);
			}

			void _synchronizeListToArray(const NEStringList& source, NEStringSet& to)
			{
				to.create(source.getLength());

				for(const NEStringList::Iterator* itr=source.getIterator(0); itr ;itr=itr->getNext())
					to.push(itr->getValue());
			}

			virtual void onItemChoosed(type_index chosen, const NEString& content)
			{
				if(chosen < 0) return;
				
				::Core::commander.command("load " + content);
				
				getCaller().delete_me = delete_me = true;
			}
		};

		call( LoadScript() );
		break;

	case 2:	//	�����ϱ�
		class SaveScript : public LG::InputWindow
		{
		public:
			SaveScript(const NEString& hint) : LG::InputWindow("������ �����̸��� �Է��ϼ���.", BLACK, WHITE, hint) {}
			virtual NEObject& clone() const { return *(new SaveScript(*this)); }

			virtual void onInputed()
			{
				::Core::commander.command("save " + input.text);

				getCaller().delete_me = delete_me = true;
			}
		};

		call(SaveScript(Kernal::getInstance().getScriptManager().getScriptFilePath()) );
		break;

	case 3:	//	��� ����
		call(HeaderModifier());
		delete_me = true;
		break;
	}
}

ScriptFilePopUpMenu::ScriptFilePopUpMenu()
	: LG::ListWindow("��ũ��Ʈ ���� > ", 30, 8, 15, 5, BLACK, WHITE, WHITE, LIGHTCYAN)
{

}
