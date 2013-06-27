/*
 * myserver.h
 *
 *  Created on: 2013-2-20
 *      Author: zhaojun
 */

#ifndef MYSERVER_H_
#define MYSERVER_H_

#include "dataserver.h"
#include "mytimer.h"
#include "faketran.h"
#include "mymessage.h"
//add by sunpy@0518
#include "dbuffer.h"
#include "dbufferlru.h"
#include "dbufferpr.h"
#include "dbufferdw.h"
#include "dbufferdws.h"
#include "dbufferlfru.h"
#include "dbufferlfu.h"
#include "dbufferlfus.h"
#include "dbufferlrus.h"
#include "dbufferlrfu.h"
#include "dbufferfifo.h"
//add end @0518
#include <sys/socket.h>
#include <sys/epoll.h>
#include <list>
#include <fstream>

class MyServer{
public:
	MyServer(double bandWidth,int blockSize,int blockNums,string bufStrategy,int period,double lrfuLambda,int perSendSize,bool isP2POpen,
			int fileNum,int maxLength,int minLength,double minbitRate,double maxBitRate,int serverPort,int clientPort,int devNums,
			int clientNums,char **clusterAddress,int takeSampleFre,bool isUseRealDevice);
	~MyServer();
	void Init();
	void Run();
	void DealWithMessage(char *buf,int length);
	void GetNextBlock();
	void TakeSample();
	pthread_t GetTid(){return mtid;}
	list<ClientReqBlock>::iterator SearchTheReqList(int fileId,int segId);
	void ReadFromDevice();
	//add by sunpy @0518
public: 
	void writeRequestSequence(int clientId,int fileId,int segId);
	void initServerBuf();
	void BufferReset();
private:
	fstream sequenceOfs;
	unsigned int mTotalRequest;
	unsigned int mReadFromServer;
	unsigned int mReadFromBuf;
	unsigned int mReadFromDisk;
	int mBlockNum;
	int mPeriod;
	double mLrfuLambda;
	string mBufStrategy;
	DBuffer *mDbuffer;
	int mBufferResetFd[2];
	map<unsigned int,bool> connectStatus;
	//add end
//	int GetListenSockFd(){return mListenSockFd[1];}

//	int JudgeCluster(char *address);
private:
//	int mListenSockFd[2];//AF_UNIX协议，1号对外公布，0号用于epoll循环
	int mReadDevice[2];
	int mREpollFd;

	int mTakeSample[2];
	int mTakeSampleFre;

	int mNetSockFd;
	int mServerPort;
	int mClientPort;

	char *mClusterAddress[MAX_DEV_NUM];
	int mDevNums;
	int mClientNums;

//	int mCurTranFd;
	int mEpollFd;
	DataServer *mDataServer;
	bool mIsP2POpen;
	double mBand;
	FakeTran mFakeTran;
	ClientInfoBlock mClientInfo[MAX_CLIENT_NUM + 1];
	list<ClientReqBlock> mReqList;
	list<ClientReqBlock> mPreReqList;
	double mBlockSize;
	int mPerSendSize;
	list<ClientReqBlock>::iterator mCurReqBlock;

	int mFileNum;
	int mMinLength;
	int mMaxLength;
	double mMinBitRate;
	double mMaxBitRate;

	pthread_t mtid;
	pthread_t mRtid;

	int mLinkedNums;
	int mMaxBandSize;
	int mUsedBandSize;
	int mNeedSendBandSize;

	bool mNeverShow;
	ofstream mOFs;

	ofstream mClientDelayOfs[MAX_CLIENT_NUM + 1];

	bool mIsUseRealDevice;

//	FileBlock mFileBlock[MAX_FILE_NUM + 1];

//	int mClientLinks[MAX_CLIENT_NUM + 1];
};


#endif /* MYSERVER_H_ */
