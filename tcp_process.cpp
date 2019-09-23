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
#include<signal.h>
#include"tcpsocket.hpp"
#include<sys/wait.h>

void sigcb(int no){
    while(waitpid(-1,NULL,WNOHANG) > 0);
}

int main(int argc,char *argv[]){
    if(argc != 3){
        std::cout<<"./tcp_srv 192.168.145.132 9000\n";
        return -1;
    }
    std::string ip =argv[1];
    uint16_t port =atoi(argv[2]);
    

    signal(SIGCHLD,sigcb);

    TcpSocket sock;
    CHECK_RET(sock.Socket());
    CHECK_RET(sock.Bind(ip,port));
    CHECK_RET(sock.Listen());
    while(1){
        TcpSocket clisock;
        std::string cliip;
        uint16_t cliport; 
        if(sock.Accept(clisock,cliip,cliport) == false){    //当已完成的连接队列中没有socket，会阻塞
            continue;
        }
        std::cout<<"new client:"<<cliip<<":"<<cliport<<std::endl;

        int pid =fork();
        if(pid == 0){
            //子进程专门处理每个客户端的通信
            while(1){
                std::string buf;
                clisock.Recv(buf);
                std::cout<<"client say:"<<buf<<std::endl;

                buf.clear();
                std::cout<<"server say:";
                fflush(stdout);
                std::cin>>buf;
                clisock.Send(buf);
            }
            clisock.Close();
            exit(0);
        }
        //父进程关闭套接字，因为父子进程数据独有
        //不关闭，会造成资源泄露
        clisock.Close();//父进程不通信
    }
    sock.Close();
}
