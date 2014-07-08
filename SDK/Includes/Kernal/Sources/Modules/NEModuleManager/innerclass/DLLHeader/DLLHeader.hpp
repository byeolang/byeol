//	---------------------------------------------------------------------------------
//	Ŭ������:	NEModuleManager::DLLHeader
//	����	:	���Ŵ����� ���ؼ� fetch�Ǵ� DLL�� ���.
//				���߿� DLL�� ������ �ʿ��� ����.
//	����	:	���Ŭ����.	NEObject
//	Ư��	:	
//	�˰���:	
//	�����:	
//	�޸�	:	
//	�����丮:	2011-07-07	������	���� �Ϸ�	
//	---------------------------------------------------------------------------------
typedef NEModuleList (NE_FUNCTION_CALL *EntryPoint)(void);

class NE_DLL DLLHeader : public NEObject
{
	//	������:
public:
	DLLHeader();	
	DLLHeader(const DLLHeader& source);	

	//	�Ҹ���:
public:
	~DLLHeader();

	//	������ ��ø:
public:	
	const DLLHeader& operator=(const DLLHeader& source);
	bool operator==(const DLLHeader& source) const;
	bool operator!=(const DLLHeader& source) const;
	
public:
	NETString& getPath();
	const NETString& getPath() const;
	HINSTANCE& getInstance();
	const HINSTANCE& getInstance() const;
	EntryPoint& getEntryPoint();
	const EntryPoint& getEntryPoint() const;
	
	//	�������̽�:
	//		���:
	//			NEObject:
public:
	virtual void release();
	virtual NEBinaryFileSaver& serialize(NEBinaryFileSaver& saver) const;
	virtual NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader);
	virtual type_result isValid() const;
	virtual NEType::Type getType() const;

private:
	virtual NEObject& clone() const;

	//		���� �Լ�:
public:
	bool isLoaded() const;

	//	�����Լ�:
private:	
	const DLLHeader& _assign(const DLLHeader& source);

	//	�������:
protected:
	NETString _path;
	HINSTANCE _instance;
	EntryPoint _entrypoint;
};