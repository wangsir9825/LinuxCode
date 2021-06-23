#include <stdio.h>
#include <unistd.h> // fork
#include <stdlib.h> // exit
#include <string.h> // fgets
#include <ctype.h>  // isspace
#include <sys/wait.h> //wait
int main(int argc,char *argv[])
{
    while(1){
        //第1步：获取命令行字符串
        char buf[1024] = {0};
        printf("[user@host path]$ ");//提示信息：user 是用户名，host是主机名，path是当前路径
        fflush(stdout);//手动刷新标准输出文件的缓冲区，确保提示信息能打印到屏幕上
        fgets(buf,1023,stdin); // buf最后一个字节方'\0',所以只能接收1023个字节
        buf[strlen(buf)-1] = '\0'; // 总长度减1，是为了防止将从标准输入文件中读入的换行符打印出来。
        
       // printf("buf:[%s]\n",buf); //打印读入的命令行
        
        //第2步：解析命令行
        char *ptr = buf;//获取命令行的首地址
        char myargv[32][32];// 最多读取32个参数，每个参数最长为32个字符
        int myargc = 0;// 参数个数 
        while(*ptr != '\0'){
            if(!isspace(*ptr)){//如果不是空格，则开始读取命令参数
                int count = 0;
                while(*ptr != '\0' && !isspace(*ptr)){// 遇到空格则说明第一个参数读取结束
                    myargv[myargc][count] = *ptr;
                    count++;
                    ptr++;
                }
                myargc++;//读取下一个命令行参数
            }
            ptr++;
        }

        //打印解析出来的命令行参数
        int i = 0;
       // for(i = 0; i < myargc; i++){
       //     printf("myargv[%d]=[%s]\n", i, myargv[i]);
       // }

        //第2步：建立子进程
        
        // 由于execvp的第二个参数为常字符指针数据，所以首先将读取的参数提取出来，保存在一个字符指针数组里面，为程序替换做准备
        char *arg[32] = {NULL};
        for(i = 0; i < myargc; i++){
            arg[i] = myargv[i];
        }
        arg[myargc] = NULL;// 命令行参数的组织方式要求数组的最后一个元素为空指针（NULL）

        pid_t pid = fork();
        //第3步：程序替换

        if (pid < 0){
            continue;
        }else if(pid == 0){
            execvp(arg[0],arg);// 如果程序替换成功，则直接正常退出
            exit(-1); // 如果程序替换失败，则执行这一句，退出返回值为-1····
        }
        wait(NULL);// 父进程等待子进程退出
    }
    return 0;
}



