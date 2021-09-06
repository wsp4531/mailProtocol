#include "imap.h"

imap::imap()
{
	WSADATA wsaData;
	WORD wVersionRequest = MAKEWORD(2, 1);
	WSAStartup(wVersionRequest,&wsaData);
	client = socket(AF_INET, SOCK_STREAM, 0);
}

void imap::connectToSever(string severName)
{
	HOSTENT* pHostent;//hostent是host entry的缩写，该结构记录主机的信息，包括主机名、别名、地址类型、地址长度和地址列表
	pHostent = gethostbyname(severName.c_str());  //链接到服务器
	SOCKADDR_IN addSever;
	addSever.sin_addr.S_un.S_addr = *((DWORD*)pHostent->h_addr_list[0]); //得到smtp服务器的网络字节序的ip地址
	addSever.sin_family = AF_INET;
	addSever.sin_port = htons(imapport);  //链接端口
	connect(client, (SOCKADDR*)&addSever, sizeof(SOCKADDR));
	buff[recv(client, buff, 500, 0)] = '\0';
	cout << "S:"<<buff << endl;
}

void imap::login(string user, string passWord)
{
	memset(buff, 0, sizeof(buff));
	string msg = "A" + to_string(msgId)+ " login " + user + " " + passWord + "\r\n";
	send(client, msg.c_str(), msg.length(), NULL);
	buff[recv(client, buff, 500, 0)] = '\0';
	cout << "C:" << msg << endl;
	cout << "S:" << buff << endl;
	msgId++;
}

vector<string> imap::list(string base, string temp)
{
	memset(buff, 0, sizeof(buff));
	vector<string> res;
	string msg = "A"+to_string(msgId)+" list "+base +" "+ temp + "\r\n";
	send(client, msg.c_str(), msg.length(), NULL);
	buff[recv(client, buff, 500, 0)] = '\0';
	cout << "C:" << msg << endl;
	cout << "S:" << buff << endl;
	vector<string> splitRes = splitStr(buff, "*");
	for (int i = 0; i < splitRes.size(); ++i) {
		if(splitRes[i].empty()) continue;
		else {
			vector<string> pathSplit = splitStr(splitRes[i], "\"");
			res.push_back(*(pathSplit.end() - 2));
		}
	}
	msgId++;
	return res;
}

void imap::append(string floadName, string mimmessage, int lineCount)
{
	memset(buff, 0, sizeof(buff));
	string msg = "A" + to_string(msgId) + " append " + floadName + " {" + to_string(lineCount) + "}\r\n";
	send(client, msg.c_str(), msg.length(), NULL);
	buff[recv(client, buff, 500, 0)] = '\0';
	cout << "C:" << msg << endl;
	cout << "S:" << buff << endl;
	msg = mimmessage + "\r\n";
	send(client, msg.c_str(), msg.length(), NULL);
	buff[recv(client, buff, 500, 0)] = '\0';
	cout << "C:" << msg << endl;
	cout << "S:" << buff << endl;
	msgId++;
}

void imap::createFolder(string path)
{
	memset(buff, 0, sizeof(buff));
	string msg = "A" + to_string(msgId) + " create " + path + "\r\n";
	send(client, msg.c_str(), msg.length(), NULL);
	buff[recv(client, buff, 500, 0)] = '\0';
	cout << "C:" << msg << endl;
	cout << "S:" << buff << endl;
	msgId++;
}

void imap::delteFolder(string folderName)
{
	memset(buff, 0, sizeof(buff));
	string msg = "A" + to_string(msgId) + " delete " + folderName + "\r\n";
	send(client, msg.c_str(), msg.length(), NULL);
	buff[recv(client, buff, 500, 0)] = '\0';
	cout << "C:" << msg << endl;
	cout << "S:" << buff << endl;
	msgId++;
}

void imap::renameFolder(string oldName, string newName)
{
	memset(buff, 0, sizeof(buff));
	string msg = "A" + to_string(msgId) + " rename " + oldName +" "+newName+"\r\n";
	send(client, msg.c_str(), msg.length(), NULL);
	buff[recv(client, buff, 500, 0)] = '\0';
	cout << "C:" << msg << endl;
	cout << "S:" << buff << endl;
	msgId++;
}

void imap::select(string folderName)
{
	memset(buff, 0, sizeof(buff));
	string msg = "A" + to_string(msgId) + " select \""+ folderName+"\"\r\n";
	send(client, msg.c_str(), msg.length(), NULL);
	buff[recv(client, buff, 500, 0)] = '\0';
	cout << "C:" << msg << endl;
	cout << "S:" << buff << endl;
	msgId++;
}

void imap::store(vector<int> id, char active, string flag)
{
	memset(buff, 0, sizeof(buff));
	for (int i = 0; i < id.size(); ++i) {
		string msg = "A" + to_string(msgId) + " store " + to_string(id[i]) + " " + active + "flags (" + flag + ")\r\n";
		send(client, msg.c_str(), msg.length(), NULL);
		buff[recv(client, buff, 500, 0)] = '\0';
	}
	cout << "C:" << "A" + to_string(msgId) + " store " << endl;
	cout << "S:" << buff << endl;
	msgId++;
}

void imap::subscribe(string floadName)
{
	memset(buff, 0, sizeof(buff));
	string msg = "A" + to_string(msgId) + " subscribe \""+floadName+"\"\r\n";
	send(client, msg.c_str(), msg.length(), NULL);
	buff[recv(client, buff, 500, 0)] = '\0';
	cout << "C:" << msg << endl;
	cout << "S:" << buff << endl;
	msgId++;
}

void imap::unsubscribe(string floadName)
{
	memset(buff, 0, sizeof(buff));
	string msg = "A" + to_string(msgId) + " unsubscribe \"" + floadName + "\"\r\n";
	send(client, msg.c_str(), msg.length(), NULL);
	buff[recv(client, buff, 500, 0)] = '\0';
	cout << "C:" << msg << endl;
	cout << "S:" << buff << endl;
	msgId++;
}

std::vector<std::string> imap::lsub(string fload, string mailBox)
{
	memset(buff, 0, sizeof(buff));
	vector<string> res;
	string msg = "A" + to_string(msgId) + " lsub \"" + fload + "\" \"" + mailBox + "\"\r\n";
	send(client, msg.c_str(), msg.length(), NULL);
	buff[recv(client, buff, 500, 0)] = '\0';
	cout << "C:" << msg << endl;
	cout << "S:" << buff << endl;
	msgId++;
	res = splitStr(buff, "*");
	return res;
}

void imap::copy(vector<int> id, string toFloaderName)
{
	memset(buff, 0, sizeof(buff));
	for (int i = 0; i < id.size(); ++i) {
		string msg = "A" + to_string(msgId) + " copy " + to_string(id[i]) + " \"" + toFloaderName + "\"\r\n";
		send(client, msg.c_str(), msg.length(), NULL);
		buff[recv(client, buff, 500, 0)] = '\0';
	}
	
	cout << "C:" << "A" + to_string(msgId) + " copy " << endl;
	cout << "S:" << buff << endl;
	msgId++;
}

void imap::moveMailTo(vector<int> id, string toFloader)
{
	
	copy(id, toFloader);  //先将邮件拷贝到指定文件夹
	store(id, '+', storeFlages[3]);  //将该文件夹下该邮件标记为删除
	expunge();
}

std::vector<int> imap::searchFromBody(string matchStr)
{
	memset(buff, 0, sizeof(buff));
	vector<int> res;
	string msg = "A" + to_string(msgId) + " search body" + " \"" + matchStr + "\"\r\n";
	send(client, msg.c_str(), msg.length(), NULL);
	buff[recv(client, buff, 500, 0)] = '\0';
	vector<string> revSplit = splitStr(buff, "\r\n");
	vector<string> revSearch = splitStr(revSplit[0], " ");
	for (int i = 2; i < revSearch.size(); ++i) {
		res.push_back(stoi(revSearch[i]));
	}
	cout << "C:" << msg << endl;
	cout << "S:" << buff << endl;
	msgId++;
	return res;
}

std::vector<int> imap::searchFromSender(string sender)
{
	memset(buff, 0, sizeof(buff));
	vector<int> res;
	string msg = "A" + to_string(msgId) + " search from" + " \"" + sender + "\"\r\n";
	send(client, msg.c_str(), msg.length(), NULL);
	buff[recv(client, buff, 500, 0)] = '\0';
	vector<string> revSplit = splitStr(buff, "\r\n");
	vector<string> revSearch = splitStr(revSplit[0], " ");
	for (int i = 2; i < revSearch.size(); ++i) {
		res.push_back(stoi(revSearch[i]));
	}
	cout << "C:" << msg << endl;
	cout << "S:" << buff << endl;
	msgId++;
	return res;
}

std::vector<int> imap::seachFromTo(string recUser)
{
	memset(buff, 0, sizeof(buff));
	vector<int> res;
	string msg = "A" + to_string(msgId) + " search to" + " \"" + recUser + "\"\r\n";
	send(client, msg.c_str(), msg.length(), NULL);
	buff[recv(client, buff, 500, 0)] = '\0';
	vector<string> revSplit = splitStr(buff, "\r\n");
	vector<string> revSearch = splitStr(revSplit[0], " ");
	for (int i = 2; i < revSearch.size(); ++i) {
		res.push_back(stoi(revSearch[i]));
	}
	cout << "C:" << msg << endl;
	cout << "S:" << buff << endl;
	msgId++;
	return res;
}

std::vector<int> imap::searchFromSubject(string matchStr)
{
	memset(buff, 0, sizeof(buff));
	vector<int> res;
	string msg = "A" + to_string(msgId) + " search header subject" + " \"" + matchStr + "\"\r\n";
	send(client, msg.c_str(), msg.length(), NULL);
	buff[recv(client, buff, 500, 0)] = '\0';
	vector<string> revSplit = splitStr(buff, "\r\n");
	vector<string> revSearch = splitStr(revSplit[0], " ");
	for (int i = 2; i < revSearch.size(); ++i) {
		res.push_back(stoi(revSearch[i]));
	}
	cout << "C:" << msg << endl;
	cout << "S:" << buff << endl;
	msgId++;
	return res;
}

std::vector<string> imap::getuid(string id)
{
	memset(buff, 0, sizeof(buff));
	vector<string> res;
	string msg = "A" + to_string(msgId) + " fetch "+id+" uid\r\n";
	send(client, msg.c_str(), msg.length(), NULL);
	buff[recv(client, buff, 500, 0)] = '\0';
	cout << "C:" << msg << endl;
	cout << "S:" << buff << endl;
	vector<string> buffSplit = splitStr(buff, "\r\n");
	for (int i = 0; i < buffSplit.size(); ++i) {
		if(buffSplit[i][0]!='*') continue;
		else {
			vector<string> recuidSplit=splitStr(buffSplit[i], " ");
			string uid = *(recuidSplit.end() - 1);
			uid.erase(uid.end() - 1);
			res.push_back(uid);
		}
	}
	msgId++;
	return res;
}

string imap::getHeaders(int id)
{
	memset(buff, 0, sizeof(buff));
	string recvStr;
	string msg = "A" + to_string(msgId) + " fetch "+to_string(id)+" BODY[HEADER]\r\n";
	send(client, msg.c_str(), msg.length(), NULL);
	//循环接收防止粘包
	while (true)
	{
		recv(client, buff, 500, 0);
		string temp = buff;
		recvStr += buff;
		bool isOK = temp.find("A" + to_string(msgId) + " OK") == string::npos ? false : true;
		bool isNO = temp.find("A" + to_string(msgId) + " NO") == string::npos ? false : true;
		bool isBAD = temp.find("A" + to_string(msgId) + " BAD") == string::npos ? false : true;
		if (isBAD || isNO || isOK) {
			break;
		}
		memset(buff, 0, 500);
	}
	cout << "C:" << msg << endl;
	cout << "S:" << recvStr << endl;
	msgId++;
	return recvStr;
}

std::string imap::getBody(int id)
{
	string res;
	string msg = "A" + to_string(msgId) + " fetch " + to_string(id) + " BODY[TEXT]\r\n";
	send(client, msg.c_str(), msg.length(), NULL);
	while (true)
	{
		recv(client, buff, 500, 0);
		res += buff;
		string temp = buff;
		bool isOk = temp.find("A" + to_string(msgId) + " OK") == string::npos ? false : true;
		bool isNO = temp.find("A" + to_string(msgId) + " NO") == string::npos ? false : true;
		bool isBAD = temp.find("A" + to_string(msgId) + " BAD") == string::npos ? false : true;
		if (isBAD || isNO || isOk) {
			break;
		}
		memset(buff, 0, 500);
	}
	cout << "C:" << msg << endl;
	cout << "S:" << res << endl;
	return res;
}

vector<string> imap::getAttchmentName(int id)
{
	vector<string> res;
	memset(buff, 0, sizeof(buff));
	string recvStr;
	string msg = "A" + to_string(msgId) + " fetch " + to_string(id) + " BODYSTRUCTURE\r\n";
	send(client, msg.c_str(), msg.length(), NULL);
	while (true)
	{
		recv(client, buff, 500, 0);
		recvStr += buff;
		string temp = buff;
		bool isOk = temp.find("A" + to_string(msgId) + " OK") == string::npos ? false : true;
		bool isNO = temp.find("A" + to_string(msgId) + " NO") == string::npos ? false : true;
		bool isBAD = temp.find("A" + to_string(msgId) + " BAD") == string::npos ? false : true;
		if (isBAD || isNO || isOk) {
			break;
		}
		memset(buff, 0, 500);
	}
	cout << "C:" << msg << endl;
	cout << "S:" << recvStr << endl;
	vector<string> structSplit=splitStr(recvStr, "\"name\"");
	for (int i = 0; i < structSplit.size(); ++i) {
		if (strStartWith(structSplit[i], " \"=?")) {
			res.push_back(*(splitStr(structSplit[i], "?=").begin()));
			continue;
		}
		else if (strStartWith(structSplit[i], " {")) {
			vector<string> fileNameSplit = splitStr(structSplit[i], ")");
			vector<string> fileName = splitStr(fileNameSplit[0], "\r\n");
			string longFileName;
			for (int j = 0; j < fileName.size(); ++j) {
				if (strStartWith(fileName[j], "=?")|| strStartWith(fileName[j], " =?")) {
					longFileName += *(splitStr(fileName[j], "?=").begin());
				}
			}
			res.push_back(longFileName);
		}
	}
	msgId++;
	return res;
}

std::string imap::getBodyPart(string partId)
{
	string res;
	memset(buff, 0, sizeof(buff));
	string msg = "A" + to_string(msgId) + " fetch BODY["+partId+"]\r\n";
	send(client, msg.c_str(), msg.length(), NULL);
	while (true)
	{
		recv(client, buff, 500, 0);
		res += buff;
		string temp = buff;
		bool isOk = temp.find("A" + to_string(msgId) + " OK") == string::npos ? false : true;
		bool isNO = temp.find("A" + to_string(msgId) + " NO") == string::npos ? false : true;
		bool isBAD = temp.find("A" + to_string(msgId) + " BAD") == string::npos ? false : true;
		if (isBAD || isNO || isOk) {
			break;
		}
		memset(buff, 0, 500);
	}
	return res;
}

void imap::close()
{
	memset(buff, 0, sizeof(buff));
	string msg = "A" + to_string(msgId) + " close\r\n";
	send(client, msg.c_str(), msg.length(), NULL);
	buff[recv(client, buff, 500, 0)] = '\0';
	cout << "C:" << msg << endl;
	cout << "S:" << buff << endl;
	msgId++;
}

void imap::expunge()
{
	string msg = "A" + to_string(msgId) + " expunge\r\n";
	send(client, msg.c_str(), msg.length(), NULL);
	buff[recv(client, buff, 500, 0)] = '\0';
	cout << "C:" << msg << endl;
	cout << "S:" << buff << endl;
	msgId++;
}

void imap::check()
{
	string msg = "A" + to_string(msgId) + " check\r\n";
	send(client, msg.c_str(), msg.length(), NULL);
	buff[recv(client, buff, 500, 0)] = '\0';
	cout << "C:" << msg << endl;
	cout << "S:" << buff << endl;
	msgId++;
}

void imap::noop()
{
	string msg = "A" + to_string(msgId) + " noop\r\n";
	send(client, msg.c_str(), msg.length(), NULL);
	buff[recv(client, buff, 500, 0)] = '\0';
	cout << "C:" << msg << endl;
	cout << "S:" << buff << endl;
	msgId++;
}

void imap::logout()
{
	string msg = "A" + to_string(msgId) + " logout\r\n";
	send(client, msg.c_str(), msg.length(), NULL);
	buff[recv(client, buff, 500, 0)] = '\0';
	cout << "C:" << msg << endl;
	cout << "S:" << buff << endl;
	msgId++;
	::closesocket(client);
}

bool imap::strStartWith(string str, string start)
{
	for (int i = 0; i < start.size(); ++i) {
		if (str[i] == start[i])continue;
		else return false;
	}
	return true;
}

std::vector<std::string> imap::splitStr(string str,string delim)
{
	str = str + delim;
	vector<std::string> res;
	for (int i = 0; i < (int)(str.length()); ++i) {
		int pos = static_cast<int>(str.find(delim, i));
		if (pos != str.npos) {
			std::string sub = str.substr(i, pos - i);
			res.push_back(sub);
			i = pos + (int)(delim.length()) - 1;
		}
	}
	return res;
}

imap::~imap()
{

}