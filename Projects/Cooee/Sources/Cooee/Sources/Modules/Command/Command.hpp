#pragma once

#include "../../Includes/Includes.hpp"

class Command : public NEObject {
public:
	Command(const NEString& names_delimetered_with_space, const NEString& new_help);
	NEStringSet names;
	NEString help;
	virtual NEString execute(const NEStringSet& parameters) = 0;
	virtual type_result isValid() const;
	virtual NEBinaryFileSaver& serialize(NEBinaryFileSaver& saver) const;
	virtual NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader);
	virtual void release();
	bool operator!=(const Command& rhs) const;
};

class ListCommand : public Command
{
public:
	ListCommand();
	FUNC_CLONE(ListCommand)
	virtual NEString execute(const NEStringSet& parameters);
};

class CloseCommand : public Command
{
public:
	CloseCommand() : Command("close cls",
		"���� �����츦 �ݽ��ϴ�.\n"
		"��Ī	: close, cls\n"
		"����	: close") {}
	FUNC_CLONE(CloseCommand)
	virtual NEString execute(const NEStringSet& parameters);
};

class VersionCommand : public Command
{
public:
	VersionCommand() : Command("version ver v",
		"���� ������ Ȯ���Ѵ�.\n"
		"��Ī	: version, ver, v\n"
		"����	: version") {}
	FUNC_CLONE(VersionCommand)
	virtual NEString execute(const NEStringSet& parameters);
};

class PlanetarizeCommand : public Command
{
public:
	PlanetarizeCommand() : Command("planetarium pln pl",
		"��ũ��Ʈ ��ü ������ ���ߴ� �ö��Ÿ������ ����\n"
		"��Ī	: planetarium, pl, pln\n"
		"����	: planetarium\n"
		"		  planetarium <������ ���� ���>") {}
	FUNC_CLONE(PlanetarizeCommand)
	virtual NEString execute(const NEStringSet& parameters);
};

class CopyCommand : public Command
{
public:
	CopyCommand() : Command("copy cp",
		"������ ��θ� �����صд�. paste ��ɾ �� ��θ� �����Ѵ�.\n"
		"��Ī	: copy, cp\n"
		"����	: copy <���� ���>\n"
		"		  copy <���� ���> <����� ���>") {}
	FUNC_CLONE(CopyCommand)
	virtual NEString execute(const NEStringSet& parameters);
};
class CutCommand : public Command
{
public:
	CutCommand() : Command("cut",
		"�߶� ��θ� �����صд�. paste ��ɾ �� ��θ� �����ϰ� ������ �����Ѵ�.\n"
		"��Ī	: cut\n"
		"����	: cut<���� ���>\n"
		"		  cut <���� ���> <����� ���>") {}
	FUNC_CLONE(CutCommand)
		virtual NEString execute(const NEStringSet& parameters);
};

class OrphanCommand : public Command
{
public:
	OrphanCommand(const NEString& names, const NEString& help)
		: Command(names, help) {}
	NEString _searchParent(const NEString& full_path, type_index& index_to_insert, NEObject** parent_to_insert);
};
class AddCommand : public OrphanCommand
{
public:
	AddCommand(const NEString& names, const NEString& help)
		: OrphanCommand(names, help) {}
	AddCommand() : OrphanCommand("add a",
		"�־��� ��ο� ���ο� ��ü�� �ϳ� �߰��Ѵ�.\n"
		"��Ī	: add, a\n"
		"����	: add -node <�߰��� NodeCodeSet/Key�� ���>\n"
		"		  add -module <�߰��� ����� �̸�> <������> <revision> <�߰��� ModuleCodeSet/Key�� ���>\n"
		"		  add -key <�߰��� Ű�� �̸�> <�߰��� KeyCodeSet�� ���>") {}
	FUNC_CLONE(AddCommand)
	virtual NEString execute(const NEStringSet& parameters);
private:
	NEType::Type _findKeyTypeBy(const NEString& type_name);
};
class PasteCommand : public OrphanCommand
{
public:
	PasteCommand() : OrphanCommand("paste p",
		"�̸� �����ص� ��θ� �����Ͽ�, Ư�� ��ο� �ٿ��ִ´�. ������ �ٿ����� ����� Ÿ���� ��ġ�Ǿ�� �Ѵ�.\n"
		"��Ī	: paste, p\n"
		"����	: paste <�ٿ����� ���>/<�ٿ����� �ε���>") {}
	FUNC_CLONE(PasteCommand)
	virtual NEString execute(const NEStringSet& parameters);
};
class DeleteCommand : public OrphanCommand
{
public:
	DeleteCommand() : OrphanCommand("delete d",
		"�־��� ��ο� ��ġ�� ��ü�� �����Ѵ�.\n"
		"��Ī	: delete, d\n"
		"����	: delete <���>") {}
	FUNC_CLONE(DeleteCommand)
	virtual NEString execute(const NEStringSet& parameters);
};
class RunCommand : public Command
{
public:
	RunCommand() : Command("run r",
		"������ ��ũ��Ʈ�� ����(�׽�Ʈ) �Ѵ�. CANCLEŰ�� ������ ������ �ߴ� �� �� �ִ�.\n"
		"��Ī	: run, r\n"
		"����	: run (������ �ߴ��� �κк��� �ٽ� �����Ѵ�)\n"
		"		  run <�ݺ��� ������ ī��Ʈ>\n"
		"		  run -new (ó������ �ٽ� �����Ѵ�)") {}
	FUNC_CLONE(RunCommand)
	virtual NEString execute(const NEStringSet& parameters);
};
class SaveCommand : public Command
{
public:
	SaveCommand() : Command("save s",
		"������ ��ũ��Ʈ�� ���Ϸ� �����Ѵ�. ������ ���丮�� ������ �ִ�.\n"
		"��Ī	: save, s\n"
		"����	: save <������ ���ϸ�>\n"
		"		  save (�� ��ũ��Ʈ�� ���ϸ����� ����ȴ�)") {}
	FUNC_CLONE(SaveCommand)
	virtual NEString execute(const NEStringSet& parameters);
};
class LoadCommand : public Command
{
public:
	LoadCommand() : Command("load l",
		"��ũ��Ʈ�� ���Ϸκ��� �ҷ����δ�. ����� ���� �׽�Ʈ�� �ʱ�ȭ�ȴ�.\n"
		"��Ī	: load, l\n"
		"����	: load <�о���� ���ϸ�>") {}
	FUNC_CLONE(LoadCommand)
	virtual NEString execute(const NEStringSet& parameters);
};
class NewCommand : public Command
{
public:
	NewCommand() : Command("new",
		"�ۼ����� ��ũ��Ʈ�� ���ְ� ���� �����Ѵ�.\n"
		"��Ī	: new, n\n"
		"����	: new") {}
	FUNC_CLONE(NewCommand)
	virtual NEString execute(const NEStringSet& parameters);
};
class HelpCommand : public Command
{
public:
	HelpCommand() : Command("help h",
		"���� ������ ����. ��� ��������� ��ɾ� ������ �� �� �ִ�.\n"
		"��Ī	: help\n"
		"����	: help (��ɾ���� �����Ѵ�)\n"
		"		: help -module\n"
		"		: help -module <����>") {}
	FUNC_CLONE(HelpCommand)
	virtual NEString execute(const NEStringSet& parameters);
};
class HeaderCommand : public Command
{
public:
	HeaderCommand() : Command("header head hd",
		"��ũ��Ʈ ����� ������ �� �ִ� ȭ���� ����.\n"
		"��Ī	: header, head, hd\n"
		"����	: header") {}
	FUNC_CLONE(HeaderCommand)
	virtual NEString execute(const NEStringSet& parameters);
};
class ObserveCommand : public Command
{
public:
	ObserveCommand() : Command("observe ob",
		"���� ȭ�鿡 �������� ������, �ۼ����� ��ũ��Ʈ�� "
		"�������� ���� �� � ���� �������� �����Ѵ�.\n"
		"��Ī	: observe, ob\n"
		"����	: observe	(��ũ��Ʈ, ���࿵���� �����ư��� ��ȯ)\n"
		"		  observe -debug (���࿵������ ��ȯ)\n"
		"		  observe -script (��ũ��Ʈ �������� ��ȯ)") {}
	FUNC_CLONE(ObserveCommand)
		virtual NEString execute(const NEStringSet& parameters);
	void _markDeletionToAllWindows();
};