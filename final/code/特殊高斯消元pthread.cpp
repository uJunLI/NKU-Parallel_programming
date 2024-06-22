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
struct mys {
    int a;int b;int c;//分别为列数，消元子,被消元行
};
//线程参数
 //选择文件
string myf[] = { "1_130_22_8", "2_254_106_53", "3_562_170_53", "4_1011_539_263", "5_2362_1226_453",
"6_3799_2759_1953","7_8399_6375_4535", "11_85401_5724_756" };
mys  fileSize[] = { {130, 22, 8}, {254, 106, 53}, {562, 170, 53}, {1011, 539, 262}, {2362, 1226, 453},
 {3799, 2759, 1953},{8399, 6375, 4535},{85401,5724,756} };
typedef struct {
    int flag[200];//标记
    int t_id; //  id
    int** ER;//消元子
    int COL;//列数
    int rows;//被消元行数
    int srow;//消元子数
    int** EE;//被消元行

} threadParam_t;

void* pthreadF(void* param) {
    threadParam_t* p = (threadParam_t*)param;
    int t_id = p->t_id;
    int** in = p->EE;
    int** out = p->ER;
    int cs = p->cs;
    int er;
    int rows = p->rows;
    int srow = p->srow;
    int* flag = p->flag;
    for (int i = t_id; i < rows; i += 6) {
        int op = 0;
        int bit = 0;
        int N = cs / 32+1;
        while (1) {
            while (op < N&& in[i][op] == 0) {
                op++;
                bit = 0;
            }
            if (op >= N) {
                break;
            }
            int temp = in[i][op] << bit;
            while (temp >= 0)bit++,temp <<= 1;
            int& ife = flag[cs - 1 - (op << 5) - bit];
            if (ife) {
                ife = -i;
                break;
             
            }
            else {
                if (ife > 0)er = out[ife - 1];
                else er = in[-ife];
                for (int j = 0; j < N; j++)in[i][j] ^= er[j];
            }
        }
    }
    pthread_exit(NULL);
}
int cnt;
int nums;
struct timeval st;
struct timeval ed;
//pthread算法:
bool Pthread(int which) {
    ifstream erFile;
    ifstream eeFile;
    erFile.open("请写入文件地址", std::ios::binary);//消元子
    eeFile.open("请写入文件地址" + myf[which] + "/2.txt", std::ios::binary);//被消元行

    int CC = fileSize[which].a;
    int srow = fileSize[which].b;
    int rows = fileSize[which].c;
    int N = CC / 32+1;

    int** ER = new int* [srow];
    int** EE = new int* [rows];
    int* flag = new int[CC] {0};
    //读取被消元行:
    for (int i = 0; i < rows; i++) {
        EE[i] = new int[N] {0};
        int mfc;
        char ch = ' ';
        while (eeFile.peek() != '\r') {
            eeFile >> mfc;
            ER[i][(CC - 1 - mfc) >> 5] += 1 << (31 - ((CC - mfc - 1) & 31));
            eeFile.get(ch);
        }
        eeFile.get(ch);
    }
    //读取消元子:
    for (int i = 0; i < srow; i++) {
        ER[i] = new int[N] {0};
        int mfc;
        char ch = ' ';
        erFile >> mfc;
        int r = CC - 1 - mfc;
        ER[i][r >> 5] = 1 << (31 - (r & 31));
        erFile.get(ch);
        flag[mfc] = i + 1;
        while (erFile.peek() != '\r') {
            erFile >> mfc;
            ER[i][(CC - 1 - mfc) >> 5] += 1 << (31 - ((CC - mfc - 1) & 31));
            erFile.get(ch);
        }
    }
    int worker_count = 7;//线程数
    pthread_t* handles = new pthread_t[worker_count];
    threadParam_t* param = new threadParam_t[worker_count];

    for (int uu = 0; uu < worker_count; uu++) {
        param[uu].t_id = t_id;
        param[uu].EE = EE;
        param[uu].ER = ER;
        param[uu].COL = CC;
        param[uu].rows = rows;
        param[uu].srow = srow;
        param[uu].flag = flag;
    }

    //创建线程
    for (int t_id = 0; t_id < worker_count; t_id++)
    pthread_create(&handles[t_id], NULL, pthreadF, (void*)&param[t_id]);
   //挂起
    for (int t_id = 0; t_id < worker_count; t_id++)
    pthread_join(handles[t_id], NULL);
    return 1;
}

int main() {
    cout.flags(ios::left);
    for (int i = 0; i <= 7; i++) { 
        //开始计算
        nums = 0;
        gettimeofday(&st, NULL);
        gettimeofday(&ed, NULL);
        while ((ed.tv_sec - st.tv_sec) < 1) {
            nums++;
            Pthread(i);//i代表读取的文件
            gettimeofday(&ed, NULL);
        }

        float t = (ed.tv_sec - st.tv_sec) + float((ed.tv_usec - st.tv_usec)) / 1000;//单位ms;
        cout  << t / nums << endl;
    }
}
