#include<pthread.h>
#include <sys/time.h>
#include<string>
#include<cstring>
#include<iostream>
#include<vector>
#include <semaphore.h>
//#include <immintrin.h>
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
#include <immintrin.h>
#include <omp.h>
#include <fstream>
int er;
string myf[] = { "1_130_22_8", "2_254_106_53", "3_562_170_53", "4_1011_539_263", "5_2362_1226_453",
    "6_3799_2759_1953","7_8399_6375_4535", "11_85401_5724_756" };
struct Size {
    int a;int b;int c;//分别为列数，消元子,被消元行
} fileSize[] = { {130, 22, 8}, {254, 106, 53}, {562, 170, 53}, {1011, 539, 262}, {2362, 1226, 453},
{3799, 2759, 1953},{8399, 6375, 4535},{85401,5724,756} };
bool OpenMP(int which) {
    
    ifstream inf;
    ifstream ofs;
    inf.open("输入文件位置" + myf[which] + "/1.txt", std::ios::binary);//消元子文件
    ofs.open("输入文件位置" + myf[which] + "/2.txt", std::ios::binary);//被消元行文件
      
    int CC = fileSize[which].a;
    int N = CC / 32 + 1;
    int in1 = fileSize[which].b;
    int in2 = fileSize[which].c;
    
    int** ER = new int* [in1];
    int** EE = new int* [in2];
    int* flag = new int[CC] {0};


    //被消元行:
    for (int i = 0; i < in2; i++) {
        EE[i] = new int[N] {0};
        int mfc;
        char ch = ' ';
        while (ofs.peek() != '\r') {
            ofs >> mfc;
            EE[i][(CC - 1 - mfc) >> 5] += 1 << (31 - ((CC - 1 - mfc) & 31));
            ofs.get(ch);
        }
        ofs.get(ch);
    }

    //消元子:
    for (int i = 0; i < in1; i++) {
        ER[i] = new int[N] {0};
        int mfc;
        char ch = ' ';
        inf >> mfc;
        int r = CC - 1 - mfc;
        ER[i][r >> 5] = 1 << (31 - (r & 31));
        inf.get(ch);
        flag[mfc] = i + 1;
        while (inf.peek() != '\r') {
            inf >> mfc;
            EE[i][(CC - 1 - mfc) >> 5] += 1 << (31 - ((CC - 1 - mfc) & 31));
            inf.get(ch);
        }
    }


    int ts = 7;
    #pragma omp parallel ts(ts)
    #pragma omp for
    for (int i = 0; i < in2; i++) {
        int bb = 0,int BB = 0;
        int N = CC / 32+1;
        while (1) {
            while (bb < N && EE[i][bb] == 0) {
                bb++;
                BB = 0;
            }
            if (bb >= N) {
                break;
            }
            int temp = EE[i][bb] << BB;
            while ((EE[i][bb] << BB) >= 0)BB++,temp <<= 1;
            int& mye = flag[CC - 1 - (bb << 5) - BB];
            if (mye) {
                mye = -i;
                break;
            
            }
            else 
            {
                if (mye > 0)er = ER[mye - 1];
                else er = EE[-mye];
                for (int j = 0; j < N; j++)EE[i][j] ^= er[j];
            }
        }
    }
    return 1;
}

int main() {
    for (int i = 0; i <= 7; i++) {
        //开始计算
        nums = 0;
        gettimeofday(&st, NULL);
        gettimeofday(&ed, NULL);
        while ((ed.tv_sec - st.tv_sec) < 1) {
            nums++;
            OpenMP(i);//i代表读取的文件
            gettimeofday(&ed, NULL);
        }

        float t = (ed.tv_sec - st.tv_sec) + float((ed.tv_usec - st.tv_usec)) / 1000;//单位ms;
        cout << t / nums << endl;
    }
}
