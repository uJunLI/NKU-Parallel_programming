#include<pthread.h>
#include <sys/time.h>
#include<string>
#include<cstring>
#include<iostream>
#include<vector>
#include <semaphore.h>
#include <immintrin.h>
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
struct tta 
{
	int turnt;
	int uu;//线程标号
	int nums;
};
const int num = 6;//线程数
const int N = 600;
float m[N][N];
void* testF(void* a) {
	tta* my = (tta*)a;
	int u = my->uu;
	for (int k = 0; k < N; ++k)
	{
		if (u == 0)
		{
			__m256 vt = _mm256_set1_ps(m[k][k]);
			for (int j = k + 1; j < N; j += 8)
				if (j + 8 > N)
					for (; j < N; j++)
						m[k][j] = m[k][j] / m[k][k];
				else
				{
					__m256 va = _mm256_loadu_ps(m[k] + j);
					va = _mm256_div_ps(va, vt);
					_mm256_storeu_ps(m[k] + j, va);
				}
			m[k][k] = 1;
		}
		pthread_barrier_wait(&barrier_Divsion);

		for (int i = k + 1 + u; i < N; i += num) {
			for (int j = k + 1; j < N; j += 8)//接下来解决剩余值
				if (j + 8 > N)for (; j < N; j++)m[i][j] = m[i][j] - m[i][k] * m[k][j];
				else
				{
					__m256 t1 = _mm256_loadu_ps(m[i] + j);
					__m256 t2 = _mm256_loadu_ps(m[k] + j);
					__m256 t3 = _mm256_set1_ps(m[i][k]);
					t1 = _mm256_sub_ps(t1, t2), t2 = _mm256_mul_ps(t3, t2);
					_mm256_storeu_ps(m[i] + j, t1);
				}
			m[i][k] = 0;
		}
		pthread_barrier_wait(&barrier_Elimination);
	}
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
