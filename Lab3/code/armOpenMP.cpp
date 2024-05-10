#include<pthread.h>
#include <sys/time.h>
#include<string>
#include<cstring>
#include<iostream>
#include<vector>
#include<stack>
#include<iomanip>
#include<deque>
#include<queue>
#include<fstream>
#include<algorithm>
#include<sstream>
#include<set>
#include<map>
#include<numeric>
#include<list>
#include <complex>
#include <stdio.h>
#include <stdlib.h>
#include <arm_neon.h>
#include <omp.h>
using namespace std;
const int N = 1000;
const int cnt = 6;
int t1, t2, t3;float myKey;;
float m[N][N];
void calcu() {
#pragma omp parallel if(1),num_threads(cnt),private(t1,t2,t3,myKey)
	for (t3 = 0; t3 < N; t3++){
#pragma omp single
		{
			myKey = m[t3][t3];
			for (t2 = t3 + 1; t2 < N; t2++)
				m[t3][t2] = m[t3][t2] / myKey,m[t3][t3] = 1;
		}

#pragma omp for
		for (t1 = t3 + 1; t1 < N; t1++)
		{
			myKey = m[t1][t3];
			for (t2 = t3 + 1; t2 < N; t2++)
				m[t1][t2] = m[t1][t2] - myKey * m[t3][t2];
			m[t1][t3] = 0;
		}
	}
}
int main() {
	//初始化
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < i; j++)
			m[i][j] = 0;
		m[i][i] = 1.0;
		for (int j = i + 1; j < N; j++)
			m[i][j] = rand();
	}
	for (int k = 0; k < N; k++)
		for (int i = k + 1; i < N; i++)
			for (int j = 0; j < N; j++)
				m[i][j] += m[k][j];
	struct timeval st;
	struct timeval ed;
	float myt;
	m_reset();
	gettimeofday(&st, NULL);
	calcu();
	gettimeofday(&end, NULL);
	myt += (ed.tv_sec * 1000 - st.tv_sec * 1000 + ed.tv_usec - st.tv_usec;//单位：毫秒
	cout << myt;
}


