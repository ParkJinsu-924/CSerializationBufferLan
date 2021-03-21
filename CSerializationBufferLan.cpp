#include "CSerializationBufferLan.h"

CMemoryPoolTLS<CSerializationBuffer> CSerializationBuffer::memoryPool;

CSerializationBuffer::CSerializationBuffer()
{
	mpBuffer = new char[eBUFFER_DEFAULT];
	mpContentBuffer = mpWritePos = mpReadPos = mpBuffer + 2;
	mBufferSize = eBUFFER_DEFAULT;
	mRefCount = 0;
}

CSerializationBuffer::~CSerializationBuffer()
{
	delete mpBuffer;
}

void CSerializationBuffer::Release()
{
	this->~CSerializationBuffer();
}

void CSerializationBuffer::Clear()
{
	mpWritePos = mpReadPos = mpBuffer + 2;
	mEncodeFlag = false;
}

int CSerializationBuffer::GetBufferSize()
{
	return mBufferSize;
}

char* CSerializationBuffer::GetBufferPtr()
{
	return mpBuffer;
}

char* CSerializationBuffer::GetContentBufPtr()
{
	return mpContentBuffer;
}

int CSerializationBuffer::MoveWritePos(int iSize)
{
	mpWritePos += iSize;
	return iSize;
}

int CSerializationBuffer::MoveReadPos(int iSize)
{
	mpReadPos += iSize;
	return iSize;
}

int CSerializationBuffer::AddRef()
{
	InterlockedIncrement((long*)&mRefCount);
	return mRefCount;
}

void CSerializationBuffer::DeqRef()
{
	if (InterlockedDecrement((long*)&mRefCount) == 0)
	{
		Free();
	}
}

void CSerializationBuffer::PutContentData(char* chpSrc, int iSize)
{
	memcpy(mpWritePos, chpSrc, iSize);
	mpWritePos += iSize;
}

void CSerializationBuffer::GetContentData(char* chpSrc, int iSize)
{
	memcpy(chpSrc, mpReadPos, iSize);
	mpReadPos += iSize;
}

void CSerializationBuffer::PutNetworkHeader(char* chpSrc, int iSize)
{
	memcpy(mpBuffer, chpSrc, iSize);
}

void CSerializationBuffer::GetNetworkHeader(char* chpSrc, int iSize)
{
	memcpy(chpSrc, mpBuffer, iSize);
}

int CSerializationBuffer::GetContentUseSize()
{
	int retVal = mpWritePos - mpReadPos;
	return retVal;
}

int CSerializationBuffer::GetTotalUseSize()
{
	return mpWritePos - mpBuffer;
}

int CSerializationBuffer::GetFreeSize()
{
	return mpBuffer + mBufferSize - mpWritePos;
}

CSerializationBuffer* CSerializationBuffer::Alloc()
{
	CSerializationBuffer* serialBufPtr = memoryPool.Alloc();
	
	serialBufPtr->Clear();

	serialBufPtr->AddRef();

	return serialBufPtr;
}

bool CSerializationBuffer::Free()
{
	return memoryPool.Free(this);
}
