#pragma once
#include <string>
#include <fstream>
#include "MDArray.h"

template <typename T>
void readGrid(const char* file, MDArray<T, 2>& x, MDArray<T, 2>& y) {
	//打开文件
	std::ifstream ifs(file);
	int ni = 0, nj = 0;
	ifs >> ni >> nj;

	//读取x坐标
	x.resize(nj, ni);
	for (int j = 0; j < nj; j++) {
		for (int i = 0; i < ni; i++) {
			ifs >> x(j, i);
		}
	}

	//读取y坐标
	y.resize(nj, ni);
	for (int j = 0; j < nj; j++) {
		for (int i = 0; i < ni; i++) {
			ifs >> y(j, i);
		}
	}
}