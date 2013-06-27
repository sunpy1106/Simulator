#include "dbufferpr.h"
#include <float.h>
#include <math.h>

const double PARA = 0.5;// for p = 2

DBufferPR::DBufferPR(int blockSize,int blockNums)
:DBuffer(blockSize,blockNums)
{
//	mBlockSize = blockSize;
//	mBlockNums = blockNums;
	timeslot = 0;
	prBuf.clear();
}

DBufferPR::~DBufferPR(){
	prBuf.clear();
}

int DBufferPR::initial_parameter(){
	list<prBlockInfo>::iterator it;
	for(it = prBuf.begin();it!=prBuf.end();it++){
		it->weight = 0.0;
	}
	return 0;
}

void DBufferPR::Write(int fileId,int segId,int &ofileId,int &osegId){
	ofileId = -1;
	osegId = -1;
	if(prBuf.size() >= mBlockNums){
		Strategy(fileId,segId,ofileId,osegId);
	}
	else{
		AddBlock(fileId,segId);
	}
}

bool DBufferPR::Read(int fileId,int segId){
	prBlockInfo temp;
	temp.fileId = fileId;
	temp.segId = segId;
	list<prBlockInfo>::iterator it;
//	int accessTime;
	int flag = 0;
	for(it = prBuf.begin();it!=prBuf.end();it++){
		if( it->fileId ==temp.fileId && it->segId == temp.segId){
			it->lastTime = timeslot;
			timeslot++;
			flag = 1;
			break;
		}
	}
	if(flag == 0){
//		cout<<"in DBufferPR: can't find the segment <"<<fileId<<","<<segId<<">"<<endl;
		return false;
	}
	return true;
}

bool DBufferPR::FindBlock(int fileId,int segId){
	list<prBlockInfo>::iterator it;
	for(it = prBuf.begin();it !=prBuf.end();it++){
		if(it->fileId == fileId && it->segId == segId){
			return true;
		}
	}
	return false;
}

void DBufferPR::Strategy(int fileId,int segId,int &ofileId,int &osegId){
	list<prBlockInfo>::iterator it, minIt;
	float minWeight = 111111111.0;
	double temp;
//	int baseTime;
//	gettimeofday(&tv,NULL);
//	baseTime = tv.tv_sec * 1000000 + tv.tv_usec;
	//cout<<"will write "<<fileId<<","<<segId<<endl;
	for(it = prBuf.begin();it !=prBuf.end();it++){
//		temp=(it->weight)*pow(PARA,((timeslot - (it->lastTime))*_lambda)) ;
		if(it->segId <segId)	
			temp = (double)(it->segId - segId);
		else
			temp = (timeslot - (it->lastTime))/(double)((int)it->segId - (int)segId) ;
		if(temp < minWeight){
			minWeight = temp;
			minIt=it;
		}
	//	cout <<"for " <<it->fileId<<","<< it->segId << ",weigth:" << temp << ",timeslot:" << timeslot << ",lasttime:" << it->lastTime << ",minWeight:" << minWeight << endl;

//		if(it->weight < minWeight){
//			minWeight = it->weight;
//			minIt=it;
//		}
	}
	ofileId = minIt->fileId;
	osegId = minIt->segId;
//	cout<<"eliminate "<<ofileId<<","<<osegId<<endl;
	//	sendEliminateBlock(minIt->fileId,minIt->segId);
	prBuf.erase(minIt);

	AddBlock(fileId,segId);
}

int DBufferPR::AddBlock(int fileId,int segId){
	prBlockInfo temp;
	temp.fileId = fileId;
	temp.segId = segId;
	temp.lastTime =timeslot;
	temp.weight = 1.0;
	timeslot++;
	prBuf.push_back(temp);
	return 0;
}
