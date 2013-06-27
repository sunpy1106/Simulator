#ifndef BUFMANAGERBYLFU_H
#define BUFMANAGERBYLFU_H

#include "dbuffer.h"

typedef struct LFUBlockInfo{
	unsigned int fileId;
	unsigned int segId;
	int counts;
}LFUBlockInfo;

class DBufferLFU: public DBuffer{
public:
	DBufferLFU(int blockSize,int blockNums);
	virtual ~DBufferLFU();
	virtual bool Read(int fileId,int segId);
	virtual void Write(int fileId,int segId,int &ofileId,int &osegId);
	virtual void Strategy(int fileId,int segId,int &ofileId,int &osegId);
	virtual bool FindBlock(int fileId,int segId);
	int AddBlock(int fileId,int segId);
	virtual void BlockReset(){}
private:
	list<LFUBlockInfo> buf;

//	int mBlockSize;
//	int mBlockNums;

	int initial_parameter();
};

#endif
