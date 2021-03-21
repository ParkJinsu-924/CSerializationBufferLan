#pragma once
#include <iostream>
#include <Windows.h>
#include "CMemoryPoolTLS\CMemoryPoolTLS.h"


class CSerializationBuffer
{
	//�޸�Ǯ�� �����ڸ� ȣ���ؾ��ϱ� ���� friendó��
	friend class CMemoryPoolTLS<CSerializationBuffer>;
	friend class CLanServer;
	friend class CLanClient;
	enum enPACKET
	{
		eBUFFER_DEFAULT = 1400
	};

public:
	//��Ŷ�ı�
	void Release();

	//��Ŷû��
	void Clear();

	//���ۻ�������
	int GetBufferSize();

	//���������;��
	char* GetContentBufPtr();

	//��������
	int AddRef();

	//��������
	void DeqRef();

	//������ ������ �ֱ�
	void PutContentData(char* chpSrc, int iSize);

	//������ ������ ��������
	void GetContentData(char* chpSrc, int iSize);

	//������ ������ ũ��
	int GetContentUseSize();

	//������ ���, �� �� ũ��
	int GetTotalUseSize();

	//FreeSizeũ��
	int GetFreeSize();

	//����ȭ ���� �Ҵ�
	static CSerializationBuffer* Alloc();

	//����ȭ ���� ����
	bool Free();

private:
	CSerializationBuffer();

	virtual ~CSerializationBuffer();

private:
	int mRefCount;
	int mBufferSize;
	int mHeaderSize;
	char* mpReadPos;
	char* mpWritePos;
	char* mpBufferPtr;
	char* mpContentBufferPtr;
	bool mEncodeFlag;

	static CMemoryPoolTLS<CSerializationBuffer> memoryPool;

private:
	//����pos�̵�, �����̵��� �ȵ�
	int MoveWritePos(int iSize);
	int MoveReadPos(int iSize);

	void PutNetworkHeader(char* chpSrc, int iSize);
	void GetNetworkHeader(char* chpSrc, int iSize);

	char* GetBufferPtr();
};
