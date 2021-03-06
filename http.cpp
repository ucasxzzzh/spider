#include <iostream>
#include <string>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>  
#include <unistd.h>  
#include <fcntl.h> 
#include <arpa/inet.h>
#include <sys/socket.h>
#include "http.h"

using namespace std;

CHttp::CHttp()
{
	//构造函数中新建套接字
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
}

CHttp::~CHttp()
{
	
}

bool CHttp::closesocket()
{
	//关闭套接字，对象参数清零
	close(m_socket);
	m_host.empty();
	m_object.empty();
	m_socket = 0;
	return true;
}

bool CHttp::AnalyseUrl(string url)
{
	//截取关键信息
	//http://www.quanshuwang.com/book_38304.html
	int len = 7;
	if(url.find("http://") == string::npos && url.find("https://") == string::npos)
		return false;
	if(url.find("https://") != string::npos)
		len = 8;
	if(url.size() <=7)
		return false;
	int pos = url.find('/',len);
	if(pos == string::npos)
	{
		m_host = url.substr(len);
		m_object = '/';
	}
	else
	{
		m_host = url.substr(len,pos-len);
		m_object = url.substr(pos);
	}
	cout<<"主机名 = "<<m_host<<endl;
	cout<<"资源路径 = "<<m_object<<endl;
	return true;
}

bool CHttp::Connect()
{
	//获取域名对应的ip地址
	hostent *p = gethostbyname(m_host.c_str());
	if(p == NULL)
		return false;
	string ip = inet_ntoa(*((in_addr*)p->h_addr));
	cout<<inet_ntoa(*((in_addr*)p->h_addr))<<endl;

	//网络编程必备
	sockaddr_in sa;
	sa.sin_family = AF_INET;
	sa.sin_port = htons(80);
	sa.sin_addr.s_addr = inet_addr(ip.c_str());
	if(connect(m_socket,(sockaddr*)&sa,sizeof(sa)) == -1)
	{
		cout<<"connect failed!!!"<<endl;
		return false;
	}

	return true;
}

bool CHttp::FetchHtml(string currentUrl,string &html)
{
	//拆分URL
	if(!AnalyseUrl(currentUrl))
	{
		cout<<"AnalyseUrl failed!!!"<<endl;
		return false;
	}
	//连接服务器
	if(!Connect())
	{
		cout<<"connect to host failed!!!"<<endl;
		return false;
	}
	if(!SendRequest())
	{
		cout<<"send request failed!!!"<<endl;
		return false;
	}
	
	//一个字节一个字节接收数据
	char ch;
	while(recv(m_socket,&ch,sizeof(ch),0))
	{
		html += ch;
	}

	return true;
}

bool CHttp::SendRequest()
{
	
	//模拟浏览器发送请求info
	/*
	GET https://blog.csdn.net/u012836896 HTTP/1.1
	Host: blog.csdn.net
	Connection: keep-alive
	Cache-Control: max-age=0
	Upgrade-Insecure-Requests: 1
	User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/83.0.4103.7 Safari/537.36
	*/
	string info;
	info = info + "GET " + m_object + " HTTP/1.1\r\n";
	info = info + "Host: " + m_host + "\r\n";
	info = info + "User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/83.0.4103.7 Safari/537.36\r\n";
	info = info + "Connection: Close\r\n";
	info += "\r\n";
	cout<<info;

	//发送GET请求
	send(m_socket,info.c_str(),info.size(),0);
	return true;
}

bool CHttp::Download(string url,string filename)
{
	AnalyseUrl(url);
	Connect();
	SendRequest();
	
	//打开指定文件
	FILE *fp = fopen(filename.c_str(),"wb");
	if(NULL == fp)
	{
		cout<<"fp error"<<endl;
		return false;
	}

	//删除HTTP请求回复的头部分
	char ch = 0;
	while(recv(m_socket,&ch,sizeof(ch),0))
	{
		if(ch == '\r')
		{
			recv(m_socket,&ch,sizeof(ch),0);
			if(ch == '\n')
			{
				recv(m_socket,&ch,sizeof(ch),0);
				if(ch == '\r')
				{
					recv(m_socket,&ch,sizeof(ch),0);
					if(ch == '\n')
					{
						break;
					}
				}
			}
		}

	}

	//一字节一字节写入文件
	while(recv(m_socket,&ch,sizeof(ch),0))
	{
		fwrite(&ch,1,1,fp);
	}

	fclose(fp);

	return true;
}