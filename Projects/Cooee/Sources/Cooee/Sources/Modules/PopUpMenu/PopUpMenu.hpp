#pragma once

#include "../ModuleEncyclo/ModuleEncyclo.hpp"
#include "../Helper/Helper.hpp"

class MainPopUpMenu : public ListWindow
{
public:
	MainPopUpMenu() : ListWindow("Main Menu", 30, 8, 20, 7, BLACK, WHITE, WHITE, LIGHTRED)
		FUNC_CLONE(MainPopUpMenu)
		virtual void onUpdateData()
	{
		ListWindow::onUpdateData();

		list.items.create(7);
		list.items.push("���ο� ����");
		list.items.push("�ҷ�����");
		list.items.push("�����ϱ�");
		list.items.push("�������");
		list.items.push("�����ϱ�");
		list.items.push("������");
		list.items.push("����");
	}
	virtual void onItemChoosed(type_index index, const NEString& chosen_content) 
	{ 
		NEEventHandler& handler = NEEditor::getInstance().getEventHandler();

		switch(index) 
		{
		case 0:	//	���ο� ����
			handler.initailizeNewFile();
			break;
		case 1:	//	�ε�
			class LoadScript : public ListWindow
			{
			public:
				NEStringSet real_paths;

				LoadScript() : ListWindow("�о���� ������ �����ϼ���", 30, 5, 20, 15, BLACK, WHITE, WHITE, LIGHTRED)
				{
					updateFileList();
				}
				FUNC_CLONE(LoadScript)

					void updateFileList()
				{
					NEStringList bucket, path_bucket;

					NEFileSystem fs(Kernal::getInstance().getSettings().getScriptDirectory().toCharPointer(), true);
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

					Editor::getInstance().getEventHandler().loadScriptFile(real_paths[chosen]);
					delete_me = true;
				}
			};

			LG::Core::open( LoadScript() );
			break;
		case 2:	//	����

			class SaveScript : public InputWindow
			{
			public:
				SaveScript(const NEString& hint) : InputWindow("������ �����̸��� �Է��ϼ���.", BLACK, WHITE, hint) {}
				FUNC_CLONE(SaveScript)

					virtual void onInputed()
				{
					updateExtendWhenVacant();

					type_result result = Editor::getInstance().getEventHandler().saveScriptFile(input.text);
					if(NEResult::hasError(result))


						delete_me;
				}

				void _updateExtendWhenVacant()
				{
					NEString& inputed = input.text;
					NEString extend = Kernal::getInstance().getSettings().getScriptExtractor();
					type_index 	text_last_index = inputed.getLengthLastIndex() - 1, // without null
						extend_last_index = extend.getLengthLastIndex();
					if(text_last_index <= 0) return;
					NEString has_inputed_extend = inputed.extract(text_last_index - extend_last_index, text_last_index);

					if(has_inputed_extend != extend)
						inputed += extend;
				}
			};

			LG::Core::open( SaveScript() );
			break;
		case 3:	//	�������
			break;
		case 4:	//	�׽�Ʈ
			handler.runTest();
			break;
		case 5:	//	��� ����
			LG::Core::open( ModuleEncyclo());
			break;
		case 6:	//	����
			LG::Core::open(Helper());
			break;
		}
	}

};