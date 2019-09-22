/*
 *基于封装的Tcpsocket实现tco客户端程序
 1.创建套接字
 2.为套接字绑定地址信息(不推荐用户手动绑定)
 while(1){
4.发送数据
5.接收数据
 }
 6.关闭套接字
 */

#include"tcpsocket.hpp"

int main(int argc,char *argv[]){

    if(argc != 3){
        std::cout<<"./tco_cli 192.168.145.132 9000\n";
        return -1;
    }
    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);

    TcpSocket sock;

    CHECK_RET(sock.Socket());
    CHECK_RET(sock.Connect(ip,port));

    while(1){
        std::string buf;
        std::cout<<"client say:";
        fflush(stdout);
        std::cin >> buf;
        sock.Send(buf);

        buf.clear();
        sock.Recv(buf);
        std::cout<<"server say:"<<buf<<std::endl;
    }
    sock.Close();
    return 0;
}
