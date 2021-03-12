#include"json.hpp"
using json = nlohmann::json;

#include<iostream>
#include<string>
#include<vector>
#include<map>
using namespace std;

//json序列化示例1
string fun1(){
    json js;
    js["msg_type"] = 2;
    js["from"] = "cuihua";
    js["to"] = "tiezhu";
    js["msg"] = "oh~ my love~ my dearling~";

    string sendbuf = js.dump();//dump()方法序列化数据,即把json对象序列化为json字符串
    //cout<<sendbuf.c_str()<<endl;
    return sendbuf;//返回了经过json序列化后的json字符串
}

//json序列化示例2
string fun2(){
    json js;
    js["id"] = {1,2,3,4,5};
    js["name"] = "zhangsan";
    js["msg"]["tiezhu"] = "你好啊";
    js["msg"]["cuihua"] = "你好啊";
    js["msg"] = {{"tiezhu","多喝热水"},{"cuihua","？"}};
    //cout<<js<<endl;
    return js.dump();
}

//json序列化示例3
//json序列化示例2
string fun3(){
    json js;

    vector<int>vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(5);
    js["list"] = vec;

    map<int,string> m;
    m.insert({1,"泰山"});
    m.insert({2,"华山"});
    m.insert({3,"黄山"});
    js["path"] = m;

    string sendbuf = js.dump();

    //cout<<sendbuf<<endl;
    return sendbuf;
}

int main()
{
    string recvebuf = fun3();//接收到json序列化的数据(json字符串)
    cout<<recvebuf<<endl;
    //数据的反序列化 =》json字符串 =》json数据对象(看作容器，方便访问)
    json jsbuf = json::parse(recvebuf);
    //fun3()的反序列化
    //cout<<jsbuf["list"]<<endl;
    vector<int>vec = jsbuf["list"];
    for(int &it:vec)
    {
        cout<<it<<",";
    }
    cout<<endl;
    //cout<<jsbuf["path"]<<endl;
    map<int,string>m = jsbuf["path"];
    for(auto &it:m)
    {
        cout<<it.first<<","<<it.second<<" ";
    }
    cout<<endl;
    //fun2（）的反序列化
    //cout<<jsbuf["id"]<<endl;
    //cout<<jsbuf["name"]<<endl;
    //cout<<jsbuf["msg"]<<endl;
    //auto msgjs = jsbuf["msg"];
    //cout<<msgjs["tiezhu"]<<","<<msgjs["cuihua"]<<endl;

    //fun1()的反序列化
    //cout<<jsbuf["msg_type"]<<endl;
    //cout<<jsbuf["from"]<<endl;
    //cout<<jsbuf["to"]<<endl;
    //cout<<jsbuf["msg"]<<endl;
    return 0;
}