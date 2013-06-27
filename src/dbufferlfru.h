#ifndef BUFMANAGERBYLFRU_H
#define BUFMANAGERBYLFRU_H

#include "dbuffer.h"
#include <sys/time.h>

using namespace std;

typedef struct LFRUBlockInfoo{
	unsigned int fileId;
	unsigned int segId;
	double weight;
	int periodCounter;
//	unsigned long  lastAccessTime;
	struct timeval lastAccessTime;
}LFRUBlockInfoo;


class DBufferLFRU : public DBuffer{

public:
	DBufferLFRU(int blockSize,int blockNums,int period);
	virtual ~DBufferLFRU();
	virtual bool Read(int fileId,int segId);
	virtual void Write(int fileId,int segId,int &ofileId,int &osegId);
	virtual void Strategy(int fileId,int segId,int &ofileId,int &osegId);
	virtual bool FindBlock(int fileId,int segId);
	int AddBlock(int fileId,int segId);
	void BlockReset();
private:
	list<LFRUBlockInfoo> buf;
//	unsigned long  recallTime;
	struct timeval recallTime;
	struct timeval t0;
//	unsigned long t0;
	unsigned int _period;
//	int mBlockSize;
//	int mBlockNums;
};

#endif
