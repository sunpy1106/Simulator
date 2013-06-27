#include "dbufferlfu.h"


DBufferLFU::DBufferLFU(int blockSize,int blockNums)
: DBuffer(blockSize,blockNums)
{
//	mBlockNums = blockNums;
//	mBlockSize = blockSize;
	buf.clear();
	initial_parameter();
}
DBufferLFU::~DBufferLFU(){
	buf.clear();
}

int DBufferLFU::initial_parameter(){
	list<LFUBlockInfo>::iterator it;
	for(it = buf.begin();it!=buf.end();it++){
		it->counts = 0;
	}
	return 0;
}

void DBufferLFU::Write(int fileId,int segId,int &ofileId,int &osegId){
	ofileId = -1;
	osegId = -1;
	if(buf.size() >= mBlockNums){
		Strategy(fileId,segId,ofileId,osegId);
	}
	else{
		AddBlock(fileId,segId);
	}
}

bool DBufferLFU::Read(int fileId,int segId){
	LFUBlockInfo temp;
	temp.fileId = fileId;
	temp.segId = segId;
	list<LFUBlockInfo>::iterator it;
	int flag = 0;

	for(it = buf.begin();it!=buf.end();it++){
		if( it->fileId ==temp.fileId && it->segId == temp.segId){
			it->counts++;
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

bool DBufferLFU::FindBlock(int fileId,int segId){
	list<LFUBlockInfo>::iterator it;
	for(it = buf.begin();it !=buf.end();it++){
		if(it->fileId == fileId && it->segId == segId){
			return true;
		}
	}
	return false;
}

void DBufferLFU::Strategy(int fileId,int segId,int &ofileId,int &osegId){
	list<LFUBlockInfo>::iterator it, minIt;
	int minWeight = 0x7fffffff;

	for(it = buf.begin();it !=buf.end();it++){
		//if(it->segId == segId){
		//	it->counts = 1;
		//}	
		if(it->counts < minWeight){
			minWeight = it->counts;
			minIt=it;
		}
	}

	ofileId = minIt->fileId;
	osegId = minIt->segId;

	buf.erase(minIt);
	AddBlock(fileId,segId);
}

int DBufferLFU::AddBlock(int fileId,int segId){
	LFUBlockInfo temp;
	temp.fileId = fileId;
	temp.segId = segId;
	temp.counts = 1;
	buf.push_back(temp);
	return 0;
}
