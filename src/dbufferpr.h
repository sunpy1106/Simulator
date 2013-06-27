#ifndef BUFMANAGERBYPR_H
#define BUFMANAGERBYPR_H

#include "dbuffer.h"

#include <sys/time.h>

typedef struct prBlockInfo{
	int fileId;
	int segId;
	float weight;
	int lastTime;
}prBlockInfo;

class DBufferPR: public DBuffer{
public:
	DBufferPR(int blockSize,int blockNums);
	virtual ~DBufferPR();
	virtual bool Read(int fileId,int segId);
	virtual void Write(int fileId,int segId,int &ofileId,int &osegId);
	virtual void Strategy(int fileId,int segId,int &ofileId,int &osegId);
	virtual bool FindBlock(int fileId,int segId);
	int AddBlock(int fileId,int segId);
	virtual void BlockReset(){}
private:
//	struct timeval tv;
	list<prBlockInfo> prBuf;
	int initial_parameter();
//	int mBlockSize;
//	int mBlockNums;
	int timeslot;
};

#endif
