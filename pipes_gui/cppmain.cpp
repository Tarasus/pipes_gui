//
//  cppmain.cpp
//  pipes_gui
//
//  Created by admin on 10.09.2021.
//

#include "cppmain.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <spawn.h>
#include <string.h>
#include <sys/wait.h>
#include <iostream>
#include <string>
#include <vector>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include <mach-o/dyld.h>

extern char **environ;

std::string run_child(char *cmd)
{
    int exit_code;
    
    
    char rawPathName[PATH_MAX];
    char realPathName[PATH_MAX];
    uint32_t rawPathSize = (uint32_t)sizeof(rawPathName);

    if(!_NSGetExecutablePath(rawPathName, &rawPathSize))
    {
        realpath(rawPathName, realPathName);
    }
    
    printf("path = [%s]\n",rawPathName);
    
    char maps[PATH_MAX];
    readlink("proc/self/maps", maps, PATH_MAX);
    
    printf("maps = [%s]\n",maps);
    
    
    char* reqpath = "req";
    char* respath = "res";
    int r = -1;
    
    r = unlink(reqpath);
    if(r != 0)
    {
        printf("err -1 %d %s\n",errno,(strerror(errno)));
    }
    r = unlink(respath);
    if(r != 0)
    {
        printf("err -0 %d %s\n",errno,(strerror(errno)));
    }
    
    r = mkfifo(reqpath,0666);
    if(r != 0)
    {
        printf("err 1 %d %s\n",errno,(strerror(errno)));
    }
    r = mkfifo(respath,0666);
    if(r != 0)
    {
        printf("err 1 %d %s\n",errno,(strerror(errno)));
    }
    
    const char * args[] = {reqpath,respath, nullptr};

    pid_t pid;
    
    printf("PARENT: Run child: %s\n", cmd);
    int status = posix_spawn(&pid, cmd, NULL, NULL, (char *const *)args, environ);
    
    if (status != 0)
    {
        printf("PARENT: posix_spawn error %d: [%s]\n",status, strerror(status));
        return std::string("fail");
    }
    printf("PARENT: Child pid: %i\n", pid);
    
    
    int req = open(reqpath,O_WRONLY);
    int res = open(respath,O_RDONLY);

    char * buf = "@ Hello, Gordon!\0";
    char readbuf[256];
    memset(readbuf,0,256);
    
    
    printf("PARENT write \n");
    write(req,buf,strlen(buf));
    printf("PARENT write done\n");
    
    int timeout_cnt = 0;
    do
    {
        ssize_t readbytes = read(res, readbuf, 256);
        if(readbytes == 0)
        {
            printf("PARENT EOF\n");
            if(timeout_cnt > 5)
            {
                break;
            }
            sleep(1);
            timeout_cnt++;
        }
        else if (readbytes == -1)
        {
            printf("PARENT IO ERROR %d [%s]\n",errno,strerror(errno));
            break;
        }
        else
        {
            printf("PARENT read %ld chars: [%s]\n",readbytes,readbuf);
            write(req,readbuf,strlen(readbuf));
        }
        sleep(1);

    }
    while ( true );

//    while ( !WIFEXITED(status) && !WIFSIGNALED(status) );
    
    printf("close\n");
    if (waitpid(pid, &exit_code, 0) != -1)
    {
        printf("PARENT: Child status %d\n", WEXITSTATUS(status));
    }
    else
    {
        perror("PARENT: waitpid");
        exit(1);
    }
    close(req);
    close(res);
    return std::string(buf,strlen(buf));
}

std::string PipesTest::PipesTest_run()
{
    printf("result = [%s]",run_child("pipes_cmd_child").c_str());
    return std::string("abc");
}
