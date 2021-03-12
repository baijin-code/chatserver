
/*
muduo网络库给用户提供的两个主要的类
1.TCPServer
2.TCPClient

就是epool+线程池
好处：把网络I/O代码和业务代码区分开来
                   用户的连接断开  用户的可读写事件
*/
#include<muduo/net/TcpServer.h>
#include<muduo/net/EventLoop.h>
#include<iostream>
#include<functional>
#include<string>

using namespace std;
using namespace muduo;
using namespace muduo::net;
using namespace placeholders;
/*基于muduo网络库开发服务器程序
1.组合TCPServer对象
2.创建eventloop事件循环对象的指针
3.明确TCPServer构造函数需要什么参数，输出ChatServer的构造函数
4.在当前服务器构造函数中，注册处理链接的回调函数和处理读写事件的回调函数
5.设置合适的线程数量，服务器会自动分配I/O线程和worker线程
*/
class ChatServer
{
public:
    ChatServer(EventLoop* loop,         //事件循环
            const InetAddress& listenAddr,    //IP+端口号
            const string& nameArg)           //服务器名称
            :_server(loop,listenAddr,nameArg),_loop(loop)
    {
        //给服务器提供用户连接的创建和断开回调
         _server.setConnectionCallback(std::bind(&ChatServer::onConnection,this,_1));
        //给服务器提供用户可读写事件的回调
        _server.setMessageCallback(std::bind(&ChatServer::onMessage,this,_1,_2,_3));
        //设置服务器线程数量，设置I/O线程1个，worker线程3个
        _server.setThreadNum(4);
    }
    //开启事件循环
    void strat()
    {
        _server.start();
    }

private:
//专门处理用户的连接创建断开
    void onConnection(const TcpConnectionPtr&conn){
        if(conn->connected())
        {
            cout<<conn->peerAddress().toIpPort()<<" -> "<<conn->localAddress().toIpPort()<<" state:online"<<endl;
        }
        else{
            cout<<conn->peerAddress().toIpPort()<<" -> "<<conn->localAddress().toIpPort()<<" state:offline"<<endl;
            conn->shutdown(); //close(fd)
            //loop->qiut();
        }
        
    }

    //专门处理用户的读写事件
    void onMessage(const TcpConnectionPtr& conn,  //表示连接
                    Buffer* buffer,    //缓冲区
                    Timestamp time)   //接受收到数据的时间信息
    {
        string buf = buffer->retrieveAllAsString();
        cout<<"recv data"<<buf<<"time"<<time.toString();
        conn->send(buf);
    }
    TcpServer _server;   //1.
    EventLoop *_loop;   //2  相当于epool
};

int main()
{
    EventLoop loop;
    InetAddress addr("127.0.0.1",6000);
    ChatServer server(&loop,addr,"No1.ChatServer");

    server.strat(); //listenfd  epool_ctl epool
    loop.loop();  //epool_wait以阻塞的方式等待新用户连接，已连接用户的读写事件
}
