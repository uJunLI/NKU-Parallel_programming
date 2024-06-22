#include <sys/time.h>
#include<string>
#include<cstring>
#include<iostream>
#include<vector>
//1
#include<pthread.h>
#include <semaphore.h>
#include <mpi.h>
//1
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
#include <unistd.h>
#include <fstream>

using namespace std;
struct timeval st;
struct timeval ed;
int m_id, np;
ifstream readzi;//读取消元子用
ifstream readhang;//读取被消元行用
struct Size {
    int a, b, c;
};
char kong = ' ';
Size fdata[8] = { {130, 22, 8}, {254, 106, 53}, {562, 170, 53}, {1011, 539, 262}, {2362, 1226, 453},
{3799, 2759, 1953},{8399, 6375, 4535},{85401,5724,756} };


bool myMPI(int which) {//读取的文件
    int fuer = fdata[which].c;
    int cs = fdata[which].a;
    int zir = fdata[which].b;
    int N = cs;
    int** xyz = new int* [zir];
    int** bxyh = new int* [fuer];
    int* f = new int[cs] {0};
    string files[8] = { "1_130_22_8", "2_254_106_53", "3_562_170_53", "4_1011_539_263", "5_2362_1226_453","6_3799_2759_1953","7_8399_6375_4535", "11_85401_5724_756" };
   

    readzi.open("这里填写文件位置", std::ios::binary);//消元子文件
    readhang.open("这里填写文件位置" , std::ios::binary);//被消元行文件



    for (int i = 0; i < zir; i++) {
        xyz[i] = new int[N] {0};
        int tempc;
        readzi >> tempc;
        int r = cs - 1 - tempc;
        xyz[i][r >> 5] = 1 << (31 - (r & 31));
        readzi.get(kong);
        f[tempc] = i + 1;
        while (readzi.peek() != '\r') 
        {
            readzi >> tempc;
            int df = cs  - tempc;
            xyz[i][df >> 5] = xyz[i][df >> 5]+ 1 << (31 - (df & 31));
            readzi.get(kong);
        }
    }//读消元子

    for (int i = 0; i < fuer; i++) 
    {
        bxyh[i] = new int[N] {0};

        
        int tempc;
        while (readhang.peek() != '\r') 
        {
            readhang >> tempc;
            int df = cs - 1 - tempc;
            bxyh[i][df >> 5] += 1 << (31 - (df & 31));
            readhang.get(' ');
        }
        readhang.get(' ');
    }//读被消元行



    MPI_Comm_rank(MPI_COMM_WORLD, &m_id);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Request request;

    int one, two;
    int num = N / np;
    one = m_id * num;
    if (m_id == np - 1) {
        two = (m_id + 1) * N / np;
    }
    else {
        two = (m_id + 1) * num;
    }

    for (int i = one; i < two; i ++) 
    {
        int bs = 0;
        int Bss = 0;
        int N = cs;
        while (bs < N)
        {
            while (bs < N&& bxyh[i][bs] == 0) bs++, Bss = 0;

          
            int temp = bxyh[i][bs] << Bss;
            while (temp >= 0)  Bss++,temp <<= 1;

            int& isExist = f[cs - 1 - (bs << 5) - Bss];
            if (!isExist != 0) 
            {
                isExist = ~i;
                MPI_Ibcast(&bxyh[i][0], N, MPI_FLOAT, m_id, MPI_COMM_WORLD, &request);
                MPI_Ibcast(&f[cs - 1 - (bs << 5) - Bss], 1, MPI_FLOAT, m_id, MPI_COMM_WORLD, &request);
                break;
                
            }
            else {
                int* er = isExist > 0 ? xyz[isExist - 1] : bxyh[~isExist];
                for (int j = 0; j < N; j++) {
                    bxyh[i][j] ^= er[j];
                }
            }
        }
    }


    MPI_Barrier(MPI_COMM_WORLD);
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int m_id;
    MPI_Comm_rank(MPI_COMM_WORLD, &m_id);




    for (int i = 0; i <= 7; i++) 
    { 
        if (m_id == 0) gettimeofday(&st, NULL);
        myMPI(i);
        if (m_id == 0) gettimeofday(&ed, NULL);

        float time1 = (ed.tv_sec - st.tv_sec) + ((ed.tv_usec - st.tv_usec)) / 1000000;;
        if (m_id == 0) {
            //每一行打印出一种测试集的时间结果
            cout << time1 << endl;
        }
    }
    MPI_Finalize();
}