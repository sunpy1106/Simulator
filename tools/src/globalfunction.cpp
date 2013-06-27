/*
 * globalfunction.cc
 *
 *  Created on: 2013-1-1
 *      Author: zhaojun
 */

#include "globalfunction.h"

void Trim(std::string &s){
	s.erase(0,s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);
}

void ParseConfigFile(char *configFileName,std::map<std::string,std::string> &keyMap){
	std::ifstream infile;
	infile.open(configFileName);

	std::string line;
	while(std::getline(infile,line)){
		if(line.empty())
			continue;
		Trim(line);
		if(line.at(0) == '#')
			continue;
		int equalPos = line.find_first_of('=');
		std::string key = line.substr(0,equalPos);
		std::string value = line.substr(equalPos + 1);
		Trim(key);
		Trim(value);
		keyMap.insert(std::make_pair(key,value));

//		cout << key << "=" << value << endl;

//		if(!infile.good())
//			break;
	}


	infile.close();
}

int RandomI(int first,int second){
	double temp = random() / (RAND_MAX * 1.0);
	return (int)(first + (second - first) * temp);
}

double RandomF(int a,int b){
	double temp = random() / (RAND_MAX * 1.0);
	return a + (b - a) * temp;
}

double RandomF(double a,double b){
	double temp = random() / (RAND_MAX * 1.0);
	return a + (b - a) * temp;
}



