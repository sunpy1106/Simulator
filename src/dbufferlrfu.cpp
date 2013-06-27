#include "dbufferlrfu.h"
#include <float.h>
#include <math.h>

const double PARA = 0.5;// for p = 2

DBufferLRFU::DBufferLRFU(int blockSize,int blockNums,float lambda)
:DBuffer(blockSize,blockNums)
{
//	mBlockSize = blockSize;
//	mBlockNums = blockNums;
	_lambda = lambda;
	timeslot = 0;
	lrfuBuf.clear();
}

DBufferLRFU::~DBufferLRFU(){
	lrfuBuf.clear();
}

int DBufferLRFU::initial_parameter(){
	list<lrfuBlockInfo>::iterator it;
	for(it = lrfuBuf.begin();it!=lrfuBuf.end();it++){
		it->weight = 0.0;
	}
	return 0;
}

void DBufferLRFU::Write(int fileId,int segId,int &ofileId,int &osegId){
	ofileId = -1;
	osegId = -1;
	if(lrfuBuf.size() >= mBlockNums){
		Strategy(fileId,segId,ofileId,osegId);
	}
	else{
		AddBlock(fileId,segId);
	}
}

bool DBufferLRFU::Read(int fileId,int segId){
	lrfuBlockInfo temp;
	temp.fileId = fileId;
	temp.segId = segId;
	list<lrfuBlockInfo>::iterator it;
//	int accessTime;
	int flag = 0;
	for(it = lrfuBuf.begin();it!=lrfuBuf.end();it++){
		if( it->fileId ==temp.fileId && it->segId == temp.segId){
//			gettimeofday(&tv,NULL);
//			accessTime = tv.tv_sec * 1000000 + tv.tv_usec;
			it->weight = (it->weight)*pow(PARA,((timeslot - (it->lastTime))*_lambda)) + 1.0;
//			it->Weight = it->weight;
			it->lastTime = timeslot;
			timeslot++;
			flag = 1;
			break;
		}
	}
	if(flag == 0){
		cout<<"in DBufferLRFU: can't find the segment <"<<fileId<<","<<segId<<">"<<endl;
		return false;
	}
	return true;
}

bool DBufferLRFU::FindBlock(int fileId,int segId){
	list<lrfuBlockInfo>::iterator it;
	for(it = lrfuBuf.begin();it !=lrfuBuf.end();it++){
		if(it->fileId == fileId && it->segId == segId){
			return true;
		}
	}
	return false;
}

void DBufferLRFU::Strategy(int fileId,int segId,int &ofileId,int &osegId){
	list<lrfuBlockInfo>::iterator it, minIt;
	float minWeight = 111111111.0;
	double temp;
//	int baseTime;
//	gettimeofday(&tv,NULL);
//	baseTime = tv.tv_sec * 1000000 + tv.tv_usec;
	for(it = lrfuBuf.begin();it !=lrfuBuf.end();it++){
		temp=(it->weight)*pow(PARA,((timeslot - (it->lastTime))*_lambda)) ;
		if(temp < minWeight){
			minWeight = temp;
			minIt=it;
		}
		//cout << fileId << "," << segId << " weight:" << it->weight << ",timeslot" << timeslot << ",lasttime:" << it->lastTime << ",lambda:" << _lambda << endl;

//		if(it->weight < minWeight){
//			minWeight = it->weight;
//			minIt=it;
//		}
	}
	ofileId = minIt->fileId;
	osegId = minIt->segId;
	//	sendEliminateBlock(minIt->fileId,minIt->segId);
	lrfuBuf.erase(minIt);

	AddBlock(fileId,segId);
}

int DBufferLRFU::AddBlock(int fileId,int segId){
	lrfuBlockInfo temp;
	temp.fileId = fileId;
	temp.segId = segId;
	temp.lastTime =timeslot;
	temp.weight = 1.0;
	timeslot++;
	lrfuBuf.push_back(temp);
	return 0;
}
