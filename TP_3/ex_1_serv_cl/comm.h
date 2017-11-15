#ifndef COMM
#define COMM

#define     PROJECT_ID           2
#define     SOURCE_FILE          "./server.c"

#define     MSG_GET_FLAGS        0

#define     CLIENT_MSG_TYPE      1
#define     SERVER_MSG_TYPE      2

#define     ERR_ARG              1
#define     ERR_KEY_GEN          2
#define     ERR_GET_QUEUE        3
#define     ERR_SEND_VAL         4
#define     ERR_QUEUE_RESP       5

typedef struct msgbuf 
{
    long mtype ;
    int  value ;
} payload ;

#endif 