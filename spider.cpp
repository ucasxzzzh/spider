#include<iostream>
#include<string>
#include<queue>
#include<regex.h>
#include "http.h"

using namespace std;

//开抓
bool StartCatch(string url);

int main() {

	cout << "Input URL：";
	string url;
	cin >> url;
	
	StartCatch(url);

	return 0;
}

bool StartCatch(string url) 
{
	queue<string> q;
	q.push(url);
	while (!q.empty()) 
	{
		//取出队列头url
		string currentUrl = q.front();
		q.pop();
		
		
		CHttp http;
		//网络初始化，新建套接字
		if(!http.Init())
		{
			cout<<"network init failed!!!"<<endl;
			return false;
		}
		//解析url，分解域名和资源
		if(!http.AnalyseUrl(currentUrl))
		{
			cout<<"AnalyseUrl failed!!!"<<endl;
			return false;
		}
		//连接服务器
		if(!http.Connect())
		{
			cout<<"connect to host failed!!!"<<endl;
			return false;
		}
		//抓取html源代码
		string html;
		if(!http.FetchHtml(html))
		{
			cout<<"fetch html failed!!!"<<endl;
			return false;
		}
		cout<<html<<endl;

		//解析网页内容

	
	}
	return true;
}