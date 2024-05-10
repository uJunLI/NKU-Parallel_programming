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
float32x4_t u, uu, temp1, temp2, temp3;
int t1, t2, t3;
int t1, t2, t3; float myKey;;
float m[N][N];
void calcu() 
{
		 #pragma omp parallel
		if (1),num_threads(thread_num), private(t1, j, t3, u, uu, temp1, temp2, temp3)
			 for (t3 = 0; t3 < N; t3++)
			 {
			 #pragma omp single
				 {
					 u = vmovq_n_f32(m[t3][t3]);
				 for (t2 = t3 + 1; t2 < N; t2 += 4)
					 {
					 if (t2 + 4 > N)
						 for (; t2 < N; t2++)
						 m[t3][t2] = m[t3][t2] / m[t3][t3];
					 else
						 {
						 uu = vld1q_f32(m[t3] + t2);
						 uu = vdivq_f32(uu, u);
						 vst1q_f32(m[t3] + t2, uu);
						 }
					 m[t3][t3] = 1;
					 }
				 m[t3][t3] = 1.0;
				 }
				 #pragma omp for schedule(dynamic, chunksize)
				 for (t1 = t3 + 1; t1 < N; t1++) // 行划分并行
				 {
				 for (t2 = t3; t2 < N; t2 += 4)
					 {
					 if (t2 + 4 > N)
						 for (; t2 < N; t2++)
						 m[t1][t2] = m[t1][t2]−m[t1][t3]∗m[t3][t2];
					 else
						 {
						 temp1 = vld1q_f32(m[t1] + t2);
						 temp2 = vld1q_f32(m[k] + t2);
						 temp3 = vld1q_f32(m[t1] + k);
						 temp2 = vmulq_f32(temp3, temp2);
						 temp1 = vsubq_f32(temp1, temp2);
						 vst1q_f32(m[t1] + t2, temp1);
						 }
					 m[t1][k] = 0;
					 }
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
