/*
 * myclient.cpp
 *
 *  Created on: 2013-2-21
 *      Author: zhaojun
 */

#include "myclient.h"

#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <memory.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netinet/tcp.h>
#include <sstream>
#include <iostream>

#include "log.h"

void *ThreadToClient(void *arg){
	MyClient *client = (MyClient *)arg;
	client->Run();
	return NULL;
}

MyClient::MyClient(ModelAssemble *model,int blockSize,int perSendSize,
		double bandWidth,int blockNums,int clientNum,int serverPort,int clientPort,int devNums,
		int clientNums,char **clusterAddress,char *serverAddress,char *bufferStrategy,
		int period,double lrfuLambda,bool isRepeat){
	mModel = model;
	mClientNum = clientNum;
	mBlockSize = blockSize / 1000;

	mJumpSeg = 0;
	mPlayerStatus = PLAY;
	mMaxSegId = 2000;
	Run();
}

MyClient::~MyClient(){
}

void MyClient::Init(){
}
//int MyClient::JudgeCluster(char *address){
//	for(int i = 0;i < mDevNums;i++){
//		if(strcmp(address,mClusterAddress[i]) == 0)
//			return i;
//	}
//	return -1;
//}

void MyClient::Run(){
	fstream iofs;
	stringstream sstream;
	sstream.str("");
	sstream << "data/requestFile" << mClientNum << ".log";
	string requestFilename = sstream.str();
	iofs.open(requestFilename.c_str(),ios::out);

	mFileId = mModel->GetStartFileId();
	mSegId = mModel->GetStartSegId();
	iofs << mFileId << endl;
	iofs << mSegId << endl;
	long long leftTime = mModel->GetStartTime(mClientNum) * 1000000;
	iofs << leftTime << endl;

	int times = 0;
	bool firstTime = true;

	while(times < 4000){
		if(firstTime){
			unsigned int length = mModel->GslRandLogNormal(mPlayerStatus);
			mJumpSeg = length / 4;
			firstTime = false;
			continue;
		}

		if(mJumpSeg == 0){
			mPlayerStatus = mModel->GetNextStatus(mPlayerStatus);

			unsigned int length = mModel->GslRandLogNormal(mPlayerStatus);
			mJumpSeg = length / 4;

			if(mJumpSeg == 0)
				mJumpSeg++;
			if(mPlayerStatus == FORWARD){
				mSegId += mJumpSeg;
				if(mSegId > mMaxSegId){
					mSegId = 1;
				}
				mJumpSeg = 0;
				mModel->AdjustHotPlace(mFileId,mSegId);
			}
			else if(mPlayerStatus == BACKWARD){
				mSegId -= mJumpSeg;
				if(mSegId < 1)
					mSegId = 1;
				mJumpSeg = 0;
//				cout << "BACKWARD" << endl;
				mModel->AdjustHotPlace(mFileId,mSegId);
			}
		}


		if(mJumpSeg != 0){
			mJumpSeg--;
			iofs << mSegId << endl;
			mSegId++;
		}
		times++;
	}

	iofs.close();
}

bool MyClient::SearchTheReqList(int fileId,int segId){
}

void MyClient::DealWithMessage(char *buf,int length){
}

void MyClient::GetNextBlock(){
}
