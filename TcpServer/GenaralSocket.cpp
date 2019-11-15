#include "GenaralSocket.h"
#include<errno.h>
#include<unistd.h>
#include<netinet/in.h>
#define SERVER_PORT 6000
#define LISTENQ     5
#define     MAX_THREAD_POOL  10
#define    MAX_CONNECTED_CLIENT 32
#define     MAXN 16384
CGenaralSocket* CGenaralSocket::m_pInstance = NULL;
pthread_mutex_t CGenaralSocket::m_clifd_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t CGenaralSocket::m_clifd_cond = PTHREAD_COND_INITIALIZER;


CGenaralSocket* CGenaralSocket::GetInstance()
{
    if(m_pInstance == NULL)
    {
        m_pInstance = new CGenaralSocket;
    }
    return m_pInstance;
}

void web_child(int sockfd)
{
    int ntowrite;
    ssize_t nread;
    char line[MAXLINE], result[MAXN];
    for(;;)
    {
        if((nread = read(sockfd, line, MAXLINE)) == 0)
            return ;
        ntowrite = atol(line);
        if((ntowrite <= 0) || (ntowrite > MAXN))
            return ;
        write(sockfd, result, ntowrite);
    }
}

void* thread_main(void *pParam)
{
    Thread_param* pThreadParam = (Thread_param*) pParam;
    int connfd;
    //printf("");
    for(;;)
    {
        pthread_mutex_lock(&CGenaralSocket::m_clifd_mutex);
        while(*(pThreadParam->pget) == pThreadParam->iput)
            pthread_cond_wait(&CGenaralSocket::m_clifd_cond, &CGenaralSocket::m_clifd_mutex);
        connfd = pThreadParam->pClientArray[*(pThreadParam->pget)];
        if(++(*(pThreadParam->pget)) == MAX_CONNECTED_CLIENT)
            *(pThreadParam->pget) = 0;
        pthread_mutex_unlock(&CGenaralSocket::m_clifd_mutex);
        pThreadParam->pThreadInfo->thread_count++;
        web_child(connfd);
        close(connfd);
    }

}


void CGenaralSocket::Thread_make(int nThreadIndex)
{
    Thread_param* pTParam;
    pTParam = new Thread_param;
    pTParam->pget = &m_iget;
    pTParam->iput = m_iput;
    pTParam->pThreadInfo = m_pThreads;
    pTParam->nthreadIndex = nThreadIndex;
    pTParam->pClientArray = m_connectedfd;
    pthread_create(&(m_pThreads[nThreadIndex].thread_tid), NULL, &thread_main, (void*)pTParam);
}

bool CGenaralSocket::InitSocketTCP()
{
    m_socketTcpServer = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&m_socketTcpServerAddr, sizeof(m_socketTcpServerAddr));
    m_socketTcpServerAddr.sin_family = AF_INET;
    m_socketTcpServerAddr.sin_addr.s_addr = htons(INADDR_ANY);
    m_socketTcpServerAddr.sin_port = htons(SERVER_PORT);
    bind(m_socketTcpServer, (Socketaddr*)&m_socketTcpServerAddr, sizeof(Socketaddr));
    listen(m_socketTcpServer, LISTENQ);
    Socketfd connectedfd;
    Socketaddr connectedaddr;
    pid_t   childpid;
    m_pThreads = new Thread[MAX_THREAD_POOL];
    m_iget = m_iput = 0;
    int addrlen = sizeof(Socketaddr);
    //预先创建指定数目的线程
    for(int i = 0; i < MAX_THREAD_POOL; i++)
    {
        Thread_make(i);
    }
    for(;;)
    {
        connectedfd = accept(m_socketTcpServer, (struct sockaddr*)&connectedaddr, &addrlen);
        pthread_mutex_lock(&m_clifd_mutex);
        m_connectedfd[m_iput] = connectedfd;
        if(++m_iput == MAX_CONNECTED_CLIENT)
            m_iput = 0;
        if(m_iput == m_iget)
            err_quit("m_iput = m_iget = %d", m_iput);
        pthread_cond_signal(&m_clifd_cond);
        pthread_mutex_unlock(&m_clifd_mutex);
    }
    return true;
}

bool  CGenaralSocket::ReceiveData_Tcp()
{
    return true;
}

bool CGenaralSocket::SendData_Tcp()
{
    return true;
}

