#include<tmmintrin.h> 
#include<smmintrin.h> 
#include<nmmintrin.h>
#include<immintrin.h> 
#include<xmmintrin.h> 
#include<emmintrin.h>
#include<pmmintrin.h>
#include <sys/time.h>
#include <iomanip>
#include<iostream>
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
#include<cmath>
#include <complex>
#include <fstream>
using namespace std;
string f[8] = { "1_130_22_8", "2_254_106_53", "3_562_170_53", "4_1011_539_263", "5_2362_1226_453","6_3799_2759_1953","7_8399_6375_4535", "11_85401_5724_756" };//八个文件
struct Size {
    int a, b, c;
} filedata[] = { {130, 22, 8}, {254, 106, 53}, {562, 170, 53}, {1011, 539, 262}, {2362, 1226, 453},{3799, 2759, 1953},{8399, 6375, 4535},{85401,5724,756} };
char cha = ' ';
void Parallel(int uu) {
    ifstream outout;
    ifstream inin;
    outout.open("file", std::ios::binary);//file为文件位置
    inin.open("file", std::ios::binary);//同样
    int cs = filedata[uu].a;
    int ar = filedata[uu].b;
    int br = filedata[uu].c;
    const int N = (cs + 31) / 32;
    int** a1 = new int* [ar];
    int** a2 = new int* [br];
    int* ur = new int[cs] ;
    for (int i = 0; i < br; i++) {//被消元行读入
        a2[i] = new int[N] {0};
        int col;
        while (1) {
            inin >> col;
            int te = cs - 1 - col;
            a2[i][te >> 5] += 1 << (31 - (te & 31));
            inin.get(cha);
        }
        inin.get(cha);
        if (inin.peek() == '\r')break;
    }
 
    for (int i = 0; i < ar; i++) {//消元子读入
        a1[i] = new int[N] {0};
        int co;
        char ch = ' ';
        outout >> co;
        int tt = cs - 1 - co;
        a1[i][tt >> 5] = 1 << (31 - (tt & 31));
        outout.get(ch);
        ur[co] = i + 1;
        while (outout.peek() != '\r') {
            outout >> co;
            a1[i][cs - 1 - co >> 5] += 1 << (31 - (cs - 1 - co & 31));
            outout.get(ch);
        }
    }
    //开始计算
    for (int i = 0; i < br; i++) {
        int init = 0, sd = 0;
        while (1) {
            while (sd < N&& a2[i][sd] == 0) ++sd,init = 0;
            if (sd >= N) break;
            int temp = a2[i][sd] << init;
            while (temp >= 0) 
                init++,temp <<= 1;
            if (!ur[cs - 1 - (sd << 5) - init] != 0) {ur[cs - 1 - (sd << 5) - init] = ~i;break;}
            else {
                int* er = ur[cs - 1 - (sd << 5) - init] > 0 ? a1[ur[cs - 1 - (sd << 5) - init] - 1] : a2[~ur[cs - 1 - (sd << 5) - init]];
                for (int j = 0; j + 8 <= N; j += 8) {
                    __m256 up = _mm256_load_ps(&a2[i][j]);
                    __m256 down = _mm256_load_ps(&er[j]);
                    up = _mm256_sub_ps(up, down);
                    _mm256_storeu_ps(&a2[i][j], up);
                    //对剩余元素进行处理
                    if (j + 16 > N) {
                        while (j < N)
                            a2[i][j] = a2[i][j]^er[j], j++;
                        break;
                    }
                }
            }
        }
    }
}
int main() {
    int t;
    struct timeval st;
    struct timeval en;
        t = 0;
        gettimeofday(&st, NULL);
            Parallel(1);//举例，读入第一个文件
        gettimeofday(&en, NULL);          
        //转换为ms
        float tt = (en.tv_sec - st.tv_sec) * 1000000 + en.tv_usec - st.tv_usec;
        cout << tt;//单位为微秒
}