#include<iostream>
#include<string>
#include<queue>
#include<regex>
#include<vector>
#include<unistd.h>
#include<set>
#include<sys/stat.h>
#include<sys/types.h>
#include "http.h"

using namespace std;

//抓取函数
bool StartCatch(string url);

//入口函数
int main() {

	cout << "Input URL：";
	string url;
	cin >> url;
	
	StartCatch(url);

	return 0;
}

bool StartCatch(string url) 
{
	//定义队列用于存放待访问的URL
	queue<string> q;
	q.push(url);
	//只要队列不空就一直执行
	while (!q.empty()) 
	{
		//取出队列头URL
		string currentUrl = q.front();
		q.pop();
		
		CHttp http;		
		//抓取html源代码
		string html;
		http.FetchHtml(currentUrl,html);
		// cout<<html<<endl;
		http.closesocket();		

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
				// //去除w3c
				// if(tmp.find("http://www.w3.org/") == -1)
				// 	q.push(tmp);
			}
			//cout<<tmp<<endl;
			start = sm[0].second;
		}

		//vector中元素去重
    	sort(vecImage.begin(),vecImage.end());
 	    auto it = unique(vecImage.begin(), vecImage.end());
	    vecImage.erase(it, vecImage.end());

		//输出所有带下载图片的链接
		for(auto iter=vecImage.begin();iter!=vecImage.end();iter++)
		{
			cout<<*iter<<endl;
		}
		for(auto iter=vecImage.begin();iter!=vecImage.end();iter++)
		{
			CHttp httpDownload;
			
			//求当前路径
			char buf[256] = {0};
    		getcwd(buf, 256);
			string currentPath = buf;

			//新建文件夹img
			int isCreate = mkdir((currentPath+"/img/").c_str(),S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);
			if(-1 == isCreate)
			{
				cout<<"new folder error!!!"<<endl;
			}
			//下载文件
			string filename = currentPath + "/img/" + (*iter).substr((*iter).find_last_of('/')+1);
			if(!httpDownload.Download(*iter,filename))
			{
				cout<<"download failed!!!"<<endl;
			}
		}

	}
	return true;
}