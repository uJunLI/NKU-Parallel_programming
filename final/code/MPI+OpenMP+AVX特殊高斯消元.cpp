#include <sys/time.h>
#include<string>
#include<cstring>
#include<iostream>
#include<vector>
//1

#include<smmintrin.h> 
#include<nmmintrin.h> 
#include<immintrin.h> 
#include <mpi.h>
#include <unistd.h>
#include <omp.h>
#include<xmmintrin.h> 
#include<pmmintrin.h> 
#include<emmintrin.h> 
#include<tmmintrin.h>
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
int cc;
struct fs 
{
    int a;
    int b;
    int c;
};
char kong = ' ';
fs thefile[8] = { {130, 22, 8}, {254, 106, 53}, {562, 170, 53}, {1011, 539, 262}, {2362, 1226, 453},{3799, 2759, 1953},{8399, 6375, 4535},{85401,5724,756} };
string myfile[8] = { "1_130_22_8", "2_254_106_53", "3_562_170_53", "4_1011_539_263", "5_2362_1226_453","6_3799_2759_1953","7_8399_6375_4535", "11_85401_5724_756" };
int times;
struct timeval ed;
struct timeval st;

bool myFinalHonework(int which) 
{
    ifstream xyz;
    ifstream bxyh;
    //被消元子和消元行
    xyz.open("文件地址", std::ios::binary);
    bxyh.open("文件地址" , std::ios::binary);


    
    int erhang = thefile[which].c;
    int CS = thefile[which].a;
    int N = CS;
    int rr = thefile[which].b;



    int** myxyz = new int* [rr];
    int** bybxyh = new int* [erhang];
    int* flag = new int[CS] {0};

    for (int i = 0; i < erhang; i++) 
    {
        bybxyh[i] = new int[N] {0};
        int cc;
        while (bxyh.peek() != '\r') {
            bxyh >> cc;
            int df = CS - 1 - cc;
            bybxyh[i][df >> 5] = bybxyh[i][df >> 5]+1 << (31 - (df & 31));
            bxyh.get(kong);
        }
        bxyh.get(kong);
    }//被消元行

    for (int i = 0; i < rr; i++)
    {
        myxyz[i] = new int[N] {0};
        xyz >> cc;
        int r = CS - 1 - cc;
        myxyz[i][r >> 5] = 1 << (31 - (r & 31));
        xyz.get(kong);
        flag[cc] = i + 1;
        while (xyz.peek() != '\r') {
            xyz >> cc;
            int df = CS - 1 - cc;
            myxyz[i][df >> 5] = myxyz[i][df >> 5] + 1 << (31 - (df & 31));
            xyz.get(kong);
        }
    }//消元子


    int t_id, nps;
    MPI_Comm_rank(MPI_COMM_WORLD, &t_id);
    MPI_Comm_size(MPI_COMM_WORLD, &nps);
    MPI_Request request;

    int one, two;
    int num = N / nps;
    one = t_id * num;
    if (t_id != nps - 1)two = (t_id + 1) * num;
    else two = (t_id + 1) * N / nps;
   

#pragma omp parallel num_threads(NUM_THREADS)
#pragma omp for
    for (int i = one; i < two; i += 1)
    {
        int BB = 0;
        int bb = 0;
        while (BB<N) {
            while (BB < N&& es[i][BB] == 0)BB++,bb = 0.0;
            int temp = es[i][BB] << bb;
            while (!(temp < 0)) 
            {
                bb++;
                temp <<= 1;
            }
            int& ES = flag[CS  - (BB << 5) - bb];
            if (ES == 0)
            {
                ES = ~i;
                MPI_Ibcast(&es[i][0], N, MPI_FLOAT, t_id, MPI_COMM_WORLD, &request);
                MPI_Ibcast(&flag[CS  - (BB << 5) - bb], 1, MPI_FLOAT, t_id, MPI_COMM_WORLD, &request);
                break;

            }
            else 
            {
                if (ES > 0)er = ER[ES - 1];
                else er = es[ES];
                for (int j = 0; j + 8 <= N; j = j+8) 
                {
                    __m256 vi = _mm256_load_ps(&es[i][j]);
                    __m256 vj = _mm256_load_ps(&er[j]);
                    vi = _mm256_sub_ps(vi, vj);
                    _mm256_storeu_ps(&es[i][j], vi);
                    if (j  > N-16) 
                    {
                        while (j < N) es[i][j] ^= er[j];j++;
                        break;
                    }
                }
            }
        }
    }
    MPI_Barrier(MPI_COMM_WORLD); 
    return 1;
}


int main() {
    for (int i = 0; i <= 7; i++) 
    { 
        times = 0;
        gettimeofday(&st, NULL);
        gettimeofday(&ed, NULL);
        while ((ed.tv_sec - ed.tv_sec) < 1) {
            times++;
            myFinalHonework(i);
            gettimeofday(&ed, NULL);
        }//遍历所有文件
        float myt = (ed.tv_sec - st.tv_sec) + ((ed.tv_usec - st.tv_usec)) / 1000000;
        //单位ms;
        cout  <<myt / times << endl;//计算时间并输出
        
    }
    return 0;
}