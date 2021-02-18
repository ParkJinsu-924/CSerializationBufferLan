#include "CSerializationBufferLan.h"

int debugIndex = 0;

CMemoryPoolTLS<CSerializationBuffer> CSerializationBuffer::memoryPool;

CSerializationBuffer::CSerializationBuffer()
{
	chpBufferPtr = new char[eBUFFER_DEFAULT];
	chpContentBufferPtr = chpWritePos = chpReadPos = chpBufferPtr + 2;
	iBufferSize = eBUFFER_DEFAULT;
	refCount = 0;
}

CSerializationBuffer::~CSerializationBuffer()
{
	delete chpBufferPtr;
}

void CSerializationBuffer::Release()
{
	this->~CSerializationBuffer();
}

void CSerializationBuffer::Clear()
{
	chpWritePos = chpReadPos = chpBufferPtr + 2;
	bEncodeFlag = false;
}

int CSerializationBuffer::GetBufferSize()
{
	return iBufferSize;
}

char* CSerializationBuffer::GetBufferPtr()
{
	return chpBufferPtr;
}

char* CSerializationBuffer::GetContentBufPtr()
{
	return chpContentBufferPtr;
}

int CSerializationBuffer::MoveWritePos(int iSize)
{
	chpWritePos += iSize;
	return iSize;
}

int CSerializationBuffer::MoveReadPos(int iSize)
{
	chpReadPos += iSize;
	return iSize;
}

int CSerializationBuffer::AddRef()
{
	InterlockedIncrement((long*)&refCount);
	return refCount;
}

void CSerializationBuffer::DeqRef()
{
	if (InterlockedDecrement((long*)&refCount) == 0)
	{
		Free();
	}
}

CSerializationBuffer& CSerializationBuffer::operator=(CSerializationBuffer& clSrcPacket)
{
	// TODO: 여기에 return 문을 삽입합니다.
	return *this;
}

//int CSerializationBuffer::GetData(char* chpDest, int iSize)
//{
//	memcpy(chpDest, chpReadPos, iSize);
//	chpReadPos += iSize;
//	return *chpDest;
//}
//
//int CSerializationBuffer::PutData(char* chpSrc, int iSrcSize)
//{
//	memcpy(chpWritePos, chpSrc, iSrcSize);
//	chpWritePos += iSrcSize;
//	return *chpSrc;
//}

void CSerializationBuffer::PutContentData(char* chpSrc, int iSize)
{
	memcpy(chpWritePos, chpSrc, iSize);
	chpWritePos += iSize;
}

void CSerializationBuffer::GetContentData(char* chpSrc, int iSize)
{
	memcpy(chpSrc, chpReadPos, iSize);
	chpReadPos += iSize;
}

void CSerializationBuffer::PutNetworkHeader(char* chpSrc, int iSize)
{
	memcpy(chpBufferPtr, chpSrc, iSize);
}

void CSerializationBuffer::GetNetworkHeader(char* chpSrc, int iSize)
{
	memcpy(chpSrc, chpBufferPtr, iSize);
}

int CSerializationBuffer::GetContentUseSize()
{
	int retVal = chpWritePos - chpReadPos;
	return retVal;
}

int CSerializationBuffer::GetTotalUseSize()
{
	return chpWritePos - chpBufferPtr;
}

int CSerializationBuffer::GetFreeSize()
{
	return chpBufferPtr + iBufferSize - chpWritePos;
}

CSerializationBuffer* CSerializationBuffer::Alloc()
{
	CSerializationBuffer* serialBufPtr = memoryPool.Alloc();

	InterlockedIncrement((LONG*)&debugIndex);

	serialBufPtr->Clear();

	serialBufPtr->AddRef();

	return serialBufPtr;
}

bool CSerializationBuffer::Free()
{
	InterlockedDecrement((LONG*)&debugIndex);
	return memoryPool.Free(this);
}
