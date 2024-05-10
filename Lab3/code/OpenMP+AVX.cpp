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
#include <omp.h>
using namespace std;
const int N = 1000;
const int cnt = 6;
int i, j, k;
__m256 u, uu, t1, t2, t3;
float m[N][N];
void calcu()
{
#pragma omp parallel if(1),num_threads(thread_num),private(i,j,k,u,uu,t1,t2,t3)
	for (k = 0; k < N; k++)
	{
#pragma omp single
		{
			u = _mm256_set1_ps(m[k][k]);
			for (j = k + 1; j < N; j += 8)
			{
				if (j + 8 > N)
					for (; j < N; j++)
						m[k][j] = m[k][j] / m[k][k];
				else
				{
					uu = _mm256_loadu_ps(m[k] + j);
					uu = _mm256_div_ps(uu, u);
					_mm256_storeu_ps(m[k] + j, uu);
				}
				m[k][k] = 1.0;
			}
			m[k][k] = 1.0;

		}

#pragma omp for schedule(dynamic,chunksize)
		for (i = k + 1; i < N; i++)
		{
			for (j = k; j < N; j += 8)
			{
				if (j + 8 > N)
					for (; j < N; j++)
						m[i][j] = m[i][j] - m[i][k] * m[k][j];
				else
				{
					t1 = _mm256_loadu_ps(m[i] + j);
					t2 = _mm256_loadu_ps(m[k] + j);
					t3 = _mm256_set1_ps(m[i][k]);
					t2 = _mm256_mul_ps(t3, t2);
					t1 = _mm256_sub_ps(t1, t2);
					_mm256_storeu_ps(m[i] + j, t1);
				}
				m[i][k] = 0;
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
