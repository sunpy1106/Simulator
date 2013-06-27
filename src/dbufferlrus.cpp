#include "dbufferlrus.h"
#include<math.h>
DBufferLRUS::DBufferLRUS(int blockSize,int blockNums)
	: DBuffer(blockSize,blockNums)
{
	lruQueue.clear();
}


DBufferLRUS::~DBufferLRUS(){
}


void DBufferLRUS::Write(int fileId,int segId,int &ofileId,int &osegId){
	ofileId = -1;
	osegId = -1;
	if(lruQueue.size() >= mBlockNums){
		Strategy(fileId,segId,ofileId,osegId);
	}
	else{
		AddBlock(fileId,segId);
	}
}

bool DBufferLRUS::Read(int fileId,int segId){
	list<LRUSBlockInfo>::iterator it;
	LRUSBlockInfo readedBlock;
	for(it = lruQueue.begin();it!=lruQueue.end();it++){
	//	cout<<"block <"<<(*it)->fileId<<","<<(*it)->segId<<">"<<endl;
		if( (*it).fileId ==fileId && (*it).segId == segId){
			readedBlock = *it;
			lruQueue.erase(it);
			break;
		}
	}
	if(it == lruQueue.end()){
//		cout<<"in DBufferLRUS: can't find the segment <"<<fileId<<","<<segId<<">"<<endl;
		return false;
	}
	else{
	//	cout<<"read block "<<readedBlock.fileId<<","<<readedBlock.segId<<endl;
		lruQueue.push_back(readedBlock);
	}
	//readBlock(fileId,segId);
	return true;
}

bool DBufferLRUS::FindBlock(int fileId,int segId){
	list<LRUSBlockInfo>::iterator it;
	for(it = lruQueue.begin();it !=lruQueue.end();it++){
		if((*it).fileId == fileId && (*it).segId == segId){
			return true;
		}
	}
	return false;
}

void DBufferLRUS::Strategy(int fileId,int segId,int &ofileId,int &osegId){
//	unsigned int fileId,segId;
	int minWeight =111111111;
	int temp;
	LRUSBlockInfo eliminateBlockPtr;
	list<LRUSBlockInfo>::iterator it,minHistIt,minHistNewIt,minIt;
	int minHistNew;
	minHistNew = minWeight;
	cout<<endl<<endl;
	for(it = lruQueue.begin();it != lruQueue.end();it++){
		temp =  getWeight((*it).fileId);
	//	cout<<"for file <"<<it->fileId<<","<<it->segId<<">,its  file weight = "<<temp<<endl;
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
	cout<<"delete "<< (*minIt).fileId <<" "<<(*minIt).segId<<endl;
	ofileId = eliminateBlockPtr.fileId;
	osegId = eliminateBlockPtr.segId;
	//	delete eliminateBlockPtr;
	lruQueue.erase(minIt);

	AddBlock(fileId,segId);
}

int DBufferLRUS::AddBlock(int fileId,int segId){
	LRUSBlockInfo temp(fileId,segId);
	lruQueue.push_back(temp);
	return 0;
}

int
DBufferLRUS::getInLine( int clientId){
	isOnline[clientId] = true;
	//cout<<"client "<<clientId<<" is on line"<<endl;
	return 0;
}

int
DBufferLRUS::getOutLine( int clientId){
	isOnline[clientId] = false;
	return 0;
}

int
DBufferLRUS::addNewClient( int clientId, int fileId){
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
DBufferLRUS::getWeight( int fileId){
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
