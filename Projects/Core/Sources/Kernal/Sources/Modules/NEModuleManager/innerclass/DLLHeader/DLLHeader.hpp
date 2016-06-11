
public:
	NETString& getPath();
	const NETString& getPath() const;
	HINSTANCE& getInstance();
	const HINSTANCE& getInstance() const;
	EntryPoint& getEntryPoint();
	const EntryPoint& getEntryPoint() const;
	
	//	�������̽�:
	//		���
	//			NEObject:
public:
	virtual const NEClassBase& getClass() const;
	virtual void release();
	virtual NEBinaryFileSaver& serialize(NEBinaryFileSaver& saver) const;
	virtual NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader);
	virtual type_result isValid() const;

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

public:
	static const NEClassBase& getClassStatically();
};