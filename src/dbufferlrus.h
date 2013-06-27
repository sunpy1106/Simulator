#ifndef BUFMANAGERBYLRUS_H
#define BUFMANAGERBYLRUS_H

#include "dbuffer.h"
#include<vector>
#include<map>
using namespace std;

class LRUSBlockInfo{
public:
	LRUSBlockInfo(){
		fileId = 0;
		segId = 0;
	}
	LRUSBlockInfo( int _fileId, int _segId):fileId(_fileId),segId(_segId){
	}
	int fileId;
	int segId;
};



class DBufferLRUS: public DBuffer{
public:
	DBufferLRUS(int blockSize,int blockNums);
	virtual ~DBufferLRUS();
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
	list<LRUSBlockInfo> lruQueue;
	pthread_mutex_t dwbuf_mutex;
	map<unsigned int ,bool> isOnline;//the status of client ,online or outline
	//map<unsigned int,unsigned int> visitedFile;//the file that each client visit
	map<unsigned int,vector<unsigned int> > FileVisitors;//the files visitors;

};

#endif
