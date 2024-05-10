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
#include <arm_neon.h>
#include <complex>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
const int num = 6;//线程数
const int N = 500;
float m[N][N];

struct tta {
	int turnt;//turn
	int uu;//id of thread
	int nums;
};//thread

void*   testF(void* pr) {//6线程+neon
	tta* p = (tta*)pr;
	int uu = p->uu;
	int nums = 6;
	for (int k = 0; k < N; ++k)
	{
		if (uu == 0)
		{
			float32x4_t vt = vmovq_n_f32(m[k][k]);
			for (int j = k + 1; j < N; j += 4)
				if (j + 4 > N)
					for (; j < N; j++)
						m[k][j] = m[k][j] / m[k][k];
				else
				{
					float32x4_t va = vld1q_f32(m[k] + j);
					va = vmulq_f32(va, vt);
					vst1q_f32(m[k] + j, va);
				}
			m[k][k] = 1.0;
		}
		pthread_barrier_wait(&barrier_Divsion);
		for (int i = k + 1 + uu; i < N; i += nums)//主体循环
		{
			for (int j = k + 1; j < N; j += 4)
			{
				if (j + 4 > N)
					for (; j < N; j++)
						m[i][j] = m[i][j] - m[i][k] * m[k][j];//剩余的部分
				else//并行化处理
				{
					float32x4_t t1 = vld1q_f32(m[i] + j);
					float32x4_t t2 = vld1q_f32(m[k] + j);
					float32x4_t t3 = vld1q_f32(m[i] + k);
					t2 = vmulq_f32(t3, t2);
					t1 = vsubq_f32(t1, t2);
					vst1q_f32(m[i] + j, t1);
				}
				m[i][k] = 0;
			}
		}
		pthread_barrier_wait(&barrier_Elimination);//等待
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
