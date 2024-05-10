#include<iostream>
#include<string>
#include<cstring>
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
#include<cmath>
#include <complex>
#include <windows.h>
using namespace std;
const int N = 1000;
float m[N][N];
 void m_reset(){
 for (int i = 0; i < N; i++)
     {
     m[i][i] = 1.0;
     for (int j = i + 1; j < N; j++)
         m[i][j] = rand();
     }
 for (int k = 0; k < N; k++)
     for (int i = k + 1; i < N; i++)
     for (int j = 0; j < N; j++)
     m[i][j] += m[k][j];
 }
int main() {   
    long long head, tail, freq; // timers
    m_reset();
    QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    for (int k = 1; k < N; k++) {
        for (int j = k + 1; j < N; j++) {
            m[k][j] /= m[k][k];
        }
        m[k][k] = 1.0;
        for (int i = k + 1; i < N; i++) {
            for (int j = k + 1; j < N; j++) {
                m[i][j] -= m[i][k] * m[k][j];
            }
            m[i][k] = 0;
        }
    } 
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    cout << "Col: " << (tail - head) * 1000.0 / freq<< "ms" << endl;
}