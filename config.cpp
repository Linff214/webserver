#include "config.h"

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

void Config::parse_arg(int argc, char*argv[]){
    int opt;
    const char *str = "p:l:m:o:s:t:c:a:h";
    while ((opt = getopt(argc, argv, str)) != -1)
    {
        switch (opt)
        {
        case 'p':
        {
            PORT = atoi(optarg);
            break;
        }
        case 'l':
        {
            LOGWrite = atoi(optarg);
            break;
        }
        case 'm':
        {
            TRIGMode = atoi(optarg);
            break;
        }
        case 'o':
        {
            OPT_LINGER = atoi(optarg);
            break;
        }
        case 's':
        {
            sql_num = atoi(optarg);
            break;
        }
        case 't':
        {
            thread_num = atoi(optarg);
            if (thread_num <= 0) {
            printf("错误：线程数必须大于0\n");
            exit(EXIT_FAILURE);
    }
            break;
        }
        case 'c':
        {
            close_log = atoi(optarg);
            break;
        }
        case 'a':
        {
            actor_model = atoi(optarg);
            break;
        }
        case 'h':
        {
            printf("使用方法：./server [选项]\n");
            printf("  -p <端口号>       设置服务器监听端口（默认9006）\n");
            printf("  -l <日志方式>     0 = 同步日志，1 = 异步日志\n");
            printf("  -m <触发模式>     0~3 表示LT/ET组合方式\n");
            printf("  -o <优雅关闭>     0 = 关闭，1 = 启用\n");
            printf("  -s <数据库连接数> 默认8\n");
            printf("  -t <线程数>       默认8\n");
            printf("  -c <关闭日志>     0 = 开启日志，1 = 关闭日志\n");
            printf("  -a <并发模型>     0 = proactor，1 = reactor\n");
            printf("  -h                显示帮助信息\n");
            exit(0); // 显示完帮助后退出
        }

        default:
            printf("Unknown option: -%c\n", optopt);
            break;
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
