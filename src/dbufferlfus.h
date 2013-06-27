#ifndef BUFMANAGERBYLFUS_H
#define BUFMANAGERBYLFUS_H

#include "dbuffer.h"
#include<vector>
#include<map>
using namespace std;

class LFUSBlockInfo{
public:
	LFUSBlockInfo(){
		fileId = 0;
		segId = 0;
		count = 1;
	}
	LFUSBlockInfo( int _fileId, int _segId):fileId(_fileId),segId(_segId){
		count = 1;
	}
	int fileId;
	int segId;
	int count;
};



class DBufferLFUS: public DBuffer{
public:
	DBufferLFUS(int blockSize,int blockNums);
	virtual ~DBufferLFUS();
	virtual bool Read(int fileId,int segId);
	virtual void Write(int fileId,int segId,int &ofileId,int &osegId);
	virtual void Strategy(int fileId,int segId,int &ofileId,int &osegId);
	virtual bool FindBlock(int fileId,int segId);
	int AddBlock(int fileId,int segId);
public:
	virtual int getInLine( int clientId);
	virtual int getOutLine( int clientId);
	int getWeight(int fileId);
	virtual int addNewClient(int clientId, int fileId);
private:
	//	 list<LFUSBlockInfo> dwBuf;
	list<LFUSBlockInfo> lruQueue;
	pthread_mutex_t dwbuf_mutex;
	//		 TimeCallBack myAlarmEvent;
//	int m_blockSize;
//	int m_blockNum;
	map<unsigned int ,bool> isOnline;//the status of client ,online or outline
	//map<unsigned int,unsigned int> visitedFile;//the file that each client visit
	map<unsigned int,vector<unsigned int> > FileVisitors;//the files visitors;

};

#endif
