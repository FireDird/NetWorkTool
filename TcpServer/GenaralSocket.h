#include<sys/socket.h>
#include<pthread.h>

typedef struct{
    pthread_t thread_tid;
    long      thread_count;
}Thread;//用作某个线程接受连接的次数
typedef struct{
    int* pget;
    int iput;
    Thread* pThreadInfo;
    int nthreadIndex;
    int* pClientArray;
}Thread_param;

class CGenaralSocket
{

    typedef int Socketfd;
    typedef struct sockaddr_in  Socketaddr;
      public:
        bool InitSocketTCP();
        void ReleaseTCPSocket();
        bool SendData_Tcp();
        bool ReceiveData_Tcp();
        static GenaralSocket* GetInstance();
    private:
        static  CGenaralSocket *m_pInstance;
        CGenaralSocket();
        CGenaralSocket(const CGenaralSocket& Object);
        CGenaralSocket& operator=(const CGenaralSocket& const Object);
        void Thread_make(int nThreadIndex);
    private:
        Socketfd  m_socketTcpServer;
        Socketaddr m_socketTcpServerAddr;
        Thread  *m_pThreads;
        int m_connectedfd[64];// 定义的连接客户端数组
        int m_iget;         
        int m_iput;
    public:
        static pthread_mutex_t m_clifd_mutex;
        static pthread_cond_t m_clifd_cond;
}
