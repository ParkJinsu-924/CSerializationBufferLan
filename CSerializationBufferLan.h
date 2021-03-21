#pragma once
#include <iostream>
#include <Windows.h>
#include "CMemoryPoolTLS\CMemoryPoolTLS.h"


class CSerializationBuffer
{
	//메모리풀은 생성자를 호출해야하기 위해 friend처리
	friend class CMemoryPoolTLS<CSerializationBuffer>;
	friend class CLanServer;
	friend class CLanClient;
	enum enPACKET
	{
		eBUFFER_DEFAULT = 1400
	};

public:
	//패킷파괴
	void Release();

	//패킷청소
	void Clear();

	//버퍼사이즈얻기
	int GetBufferSize();

	//버퍼포인터얻기
	char* GetContentBufPtr();

	//참조증가
	int AddRef();

	//참조감소
	void DeqRef();

	//컨텐츠 데이터 넣기
	void PutContentData(char* chpSrc, int iSize);

	//컨텐츠 데이터 가져오기
	void GetContentData(char* chpSrc, int iSize);

	//컨텐츠 데이터 크기
	int GetContentUseSize();

	//내부적 사용, 총 합 크기
	int GetTotalUseSize();

	//FreeSize크기
	int GetFreeSize();

	//직렬화 버퍼 할당
	static CSerializationBuffer* Alloc();

	//직렬화 버퍼 해제
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
	//버퍼pos이동, 음수이동은 안됨
	int MoveWritePos(int iSize);
	int MoveReadPos(int iSize);

	void PutNetworkHeader(char* chpSrc, int iSize);
	void GetNetworkHeader(char* chpSrc, int iSize);

	char* GetBufferPtr();
};
