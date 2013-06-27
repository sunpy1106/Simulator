#ifndef BUFMANAGERBYDW_H
#define BUFMANAGERBYDW_H

#include "dbuffer.h"
#include<map>
using namespace std;

class DWBlockInfo{
public:
	DWBlockInfo(){
		fileId = 0;
		segId = 0;
		weight = 0;
		m_histNew = 1;
		m_histOld = 0;
	}
	DWBlockInfo( int _fileId, int _segId):fileId(_fileId),segId(_segId){
		weight = 0;
		m_histNew = 1;
		m_histOld = 0;
	}
	int fileId;
	int segId;
	double  weight;
	struct timeval vtime;
	int m_histNew;
	int m_histOld;
};



class DBufferDW: public DBuffer{
public:
	DBufferDW(int blockSize,int blockNums,int period);
	virtual ~DBufferDW();
	virtual bool Read(int fileId,int segId);
	virtual void Write(int fileId,int segId,int &ofileId,int &osegId);
	virtual void Strategy(int fileId,int segId,int &ofileId,int &osegId);
	virtual bool FindBlock(int fileId,int segId);
	int AddBlock(int fileId,int segId);
	void BlockReset();
private:
	//	 list<DWBlockInfo> dwBuf;
	list<DWBlockInfo> lruQueue;
	pthread_mutex_t dwbuf_mutex;
	struct timeval _t0;
	int _period;
	int timeslot;
	//		 TimeCallBack myAlarmEvent;
//	int m_blockSize;
//	int m_blockNum;
};

#endif
