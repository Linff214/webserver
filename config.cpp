#include "config.h"
#include <iostream>
#include <map>
#include <functional>
#include <stdexcept>
Config::Config(){
    //端口号,默认9006
    PORT = 9006;

    //日志写入方式，默认同步
    LOGWrite = 0;

    //触发组合模式,默认listenfd LT + connfd LT
    TRIGMode = 0;

    //listenfd触发模式，默认LT
    LISTENTrigmode = 0;

    //connfd触发模式，默认LT
    CONNTrigmode = 0;

    //优雅关闭链接，默认不使用
    OPT_LINGER = 0;

    //数据库连接池数量,默认8
    sql_num = 8;

    //线程池内的线程数量,默认8
    thread_num = 8;

    //关闭日志,默认不关闭
    close_log = 0;

    //并发模型,默认是proactor
    actor_model = 0;
}

void Config::parse_arg(int argc, char* argv[]) {
    int opt;
    const char *optstring = "p:l:m:o:s:t:c:a:h";

    // 用 map 来简化 switch-case
    std::map<char, std::function<void(const char*)>> handlers;

    handlers['p'] = [&](const char* arg) {
        try {
            PORT = std::stoi(arg);
        } catch (...) {
            std::cerr << "错误：无效的端口号。\n";
            exit(EXIT_FAILURE);
        }
    };

    handlers['l'] = [&](const char* arg) {
        LOGWrite = std::stoi(arg);
    };

    handlers['m'] = [&](const char* arg) {
        TRIGMode = std::stoi(arg);
    };

    handlers['o'] = [&](const char* arg) {
        OPT_LINGER = std::stoi(arg);
    };

    handlers['s'] = [&](const char* arg) {
        sql_num = std::stoi(arg);
        if (sql_num <= 0) {
            std::cerr << "错误：数据库连接池数必须大于0。\n";
            exit(EXIT_FAILURE);
        }
    };

    handlers['t'] = [&](const char* arg) {
        thread_num = std::stoi(arg);
        if (thread_num <= 0) {
            std::cerr << "错误：线程数必须大于0。\n";
            exit(EXIT_FAILURE);
        }
    };

    handlers['c'] = [&](const char* arg) {
        close_log = std::stoi(arg);
    };

    handlers['a'] = [&](const char* arg) {
        actor_model = std::stoi(arg);
    };

    // 帮助信息
    handlers['h'] = [&](const char*) {
        std::cout << "使用方法：./server [选项]\n"
                  << "  -p <端口号>       设置服务器监听端口（默认9006）\n"
                  << "  -l <日志方式>     0 = 同步日志，1 = 异步日志\n"
                  << "  -m <触发模式>     0~3 表示LT/ET组合方式\n"
                  << "  -o <优雅关闭>     0 = 关闭，1 = 启用\n"
                  << "  -s <数据库连接数> 默认8\n"
                  << "  -t <线程数>       默认8\n"
                  << "  -c <关闭日志>     0 = 开启日志，1 = 关闭日志\n"
                  << "  -a <并发模型>     0 = proactor，1 = reactor\n"
                  << "  -h                显示帮助信息\n";
        exit(0);
    };
    // 主循环：解析参数并自动执行相应设置函数
    while ((opt = getopt(argc, argv, optstring)) != -1) {
        if (handlers.count(opt)) {
            handlers[opt](optarg);
        } else {
            std::cerr << "错误：未知选项 -" << static_cast<char>(optopt) << "\n";
            exit(EXIT_FAILURE);
        }
    }
}
void Config::print_config() {
    printf("当前配置：\n");
    printf("PORT = %d\n", PORT);
    printf("LOGWrite = %d\n", LOGWrite);
    printf("TRIGMode = %d\n", TRIGMode);
    printf("OPT_LINGER = %d\n", OPT_LINGER);
    printf("sql_num = %d\n", sql_num);
    printf("thread_num = %d\n", thread_num);
    printf("close_log = %d\n", close_log);
    printf("actor_model = %d\n", actor_model);
}
