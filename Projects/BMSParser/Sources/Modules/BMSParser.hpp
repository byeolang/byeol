#pragma once

#include "../Includes/Includes.hpp"
#pragma warning(push)
#pragma warning(disable: 4244)	// float ����ȯ�� ������ �ս� ���ɼ� ���
/*
	cosmic.mearie.org/2005/03/bmsguide ����
	�ճ�Ʈ ������,
	#STP Ű���� ������
	#IF, #ENDIF �� ������

note type:
01	�����			02	���� ���� ����		03	BPM		04	BGA		06	PoorBGA		07	Layer	08	Ȯ��BPM
11	Z				12	S					13	X		14	D		15	C			16	Scratch
17	Freezone		18	F					19	V
*/

namespace BMSFormatter_kniz
{
	using namespace std;
	using namespace NE;

	class BMSParser : public NEModule
	{
	public:
		//	�⺻��	���˸�		����
		NETArgument<NETStringKey>		arg_path;
		NETArgument<NEIntKey>			arg_player_count;		//						3�� �����÷���
		NETArgument<NETStringKey>		arg_genre;
		NETArgument<NETStringKey>		arg_title;
		NETArgument<NETStringKey>		arg_artist;
		NETArgument<NEIntKey>			arg_bpm;				//	130
		NETArgument<NEIntKey>			arg_playlevel;
		NETArgument<NEIntKey>			arg_judgement_level;	//			RANK
		NETArgument<NEIntKey>			arg_total_in_manual;	//			TOTAL		�������� �Է��� ��Ʈ�� ����. ���� ������Ʈ���� ���� ���� ������ ���Ұ� ������
		NETArgument<NEIntKey>			arg_volume;				//	100		VOLWAV
		NETArgument<NETStringKey>		arg_stage_bga;			//			STAGEFILE
		NETArgument<NETStringKey>		arg_videofile;			//			VIDEOFILE	BGA��ſ� ����ȴ�. ��, ��� BGA ����� ���õȴ�.
		NETArgument<NEModuleCodeSetKey>	arg_on_found_headers;
		NETArgument<NETStringKey>		arg_type;				//	BMP or WAV
		NETArgument<NEFloatKey>			arg_value;
		NETArgument<NETStringKey>		arg_resource_path;
		NETArgument<NEModuleCodeSetKey>	arg_on_load_resource;	//	on_load_resource(resource_type, resource_path, value)
		NETArgument<NEIntKey>			arg_pos;
		NETArgument<NEModuleCodeSetKey>	arg_on_found_note;		//	on_found_note(type, pos, value)

	protected:
		virtual type_result _onFetchModule()
		{
			arg_path.setPurposeLimitation(NEArgumentBase::READ_BY);
			arg_player_count.setValue(1);
			arg_player_count.setPurposeLimitation(NEArgumentBase::WRITTEN);
			arg_genre.setValue("Unknown");
			arg_genre.setPurposeLimitation(NEArgumentBase::WRITTEN);
			arg_title.setValue("Untitled");
			arg_title.setPurposeLimitation(NEArgumentBase::WRITTEN);
			arg_artist.setValue("Unknown");
			arg_artist.setPurposeLimitation(NEArgumentBase::WRITTEN);
			arg_bpm.setValue(130);
			arg_bpm.setPurposeLimitation(NEArgumentBase::WRITTEN);
			arg_playlevel.setPurposeLimitation(NEArgumentBase::WRITTEN);
			arg_judgement_level.setPurposeLimitation(NEArgumentBase::WRITTEN);
			arg_total_in_manual.setPurposeLimitation(NEArgumentBase::WRITTEN);
			arg_volume.setValue(100);
			arg_volume.setPurposeLimitation(NEArgumentBase::WRITTEN);
			arg_stage_bga.setPurposeLimitation(NEArgumentBase::WRITTEN);
			arg_videofile.setPurposeLimitation(NEArgumentBase::WRITTEN);			

			arg_type.setPurposeLimitation(NEArgumentBase::WRITTEN);
			arg_value.setPurposeLimitation(NEArgumentBase::WRITTEN);
			arg_resource_path.setPurposeLimitation(NEArgumentBase::WRITTEN);
			arg_on_load_resource.setEnable(true);

			arg_pos.setPurposeLimitation(NEArgumentBase::WRITTEN);
			arg_on_found_note.setEnable(true);

			return RESULT_SUCCESS;
		}

		virtual type_result _onFetchArguments(NEArgumentList& tray)
		{
			tray.push(arg_path);
			tray.push(arg_player_count);
			tray.push(arg_genre);
			tray.push(arg_title);
			tray.push(arg_artist);
			tray.push(arg_bpm);
			tray.push(arg_playlevel);
			tray.push(arg_judgement_level);
			tray.push(arg_total_in_manual);
			tray.push(arg_volume);
			tray.push(arg_stage_bga);
			tray.push(arg_videofile);
			tray.push(arg_on_found_headers);
			tray.push(arg_type);
			tray.push(arg_value);
			tray.push(arg_resource_path);
			tray.push(arg_on_load_resource);
			tray.push(arg_pos);
			tray.push(arg_on_found_note);

			return RESULT_SUCCESS;
		}

		virtual type_result _onExecute()
		{
			NEFloatSet bpms;
			NEIntSet stops;
			bpms.create(toIndex("ZZ") + 1);
			stops.create(toIndex("ZZ") + 1);
			bool is_header_handler_triggered = false;

			NESequencialFileLoader loader(arg_path.getValue().toCharPointer());
			loader.open();
			if (!loader.isFileOpenSuccess())
				return ALERT_WARNING("�־��� %s ������ �����ϴ�.", arg_path.getValue().toCharPointer());

			while (!loader.isEndOfFile())
			{
				TCHAR buffer[512] = { 0, };
				loader.readByLine(buffer, 512);
				NETString line = buffer;
				NETStringSet split;
				line.split(" ", split);
				NETString rhs;
				for (int n = 1; n < split.getLength(); n++)
					rhs += split[n] + " ";

				if (line[0] != '#') continue;

				arg_type.setValue(line.extract(1, 1 + 2));
				if(	arg_type.getValue() == "WAV"		||
					arg_type.getValue() == "BMP"		&&
					arg_on_load_resource.isEnable()		)
				{
					int n = toIndex(line.extract(4, 4 + 1));
					arg_value.setValue(n);
					if (split.getLength() <= 1)
						continue;

					arg_resource_path.setValue(split[1]);
					arg_on_load_resource.getValue().execute();					
				}
				else if (split[0] == "#PLAYER")
					arg_player_count.setValue(split[1].toInt());
				else if (split[0] == "#GENRE")
					arg_genre.setValue(rhs);
				else if (split[0] == "#TITLE")
					arg_title.setValue(rhs);
				else if (split[0] == "#ARTIST")
					arg_artist.setValue(rhs);
				else if (split[0] == "#BPM")
					arg_bpm.setValue(split[1].toInt());
				else if (split[0] == "#PLAYLEVEL")
					arg_playlevel.setValue(split[1].toInt());
				else if (split[0] == "#RANK")
					arg_judgement_level.setValue(split[1].toInt());
				else if (split[0] == "#TOTAL")
					arg_total_in_manual.setValue(split[1].toInt());
				else if (split[0] == "#STAGEFILE")
					arg_stage_bga.setValue(rhs);
				else if (split[0] == "#VIDEOFILE")
					arg_stage_bga.setValue(rhs);
				else if (split[0] == "#VOLWAV")
					arg_stage_bga.setValue(split[1].toInt());
				else if (split[0] == "#BPM")
				{
					type_index n = toIndex(split[0].extract(4, 4 + 1).toInt());
					if (n < 0 || n > toIndex("ZZ"))
						ALERT_ERROR("�ε��� �ʰ�")
					else
						bpms[n] = split[1].toDouble();
				}
				else if (split[0] == "#STOP")
				{
					type_index n = toIndex(split[0].extract(5, 5 + 1).toInt());
					if (n < 0 || n > toIndex("ZZ"))
						ALERT_ERROR("�ε��� �ʰ�")
					else
						stops[n] = split[1].toInt();
				}
				else if (split[0][1] >= '0'	&&
						split[0][1] <= '9')
				{
					if (!is_header_handler_triggered)
					{
						is_header_handler_triggered = true;

						if (arg_on_found_headers.isEnable())
							arg_on_found_headers.getValue().execute();
					}

					arg_type.setValue(split[0].extract(4, 5));
					type_int base_position = split[0].extract(1, 1 + 2).toInt() * 192;

					line.split(":", split);
					_parseLineForNotes(split[1], base_position, bpms, stops);
				}
			}
			loader.close();

			return RESULT_SUCCESS;
		}

	public:
		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _header;

			if (NEResult::hasError(_header.isValid()))
			{
				_header.getName() = "BMSParser";
				_header.getDeveloper() = "kniz";
				_header.setRevision(1);
				_header.getComment() =
					"������ӿ� ���Ǵ� .BMS ������ ������ �о ��ȿ�� ������ �Ľ��մϴ�.\n";
					"ũ�� 3��Ʈ�� �з��Ǹ�, �� ��Ʈ�� �Ľ��۾��� ������ ������ ModulecodeSet�� execute �մϴ�.\n"
					"��Ʈ1	:��� �Ľ�\n"
					"	PlayerCount ���� VideoFile ������ Argument�� ����� �� onFoundHeaders�� ����˴ϴ�.\n"
					"��Ʈ2	:���ҽ� �Ľ�	onFoundResource(Type, ResourcePath, Value)\n"
					"	���ҽ� �ϳ��� �Ľ��Ҷ����� Type�� ResourcePath, Value Argument�鿡 ���� ����� �� onFoundResource�� ����˴ϴ�.\n"
					"��Ʈ3	:��Ʈ �Ľ�		onFoundNote(Type, Position, Value)\n"
					"	��Ʈ�� �ϳ� �Ľ��Ҷ����� Type�� ��ġ(�Ѹ��� 192�� ǥ���������� ��ġ��), ��Ʈ�� �����ϴ� ���� ����� �� onFoundNote�� ����˴ϴ�.\n";
				_header.getVersion() = "2015-02-13";				
				NETStringSet& args = _header.getArgumentsComments();
				args.create(19);
				args.push("BMS�� ��� �Դϴ�.");
				args.push("PlayerCount\nonFoundHeader�� ����ɶ� �� ���� �� �� Player�� BMS ���� ��ϵ˴ϴ�.\n1: 1�� �÷���\t2:2�� �÷���(���� ä���� �ٸ�)\t3:1�� �����÷���\n4:2�� �÷���(��, ä���� ����)");
				args.push("Genre");
				args.push("Title");
				args.push("Artist");
				args.push("BPM\n�⺻���� 130�̸�, ���� BPM��Ʈ�� ���ؼ� ���� �� �� �ֽ��ϴ�.");
				args.push("PlayLevel\n�����ڰ� ���� BMS�� ���̵�.");
				args.push("Judgement Level\n������ �󸶳� �����ؾ� �ϴ��� �����ڰ� ��û�ϴ� ���������Դϴ�.\n0: �ſ� �����ϰ�\t1: �����ϰ�\n\t2:����\t3:�����ϰ�");
				args.push("Total\n�����ڰ� �������� �Է��� ��Ʈ�� ����");
				args.push("Volume\n���� ������ �����Դϴ�. 100�� �����Դϴ�.");
				args.push("StageBga Path\n�ε� ��, �׸��� ����� �׸������� ����Դϴ�.");
				args.push("VideoFile\nBGA ��ſ� ����� ������ �����Դϴ�. �� ���� BMS ���Ͽ� ������� ��Ÿ BGA�� ���õ� ����� ��� ��ҵǾ�� �մϴ�.");
				args.push("onFoundHeaders\n���� ������ ��� �Ľ��Ͽ����� ����˴ϴ�.");
				
				args.push(
					"Type\n��Ʈ�� ���ҽ��� Ÿ���� �ǹ��մϴ�.\n"
					"BMP: �׸� ���ҽ��Դϴ�.\tWAV: ���� �����Դϴ�.\n"
					"������ ��Ʈ�� Ÿ�Ե��Դϴ�.\n"
					"01: �����\t02: ���� ���̸� ������\t03: BPM��ȯ ��Ʈ\n"
					"04: BGA��Ʈ\t06: �������н� �� PoorBGA��Ʈ\t07: BGA���� �׷��� Layer\n"
					"11:1��° ����ǹ�\t12:1��° �����ǹ�\t13:2��° ����ǹ�\n"
					"14:2��° �����ǹ�\n15:3��° ����ǹ�\t16:��ũ��ġ\n"
					"17:FreeZone\t18:3��° �����ǹ�(7Ű)\t19:4��° ����ǹ�(7Ű)"
				);
				args.push("Value\n��Ʈ�� ���Դϴ�.");
				args.push("ResourcePath\n���ҽ��� ��ΰ� ������ϴ�.");
				args.push("onLoadResource(Type, ResourcePath, Value)\n���� ���ҽ� ������ �Ľ��Ͽ�, �ε带 �ؾ��ϴ� ����, ����˴ϴ�.\nValue���� Resource�� �ε����� ���ϴ�.");
				args.push("Position\n��Ʈ�� ��ġ�Դϴ�.\n1���� 192�� ǥ���������� ���� �����Դϴ�.\n������� 288�� 192 + 96����, 1�� index ������ �߰��� ���ϰ� �˴ϴ�.");
				args.push("onFoundNote(Type, Position, Value)\n���� ��Ʈ ������ �Ľ�������, Type, Position, Value�� �Ľ��� ���� ��� �� ModuleCodeSet�� ����˴ϴ�.\nValue�� �ռ� �ε��� Resource�� �ε����� ��ġ�ϴ� ���Դϴ�.");
			}

			return _header;
		}

		virtual NEObject& clone() const
		{
			return *(new BMSParser(*this));
		}

	public:
		type_int toDecimal(TCHAR _36hex) const
		{
			if (_36hex >= _T('0') &&
				_36hex <= _T('9'))
				return _36hex - _T('0');
			else if (_36hex >= _T('A') &&
				_36hex <= _T('Z'))
				return _36hex - _T('A') + 10;
			else if (_36hex >= _T('a') &&
				_36hex <= _T('z'))
				return _36hex - _T('a') + 10;

			return 0;
		}

		type_index toIndex(const NETString& _36hex_token) const 
		{
			type_index to_return = 0;

			if (_36hex_token.getLength() >= 2)
				to_return = toDecimal(_36hex_token[0]) * 36;

			return to_return += toDecimal(_36hex_token[1]);
		}

	private:
		void _parseLineForNotes(const NETString& line, type_int base_position, const NEFloatSet& bpms, const NEIntSet& stops)
		{
			if (line == "00") return;
			if (arg_type.getValue().toInt() == 2)
			{
				arg_value.setValue(line.toDouble());
				arg_pos.setValue(base_position);
				if(arg_on_found_note.isEnable())
					arg_on_found_note.getValue().execute();
			}
			else
			{
				type_index length = (line.getLength() - 1) / 2;
				type_float pos_per_note = 192 / length;

				for (int n = 0; n < length; n++)
				{
					arg_pos.setValue(base_position + n * pos_per_note);
					NEString vv = line.extract(n * 2, n * 2 + 1);
					type_int value = toIndex(vv);
					if (!value) continue;
					cout << vv.toCharPointer();
					arg_value.setValue(value);

					type_int t = arg_type.getValue().toInt();
					switch (t)
					{
					case 8:
						arg_type.setValue("03");
						arg_value.setValue(bpms[arg_value.getValue()]);
						break;

					case 9:
						arg_value.setValue(stops[arg_value.getValue()]);
					}

					if(arg_on_found_note.isEnable())
						arg_on_found_note.getValue().execute();

				}
			}
		}
	};
}
#pragma warning(pop)