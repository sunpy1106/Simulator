/*
 * behaviormodel.h
 *
 *  Created on: 2013-2-21
 *      Author: zhaojun
 */

#ifndef BEHAVIORMODEL_H_
#define BEHAVIORMODEL_H_

#include "mymessage.h"
#include "globalfunction.h"

#include <vector>
static int adjustTimes = 0;
using namespace std;

struct MMBlock{
	int status;
	int prob;
};

class ModelAssemble{
public:
	ModelAssemble(int fileNum,double thelta,double lambda,
			double backZeta,double backSigma,double forZeta,double forSigma,int maxClientNum,
			int playToPlay,int playToPause,int playToForward,int playToBackward,
			int playToStop,bool isStartTogether,int hotPlaceNums);
	~ModelAssemble();
	int GetNextStatus(int curStatus);
	int GetStartFileId();
	int GetStartSegId(){return 1;}
	void GslInit();

	void CreateZipfDistribution();
	void CreateStartTime();

	double GetStartTime(int clientNum){
		if(mIsStartTogether)
			return 0;
		return mStartTime[clientNum - 1];
	}
	double GslRandLogNormal(int curStatus);

	void AdjustHotPlace(int fileId,int &segId);
private:
	int mFileNum;
	vector<double> mProbability;
	vector<double> mStartTime;

	vector<int> mHotPlaceVect[MAX_FILE_NUM + 1];

	double mThelta;
	double mLambda;

	double mBackZeta;
	double mBackSigma;

	double mForZeta;
	double mForSigma;

	int mMaxClientNum;

	int mHotPlaceNums;

	vector<MMBlock> mMMVect;

	bool mIsStartTogether;
};


#endif /* BEHAVIORMODEL_H_ */
