#include<iostream>
#include<GenaralSocket.h>



int main()
{
    CGenaralSocket::GetInstance()->InitSocketTcp();
    return 0;
}
