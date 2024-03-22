#include <iostream>
#include <windows.h>

using namespace std;

const int N=10000;

double b[N][N],a[N],sum[N];

//��ʼ��
void init(int n)
{
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            b[i][j] = i+j;//��ʼ������
        }
        a[i] = i;//��ʼ������
    }
}


void f_ordinary(int n)
{
    for(int i=0;i<n;i++)
    {
        sum[i]=0.0;
        for(int j=0;j<n;j++)
        {
            sum[i]+=b[j][i]*a[j];
        }
    }
}

void f_pro(int n)
{
    for (int i=0; i<n; i++)
    {
        sum[i]=0.0;
    }
    for (int j=0;j<n;j++)
    {
        for(int i=0;i<n;i++)
        {
            sum[i]+=b[j][i]*a[j];
        }
    }


}

void f_unrolled(int n)
{
    for(int i = 0; i < n; i++)
    {
        sum[i] = 0.0;
    }

    // Assume n is a multiple of 4 for simplification
    for(int i = 0; i < n; i += 4)
    {
        for(int j = 0; j < n; j++)
        {
            sum[j] += b[i][j] * a[i];
            sum[j] += b[i+1][j] * a[i+1];
            sum[j] += b[i+2][j] * a[i+2];
            sum[j] += b[i+3][j] * a[i+3];
        }
    }
}


int main()
{
    int n,step=100;
    long long counter;// ��¼����
    double seconds ;
    long long head,tail,freq,noww;
    init(N);

    for(int n=0;n<=10000;n+=step)
    {
        QueryPerformanceFrequency((LARGE_INTEGER *)&freq);
        QueryPerformanceCounter((LARGE_INTEGER *)&head);//��ʼ��ʱ

        counter=0;
        while(true)
        {
            QueryPerformanceCounter((LARGE_INTEGER *)&noww);
            if( (noww-head)*1000.0/freq > 10)
                break;
            // f_ordinary(n);//ִ�к���
            f_pro(n);
            counter++;
        }
        QueryPerformanceCounter((LARGE_INTEGER *)&tail );//������ʱ
        seconds = (tail - head) * 1000.0 / freq ;//��λ ms

        //������
        cout << n <<' '<< counter <<' '<< seconds<<' '<< seconds / counter << endl ;
       if(n==1000)
          step=1000;
    }

    return 0;
}
