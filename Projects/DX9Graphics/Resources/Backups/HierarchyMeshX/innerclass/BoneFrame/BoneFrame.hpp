struct BoneFrame : D3DXFRAME
{
	bool operator==(const BoneFrame& source) const
	{
		return true;
	}
	D3DXMATRIXA16 CombinedTransformMatrix;		//���� ���� ��ȯ ������ ��� �ִ´�.
	bool picked;

	NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader)
	{
		return loader;
	}
	NEBinaryFileSaver& serialize(NEBinaryFileSaver& saver) const
	{
		return saver;
	}	
};