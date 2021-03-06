 
/**
 * @file test_select.cpp
 * @author kymowind@gmail.com
 * @date 2016/10/15 21:32:27
 * @brief 
 *  
 **/

#include  <stdio.h>
#include  <iostream>
#include  <unistd.h>
#include  <fcntl.h>
#include  <errno.h>
#include  <sys/types.h>
#include  <sys/socket.h>
#include  <sys/epoll.h>
#include  <netinet/in.h>
#include  <arpa/inet.h>
#include "select.h"
#include "event.h"
#include <fcntl.h>
#include <csignal>
#include "define.h"
#include "server.h"

using namespace sub_framework;

// 逻辑回调函数

void sig_handler( int sig ) {
    if ( sig == SIGINT || sig == SIGSEGV || sig == SIGTSTP) {
        SubThreadPool::_get_instance()->_stop();
    } 
}

int req_task_call_back(void *a, void *b) {
    char* buf = (char*) a;
    char *ret = (char*) b; 
    char *json = "hello";
    
    return 1;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "./sub port" << std::endl;
        return 0;
    }
    signal(SIGINT, sig_handler);
    // 配置初始化
    SubStrategyConfig::_get_instance()->_read_conf_file("../conf/strategy.conf");
    SubConfig::_get_instance()->_read_conf_file("../conf/sub.conf");
    // 日志初始化
    SubLogger::_get_instance()->_init_logger();
    // init strategies
    SubStrategyMgr::_get_instance()->_init_strategies();
    // init task_mgr
    SubTaskMgr::_get_instance()->_init();
    // 设置核心逻辑的回调函数
    SubTaskMgr::_get_instance()->_set_call_back_proc(REQ_TASK, req_task_call_back);
    // 启动线程池
    SubThreadPool::_get_instance()->_init();
    SubThreadPool::_get_instance()->_start();
    SubEventQueue::_get_instance()->_init();
    // 启动服务
    //
    SubServer*svr = SubServer::_get_instance();
    svr->_run(atoi(argv[1]));

    return 0;
}









/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
