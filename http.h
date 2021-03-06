
#ifndef __HTTP_H
#define __HTTP_H

#include<string>
using namespace std;

class CHttp
{

private:
	string m_host;
	string m_object;
	int m_socket;
public:
	CHttp();
	~CHttp();
	bool AnalyseUrl(string url);
	bool Connect();
	bool FetchHtml(string url,string &html);
	bool SendRequest();
	bool Download(string url,string filename);
	bool closesocket();
};

#endif