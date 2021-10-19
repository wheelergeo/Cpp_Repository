#include <string.h>
#include <fstream>
#include "../include/HttpClient.h"
#include "../include/Logger.h"

HttpClient::HttpClient(EventPoll* my_epoll, const int my_port, const std::string my_addr): TcpClient(my_epoll, my_port, my_addr) {

}

HttpClient::HttpClient(EventPoll *my_epoll, int fd): TcpClient(my_epoll, fd) {

}

OPERATE_RET HttpClient::analysisUrl(const std::string& buf) {
    std::string str_buf = buf;
    if (std::string::npos == str_buf.find("HTTP/")) {
        LogError() << "analysisUrl:Url Error";
        return OPRT_HTTP_PROTO_ERROR;
    }
    
    http_method_ = str_buf.substr(0, str_buf.find(" /"));
    http_file_name_ = str_buf.substr(str_buf.find("/") + 1, str_buf.find("HTTP") - 6);
    LogInfo() << "http_method:" << http_method_;
    LogInfo() << "http_file_name:" << http_file_name_;       
    if (0 != http_method_.compare("GET")) {
        LogError() << "analysisUrl:method Error";
		return OPRT_HTTP_METHOD_ERROR;
	}

    return OPRT_OK;
}

OPERATE_RET HttpClient::responseSuccess(const std::string& http_file_name) {
    std::string send_buff = "";
    std::string protocol = "HTTP/1.1 200 OK\r\n";
    std::string server = "Server:Linux Web Server\r\n";
    std::string cnt_len = "Content-length:";
    
    std::ifstream ifs(http_file_name, std::ifstream::in);
    std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>(0));
    ifs.close();

    cnt_len.append(std::to_string(str.size())).append("\r\n\r\n");
    send_buff.append(protocol).append(server).append(cnt_len).append(str);
    
    if (0 > sendData(send_buff)) {
        LogError() << "HttpClient:senddata";
        return OPRT_SOCK_SEND_ERROR;
    }

    LogInfo() << send_buff;
    return OPRT_OK;
}

OPERATE_RET HttpClient::responseSuccess(void) {
    std::string send_buff = "";
    std::string protocol = "HTTP/1.1 200 OK\r\n";
    std::string server = "Server:Linux Web Server\r\n";
    std::string cnt_len = "Content-length:";

    std::ifstream ifs(http_file_name_.c_str(), std::ifstream::in);
    std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>(0));
    ifs.close();

    cnt_len.append(std::to_string(str.size())).append("\r\n\r\n");
    send_buff.append(protocol).append(server).append(cnt_len).append(str);
    
    if (0 > sendData(send_buff)) {
        LogError() << "HttpClient:senddata";
        return OPRT_SOCK_SEND_ERROR;
    }
    LogInfo() << send_buff;
    return OPRT_OK;
}

OPERATE_RET HttpClient::responseFailed(const std::string& error_file_name) {
    std::string send_buff = "";
    std::string protocol = "HTTP/1.1 400 Bad Request\r\n";
    std::string server = "Server:Linux Web Server\r\n";
    std::string cnt_len = "Content-length:";

    std::ifstream ifs(error_file_name, std::ifstream::in);
    std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>(0));
    ifs.close();

    cnt_len.append(std::to_string(str.size())).append("\r\n\r\n");
    send_buff.append(protocol).append(server).append(cnt_len).append(str);
    
    if (0 > sendData(send_buff)) {
        LogError() << "HttpClient:senddata";
        return OPRT_SOCK_SEND_ERROR;
    }
    LogInfo() << send_buff;
    return OPRT_OK;
}