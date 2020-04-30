#include<iostream>
#include<string>
#include<queue>
#include<regex>
#include<vector>
#include<unistd.h>
#include<set>
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
		//抓取html源代码
		string html;
		http.FetchHtml(currentUrl,html);
		// cout<<html<<endl;
		http.closesocket();

		//编码转换
		

		//解析网页内容
		vector<string> vecImage;
		smatch sm;
		regex re("http://[^\\s'\"<>(){}]+");
		string::const_iterator start = html.begin();
		string::const_iterator end = html.end();
		while(regex_search(start,end,sm,re))
		{
			string tmp = sm[0];
			if(tmp.find(".jpg") != -1 || tmp.find(".png") != -1 
			|| tmp.find(".jpeg") != -1)
			{
				vecImage.push_back(tmp);
			}
			else
			{
				//去除w3c
				if(tmp.find("http://www.w3.org/") == -1)
					q.push(tmp);
			}
			//cout<<tmp<<endl;
			start = sm[0].second;
		}

    	sort(vecImage.begin(),vecImage.end());
 	    auto it = unique(vecImage.begin(), vecImage.end());
	    vecImage.erase(it, vecImage.end());

		for(auto iter=vecImage.begin();iter!=vecImage.end();iter++)
		{
			cout<<*iter<<endl;
		}
		for(auto iter=vecImage.begin();iter!=vecImage.end();iter++)
		{
			CHttp httpDownload;
			char buf[256] = {0};
    		getcwd(buf, 256);
			string currentPath = buf;
			cout<<currentPath<<endl;
			string filename = currentPath + "/img/" 
			+ (*iter).substr((*iter).find_last_of('/')+1);
			if(!httpDownload.Download(*iter,filename))
			{
				cout<<"download failed!!!"<<endl;
			}
		}

	
	}
	return true;
}