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
using namespace std;
const int N = 500;
float m[N][N];
struct tta {
	int turnt;
	int uu;//线程标号
	int nums;
};
void* testF(void* pr) {//6线程
	tta* p = (tta*)pr;
	int ss = p->turnt;
	int uu = p->uu;
	int r = ss + uu + 1;
	int con = p->nums;

	for (int i = r; i < N; i += con)
		for (int j = ss + 1; j < N; ++j)
			m[i][j] = m[i][j] - m[i][ss] * m[ss][j];
		m[i][ss] = 0;
	pthread_exit(NULL);
	return NULL;
}

int main() {
	for (int i = 0; i < N; i++)//初始化
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
	//初始化结束
	struct timeval st;
	struct timeval ed;
	double mytime;
	gettimeofday(&start, NULL);
	int w = 8;
	pthread_t* s = new pthread_t[w];
	tta* pa = new tta[w];//线程初始化
	for (int s = 0; s < N; s++)//主要循环如下
	{
		for (int j = s + 1; j < N; j++)m[s][j] = m[s][j] / m[s][s];
		m[s][s] = 1;
		for (int uu = 0; uu < w; uu++)
			pa[uu].turnt = s, pa[uu].uu = uu, pa[uu].nums = w;
		for (int uu = 0; uu < w; uu++)pthread_create(&s[uu], NULL, testF, (&pa[uu]));
		for (int uu = 0; uu < w; uu++)pthread_join(s[uu], NULL);
	}
	gettimeofday(&end, NULL);
	mytime = ed.tv_sec * 1000 - st.tv_sec * 1000 + ed.tv_usec - st.tv_usec;//单位：毫秒
	cout << mytime;
}