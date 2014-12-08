#pragma once

#include "../Includes/Includes.hpp"

namespace NE
{
	class NE_DLL BinaryFiler : public NEModule
	{
	public:
		NETArgument<NEBooleanKey>		arg_is_load;
		NETArgument<NETStringKey>		arg_path;		
		NETArgument<NEIntKey>			arg_target0_manager_type;
		NETArgument<NENodeSelector>		arg_target1;
		NETArgument<NEModuleSelector>	arg_target2;
		NETArgument<NEKeySelector>		arg_target3;
		NETArgument<NEIntKey>			arg_mode;

	public:
		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _header;
			if (NEResult::hasError(_header.isValid()))
			{
				_header.getName() = "BinaryFiler";
				_header.getDeveloper() = "kniz";
				_header.setRevision(2);
				_header.getVersion() = "0.0.1a";
				_header.getReleaseDate() = "2014-12-03";
				_header.getComment() =
					"File�� �ε� �� �����ϴ� ����Դϴ�.\n"
					"Binary �� Text ���·� ����/�ε尡 �����ϸ�, \n"
					"Target���� Manager ��ü�� �����̳� Selector�� Ư���� �����͸� ������ �� �ֽ��ϴ�.\n"
					"Target���� �� 4������ �����ϸ�, disable�� �� ���꿡 ������� �ʽ��ϴ�.\n\n"
					"ManagerType�� ������ �����ϴ�.\n"
					"\t0\t:\tNodeManager""\t1\t:\tScriptManager\n"
					"\t2\t:\tModuleManager""\t3\t:\tINIManager\n"
					"\t4\t:\tDebugManager""\t5\t:\tScriptManager\n"
					"\t6\t:\tPanelManager""\t7\t:\tEventHandler";
				NETStringSet& argcomments = _header.getArgumentsComments();
				argcomments.create(7);
				argcomments.push("��������\n�־��� ��� ���Ͽ�, Load(true), Save(false)������ ���մϴ�.");
				argcomments.push("���\n���α׷��� ����� ��θ� �������� ������ ��ġ�� �����ݴϴ�. �̶�, ���ϸ�� Ȯ���ڱ��� ��� ������� �մϴ�.\n��) ../resources/actor.bmp");
				argcomments.push("���0\n�ý��ۿ� �����ϴ� Manager�� ��ä�� ����/�ε� �մϴ�.\n0: NodeManager, 1:ScriptManager, 2:ScriptEditor, 3:ModuleManager, 4:INIManager, 5:DebugManager, 6:PanelManager, 7:EventHandler");
				argcomments.push("���1\nSelector�� ������ ��� ���Ͽ� ������ �մϴ�.\n���0�� ����������, disabled �Ǹ� ������� �ʽ��ϴ�.");
				argcomments.push("���2\nSelector�� ������ ��� ���Ͽ� ������ �մϴ�.\n���0�� ����������, disabled �Ǹ� ������� �ʽ��ϴ�.");
				argcomments.push("���3\nSelector�� ������ ��� ���Ͽ� ������ �մϴ�.\n���0�� ����������, disabled �Ǹ� ������� �ʽ��ϴ�.");
				argcomments.push("����-�ɼ�\n�����, �̹� ������ �ִ� ��쿡 ���� �ɼ��Դϴ�.\n0:�̹� ������ ������ ���[�⺻��], 1:������ ���� �߰�, 2: ������ �������, 3:�������� ����");
			}

			return _header;
		}		

	protected:
		virtual type_result _onExecute()
		{
			NEFileSaverFlag flag = ! arg_mode.isEnable() ? NEFileSaverFlag::NEFILESAVERFLAG_OVERWRITE : NEFileSaverFlag(arg_mode.getValue());
			NEBinaryFileAccessor* accessor = 0x00;
			if(arg_is_load.getValue())
				accessor = &NEBinaryFileLoader(arg_path.getValue().toCharPointer());
			else
				accessor = &NEBinaryFileSaver(arg_path.getValue().toCharPointer(), flag);
			
			accessor->open();
			if( ! accessor->isFileOpenSuccess())
			{
				ALERT_ERROR("������ �� �� �������ϴ�.");

				return RESULT_TYPE_ERROR;
			}


			//	main:
			NEListTemplate<NEObject*> tray;

			if(arg_target0_manager_type.isEnable())
			{
				switch (arg_target0_manager_type.getValue())
				{
				case 0:	tray.push(Kernal::getInstance().getNodeManager());		break;
				case 1: tray.push(Kernal::getInstance().getScriptManager());	break;
				//case 2:	tray.push(Editor::getInstance().getScriptEditor());		break;
				case 2:	tray.push(Kernal::getInstance().getModuleManager());	break;
				case 3:	tray.push(Kernal::getInstance().getINIManager());		break;
				case 4:	tray.push(Kernal::getInstance().getDebugManager());		break;
				//case 6:	tray.push(Editor::getInstance().getPanelManager());		break;
				//case 7:	tray.push(Editor::getInstance().getEventHandler());		break;
				default:
					ALERT_WARNING("%d�� �ش��ϴ� Manager�� �����ϴ�.", arg_target0_manager_type.getValue());

					return RESULT_TYPE_WARNING;
				}					
			}
			if(arg_target1.isEnable())
				while (NENode* e = &arg_target1.getValue().getNode())
					tray.push(e);
			if(arg_target2.isEnable())
				while (NEModule* e = &arg_target2.getValue().getModule())
					tray.push(e);
			if(arg_target3.isEnable())
				while (NEKey* e = &arg_target3.getValue().getKey())
					tray.push(e);


			//	post:
			NEBinaryFileLoader& casted_l = static_cast<NEBinaryFileLoader&>(*accessor);
			NEBinaryFileSaver& casted_s = static_cast<NEBinaryFileSaver&>(*accessor);

			for (NEListTemplate<NEObject*>::Iterator* e = tray.getIterator(0);
				e;
				e = e->getNext())
				if (accessor->isSaver())
					e->getValue().serialize(casted_s);
				else
					e->getValue().serialize(casted_l);

			return RESULT_SUCCESS;
		}

		virtual type_result _onFetchArguments(NEArgumentList& tray)
		{
			tray.push(arg_is_load);
			tray.push(arg_path);
			tray.push(arg_target0_manager_type);
			tray.push(arg_target1);
			tray.push(arg_target2);
			tray.push(arg_target3);
			tray.push(arg_mode);
			return RESULT_SUCCESS;
		}
		virtual type_result _onFetchModule()
		{
			arg_target1.setEnable(false);
			arg_target2.setEnable(false);
			arg_target3.setEnable(false);
			arg_is_load.setDefault(false);

			return RESULT_SUCCESS;
		}

	public:
		virtual NEObject& clone() const
		{
			return *(new BinaryFiler(*this));
		}
	};
}
