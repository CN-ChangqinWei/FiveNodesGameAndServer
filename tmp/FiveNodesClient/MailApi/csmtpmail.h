#ifndef CSMTPMAIL_H
#define CSMTPMAIL_H

#include <windows.h>
#include <WinSock.h>
#include <iostream>


using namespace std;


const string MyName = "864378655";
const string MyQQMailAddr = "864378655@qq.com";
//QQ 邮箱获取授权码
const string MyAppCode = "uvxdcxljecgibecf";

class CSmtpMail
{
public:
    CSmtpMail();

    // 协议中加密部分使用的是base64方法
    static char ConvertToBase64(char c6);
    static void EncodeBase64(char *dbuf, char *buf128, int len);

    //向哪里发什么
    void SendMail( string receiverEmailAddr , string emailContent  );

private:
    //供内部调用 连接服务器 需要提供地址
    int  ConnectServer(struct sockaddr *addr);

private:

    u_short m_serverPort;           //服务器端口
    string m_smtpServer;            //邮箱服务器地址
    string m_emailSenderAddr;       //发送方地址
    string m_emailSenderName;       //发送方名字
    string m_emailSenderPasswd;     //发送方签证
    string m_emailReceiverAddr;     //接收方地址
};

#endif // CSMTPMAIL_H




