#include "dbufferlfru.h"

long long int getTimeIntervallfru(struct timeval *a,struct timeval *b){
	long long int usec;
	usec = a->tv_sec - b->tv_sec;
	usec = 1000000*usec;
	usec += a->tv_usec - b->tv_usec;
	return usec;
}
DBufferLFRU::DBufferLFRU(int blockSize,int blockNums,int period)
: DBuffer(blockSize,blockNums)
{
	gettimeofday(&t0,NULL);
//	t0 = tv.tv_sec * 1000000 + tv.tv_usec;
	if(period ==0)
		period = 500;
	_period = period;

//	mBlockSize = blockSize;
//	mBlockNums = blockNums;
	buf.clear();
	m_isblockReset = true;
}

DBufferLFRU::~DBufferLFRU(){
	buf.clear();
}

void DBufferLFRU::BlockReset(){
        list<LFRUBlockInfoo>::iterator it;
	gettimeofday(&t0,NULL);
        for(it = buf.begin();it!=buf.end();it++){
                it->weight = 0.0;
                it->periodCounter = 1;
//		gettimeofday(&(it->lastAccessTime),NULL);
//                it->lastAccessTime = t0;
		it->lastAccessTime.tv_sec = t0.tv_sec;
		it->lastAccessTime.tv_usec = t0.tv_usec;
	
        }
}

void DBufferLFRU::Write(int fileId,int segId,int &ofileId,int &osegId){
	ofileId = -1;
	osegId = -1;
	if(buf.size() >= mBlockNums){
		Strategy(fileId,segId,ofileId,osegId);
	}
	else{
		AddBlock(fileId,segId);
	}
}

bool DBufferLFRU::Read(int fileId,int segId){
	LFRUBlockInfoo temp;
	temp.fileId = fileId;
	temp.segId = segId;
	list<LFRUBlockInfoo>::iterator it;
	int flag = 0;
	struct timeval tv;

	for(it = buf.begin();it!=buf.end();it++){
		if( it->fileId ==temp.fileId && it->segId == temp.segId){
			it->periodCounter++;
			gettimeofday(&(it->lastAccessTime),NULL);
		//	it->lastAccessTime = tv.tv_sec * 1000000 + tv.tv_usec;
			flag = 1;
			break;
		}
	}

	if(flag == 0){
		cout<<"in bufManagerByLRU: can't find the segment <"<<fileId<<","<<segId<<">"<<endl;
		return false;
	}
	//readBlock(fileId,segId);
	return true;
}

bool DBufferLFRU::FindBlock(int fileId,int segId){
	list<LFRUBlockInfoo>::iterator it;
	for(it = buf.begin();it !=buf.end();it++){
		if(it->fileId == fileId && it->segId == segId){
			return true;
		}
	}
	return false;
}

void DBufferLFRU::Strategy(int fileId,int segId,int &ofileId,int &osegId){
	double Fk;
	unsigned long  Rk;
	list<LFRUBlockInfoo>::iterator it, maxIt;
	double maxWeight =0.0;
	gettimeofday(&recallTime,NULL);

//	recallTime = tv.tv_sec * 1000000 + tv.tv_usec;
	for(it = buf.begin();it !=buf.end();it++){
		if(it->segId == segId && it->fileId == fileId ){
			it->weight = 0.0;
			it->periodCounter = 1;
			gettimeofday(&(it->lastAccessTime),NULL);
			return;
		}
		if(it->periodCounter ==0){
			cout<<"counter error"<<endl;
			exit(1);
		}
		Fk = getTimeIntervallfru(&recallTime,&t0)/(double)(it->periodCounter * 1000000.0);
		Rk = getTimeIntervallfru(&recallTime ,&(it->lastAccessTime))/1000000.0;
		it->weight =((long  long)_period- getTimeIntervallfru(&recallTime,&t0)/ 1000000.0) ;
		it->weight = it->weight * Rk /(double)_period ;
		it->weight +=  (getTimeIntervallfru(&recallTime ,&t0)*Fk/ (1000000.0 * (double)_period ));
		//cout << fileId << "," << segId << "Fk:" << Fk << ",Rk:" << Rk << ",period:" << _period << ",recallTime:" << recallTime.tv_sec<<":"<<recallTime.tv_usec << ",weight:" << it->weight << ",t0:" << t0.tv_sec<<":"<<t0.tv_usec << ",periodCounter:" << it->periodCounter << ",accessTime:" << it->lastAccessTime.tv_sec<<":"<<it->lastAccessTime.tv_usec << endl;
		if(it->weight > maxWeight){
			maxWeight = it->weight;
			maxIt=it;
		}
	}
	ofileId = maxIt->fileId;
	osegId = maxIt->segId;
	buf.erase(maxIt);
	AddBlock(fileId,segId);
}

int DBufferLFRU::AddBlock(int fileId,int segId){
	LFRUBlockInfoo temp;
	temp.fileId = fileId;
	temp.segId = segId;
	temp.weight = 0.0;
	temp.periodCounter = 1;
	gettimeofday(&(temp.lastAccessTime),NULL);
//	temp.lastAccessTime = t0;
	buf.push_back(temp);
	return 0;
}
