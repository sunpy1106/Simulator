#include "dbufferdw.h"
#include<math.h>
DBufferDW::DBufferDW(int blockSize,int blockNums,int period)
	: DBuffer(blockSize,blockNums)
{
	if(period ==0)
		period = 500;
	_period = period;
	m_isblockReset = true;
	gettimeofday(&_t0,NULL);
}


DBufferDW::~DBufferDW(){
}

void DBufferDW::BlockReset(){
        list<DWBlockInfo>::iterator it;
	gettimeofday(&_t0,NULL);
        for(it = lruQueue.begin();it!=lruQueue.end();it++){
                (*it).m_histOld = (*it).m_histNew;
                //(*it)->m_histOld = (*it)->m_histOld   + (*it)->m_histNew ;
                (*it).m_histNew = 0;
	//			cout<<"reset <"<<it->fileId<<","<<it->segId<<">,old hist = "<<(*it).m_histOld<<",new hist ="<<(*it).m_histNew<<",weight = "<<(*it).weight<<endl;

        }
}

void DBufferDW::Write(int fileId,int segId,int &ofileId,int &osegId){
	ofileId = -1;
	osegId = -1;
	if(lruQueue.size() >= mBlockNums){
		Strategy(fileId,segId,ofileId,osegId);
	}
	else{
		AddBlock(fileId,segId);
	}
}

bool DBufferDW::Read(int fileId,int segId){
	list<DWBlockInfo>::iterator it;
	DWBlockInfo readedBlock;
	for(it = lruQueue.begin();it!=lruQueue.end();it++){
		if( (*it).fileId ==fileId && (*it).segId == segId){
			(*it).m_histNew ++;
//			readedBlock = *it;
//			lruQueue.erase(it);
			break;
		}
	//	cout<<"for file "<<it->fileId<<","<<it->segId<<",curTime = "<<it->vtime.tv_sec<<":"<<it->vtime.tv_usec<<endl;
		
	}
	if(it == lruQueue.end()){
//		cout<<"in DBufferDW: can't find the segment <"<<fileId<<","<<segId<<">"<<endl;
		return false;
	}
//	else{
//		lruQueue.push_back(readedBlock);
//	}
	//readBlock(fileId,segId);
	return true;
}

bool DBufferDW::FindBlock(int fileId,int segId){
	list<DWBlockInfo>::iterator it;
	for(it = lruQueue.begin();it !=lruQueue.end();it++){
		if((*it).fileId == fileId && (*it).segId == segId){
			return true;
		}
	}
	return false;
}

void DBufferDW::Strategy(int fileId,int segId,int &ofileId,int &osegId){
//	unsigned int fileId,segId;
	double maxWeight =-111111111.0;
	DWBlockInfo eliminateBlockPtr;
	list<DWBlockInfo>::iterator it,minIt;
	double pfnew ,pfold;
	struct timeval cur_tv;
	gettimeofday(&cur_tv,NULL);
	pfnew = getTimeInterval(&cur_tv,&_t0)/(1000000.0 * _period);
	pfold = 1 - pow(pfnew,2);
	pfnew = 1 + (pfnew);
	minIt = lruQueue.begin();
	for(it = lruQueue.begin();it != lruQueue.end();it++){
		(*it).weight = (*it).m_histOld  + (*it).m_histNew   ;
	//	cout<<"for <"<<it->fileId<<","<<it->segId<<">,old hist = "<<(*it).m_histOld<<",new hist ="<<(*it).m_histNew<<",weight = "<<(*it).weight<<endl;
		if( (*it).weight > maxWeight){
			maxWeight = (*it).weight;
			minIt = it;
		}
	//	cout<<"for file "<<it->fileId<<","<<it->segId<<",curTime = "<<it->vtime.tv_sec<<":"<<it->vtime.tv_usec<<",minIt = "<<minIt->fileId<<","<<minIt->segId<<endl;
		
	}
	ofileId = minIt->fileId;
	osegId = minIt->segId;
//	cout<<"delete "<< ofileId <<" "<<osegId<<",weight = "<<minIt->m_histOld + minIt->m_histNew<<endl;
	lruQueue.erase(minIt);

	AddBlock(fileId,segId);
}

int DBufferDW::AddBlock(int fileId,int segId){
	DWBlockInfo temp(fileId,segId);
	lruQueue.push_back(temp);
	return 0;
}
