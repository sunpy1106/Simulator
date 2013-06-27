/*
 * myclient.h
 *
 *  Created on: 2013-2-21
 *      Author: zhaojun
 */

#ifndef MYCLIENT_H_
#define MYCLIENT_H_

#include "mymessage.h"
#include "modelassemble.h"

#include <sys/socket.h>
#include <pthread.h>

#include <fstream>

class MyClient{
public:
	MyClient(ModelAssemble *model,int blockSize,int perSendSize,
			double bandWidth,int blockNums,int clientNum,int serverPort,int clientPort,int devNums,
			int clientNums,char **clusterAddress,char *serverAddress,char *bufferStrategy,
			int period,double lrfuLambda,bool isRepeat);
	~MyClient();

	void Init();
	void Run();
	void DealWithMessage(char *buffer,int length);
	void Play();
	void GetNextBlock();
	bool SearchTheReqList(int fileId,int segId);

	int getClientNum() const {
		return mClientNum;
	}

	pthread_t GetTid(){return mtid;}

//	int JudgeCluster(char *address);
private:
//	int mListenSockFd[2];
	bool mIsReadFin;

	int mNetSockFd;
	int mMyPort;
	int mClientPort;

	int mAskNum;

	char *mClusterAddress[MAX_DEV_NUM];
	int mDevNums;
	int mClientNums;

	char *mServerAddress;
	int mServerPort;

	int mPlaySockFd[2];

//	int mServerFd;
	int mEpollFd;

	ModelAssemble *mModel;

	int mBlockSize;
	int mPerSendSize;

	int mFileId;
	int mSegId;
	int mClientNum;
	int mBlockNum;
	bool mIsPlaying;
	double mBitRate;
	int mMaxSegId;

	int mPlayerStatus;
	int mJumpSeg;

	int mLinkedNums;
	double mBand;


	pthread_t mtid;

	string mFileName;
	ofstream mOFs;
	bool mDelay;

	char *mBufferStrategy;

	int mPeriod;
	double mLrfuLambda;
	bool mIsRepeat;

	int mHitTimes;
	int mTotalTimes;
};

#endif /* MYCLIENT_H_ */
