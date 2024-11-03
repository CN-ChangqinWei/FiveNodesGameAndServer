#include "csmtpmail.h"

CSmtpMail::CSmtpMail()
{
    m_serverPort = 25;
    m_smtpServer = "smtp.qq.com";
    m_emailSenderAddr = MyQQMailAddr;//发送邮箱  我自己的邮箱
    m_emailSenderName = MyName;
    m_emailSenderPasswd = MyAppCode;
    m_emailReceiverAddr = "receiver@qq.com";//接收邮箱  需要给定
}


struct Base64Date6
{
    unsigned int d4 : 6;
    unsigned int d3 : 6;
    unsigned int d2 : 6;
    unsigned int d1 : 6;
};


char CSmtpMail::ConvertToBase64(char uc)
{
    if (uc < 26)
    {
        return 'A' + uc;
    }
    if (uc < 52)
    {
        return 'a' + (uc - 26);
    }
    if (uc < 62)
    {
        return '0' + (uc - 52);
    }
    if (uc == 62)
    {
        return '+';
    }
    return '/';
}

// base64的实现
void CSmtpMail::EncodeBase64(char *dbuf, char *buf128, int len)
{
    struct Base64Date6 *ddd = NULL;
    int           i = 0;
    char          buf[256] = { 0 };
    char          *tmp = NULL;
    char          cc = '\0';

    memset(buf, 0, 256);
    strcpy_s(buf, 256, buf128);
    for (i = 1; i <= len / 3; i++)
    {
        tmp = buf + (i - 1) * 3;
        cc = tmp[2];
        tmp[2] = tmp[0];
        tmp[0] = cc;
        ddd = (struct Base64Date6 *)tmp;
        dbuf[(i - 1) * 4 + 0] = ConvertToBase64((unsigned int)ddd->d1);
        dbuf[(i - 1) * 4 + 1] = ConvertToBase64((unsigned int)ddd->d2);
        dbuf[(i - 1) * 4 + 2] = ConvertToBase64((unsigned int)ddd->d3);
        dbuf[(i - 1) * 4 + 3] = ConvertToBase64((unsigned int)ddd->d4);
    }
    if (len % 3 == 1)
    {
        tmp = buf + (i - 1) * 3;
        cc = tmp[2];
        tmp[2] = tmp[0];
        tmp[0] = cc;
        ddd = (struct Base64Date6 *)tmp;
        dbuf[(i - 1) * 4 + 0] = ConvertToBase64((unsigned int)ddd->d1);
        dbuf[(i - 1) * 4 + 1] = ConvertToBase64((unsigned int)ddd->d2);
        dbuf[(i - 1) * 4 + 2] = '=';
        dbuf[(i - 1) * 4 + 3] = '=';
    }
    if (len % 3 == 2)
    {
        tmp = buf + (i - 1) * 3;
        cc = tmp[2];
        tmp[2] = tmp[0];
        tmp[0] = cc;
        ddd = (struct Base64Date6 *)tmp;
        dbuf[(i - 1) * 4 + 0] = ConvertToBase64((unsigned int)ddd->d1);
        dbuf[(i - 1) * 4 + 1] = ConvertToBase64((unsigned int)ddd->d2);
        dbuf[(i - 1) * 4 + 2] = ConvertToBase64((unsigned int)ddd->d3);
        dbuf[(i - 1) * 4 + 3] = '=';
    }
    return;
}

// 发送邮件
void CSmtpMail::SendMail(   string receiverEmailAddr , string emailContent   )
{

    m_emailReceiverAddr = receiverEmailAddr;

    char contentBuf[1024] ={ 0 };

    sprintf( contentBuf , "From: \"%s\" <%s>\r\nTo:\"%s\"<%s>\r\nSubject:login verify\r\n\r\ncode:%s.\n"
                        , m_emailSenderName.c_str()
                        , m_emailSenderAddr.c_str()
                        , "user"
                        , m_emailReceiverAddr.c_str()
                        , emailContent.c_str());

    std::cout << contentBuf << std::endl;

    const char *body = contentBuf;   //邮件构建完的内容

    int     sockfd = { 0 };
    char    buf[1500] = { 0 };
    char    rbuf[1500] = { 0 };
    char    login[128] = { 0 };
    char    pass[128] = { 0 };

    WSADATA WSAData;
    struct sockaddr_in their_addr = { 0 };
    WSAStartup(MAKEWORD(2, 2), &WSAData);
    memset(&their_addr, 0, sizeof(their_addr));

    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(m_serverPort);    // 一般是25端口不需要改
    hostent* hptr = gethostbyname(m_smtpServer.c_str());          // 端口和服务器
    memcpy(&their_addr.sin_addr.S_un.S_addr, hptr->h_addr_list[0], hptr->h_length);
    printf("IP of %s is : %d:%d:%d:%d\n",
        m_smtpServer.c_str(),
        their_addr.sin_addr.S_un.S_un_b.s_b1,
        their_addr.sin_addr.S_un.S_un_b.s_b2,
        their_addr.sin_addr.S_un.S_un_b.s_b3,
        their_addr.sin_addr.S_un.S_un_b.s_b4);

    // 连接邮件服务器，如果连接后没有响应，则2ms后重新连接
    sockfd = ConnectServer((struct sockaddr *)&their_addr);
    memset(rbuf, 0, 1500);
    while (recv(sockfd, rbuf, 1500, 0) == 0)
    {
        cout << "reconnect..." << endl;
        Sleep(2);
        sockfd = ConnectServer((struct sockaddr *)&their_addr);
        memset(rbuf, 0, 1500);
    }

    cout << rbuf << endl;

    // EHLO
    sprintf_s(buf, 1500, "EHLO HYL-PC\r\n");
    send(sockfd, buf, strlen(buf), 0);
    memset(rbuf, 0, 1500);
    recv(sockfd, rbuf, 1500, 0);
    cout << "EHLO REceive: " << rbuf << endl;

    // AUTH LOGIN
    sprintf_s(buf, 1500, "AUTH LOGIN\r\n");
    send(sockfd, buf, strlen(buf), 0);
    memset(rbuf, 0, 1500);
    recv(sockfd, rbuf, 1500, 0);
    cout << "Auth Login Receive: " << rbuf << endl;

    // USER
    sprintf_s(buf, 1500, m_emailSenderAddr.c_str());//你的邮箱账号
    memset(login, 0, 128);
    EncodeBase64(login, buf, strlen(buf));
    sprintf_s(buf, 1500, "%s\r\n", login);
    send(sockfd, buf, strlen(buf), 0);
    cout << "Base64 UserName: " << buf << endl;
    memset(rbuf, 0, 1500);
    recv(sockfd, rbuf, 1500, 0);
    cout << "User Login Receive: " << rbuf << endl;

    // PASSWORD
    sprintf_s(buf, 1500, m_emailSenderPasswd.c_str());//你的邮箱密码
    memset(pass, 0, 128);
    EncodeBase64(pass, buf, strlen(buf));
    sprintf_s(buf, 1500, "%s\r\n", pass);
    send(sockfd, buf, strlen(buf), 0);
    cout << "Base64 Password: " << buf << endl;

    memset(rbuf, 0, 1500);
    recv(sockfd, rbuf, 1500, 0);
    cout << "Send Password Receive: " << rbuf << endl;

    // MAIL FROM
    sprintf_s(buf, 1500, "MAIL FROM: <%s>\r\n",m_emailSenderAddr.c_str());  //此处要和发邮件的邮箱保持一致
    send(sockfd, buf, strlen(buf), 0);
    memset(rbuf, 0, 1500);
    recv(sockfd, rbuf, 1500, 0);
    cout << "set Mail From Receive: " << rbuf << endl;

    // RCPT TO 第一个收件人
    sprintf_s(buf, 1500, "RCPT TO:<%s>\r\n", m_emailReceiverAddr.c_str() );
    send(sockfd, buf, strlen(buf), 0);
    memset(rbuf, 0, 1500);
    recv(sockfd, rbuf, 1500, 0);
    cout << "Tell Sendto Receive: " << rbuf << endl;

    // DATA 准备开始发送邮件内容
    sprintf_s(buf, 1500, "DATA\r\n");
    send(sockfd, buf, strlen(buf), 0);
    memset(rbuf, 0, 1500);
    recv(sockfd, rbuf, 1500, 0);
    cout << "Send Mail Prepare Receive: " << rbuf << endl;

    // 发送邮件内容，\r\n.\r\n内容结束标记
    sprintf_s(buf, 1500, "%s\r\n.\r\n", body);
    send(sockfd, buf, strlen(buf), 0);
    memset(rbuf, 0, 1500);
    recv(sockfd, rbuf, 1500, 0);
    cout << "Send Mail Receive: " << rbuf << endl;

    // QUIT
    sprintf_s(buf, 1500, "QUIT\r\n");
    send(sockfd, buf, strlen(buf), 0);
    memset(rbuf, 0, 1500);
    recv(sockfd, rbuf, 1500, 0);
    cout << "Quit Receive: " << rbuf << endl;

    //清理工作
    closesocket(sockfd);
    WSACleanup();
    return;
}
// 打开TCP Socket连接
int CSmtpMail::ConnectServer(struct sockaddr *addr)
{
    int sockfd = 0;
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (sockfd < 0)
    {
        cout << "Open sockfd(TCP) error!" << endl;
        exit(-1);
    }
    if (connect(sockfd, addr, sizeof(struct sockaddr)) < 0)
    {
        cout << "Connect sockfd(TCP) error!" << endl;
        exit(-1);
    }
    return sockfd;
}
