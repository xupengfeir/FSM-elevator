/* 
    以电梯为例实现电梯的有限状态机（常规方法实现）
 */

#include <stdio.h> 
#include <conio.h>
#include "stdlib.h"
#include <string.h>
#include <ctype.h>
#include "math.h"

/* 状态机状态编码 */
enum state_codes {
    _idle=0,          // 空闲态
    _goingUp=1,       // 上升态
    _goingDown=2,     // 下降态
    _atTop=3,         // 顶层态
    _atBottom=4,      // 底层态
    _malFunction=5,   // 检修态
    _end=6,            // 终止态
    _init
};

/* 状态机驱动事件 */
enum event_codes {
    event_up=0,             // 上升 
    event_down=1,           // 下降
    event_idle=2,           // 空闲态
    event_mal_enter=3,      // 进入检修
    event_mal_completed=4,  // 检修完毕
    event_up_top=5,         // 上升至top
    event_down_bottom=6,    // 下降至bottom
    event_exit=7,           // 退出运行
    event_halt=8,            // 停止
    event_init=9
};
/* 状态表结构体 */
typedef struct FsmTable_s{
    int event;     // 事件
    int CurState;   // 当前状态
    int (*eventAction)();  // 函数指针
    int NextState;  // 下一个状态
}FsmTable_t;
/* 状态机 */
typedef struct FSM_s{
    FsmTable_t* FsmTable;   // 指向的状态表
    int curState;       // FSM当前所处的状态
}Fsm_t;

#define TOP_FLOOR   21
#define BOTTOM_FLOOR    1

int target_floor_num = 1;       // 目标楼层
int current_floor_num = 1;      // 当前所处楼层
int accumulated_floor_num = 0;  // 电梯运行层数累加值，作为进入检修状态的触发标志
int g_max_num;                  //状态表里含有的状态个数
int nextEvent = event_init;                  // 下一个事件

int event_atIdle(void){
    printf("Idle! Current floor is %d.\n", current_floor_num);
    if(accumulated_floor_num > 1000)    // 达到检修条件，在空闲条件的判断优先级最高
        return event_mal_enter;
    printf("enter what do you want to do?\n \
            if runing, enter floor number in [1, %d]. if exit, enter [exit]. if Overhaul, enter [mal]\n", TOP_FLOOR);
    char userInput[10];     // 临时变量
    fgets(userInput, sizeof(userInput), stdin);
    userInput[strcspn(userInput, "\n")] = '\0';     // 去除换行符

    if(isdigit(userInput[0])){    // 判断前两个字符是否为数字
        // printf("Input target floor number in [1, %d] (which floor you want to go?): \n", TOP_FLOOR);
        target_floor_num = atoi(userInput);     // 转换成整数
        printf("%d\n", target_floor_num);
        if(target_floor_num > TOP_FLOOR || target_floor_num < BOTTOM_FLOOR)
            return event_halt;
        else if(current_floor_num < target_floor_num)
            return event_up;
        else if(current_floor_num > target_floor_num)
            return event_down;
        else if(current_floor_num == target_floor_num)
            return event_halt;        
    }
    else if(strcmp(userInput, "exit") == 0 ){    // 退出运行
        printf("Exit.\n");
        return event_exit;
    }
    else if(strcmp(userInput, "mal") == 0){    // 进入检修
        printf("Enter for maintenance.\n");
        return event_mal_enter; 
    }
    else{
        printf("invalid input, please enter as required.\n");
        return event_halt;  // 停止，不动作
    }
}
int event_goingUp(void){
    if(TOP_FLOOR == current_floor_num)
        return event_up_top;
    // else if(current_floor_num == target_floor_num)
    //     return event_halt;
    while(current_floor_num != target_floor_num){
        current_floor_num += 1;
        accumulated_floor_num += 1;
        printf("Going up! Floor number is %d\n", current_floor_num);
    }
    return event_halt;
}
int event_goingDown(void){
    if(BOTTOM_FLOOR == current_floor_num)
        return event_down_bottom;
    while(current_floor_num != target_floor_num){
        current_floor_num -= 1;
        accumulated_floor_num += 1;
        printf("Going up! Floor number is %d\n", current_floor_num);
    }
    return event_halt;
}
int event_atTop(void){
    printf("At top! current_floor_number = %d\n", current_floor_num);

    if(TOP_FLOOR == current_floor_num){
        if(accumulated_floor_num > 1000)    // 达到检修条件，在空闲条件的判断优先级最高
            return event_mal_enter;
        printf("enter what do you want to do?\n \
                if runing, enter floor number in [1, %d]. if exit, enter [exit]. if Overhaul, enter [mal]\n", TOP_FLOOR);
        char userInput[10];     // 临时变量
        fgets(userInput, sizeof(userInput), stdin);
        userInput[strcspn(userInput, "\n")] = '\0';     // 去除换行符

        if(isdigit(userInput[0])){    // 判断前两个字符是否为数字
            // printf("Input target floor number in [1, %d] (which floor you want to go?): \n", TOP_FLOOR);
            target_floor_num = atoi(userInput);     // 转换成整数
            printf("%d\n", target_floor_num);
            if(target_floor_num > TOP_FLOOR || target_floor_num < BOTTOM_FLOOR)
                return event_halt;
            // else if(current_floor_num < target_floor_num)
            //     return event_up;
            else if(current_floor_num > target_floor_num)
                return event_down;
            else if(current_floor_num == target_floor_num)
                return event_halt;
            else
                return event_halt;       
        }
        else{
            printf("invalid input, please enter as required.\n");
            return event_halt;  // 停止，不动作
        }        
    }
}
int event_atBottom(void){
    printf("At bottom! current_floor_number = %d\n", current_floor_num);

    if(BOTTOM_FLOOR == current_floor_num){
        if(accumulated_floor_num > 1000)    // 达到检修条件，在空闲条件的判断优先级最高
            return event_mal_enter;
        printf("enter what do you want to do?\n \
                if runing, enter floor number in [1, %d]. if exit, enter [exit]. if Overhaul, enter [mal]\n", TOP_FLOOR);
        char userInput[10];     // 临时变量
        fgets(userInput, sizeof(userInput), stdin);
        userInput[strcspn(userInput, "\n")] = '\0';     // 去除换行符

        if(isdigit(userInput[0])){    // 判断前两个字符是否为数字
            // printf("Input target floor number in [1, %d] (which floor you want to go?): \n", TOP_FLOOR);
            target_floor_num = atoi(userInput);     // 转换成整数
            printf("%d\n", target_floor_num);
            if(target_floor_num > TOP_FLOOR || target_floor_num < BOTTOM_FLOOR)
                return event_halt;
            else if(current_floor_num < target_floor_num)
                return event_up;
            // else if(current_floor_num > target_floor_num)
            //     return evnet_down;
            else if(current_floor_num == target_floor_num)
                return event_halt;
            else
                return event_halt;       
        }
        else{
            printf("invalid input, please enter as required.\n");
            return event_halt;  // 停止，不动作
        }        
    }
}
int event_malFunction(void){
    printf("Elevator needs maintanence!\n");
    printf("accumulated_floor_number = %d\n", accumulated_floor_num);
    /* 
        检修电梯 
    */
    return event_mal_completed;
}
int event_end(void){
    printf("Exit!\n");
    return event_idle;
}

/* 状态机注册 */
void FSM_regist(Fsm_t* pFsm, FsmTable_t* pTable){
    pFsm->FsmTable = pTable;
}
/* 状态迁移 */
void FSM_stateTransfer(Fsm_t* pFsm, int state){
    pFsm->curState = state;
}
/* 事件处理 */
int FSM_EventHandle(Fsm_t* pFsm, int event){
    FsmTable_t* pActTable = pFsm->FsmTable;
    int (*eventActFun)() = NULL;   // 函数指针初始为空
    int NextState;
    int CurState = pFsm->curState;
    int flag = 0;   // 判断标志

    /* 遍历状态表，获取当前动作函数 */
    for(int i = 0; i < g_max_num; i++){

        // 当且仅当当前状态下来个指定事件，才执行
        if(event == pActTable[i].event && CurState == pActTable[i].CurState){
            flag = 1;
            eventActFun = pActTable[i].eventAction;
            NextState = pActTable[i].NextState;
            break;  // 找到对应状态 及事件  退出遍历
        }
    }
    if(flag){
        /* 动作执行 */
        if(eventActFun)
            nextEvent = eventActFun();  // 返回下一个事件
        
        /* 跳转到下一个状态 */
        FSM_stateTransfer(pFsm, NextState);
    }
    return nextEvent;
}

/* 状态表 */
FsmTable_t ElevatorTable[] = 
{
    //{到来的事件， 当前状态， 将要执行的函数， 下一个状态}
    { event_up,            _idle,       event_goingUp,      _goingUp    },
    { event_down,          _idle,       event_goingDown,    _goingDown  },
    { event_exit,          _idle,       event_end,          _end        },
    { event_init,          _init,       event_atIdle,       _idle       },
    { event_halt,          _idle,       event_atIdle,       _idle       },   
    { event_idle,          _end ,       event_end,          _idle       },
    { event_mal_completed, _malFunction,event_atIdle,       _idle       },
    { event_mal_enter,     _idle ,      event_malFunction,  _malFunction},
    { event_halt,          _goingUp,    event_atIdle,       _idle       },
    { event_halt,          _goingDown,  event_atIdle,       _idle       },
    { event_up_top,        _goingUp,    event_atTop,        _atTop      },
    { event_down_bottom,   _goingDown,  event_atBottom,     _atBottom   },
    { event_down,          _atTop,      event_goingDown,    _goingDown  },
    { event_up,            _atBottom,   event_goingUp,      _goingUp    },
    // { event_halt,          _atTop,      event_atTop,        _atTop      },
    // { event_halt,          _atBottom,   event_atBottom,     _atBottom   },   
};


#define ENTRY_STATE _init
#define END_STATE   _end

/* 初始化状态机 */
void InitFsm(Fsm_t* pFsm){
    g_max_num = sizeof(ElevatorTable) / sizeof(FsmTable_t);
    pFsm->curState = ENTRY_STATE;
    FSM_regist(pFsm, ElevatorTable);
}

int main(){
    
    printf("Start the elevator!\n");
    Fsm_t fsm;
    InitFsm(&fsm);
    int event = nextEvent;     // 初始化驱动事件
    for(;;){
        printf("event_%d is coming...\n", event);
        event = FSM_EventHandle(&fsm, event);
        printf("fsm current event %d\n", event);
        printf("fsm current state %d\n", fsm.curState);
        if(END_STATE == fsm.curState)
            break;
    }
    printf("END.");
    getch();    // enter space
    getch();
    getch();
    return 0;
}