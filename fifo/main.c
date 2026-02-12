#include <stdio.h>
#include <stdint.h>

// fifo can frames queue max size 
#define MAX_CANFIFO 5



// can frames fifo struct
typedef struct fifo 
{
    CAN_MSG_t frames[MAX_CANFIFO];
    uint32_t size;
    /* data */
} fifo;

// prototypes
int push_frame(CAN_MSG_t *canmsg, CAN_MSG_t *canmsgTx, fifo *canbuffer);


int main(void)
{
    // fifo queue
    fifo msg_queue = {.size = 0};
    // output msg
    CAN_MSG_t out_msg;
    // counter
    int cnt = 0;
    // test loop
    while (cnt < 100)
    {
        // new msg to put in queue
        CAN_MSG_t new_msg = {0x180ddb00+cnt};
        // put new message 
        if (push_frame(&new_msg, &out_msg, &msg_queue) == 1)
        {
            // print output message
            printf("output message: %i\n", out_msg.id);
        } 
        else
        {
            printf("output message empty, buffer size: %i\n", msg_queue.size);
        }
        cnt++;
    }
    //
    printf("END\n");
    //
    return 0;
}

int push_frame(CAN_MSG_t *canmsg, CAN_MSG_t *canmsgTx, fifo *canbuffer)
{
    // check if pointers exist
    if (canmsg == NULL|| canbuffer == NULL || canmsgTx == NULL)
        return -1;
    // init output value
    int output = 0;
    // max qsize index
    int qindex = MAX_CANFIFO -1;

    // buffer size
    int qsize = canbuffer->size;
    // iterate from last
    for (int i = qsize; i >= 0; i--)
    {
        if (i == qindex) {
            // put last to output
            *canmsgTx = canbuffer->frames[qindex];
            // change output value
            output = 1;
        }
        else if (i == 0)
        {   
            // shift slot one up
            canbuffer->frames[i+1] = canbuffer->frames[i];
            // add new in position 0
            canbuffer->frames[i] = *canmsg;
            if (qsize < qindex)
            {
                // increament queue size
                canbuffer->size++;
            }
            
        }
        else
        {   
            // shift slot one up
            canbuffer->frames[i+1] = canbuffer->frames[i];
        }
    }
    // return output
    return output;
    
}
