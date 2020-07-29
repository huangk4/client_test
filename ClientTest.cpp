// ClientTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include<string>
#include<fstream>
#include<random>
#include<thread>
#include<ctime>
#include<sstream>
#include"pch.h"
#include"ClientTest.h"
#include"MD5.h"

using namespace std;


void InitWSA()
{
	WORD w_req = MAKEWORD(2, 2);//版本号
	WSADATA wsadata;
	int err;
	err = WSAStartup(w_req, &wsadata);
	if (err != 0) {
		cout << "初始化套接字库失败！" << endl;
	}
	else {
		cout << "初始化套接字库成功！" << endl;
	}
	//检测版本号
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2) {
		cout << "套接字库版本号不符！" << endl;
		WSACleanup();
	}
	else {
		cout << "套接字库版本正确！" << endl;
	}
	//填充服务端地址信息
}

string g_savepath="E:\\files\\本地文件\\test\\client\\";


void SaveFile(int connfd,size_t pos=0)
{
	Sleep(3000);
	const int n=65535;
	int times=time(0);
	srand(size_t(time(0)));
	int a=rand()%1000;
	stringstream ss;
	ss<<a;
	string temp;
	ss>>temp;
	g_savepath+=temp+"test.png";
	ofstream saveF(g_savepath, ios::app | ios::binary);
	if (saveF.fail())
	{
		std::cout << "打开文件错误\n";
		std::cout << GetLastError();
	}
	std::unique_ptr<char> recvline(new char[n]);
	while (true)
	{
		memset(recvline.get(), 0, n);
		int recvlen = recv(connfd, recvline.get(), n, 0);
		if (recvlen < 0)
		{
			std::cout << "接受服务端信息错误\n";
			break;
		}
		if (recvlen == 0)
			break;
		saveF.write(recvline.get(), recvlen);
	}
	saveF.close();
}




void conn(int info=0)
{
	cout<<"线程"+to_string(info)+":\n";
	//定义服务端套接字，接受请求套接字
	SOCKET s_server;
	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	inet_pton(AF_INET,"127.0.0.1",&server_addr.sin_addr.S_un.S_addr);
	server_addr.sin_port = htons(1333);
	s_server = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(s_server, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		cout << "服务器连接失败！" << endl;
		WSACleanup();
	}
	else
	{
		cout << "服务器连接成功！" << endl;
	}
	int send_len = 0;
	int recv_len = 0;
	//定义发送缓冲区和接受缓冲区
	const int max = 10000;//发送和接收缓冲区大小
	char send_buf[max];
	char recv_buf[max];
	string senddata;
	string path="E:\\files\\本地文件\\picture\\favor\\3ye.png";
	string path2="E:\\files\\本地文件\\test\\chash.txt";
	ifstream mf(path,ios::binary|ios::in);
	
	MD5 md5(mf);
	string hash=md5.toString();
	ofstream hashf(path2,ios::out);
	hashf<<hash;
	hashf.close();



	memset(send_buf,0,max);
	cout << "请输入发送信息:";
	//cin >> senddata;
	senddata="g9f9bb0f6365d23e6d82e76ce9c5cf686";
	int sendlen= senddata.length();
	strcpy_s(send_buf,max,senddata.c_str());
	send_len = send(s_server, send_buf, sendlen, 0);
	if (send_len < 0) {
		cout << "发送失败！" << endl;
	}
	SaveFile(s_server);

	








	//if (mf.fail())
	//{
	//	cout<<"文件打开错误\n";
	//	cout<<GetLastError();
	//}
	//filebuf *pbuf=mf.rdbuf();
	//int size=pbuf->pubseekoff(0,ios_base::end,ios_base::in);
	//pbuf->pubseekpos(0,ios_base::in);
	//char *buf=new char[size];
	//pbuf->sgetn(buf,size);
	//mf.close();





	//send_len = send(s_server, buf, size, 0);
	//if (send_len < 0) {
	//	cout << "发送失败！" << endl;
	//}

	//while (true)
	//{

	//	//memset(send_buf,0,max);
	//	//cout << "请输入发送信息:";
	//	//cin >> senddata;
	//	//int sendlen= senddata.length();
	//	//strcpy_s(send_buf,max,senddata.c_str());
	//	//send_len = send(s_server, send_buf, sendlen, 0);
	//	//if (send_len < 0) {
	//	//	cout << "发送失败！" << endl;
	//	//	break;
	//	//}
	//	//memset(recv_buf,0,100);
	//	//recv_len = recv(s_server, recv_buf, max, 0);
	//	//if (recv_len < 0) {
	//	//	cout << "接受失败！" << endl;
	//	//	break;
	//	//}
	//	//else {
	//	//	cout << "服务端信息:" << recv_buf << endl;

	//	//}
	//	break;
	//}
	closesocket(s_server);

}


int main()
{
	int a=1;
	cout<<sizeof(a);
	InitWSA();
	const int tnum=2;
	thread t[tnum];
	for (int i = 0; i < tnum; ++i)
	{
		t[i]=thread(conn,i);
	}
	for (int i = 0; i < tnum; ++i)
	{
		t[i].join();
	}
	WSACleanup();
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
