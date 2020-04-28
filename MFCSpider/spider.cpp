#include "stdafx.h"
#include "MFCSpider.h"
#include "MFCSpiderDlg.h"
#include "afxdialogex.h"
  
#pragma comment(lib, "ws2_32.lib")   
using namespace std;  
  
#define _WIN32_WINNT    0x0500
#define DEFAULT_PAGE_BUF_SIZE 1048576
  
queue<string> hrefUrl;
unordered_set<string> visitedUrl;
unordered_set<string> visitedImg;
int depth=0;
extern CEdit* pEdit;
extern int nLength;
extern int flag;
extern string murl;
extern string mpath;
extern int mtype; 

void display(const char* str)
{
	nLength = pEdit->GetWindowTextLength();
    pEdit->SetSel(nLength, nLength);
    pEdit->ReplaceSel(str);
}
bool ParseURL( const string & url, string & host, string & resource){  
    if ( strlen(url.c_str()) > 2000 ) {  
        return false;  
    }  
    string tpos;
    const char * pos = strstr( url.c_str(), "http://" );  
    if( pos==NULL )
	{
		pos = strstr( url.c_str(), "https://" );  
		if( pos==NULL ) pos = url.c_str();  
		else pos += strlen("https://");  
	}
    else pos += strlen("http://");  
	while(pos[0]=='/')pos++;
    if( strstr( pos, "/")==NULL )
	{
        tpos=pos;tpos=tpos+"/";
		pos=tpos.c_str();
	}
    char pHost[100];  
    char pResource[2000];  
    sscanf( pos, "%[^/]%s", pHost, pResource );  
    host = pHost;  
    resource = pResource;  
    return true;  
}  
bool GetHttpResponse( const string & url, char * &response, int &bytesRead ){  
    string host, resource;  
    if(!ParseURL( url, host, resource )){  
		display("无法解析的URL：");display(url.c_str());display("\n"); 
        return false;  
    }  
    struct hostent * hp= gethostbyname( host.c_str() );  
    if( hp==NULL ){  
		display("无法得到主机信息：");display(host.c_str());display("\n");
        return false;  
    }  
  
    SOCKET sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP);  
    if( sock == -1 || sock == -2 ){  
		display("无法创建SOCKET\n");
        return false;  
    }  
    SOCKADDR_IN sa;  
    sa.sin_family = AF_INET;  
    sa.sin_port = htons( 80 );  
    memcpy( &sa.sin_addr, hp->h_addr, 4 );  
    if( 0!= connect( sock, (SOCKADDR*)&sa, sizeof(sa) ) ){  
		display("无法连接：");display(url.c_str());display("\n");
        closesocket(sock);  
        return false;  
    };  
    string request = "GET " + resource + " HTTP/1.1\r\nHost:" + host + "\r\nConnection:Close\r\n\r\n";  
    if( SOCKET_ERROR ==send( sock, request.c_str(), request.size(), 0 ) ){  
        display("发送HTTP请求失败\n");  
        closesocket( sock );  
        return false;  
    }   
    int m_nContentLength = DEFAULT_PAGE_BUF_SIZE;  
    char *pageBuf = (char *)malloc(m_nContentLength);  
    memset(pageBuf, 0, m_nContentLength);  
    bytesRead = 0;  
    int ret = 1;  
    while(ret > 0){  
        ret = recv(sock, pageBuf + bytesRead, m_nContentLength - bytesRead, 0);  
          
        if(ret > 0)  
        {  
            bytesRead += ret;  
        }  
  
        if( m_nContentLength - bytesRead<100){  
            m_nContentLength *=2;  
            pageBuf = (char*)realloc( pageBuf, m_nContentLength);
        }  
    }  
    pageBuf[bytesRead] = '\0';  
    response = pageBuf;  
    closesocket( sock );  
    return true;  
}  
void HTMLParse ( string & htmlResponse, vector<string> & imgurls ){  
	display("html解析开始\n");
	int urlnum=0,imgurlnum=0;
    const char *p= htmlResponse.c_str();  
    char *tag="href=\"";  
    const char *pos = strstr( p, tag );  
    ofstream ofile(mpath+"/url.txt", ios::app);  
    while( pos ){  
        pos +=strlen(tag);  
        const char * nextQ = strstr( pos, "\"" );  
        if( nextQ ){  
            char * url = new char[ nextQ-pos+1 ];  
            sscanf( pos, "%[^\"]", url);  
            string surl = url;
            if( visitedUrl.find( surl ) == visitedUrl.end() ){  
                visitedUrl.insert( surl );  
                ofile << surl<<endl;  
                hrefUrl.push( surl ); 
				urlnum++;
            }  
            pos = strstr(pos, tag );  
            delete [] url;
        }  
    }  
    ofile << endl << endl;  
    ofile.close();  
  
    tag ="<img ";  
    const char* att1= "src=\"";  
    const char* att2="lazy-src=\"";  
    const char *pos0 = strstr( p, tag );  
    while( pos0 ){  
        pos0 += strlen( tag );  
        const char* pos2 = strstr( pos0, att2 );  
        if( !pos2 || pos2 > strstr( pos0, ">") ) 
			{  
				pos = strstr( pos0, att1);  
				if(!pos){pos0 = strstr(pos0, tag );continue;} 
				else {pos = pos + strlen(att1);}  
			}  
        else
			{  
				pos = pos2 + strlen(att2);  
			}  
        const char * nextQ = strstr( pos, "\"");  
        if( nextQ )
			{  
				char * url = new char[nextQ-pos+1];  
				sscanf( pos, "%[^\"]", url);  
				string imgUrl = url;  
				if( visitedImg.find( imgUrl ) == visitedImg.end() )
					{  
						visitedImg.insert( imgUrl );  
						imgurls.push_back( imgUrl );
						imgurlnum++;
					}  
				pos0 = strstr(pos0, tag );  
				delete [] url;  
			}  
    }
	char surlnum[10],simgurlnum[10];
	itoa(urlnum,surlnum,10);
	itoa(imgurlnum,simgurlnum,10);
	display("链接数：");display(surlnum);display("\n"); 
	display("图片数：");display(simgurlnum);display("\n"); 
	display("html解析结束\n");
}  
string ToFileName( const string &url ){
    string fileName;  
    fileName.resize( url.size());  
    int k=0,p=0;  
    for( int i=0; i<(int)url.size(); i++){  
        char ch = url[i];  
        if( ch!='\\'&&ch!='/'&&ch!=':'&&ch!='*'&&ch!='?'&&ch!='"'&&ch!='<'&&ch!='>'&&ch!='|')  
            fileName[k++]=ch;  
		else fileName[k++]='_';
    }
	while(fileName[0]=='_')fileName.erase(0,1);
	if(p=fileName.find("http___")!=string::npos)fileName.erase(p-1,7);
	if(p=fileName.find("https___")!=string::npos)fileName.erase(p-1,8);
    return fileName.substr(0,fileName.size()) + ".html";  
}  
void DownLoadImg( vector<string> & imgurls, const string &url ){  
    string htmlname = ToFileName( url );  
    htmlname = mpath+"/img/"+htmlname;  
    if(!CreateDirectory( htmlname.c_str(),NULL ))  
	display("文件夹无法创建或已存在：");display(htmlname.c_str());display("\n"); 
    char *image;  
    int byteRead,imgnum=0;
    for( int i=0; i<imgurls.size(); i++)
		{  
			string str = imgurls[i];  
			int pos = str.find_last_of(".");  
			if( pos == string::npos )  continue;  
			else
				{  
					string ext = str.substr( pos+1, str.size()-pos-1 );  
					if( ext!="bmp"&& ext!="jpg" && ext!="jpeg"&& ext!="gif"&&ext!="png")  
						continue;  
				}  
			if( GetHttpResponse(imgurls[i], image, byteRead))
				{  
					if ( strlen(image) ==0 )continue;
					const char *p=image;  
					const char * pos = strstr(p,"\r\n\r\n")+strlen("\r\n\r\n");  
					int index = imgurls[i].find_last_of("/");  
					if( index!=string::npos )
						{  
							string imgname = imgurls[i].substr( index , imgurls[i].size() );  
							ofstream ofile( htmlname+imgname, ios::binary );  
							if( !ofile.is_open() )  continue;   
							if(ofile.write(pos, byteRead - (pos - p))){
								char simgnum[10];
								itoa(++imgnum,simgnum,10);
								display("成功抓取第");display(simgnum);display("张图片：");display(imgname.c_str());display("\n"); 
							}
							ofile.close();  
						}  
					free(image);  
				} 
			if(!flag)break;
		}
	if(!imgnum)RemoveDirectory(htmlname.c_str());
}  
void BFS( const string & url ){  
    char * response;  
    int bytes;   
    if( !GetHttpResponse( url, response, bytes ) ){   
		display("无效的URL：");display(url.c_str());display("\n"); 
        return;  
    }  
    string httpResponse=response;  
    free( response );
	vector<string> imgurls;  
    HTMLParse( httpResponse,  imgurls);  
	if(mtype==1||mtype==3){
		string filename = ToFileName( url );
		ofstream ofile( mpath+"/html/"+filename );  
		if( ofile.is_open() ){  
 			ofile << httpResponse << endl;  
			ofile.close();  }  
	}
	if(mtype==2||mtype==3){DownLoadImg( imgurls, url ); }
}    
UINT Fun(LPVOID pParam)  
{  
    WSADATA wsaData;  
    if( WSAStartup(MAKEWORD(2,2), &wsaData) != 0 ){  
        display("初始化失败\n");return 0;
    }

	if(mtype==1||mtype==3){
		if(CreateDirectory((mpath+"/html").c_str(),0))display("文件夹html创建成功\n") ;
		else if(GetLastError()==183){display("文件夹html已存在，继续可能覆盖文件\n");}
		else {display("保存路径无效\n");return 0;}
	}
	if(mtype==2||mtype==3){
		if(CreateDirectory((mpath+"/img").c_str(),0))display("文件夹img创建成功\n");
		else if(GetLastError()==183){display("文件夹img已存在，继续可能覆盖文件\n");}
		else {display("保存路径无效\n");return 0;}
	}

	display("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	display("开始抓取：");display(murl.c_str());display("\n"); 
    BFS( murl );  
    visitedUrl.insert( murl );  
	display("结束抓取：");display(murl.c_str());display("\n"); 
	display("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");
    while( hrefUrl.size()!=0 && flag){  
        string url = hrefUrl.front();
		display("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
        display("开始抓取：");display(url.c_str());display("\n");   
        BFS( url );
        hrefUrl.pop();
		display("结束抓取：");display(url.c_str());display("\n"); 
		display("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");
		if(int i=pEdit->GetLineCount()>=200)
			pEdit->SetWindowTextA(_T(""));
    }  
    WSACleanup();
	display("爬取结束\n");
	return 0;
}  