/* 
    �Ե���Ϊ��ʵ�ֵ��ݵ�����״̬�������淽��ʵ�֣�
 */

#include <stdio.h> 
#include <conio.h>
#include "stdlib.h"
#include <string.h>
#include <ctype.h>
#include "math.h"

/* ״̬��״̬���� */
enum state_codes {
    _idle=0,          // ����̬
    _goingUp=1,       // ����̬
    _goingDown=2,     // �½�̬
    _atTop=3,         // ����̬
    _atBottom=4,      // �ײ�̬
    _malFunction=5,   // ����̬
    _end=6,            // ��ֹ̬
    _init
};

/* ״̬�������¼� */
enum event_codes {
    event_up=0,             // ���� 
    event_down=1,           // �½�
    event_idle=2,           // ����̬
    event_mal_enter=3,      // �������
    event_mal_completed=4,  // �������
    event_up_top=5,         // ������top
    event_down_bottom=6,    // �½���bottom
    event_exit=7,           // �˳�����
    event_halt=8,            // ֹͣ
    event_init=9
};
/* ״̬��ṹ�� */
typedef struct FsmTable_s{
    int event;     // �¼�
    int CurState;   // ��ǰ״̬
    int (*eventAction)();  // ����ָ��
    int NextState;  // ��һ��״̬
}FsmTable_t;
/* ״̬�� */
typedef struct FSM_s{
    FsmTable_t* FsmTable;   // ָ���״̬��
    int curState;       // FSM��ǰ������״̬
}Fsm_t;

#define TOP_FLOOR   21
#define BOTTOM_FLOOR    1

int target_floor_num = 1;       // Ŀ��¥��
int current_floor_num = 1;      // ��ǰ����¥��
int accumulated_floor_num = 0;  // �������в����ۼ�ֵ����Ϊ�������״̬�Ĵ�����־
int g_max_num;                  //״̬���ﺬ�е�״̬����
int nextEvent = event_init;                  // ��һ���¼�

int event_atIdle(void){
    printf("Idle! Current floor is %d.\n", current_floor_num);
    if(accumulated_floor_num > 1000)    // �ﵽ�����������ڿ����������ж����ȼ����
        return event_mal_enter;
    printf("enter what do you want to do?\n \
            if runing, enter floor number in [1, %d]. if exit, enter [exit]. if Overhaul, enter [mal]\n", TOP_FLOOR);
    char userInput[10];     // ��ʱ����
    fgets(userInput, sizeof(userInput), stdin);
    userInput[strcspn(userInput, "\n")] = '\0';     // ȥ�����з�

    if(isdigit(userInput[0])){    // �ж�ǰ�����ַ��Ƿ�Ϊ����
        // printf("Input target floor number in [1, %d] (which floor you want to go?): \n", TOP_FLOOR);
        target_floor_num = atoi(userInput);     // ת��������
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
    else if(strcmp(userInput, "exit") == 0 ){    // �˳�����
        printf("Exit.\n");
        return event_exit;
    }
    else if(strcmp(userInput, "mal") == 0){    // �������
        printf("Enter for maintenance.\n");
        return event_mal_enter; 
    }
    else{
        printf("invalid input, please enter as required.\n");
        return event_halt;  // ֹͣ��������
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
        if(accumulated_floor_num > 1000)    // �ﵽ�����������ڿ����������ж����ȼ����
            return event_mal_enter;
        printf("enter what do you want to do?\n \
                if runing, enter floor number in [1, %d]. if exit, enter [exit]. if Overhaul, enter [mal]\n", TOP_FLOOR);
        char userInput[10];     // ��ʱ����
        fgets(userInput, sizeof(userInput), stdin);
        userInput[strcspn(userInput, "\n")] = '\0';     // ȥ�����з�

        if(isdigit(userInput[0])){    // �ж�ǰ�����ַ��Ƿ�Ϊ����
            // printf("Input target floor number in [1, %d] (which floor you want to go?): \n", TOP_FLOOR);
            target_floor_num = atoi(userInput);     // ת��������
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
            return event_halt;  // ֹͣ��������
        }        
    }
}
int event_atBottom(void){
    printf("At bottom! current_floor_number = %d\n", current_floor_num);

    if(BOTTOM_FLOOR == current_floor_num){
        if(accumulated_floor_num > 1000)    // �ﵽ�����������ڿ����������ж����ȼ����
            return event_mal_enter;
        printf("enter what do you want to do?\n \
                if runing, enter floor number in [1, %d]. if exit, enter [exit]. if Overhaul, enter [mal]\n", TOP_FLOOR);
        char userInput[10];     // ��ʱ����
        fgets(userInput, sizeof(userInput), stdin);
        userInput[strcspn(userInput, "\n")] = '\0';     // ȥ�����з�

        if(isdigit(userInput[0])){    // �ж�ǰ�����ַ��Ƿ�Ϊ����
            // printf("Input target floor number in [1, %d] (which floor you want to go?): \n", TOP_FLOOR);
            target_floor_num = atoi(userInput);     // ת��������
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
            return event_halt;  // ֹͣ��������
        }        
    }
}
int event_malFunction(void){
    printf("Elevator needs maintanence!\n");
    printf("accumulated_floor_number = %d\n", accumulated_floor_num);
    /* 
        ���޵��� 
    */
    return event_mal_completed;
}
int event_end(void){
    printf("Exit!\n");
    return event_idle;
}

/* ״̬��ע�� */
void FSM_regist(Fsm_t* pFsm, FsmTable_t* pTable){
    pFsm->FsmTable = pTable;
}
/* ״̬Ǩ�� */
void FSM_stateTransfer(Fsm_t* pFsm, int state){
    pFsm->curState = state;
}
/* �¼����� */
int FSM_EventHandle(Fsm_t* pFsm, int event){
    FsmTable_t* pActTable = pFsm->FsmTable;
    int (*eventActFun)() = NULL;   // ����ָ���ʼΪ��
    int NextState;
    int CurState = pFsm->curState;
    int flag = 0;   // �жϱ�־

    /* ����״̬����ȡ��ǰ�������� */
    for(int i = 0; i < g_max_num; i++){

        // ���ҽ�����ǰ״̬������ָ���¼�����ִ��
        if(event == pActTable[i].event && CurState == pActTable[i].CurState){
            flag = 1;
            eventActFun = pActTable[i].eventAction;
            NextState = pActTable[i].NextState;
            break;  // �ҵ���Ӧ״̬ ���¼�  �˳�����
        }
    }
    if(flag){
        /* ����ִ�� */
        if(eventActFun)
            nextEvent = eventActFun();  // ������һ���¼�
        
        /* ��ת����һ��״̬ */
        FSM_stateTransfer(pFsm, NextState);
    }
    return nextEvent;
}

/* ״̬�� */
FsmTable_t ElevatorTable[] = 
{
    //{�������¼��� ��ǰ״̬�� ��Ҫִ�еĺ����� ��һ��״̬}
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

/* ��ʼ��״̬�� */
void InitFsm(Fsm_t* pFsm){
    g_max_num = sizeof(ElevatorTable) / sizeof(FsmTable_t);
    pFsm->curState = ENTRY_STATE;
    FSM_regist(pFsm, ElevatorTable);
}

int main(){
    
    printf("Start the elevator!\n");
    Fsm_t fsm;
    InitFsm(&fsm);
    int event = nextEvent;     // ��ʼ�������¼�
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