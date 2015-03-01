#pragma once

#include "../Includes/Includes.hpp"

namespace NE
{
	class NE_DLL FileLister : public NEModule
	{
	public:
		typedef FileLister ThisClass;
		typedef NEModule SuperClass;

	public:
		NETArgument<NETStringKey> arg_path;
		NETArgument<NETStringSetKey> arg_list;
		NETArgument<NEBooleanKey> arg_search_sub_directory;
		NETArgument<NEBooleanKey> arg_get_filename_only;

	protected:
		virtual type_result _onFetchModule()
		{
			arg_path.setPurposeLimitation(NEArgumentBase::READ_BY);
			arg_list.setPurposeLimitation(NEArgumentBase::WRITTEN);
			arg_search_sub_directory.setPurposeLimitation(NEArgumentBase::READ_BY);
			arg_search_sub_directory.setValue(true);
			arg_get_filename_only.setPurposeLimitation(NEArgumentBase::READ_BY);
			arg_get_filename_only.setValue(false);

			return RESULT_SUCCESS;
		}
		virtual type_result _onFetchArguments(NEArgumentList& tray)
		{
			tray.push(arg_path);
			tray.push(arg_list);
			tray.push(arg_search_sub_directory);
			tray.push(arg_get_filename_only);

			return RESULT_SUCCESS;
		}

	protected:
		virtual type_result _onExecute()
		{
			NETStringSet& list = arg_list.getValue();
			type_bool	sub_dir = arg_search_sub_directory.getValue(),
				name_only = arg_get_filename_only.getValue();

			NETStringList buffer;
			NEFileSystem filer(arg_path.getValue().toCharPointer());
			filer.setSearchLowerDirectory(sub_dir);

			typedef NEFileSystem::TaskUnit TaskUnit;

			filer.findFile();
			while (const TaskUnit* e = &filer.getLastElement())
			{
				if (name_only)
					buffer.push(e->getFileName());
				else
					buffer.push(e->getFilePath());
			}


			//	post:
			list.create(buffer.getLength());
			for (NETStringList::Iterator* e = buffer.getIterator(0); e; e = e->getNext())
				list.push(e->getValue());

			return RESULT_SUCCESS;
		}

	public:
		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _instance;

			if (_instance.isValid() != RESULT_NOTHING)
			{
				_instance.getName() = "FileLister";
				_instance.getDeveloper() = "kniz";
				_instance.setRevision(1);
				_instance.getReleaseDate() = "2013/08/16";
				_instance.getComment() =
					"������ ��ο���, ���ϵ�ī��� ��ġ�ϴ� ���ϵ��� Ž���Ͽ� ����մϴ�.";
				NETStringSet& args = _instance.getArgumentsComments();
				args.create(4);
				args.push("Path\nŽ���� ����Դϴ�. ���ϵ� ī�� ��밡���մϴ�.");
				args.push("Listed\nŽ���� ���ϵ��� ���⿡ ����˴ϴ�.");
				args.push("Search Sub Directories too\n���� ���丮�� Ž���մϴ�.");
				args.push("Fetch Filename only\n������ ��Ͽ� ���� �� ��δ� �����ϰ� �̸��� ����ϴ�.");
			}

			return _instance;
		}
		virtual NEObject& clone() const
		{
			return *(new ThisClass(*this));
		}
	};
}