/*
 * behaviormodel.cpp
 *
 *  Created on: 2013-2-21
 *      Author: zhaojun
 */

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <math.h>

#include <algorithm>

#include "modelassemble.h"
#include "globalfunction.h"

static const gsl_rng_type *m_t;
static gsl_rng *m_r;



ModelAssemble::ModelAssemble(int fileNum,double thelta,double lambda,
		double backZeta,double backSigma,double forZeta,double forSigma,int maxClientNum,
		int playToPlay,int playToPause,int playToForward,int playToBackward,
		int playToStop,bool isStartTogether,int hotPlaceNums){
	mIsStartTogether = isStartTogether;

	mFileNum = fileNum;
	mThelta = thelta / 1000.0;
	mLambda = lambda / 1000.0;
	mBackZeta = backZeta / 1000.0;
	mBackSigma = backSigma / 1000.0;
	mForZeta = forZeta / 1000.0;
	mForSigma = forSigma / 1000.0;
	mHotPlaceNums = hotPlaceNums;
	mMaxClientNum = maxClientNum;

	MMBlock tmpBlock;
	tmpBlock.status = PLAY;
	tmpBlock.prob = playToPlay;
	mMMVect.push_back(tmpBlock);

	tmpBlock.status = PAUSE;
	tmpBlock.prob = playToPause;
	mMMVect.push_back(tmpBlock);

	tmpBlock.status = FORWARD;
	tmpBlock.prob = playToForward;
	mMMVect.push_back(tmpBlock);

	tmpBlock.status = BACKWARD;
	tmpBlock.prob = playToBackward;
	mMMVect.push_back(tmpBlock);

	tmpBlock.status = STOP;
	tmpBlock.prob = playToStop;
	mMMVect.push_back(tmpBlock);

	GslInit();
	CreateStartTime();
	CreateZipfDistribution();
//	LOG_INFO("mMaxClientNum = " << mMaxClientNum);
}

ModelAssemble::~ModelAssemble(){
	gsl_rng_free(m_r);
	mMMVect.clear();
}

void ModelAssemble::GslInit(){
	gsl_rng_env_setup();
	m_t = gsl_rng_default;
	m_r = gsl_rng_alloc(m_t);
}

void ModelAssemble::CreateStartTime(){
	double temp;
	mStartTime.push_back(0.0);
	double sum = 0.0;
	for(int i = 1;i < mMaxClientNum;i++){
		temp = RandomF(0,1);
		temp = 1 - temp;
		temp = 0 - log(temp) / mLambda;
		//秒
		sum += temp;
		mStartTime.push_back(sum);
	}
//	for(int i = 0;i < mMaxClientNum;i++)
//		cout << mStartTime[i] << endl;
}

void ModelAssemble::AdjustHotPlace(int fileId,int &segId){
	if(mHotPlaceNums != -1){
		int nearest = 0;
		int minSegNums = 1000000;
//			cout << mHotPlaceVect[fileId].size() << endl;
		for(int i = 0;i < mHotPlaceVect[fileId].size();i++){
			if(mHotPlaceVect[fileId][i] < segId &&
					(segId - mHotPlaceVect[fileId][i]) < minSegNums){
				minSegNums = segId - mHotPlaceVect[fileId][i];
				nearest = mHotPlaceVect[fileId][i];
			}
		}
		if(minSegNums < 100){
			adjustTimes++;
			segId = nearest;
			//cout << adjustTimes << endl;
		}
	}
}

void ModelAssemble::CreateZipfDistribution(){
	double denominator = 0.0;
	double u;
	for(int i = 0;i < mFileNum;i++){
		denominator += pow(i + 1,0 - mThelta);
	}

//	LOG_INFO("Thelta " << mThelta);

	for(int i = 0;i < mFileNum;i++){
		u = pow(i + 1,0 - mThelta);
		u = u / denominator;
		mProbability.push_back(u);
	}

	for(int i = 0;i < mFileNum;i++){
		if(mHotPlaceNums != -1){
			for(int j = 0;j < mHotPlaceNums;j++){
				int segId = RandomI(1,2000);
				mHotPlaceVect[i + 1].push_back(segId);
				//cout << (i + 1) << " " << segId << endl;
			}
		}
	}
}

int ModelAssemble::GetStartFileId(){
	double temp = RandomF(0,1);
	double sum = 0.0;
	for(int i = mFileNum - 1;i >= 0;i--){
		sum += mProbability[i];
		if(sum > temp){
			return i + 1;
		}
	}
	return 0;
}

int ModelAssemble::GetNextStatus(int curStatus){
	if((curStatus == PAUSE) | (curStatus == FORWARD) | (curStatus == BACKWARD))
		return PLAY;
	else{
		int temp = RandomI(1,1000);
		int sum = 0;
		for(unsigned int i = 0;i < mMMVect.size();i++){
			sum += mMMVect[i].prob;
			if(sum >= temp){
				return mMMVect[i].status;
			}
		}
	}
}

double ModelAssemble::GslRandLogNormal(int curStatus){
	double result;
	if(curStatus == PLAY || curStatus == BACKWARD)
		result = gsl_ran_lognormal(m_r,mBackZeta,mBackSigma);
	else if(curStatus == FORWARD)
		result = gsl_ran_lognormal(m_r,mForZeta,mForSigma);
	return result;
}
