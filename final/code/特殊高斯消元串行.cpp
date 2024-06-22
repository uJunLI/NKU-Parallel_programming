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
    // 以下是矩阵列数，消元子,被消元行
    int a, b, c;
} filedata[] = { {130, 22, 8}, {254, 106, 53}, {562, 170, 53}, {1011, 539, 262}, {2362, 1226, 453},{3799, 2759, 1953},{8399, 6375, 4535},{85401,5724,756} };
char cha = ' ';
bool chuan(int uu) {
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
    int* ur = new int[cs];

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
        int col;
        char ch = ' ';
        outout >> col;
        int tt = cs - 1 - col;
        a1[i][tt >> 5] = 1 << (31 - (tt & 31));
        outout.get(ch);
        ur[col] = i + 1;
        while (outout.peek() != '\r') {
            outout >> col;
            a1[i][cs - 1 - col >> 5] += 1 << (31 - (cs - 1 - col & 31));
            outout.get(ch);
        }
    }
    //串行计算主体
    for (int i = 0; i < cs; i++) {
        int s = 0;
        int q = 0;
        while (1) {
            while (s < N&& a2[i][s] == 0) s++,q = 0;
            if (s >= N) break;
            int temp = a2[i][s] << q;
            while (temp >= 0) {
                q++;
                temp <<= 1;
            }
            if (!ur[cs - 1 - (s << 5) - q] == 0) {
                int* er= NULL;
                if (ur[cs - 1 - (s << 5) - q] > 0)er=a1[ur[cs - 1 - (s << 5) - ar] - 1];
                else er=a2[~ur[cs - 1 - (s << 5) - ar]];
             
                for (int j = 0; j < N; j++) {
                    a2[i][j] ^= er[j];
                }
            }
            else {
                ur[cs - 1 - (s << 5) - q] = ~i;
                break;
            }
        }
    }

}
int main() {
    struct timeval st;
    struct timeval en;
    int t;
    t = 0;
        gettimeofday(&st, NULL);
        chuan(1);//举例，读入第一个文件
        gettimeofday(&en, NULL);
    //转换为ms
        float tt = (en.tv_sec - st.tv_sec) * 1000000 + en.tv_usec - st.tv_usec;
        cout << tt;//单位为微秒
}
