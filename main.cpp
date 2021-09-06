#include<string>
#include<bitset>
#include<vector>
#include<iostream>
#include<WinSock2.h>
#include<fstream>
#include <vector>
#include "smtp.h"
#include "imap.h"
using namespace std;
#pragma comment(lib, "ws2_32.lib")

#define accounnt "1375134582@qq.com" 
#define passWord "fkrbphbbehsijejd"  
#define toAddress "wsp4531@sina.com"
#define sever "smtp.163.com"


int main() {
	/*smtp clienct;
	clienct.connectToSever("smtp.163.com");
	clienct.authLogin(accounnt, passWord);
	clienct.sendMailFromTo(accounnt, vector<string>(1,toAddress));
	clienct.setSubject("this is a 测试邮件");
//	clienct.sendTextContents(0, "ksfldj四大皆空帆帆sdkf!@$#@%$^");
	clienct.setMultipartHeaders("multipart/mixed");
	clienct.addMultipartContentType("multipart/alternative");
	clienct.addTextPlain("测试");
	clienct.rset();
	clienct.addAttchment("C:\\Users\\wushipeng\\Pictures\\2.jpg");
	clienct.addAttchment("D:\\flutter\\maildemo\\docs\\正则表达式.doc");
	clienct.sendMultipart();
	clienct.quit();*/
	imap client;
	client.connectToSever("imap.qq.com");
	client.login(accounnt, passWord);
	vector<string> floadList=client.list("\"\"", "\"*\"");
	client.select(floadList[1]);
	vector<int> searchSubject = client.searchFromSubject("Steam");
	client.moveMailTo(searchSubject, floadList[3]);
	//client.select(floadList[1]);
	//client.searchFromBody("额");
	//client.getuid("180:188");
	//client.getHeaders(185);
	//client.getBody(185);
	//client.getAttchmentName(185);
	//client.append(floadList[3],testContent,10);
	//client.select(floadList[2]);
	//client.store("1", '+', "\\Deleted");
	//client.expunge();
	//client.subscribe(floadList[1]);
	//client.lsub("","*");
	//client.copy(1, floadList[3]);
	client.close();
	client.logout();
	return 0;
}
