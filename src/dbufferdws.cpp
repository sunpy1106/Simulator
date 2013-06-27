#include "dbufferdws.h"
#include<math.h>
DBufferDWS::DBufferDWS(int blockSize,int blockNums,int period)
	: DBuffer(blockSize,blockNums)
{
	if(period ==0)
		period = 500;
	_period = period;
	m_isblockReset = true;
	gettimeofday(&_t0,NULL);
}


DBufferDWS::~DBufferDWS(){
}

void DBufferDWS::BlockReset(){
        list<DWSBlockInfo>::iterator it;
//	gettimeofday(&_t0,NULL);
        for(it = lruQueue.begin();it!=lruQueue.end();it++){
                (*it).m_histOld = (*it).m_histNew;
                //(*it)->m_histOld = (*it)->m_histOld   + (*it)->m_histNew ;
                (*it).m_histNew = 1;
                //it->weight = 0;
        }
}

void DBufferDWS::Write(int fileId,int segId,int &ofileId,int &osegId){
	ofileId = -1;
	osegId = -1;
	if(lruQueue.size() >= mBlockNums){
		Strategy(fileId,segId,ofileId,osegId);
	}
	else{
		AddBlock(fileId,segId);
	}
}

bool DBufferDWS::Read(int fileId,int segId){
	list<DWSBlockInfo>::iterator it;
	DWSBlockInfo readedBlock;
	for(it = lruQueue.begin();it!=lruQueue.end();it++){
	//	cout<<"block <"<<(*it)->fileId<<","<<(*it)->segId<<">"<<endl;
		if( (*it).fileId ==fileId && (*it).segId == segId){
			(*it).m_histNew ++;
//			readedBlock = *it;
//			lruQueue.erase(it);
			break;
		}
	}
	if(it == lruQueue.end()){
//		cout<<"in DBufferDWS: can't find the segment <"<<fileId<<","<<segId<<">"<<endl;
		return false;
	}
//	else{
//		lruQueue.push_back(readedBlock);
//	}
	//readBlock(fileId,segId);
	return true;
}

bool DBufferDWS::FindBlock(int fileId,int segId){
	list<DWSBlockInfo>::iterator it;
	for(it = lruQueue.begin();it !=lruQueue.end();it++){
		if((*it).fileId == fileId && (*it).segId == segId){
			return true;
		}
	}
	return false;
}

void DBufferDWS::Strategy(int fileId,int segId,int &ofileId,int &osegId){
//	unsigned int fileId,segId;
	int minWeight =111111111;
	DWSBlockInfo eliminateBlockPtr;
	list<DWSBlockInfo>::iterator it,minHistIt,minHistNewIt,minIt;
	double pfnew ,pfold;
	//struct timeval cur_tv;
	int minHistNew;
	minHistNew = minWeight;
	//gettimeofday(&cur_tv,NULL);
	//pfnew = getTimeInterval(&cur_tv,&_t0)/(double)(_period*1000000.0);
//	pfold = 1- pfnew*pfnew;
//	pfnew = pow(pfnew,0.25);
	for(it = lruQueue.begin();it != lruQueue.end();it++){
		(*it).weight =  (*it).m_histOld  + (*it).m_histNew ;
		(*it).weight = (*it).weight * getWeight((*it).fileId);
	//	cout<<"for file <"<<it->fileId<<","<<it->segId<<">,"<<"old hist = "<<(*it).m_histOld<<",new hist ="<<(*it).m_histNew<<",weight = "<<it->weight<<",the weight of file "<<it->fileId<<" is "<<getWeight(it->fileId)<<endl;
		//	<<",weight = "<<(*it).weight<<endl;
		/*if(minHistNew > (*it).m_histNew){
			minHistNewIt = it;
			minHistNew = (*it).m_histNew;
		}*/
		if( (*it).weight < minWeight){
			minWeight = (*it).weight;
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

int DBufferDWS::AddBlock(int fileId,int segId){
	DWSBlockInfo temp(fileId,segId);
	lruQueue.push_back(temp);
	return 0;
}

int
DBufferDWS::getInLine( int clientId){
	isOnline[clientId] = true;
	//cout<<"client "<<clientId<<" is on line"<<endl;
	return 0;
}

int
DBufferDWS::getOutLine( int clientId){
	isOnline[clientId] = false;
	return 0;
}

int
DBufferDWS::addNewClient( int clientId, int fileId){
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
DBufferDWS::getWeight( int fileId){
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
