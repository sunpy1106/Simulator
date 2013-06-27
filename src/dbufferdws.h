#ifndef BUFMANAGERBYDWS_H
#define BUFMANAGERBYDWS_H

#include "dbuffer.h"
#include<vector>
#include<map>
using namespace std;

class DWSBlockInfo{
public:
	DWSBlockInfo(){
		fileId = 0;
		segId = 0;
		weight = 0;
		m_histNew = 1;
		m_histOld = 1;
	}
	DWSBlockInfo( int _fileId, int _segId):fileId(_fileId),segId(_segId){
		weight = 0;
		m_histNew = 1;
		m_histOld = 1;
	}
	int fileId;
	int segId;
	double  weight;
	int m_histNew;
	int m_histOld;
};



class DBufferDWS: public DBuffer{
public:
	DBufferDWS(int blockSize,int blockNums,int period);
	virtual ~DBufferDWS();
	virtual bool Read(int fileId,int segId);
	virtual void Write(int fileId,int segId,int &ofileId,int &osegId);
	virtual void Strategy(int fileId,int segId,int &ofileId,int &osegId);
	virtual bool FindBlock(int fileId,int segId);
	int AddBlock(int fileId,int segId);
	void BlockReset();
public:
	virtual int getInLine( int clientId);
	virtual int getOutLine( int clientId);
	int getWeight(int fileId);
	virtual int addNewClient(int clientId, int fileId);
private:
	//	 list<DWSBlockInfo> dwBuf;
	list<DWSBlockInfo> lruQueue;
	pthread_mutex_t dwbuf_mutex;
	struct timeval _t0;
	int _period;
	//		 TimeCallBack myAlarmEvent;
//	int m_blockSize;
//	int m_blockNum;
	map<unsigned int ,bool> isOnline;//the status of client ,online or outline
	//map<unsigned int,unsigned int> visitedFile;//the file that each client visit
	map<unsigned int,vector<unsigned int> > FileVisitors;//the files visitors;

};

#endif
