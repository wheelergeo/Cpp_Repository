/*****************************************************************************
*  Riechan's Basic TcpClient library                                         *
*  @file     TcpClient.h                                                     *
*  @brief    TcpClient connect API                                           *
*  @author   Riechan.ye                                                      *
*  @email    Riechan.ye@outlook.com                                          *
*  @version  1.0.0                                                           *
*  @date     2021.10.15 20:00:00                                             *
*  @license  GNU General Public License (GPL)                                *
*                                                                            *
*----------------------------------------------------------------------------*
*  Remark         : Description                                              *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2021/10/15 | 1.0.1     | Riechan.ye     | Create file                     *
*****************************************************************************/
#ifndef __TCPCLIENT_H__
#define __TCPCLIENT_H__
#include <string>
#include <map>
#include <netinet/in.h>
#include "EventPoll.h"
#include "ErrorCode.h"



class TcpClient{
public:
    using EXTCALLBACK = std::function<OPERATE_RET(void*, const std::string&)>;
    TcpClient(EventPoll *my_epoll, int my_port, std::string my_addr);
    TcpClient(EventPoll *my_epoll, int fd);
    OPERATE_RET establish(void);
    static OPERATE_RET receive(void* server, int fd);
    OPERATE_RET sendData(std::string data);
    void addCallBack(void* usr_data, EXTCALLBACK callback);
    OPERATE_RET closeConnect(void);  
private:
    std::string buff_ = "";
    EventPoll* epoll_ = nullptr;
    int port_ = 0;
    std::string addr_ = {0};
    int connect_fd_ = -1;
    struct sockaddr_in cli_addr_ = {0};
    EXTCALLBACK callback_ = nullptr;
    void* usr_data_ = nullptr;
};




#endif