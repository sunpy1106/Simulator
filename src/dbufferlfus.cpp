#include "dbufferlfus.h"
#include<sstream>
#include<math.h>
DBufferLFUS::DBufferLFUS(int blockSize,int blockNums)
	: DBuffer(blockSize,blockNums)
{
}

DBufferLFUS::~DBufferLFUS(){
}


void DBufferLFUS::Write(int fileId,int segId,int &ofileId,int &osegId){
	ofileId = -1;
	osegId = -1;
	if(lruQueue.size() >= mBlockNums){
		Strategy(fileId,segId,ofileId,osegId);
	}
	else{
		AddBlock(fileId,segId);
	}
}

bool DBufferLFUS::Read(int fileId,int segId){
	list<LFUSBlockInfo>::iterator it;
	LFUSBlockInfo readedBlock;
	for(it = lruQueue.begin();it!=lruQueue.end();it++){
	//	cout<<"block <"<<(*it)->fileId<<","<<(*it)->segId<<">"<<endl;
		if( (*it).fileId ==fileId && (*it).segId == segId){
	//		cout<<endl<<"read block "<<it->fileId<<","<<it->segId<<endl;
			(*it).count ++;
//			readedBlock = *it;
//			lruQueue.erase(it);
			break;
		}
	}
	if(it == lruQueue.end()){
//		cout<<"in DBufferLFUS: can't find the segment <"<<fileId<<","<<segId<<">"<<endl;
		return false;
	}
//	else{
//		lruQueue.push_back(readedBlock);
//	}
	//readBlock(fileId,segId);
	return true;
}

bool DBufferLFUS::FindBlock(int fileId,int segId){
	list<LFUSBlockInfo>::iterator it;
	for(it = lruQueue.begin();it !=lruQueue.end();it++){
		if((*it).fileId == fileId && (*it).segId == segId){
			return true;
		}
	}
	return false;
}

void DBufferLFUS::Strategy(int fileId,int segId,int &ofileId,int &osegId){
//	unsigned int fileId,segId;
	int minWeight =111111111;
	LFUSBlockInfo eliminateBlockPtr;
	list<LFUSBlockInfo>::iterator it,minHistIt,minHistNewIt,minIt;
	double pfnew ,pfold;
	struct timeval cur_tv;
	int minHistNew;
	minHistNew = minWeight;
//	cout<<endl<<endl<<"start eliminate:"<<endl;
	for(it = lruQueue.begin();it != lruQueue.end();it++){
//		cout<<"for file <"<<it->fileId<<","<<it->segId<<">,"<<"count ="<<(*it).count<<",the file weight is "<<getWeight(it->fileId)<<endl;
		int  temp = (*it).count * getWeight(it->fileId);
		if( temp < minWeight){
			minWeight = temp;
			minHistIt = it;
		}
		
	}
	/*
	if(minHistNew == 0){
		eliminateBlockPtr = *minHistNewIt;
		minIt = minHistNewIt;
	}else{
		eliminateBlockPtr = *minHistIt;
		minIt = minHistIt;
	}*/
	eliminateBlockPtr = *minHistIt;
	minIt = minHistIt;
//	cout<<"delete "<< (*minIt).fileId <<" "<<(*minIt).segId<<endl;
	ofileId = eliminateBlockPtr.fileId;
	osegId = eliminateBlockPtr.segId;
	//	delete eliminateBlockPtr;
	lruQueue.erase(minIt);

	AddBlock(fileId,segId);
}

int DBufferLFUS::AddBlock(int fileId,int segId){
	LFUSBlockInfo temp(fileId,segId);
	lruQueue.push_back(temp);
	return 0;
}

int
DBufferLFUS::getInLine( int clientId){
	isOnline[clientId] = true;
	//cout<<"client "<<clientId<<" is on line"<<endl;
	return 0;
}

int
DBufferLFUS::getOutLine( int clientId){
	isOnline[clientId] = false;
	return 0;
}

int
DBufferLFUS::addNewClient( int clientId, int fileId){
	//check if exist
	unsigned int curClient;
	for(size_t i = 0;i<FileVisitors[fileId].size();i++){
		curClient = FileVisitors[fileId][i];
		if(curClient == clientId){
	//		cout<<"is already in client list"<<endl;
			return -1;
		}
	}
	//add to the list
	FileVisitors[fileId].push_back(clientId);
	/*
	cout<<"the file clients:"<<endl;
	map<unsigned int,vector<unsigned int> >::iterator it;
	for(it = FileVisitors.begin();it!=FileVisitors.end();it++){
		cout<<"for file "<<it->first<<":";
		for(int i=0;i<FileVisitors[it->first].size();i++){
			cout<<FileVisitors[it->first][i]<<" ";
		}
		cout<<endl;
	}*/
	return 0;
}
int 
DBufferLFUS::getWeight( int fileId){
	unsigned int curClient;
	int weight = 0;
	for(size_t i = 0;i<FileVisitors[fileId].size();i++){
		curClient = FileVisitors[fileId][i];
		if(isOnline[curClient]==true){
			//cout<<"client "<<curClient<<" is on line"<<endl;
			weight++;
		}
	}
	return weight;
}
