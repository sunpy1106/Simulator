#ifndef BUFMANAGERBYLRFU_H
#define BUFMANAGERBYLRFU_H

#include "dbuffer.h"

#include <sys/time.h>

typedef struct lrfuBlockInfo{
	unsigned int fileId;
	unsigned int segId;
	float weight;
	int lastTime;
	float lastWeight;
}lrfuBlockInfo;

class DBufferLRFU: public DBuffer{
public:
	DBufferLRFU(int blockSize,int blockNums,float lambda);
	virtual ~DBufferLRFU();
	virtual bool Read(int fileId,int segId);
	virtual void Write(int fileId,int segId,int &ofileId,int &osegId);
	virtual void Strategy(int fileId,int segId,int &ofileId,int &osegId);
	virtual bool FindBlock(int fileId,int segId);
	int AddBlock(int fileId,int segId);
	virtual void BlockReset(){}
private:
//	struct timeval tv;
	list<lrfuBlockInfo> lrfuBuf;
	int initial_parameter();
	float _lambda;
//	int mBlockSize;
//	int mBlockNums;
	unsigned int timeslot;
};

#endif
