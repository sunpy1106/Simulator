#ifndef __D_BUFFERLRU_H__
#define __D_BUFFERLRU_H__

#include "globalfunction.h"
#include "dbuffer.h"

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <list>
#include <time.h>
#include <stdlib.h>
#include <signal.h>

using namespace std;

struct Block{
	Block(int fileId,int segId){//,bool isLocked){
		this->fileId = fileId;
		this->segId = segId;
//		this->isLocked = isLocked;
	}
	int fileId;
	int segId;
//	bool isLocked;
//	int freq;
};

class DBufferLRU : public DBuffer{
public:
	DBufferLRU(int blockSize,int blockNum);
	virtual ~DBufferLRU();
	virtual bool Read(int fileId,int segId);
	virtual void Write(int fileId,int segId,int &ofileId,int &osegId);
	bool FindAndAdjustBlock(int fileId,int segId);
	virtual void Strategy(int fileId,int segId,int &ofileId,int &osegId);
	void PrintBuffer();
//	void MutexLock();
//	void MutexUnLock();
//	void NotFullSignal();
//	void NotEmptySignal();
	virtual bool FindBlock(int fileId,int segId);
protected:
//	pthread_cond_t m_notEmptyCond;
//	pthread_cond_t m_notFullCond;
//	pthread_mutex_t m_mutex;
	list<Block> m_blockList;
//	int m_blockSize;
//	int m_blockNum;
	int m_curBlockNum;
};
#endif
