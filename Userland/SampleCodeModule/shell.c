#include "shell.h"
#include "commands.h"
#include "stdio.h"
#include "stdlib.h"
#include "messagesDemo.h"
#include "sync.h"
#include "syscall.h"
#include "VideoModule.h"
#include "prodCons.h"
#include "filofochos.h"
#include "pipesDemo.h"
#include "piping.h"

void probando(){
    while(1);
}

void hinchaHuevos() {
    while (1) {
        printf("aca toy");
    }
}

Colour white = {255, 255, 255};


void processCommand(char * command, char * arg1, char * opMode, char * arg2, char * param){

    sscanf("%s & %s %s",command, arg1,arg2, param);
    *opMode = '&';
    if(*arg2 != 0)
        return;

    *param = 0;
    sscanf("%s | %s %s",command, arg1,arg2, param);
    *opMode='|';
     if(*arg2!=0)
        return;

    *opMode = 0;
    *param = 0;
    sscanf("%s %s",command, arg1,param);

    return;
}


void initializeShell() {

    tSem foreground = createSem("foreground");
    showBurro();
    static char command[MAXLENGTH];
    int running = 1;
    wait(foreground);
    while (running){
        int argc;
        char *argv[1];
        char * arg1 = calloc(MAXLENGTH);
        int flag;
        char * arg2 = calloc(MAXLENGTH);
        char * param = calloc(MAXLENGTH);
        char opMode = 0;
        char * echo = calloc(MAXLENGTH);
        char * commandName = calloc(MAXLENGTH);
        char * pid = calloc(10);
        char * niceness = calloc(3);
        printf("\n$>");
        scanAndPrint(command);

        sscanf("%s", command);
        processCommand(command,arg1, &opMode,arg2,param);

        if (*param != 0) {
            argv[0] = param;
            argc = 1;
        } else
            argc = 0;

        if(opMode == '|') {
            int flag;
            int flag2;
            functionType toExecute1 = getFunction(arg1,argc,param,&flag);
            functionType toExecute2 = getFunction(arg2,argc,param,&flag2);
            if (toExecute1 > 0 && toExecute2 > 0) {
                makePiping(toExecute1, toExecute2, argc, argv);
                if(flag == 0)
                    wait(foreground);
                if(flag2 == 0)
                    wait(foreground);
            }
            else {
                printf("\nUnknown command, type help\n");
                continue;
            }
        }
        else {
            if (*arg2) {
                functionType toExecute = getFunction(arg2,argc,param,&flag);
                if(toExecute > 0) {
                    exec(arg2,toExecute,argc,argv);
                    if(flag == 0)
                        wait(foreground);
                }
                else if(flag == -1){
                    running = 0;
                }
                else {
                    sscanf("%s %s %s", command, commandName, pid, niceness);
                    if (strcmp("nice", commandName) == 0) {
                        if (*pid == 0) {
                            printf("\nSyntax error. Command syntax should be: nice [pid] [1-100] to adjust niceness or nice [pid] to get process priority\n");
                        } else if (*niceness == 0) {

                            getProcessPriorityShell(pid);
                        } else {
                            niceShell(pid, niceness);
                        }

                    } else if (flag == 0) {
                        printf("\nUnknown command, type help\n");
                        continue;
                    }
                    else if(flag == -2) {
                        printf("\nParameter missing.\n");
                        continue;
                    }

                }
            }
            if (*arg1) {
                flag = executeFunctionFromShell(arg1,argc,param);
                if(flag == 1)
                    wait(foreground);
                if(flag == -1){
                    running = 0;
                }
                else{
                    sscanf("%s %s %s", command, commandName, pid, niceness);
                    if (strcmp("nice", commandName) == 0) {
                        if (*pid == 0) {
                            printf("\nSyntax error. Command syntax should be: nice [pid] [1-100] to adjust niceness or nice [pid] to get process priority\n");
                        } else if (*niceness == 0) {

                            getProcessPriorityShell(pid);
                        } else {
                            niceShell(pid, niceness);
                        }

                    } else if (flag == 0) {
                        printf("\nUnknown command, type help\n");
                        continue;
                    }
                    else if(flag == -2) {
                        printf("\nParameter missing.\n");
                        continue;
                    }

                }
            }
        }
        free(arg1);
        free(arg2);
        free(echo);
        free(commandName);
        free(pid);
        free(param);
        free(niceness);
    }
    printf("\n\n\nSee you soon!");

}


//Si devuelve 1 no hago wait(foreground)
int executeFunctionFromShell(char * arg, int argc, char * param){
    if (strcmp("help", arg) == 0) {
        help();
        return 1;
    } else if (strcmp("digitalTime", arg) == 0) {
        showDigitalHour();
        return 1;
    } else if (strcmp("echo", arg) == 0) {
        if(argc == 0)
            return -2;
        printf("\n%s",param);
        return 2;
    } else if (strcmp("time", arg) == 0) {
        showTime();
        return 1;
    } else if (strcmp("clear", arg) == 0) {
        cleanScreen();
        return 2;
    } else if (strcmp("zeroDivision", arg) == 0) {
        divi();
        return 1;
    } else if (strcmp("invalidOpcode", arg) == 0) {
        showOpcodeExc();
        return 1;
    } else if (strcmp("exit", arg) == 0) {
        return -1;
    } else if (strcmp("ps", arg) == 0) {
        ps();
        return 1;
    } else if (strcmp("memory", arg) == 0) {
        memory();
        return 1;
    } else if (strcmp("sushi", arg) == 0) {
        initProdCons();
        return 1;
    } else if (strcmp("necesitoquemeapapachen", arg) == 0) {
        showBurro();
        return 1;
    } else if (strcmp("backgroundTest", arg) == 0) {
        exec("stayAlive",stayAlive,0,0);
        return 2;
    } else if (strcmp("messages", arg) == 0) {
        startMessagesDemo();
        return 1;
    } else if (strcmp("philosophers", arg) == 0) {
        initFilofochos();
        return 1;
    } else if (strcmp("priority", arg) == 0) {
        schedulerDemo();
        return 1;
    } else if (strcmp("pipesdemo", arg) == 0) {
        initPipesDemo();
        return 1;
    } else if (strcmp("echoInput", arg) == 0) {
        echoInput();
        return 1;
    } else if (strcmp("remark", arg) == 0) {
        if(argc == 0)
            return -2;
        char *argv[1];
        argv[0] = param;
        exec("findAndRemark",findAndRemark,argc,argv);
        return 1;
    }
    return 0;
}


//Si es uno no hago wait
functionType getFunction(char * arg, int argc, char * param, int * flag){
    *flag = 0;
    if (strcmp("help", arg) == 0) {
        return help;
    } else if (strcmp("digitalTime", arg) == 0) {
        return showDigitalHour;
    } else if (strcmp("echo", arg) == 0) {
        if(argc == 0){
            *flag = -2;
            return 0;
        }
        printf("\n%s",param);
        *flag = 1;
        return 0;
    } else if (strcmp("time", arg) == 0) {
        return showTime;
    } else if (strcmp("cleanScreen", arg) == 0) {
        *flag = 1;
        return cleanScreen;
    } else if (strcmp("zeroDivision", arg) == 0) {
        return divi;
    } else if (strcmp("invalidOpcode", arg) == 0) {
        return showOpcodeExc;
    } else if (strcmp("exit", arg) == 0) {
        *flag = -1;
        return 0;
    } else if (strcmp("ps", arg) == 0) {
        return ps;
    } else if (strcmp("memory", arg) == 0) {
        return memory;
    } else if (strcmp("exec", arg) == 0) {
        return probando;
    } else if (strcmp("sushi", arg) == 0) {
        return initProdCons;
    } else if (strcmp("necesitoquemeapapachen", arg) == 0) {
        return showBurro;
    } else if (strcmp("backgroundTest", arg) == 0) {
        *flag = 1;
        return stayAlive;
    } else if (strcmp("messages", arg) == 0) {
        return startMessagesDemo;
    } else if (strcmp("philosophers", arg) == 0) {
        return initFilofochos;
    } else if (strcmp("test", arg) == 0) {
        return hinchaHuevos;
    }else if (strcmp("priority", arg) == 0) {
        return schedulerDemo;
    } else if (strcmp("pipesdemo", arg) == 0) {
        return initPipesDemo;
    } else if (strcmp("echoInput", arg) == 0) {
        return echoInput;
    } else if (strcmp("remark", arg) == 0) {
        if(argc == 0){
            *flag = -2;
            return 0;
        }
        return findAndRemark;
    }
    return 0;
}