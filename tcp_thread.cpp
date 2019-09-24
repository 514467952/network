/*
 *基于封装的tcpsocket，实现tcp服务端程序
 1. 创建套接字
 2.为套接字绑定地址信息
 3.开始监听，如果有连接进来，自动完成三次握手
 while(1){
 4,从已完成连接队列，获取新建的客户端连接socket
 5.通过新建的客户端连接socket，与指定的客户端进行通信，recv
 6.send
 }
 7. 关闭套接字
 */

#include"tcpsocket.hpp"
#include<pthread.h>
void* thr_start(void *arg){
    TcpSocket *clisock = (TcpSocket *)arg;
    while(1){
        //因为线程之间，共享文件描述符表，因此在一个线程中打开的文件
        //另一个线程只要能够获取文件描述符，就能在操作文件
        std::string buf;
        clisock->Recv(buf);
        std::cout<<"client say:"<<buf<<std::endl;

        buf.clear();
        std::cout<<"server say:";
        fflush(stdout);
        std::cin>>buf;
        clisock->Send(buf);

    }
    clisock->Close();
    delete clisock;
    return NULL;
}

int main(int argc,char *argv[]){
    if(argc != 3){
        std::cout<<"./tcp_srv 192.168.145.132 9000\n";
        return -1;
    }
    std::string ip =argv[1];
    uint16_t port =atoi(argv[2]);

    TcpSocket sock;
    CHECK_RET(sock.Socket());
    CHECK_RET(sock.Bind(ip,port));
    CHECK_RET(sock.Listen());
    while(1){
        TcpSocket *clisock = new TcpSocket();
        std::string cliip;
        uint16_t cliport; 
        if(sock.Accept(*clisock,cliip,cliport) == false){    //当已完成的连接队列中没有socket，会阻塞
            continue;
        }
        std::cout<<"new client:"<<cliip<<":"<<cliport<<std::endl;

        pthread_t tid;
        pthread_create(&tid,NULL,thr_start,(void *)clisock);
        pthread_detach(tid);

    }
    sock.Close();
}
