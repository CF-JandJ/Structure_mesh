#pragma once
#include <string>
#include <fstream>
#include "MDArray.h"

template <typename T>
void readGrid(const char* file, MDArray<T, 2>& x, MDArray<T, 2>& y) {
	//���ļ�
	std::ifstream ifs(file);
	int ni = 0, nj = 0;
	ifs >> ni >> nj;

	//��ȡx����
	x.resize(nj, ni);
	for (int j = 0; j < nj; j++) {
		for (int i = 0; i < ni; i++) {
			ifs >> x(j, i);
		}
	}

	//��ȡy����
	y.resize(nj, ni);
	for (int j = 0; j < nj; j++) {
		for (int i = 0; i < ni; i++) {
			ifs >> y(j, i);
		}
	}
}