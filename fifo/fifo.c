#include "fifo.h"
#include <stddef.h>



/**
 * @brief Constructor for Fifo class
 * 
 * @param canbuffer self
 */
void ctorFifo(Fifo *self)
{   
    // CAN_MSG_t frames[MAX_CANFIFO];
    // self->frames = &frames; /* allocate mem for Fifo array */
    self->size = 0; /* default actual size 0 */
    self->divider = MAX_DIVIDER; /* default divider */
    self->def_divider = MAX_DIVIDER; /* default value for default divider */
    self->divider_idx = 0; /* default divider index */
    self->timer = 0; /* default timer is stop */
    self->pushFrame = pushFrame; /* push can frame to Fifo buffer */
    self->timerSet = timerSet; /* set timer to run method*/
    self->timerTick = timerTick; /* timer tick method*/
    self->setDefDivider = setDefDivider; /* set default divide method */
    self->pauseMode = pauseMode; /* pause method */
}

/**
 *  @brief can frame to Fifo,
 *  @param *canmsg - pointer to new frame,
 *  @param *canmsgTx - pointer to output frame,
 *  @param *canbuffer - pointer to Fifo struct,
 *  @param divider -  buffer send divider
 *   @return  int, 1 if Fifo is overflow and last frame is as canmsgTx or 0
 */ 
int pushFrame(Fifo *self, CAN_MSG_t *canmsg, CAN_MSG_t *canmsgTx) 
{
    // check if pointers exist
    if (canmsg == NULL|| self == NULL || canmsgTx == NULL)
        return -1;
    // init output value
    int output = 0;
    // init tmp output value
    int tmp_out = 0;
    // max qsize index
    int qindex = MAX_CANFIFO -1;

    // buffer size
    int qsize = self->size;
    // iterate from last
    for (int i = qsize; i >= 0; i--)
    {
        if (i == qindex) {
            // put last to output
            *canmsgTx = self->frames[qindex];
            // change output value
            tmp_out = 1;
        }
        else if (i == 0)
        {   
            // shift slot one up
            self->frames[i+1] = self->frames[i];
            // add new in position 0
            self->frames[i] = *canmsg;
            if (qsize < qindex)
            {
                // increament queue size
                self->size++;
            }
            
        }
        else
        {   
            // shift slot one up
            self->frames[i+1] = self->frames[i];
        }
    }
    // check Fifo timer and set divider in struct
    setDivider(self);
    // if divider = 0 then output is always off
    if (self->divider == 0)
    {
        output = 0;
    }   
    // else, divide output flag by specific value
    // at buff overflow
    else if (tmp_out == 1)
    {   
        // increase divider index in struct
        self->divider_idx++;
        // if index div is higher than divider factor
        if (self->divider_idx >= self->divider)
        {
            // reset index divider
            self->divider_idx = 0;
            // put output to 1
            output = 1;
        }
        else 
        {
            // put output to 0
            output = 0;
        }
    }
    else 
    {
        // put output to 0
        output = 0;
    }
    
    // return output
    return output;
    
}

// TODO: func to set different divider values
/**
 * @brief Set the Fifo timer to start counting
 * 
 * @param canbuffer pointer to Fifo struct,
 */
void timerSet(Fifo *self)
{
    // check pointer
    if (self == NULL)
        return;
    // set timer in Fifo struct to max level 
    self->timer = TIMER_SET_V;
}

/**
 * @brief tick (count) the Fifo timer down
 * 
 * @param canbuffer pointer to Fifo struct,
 */
void timerTick(Fifo *self)
{
    // check pointer
    if (self == NULL)
        return;
    // check value of timer
    if (self->timer == 0)
    {
        return;
    }
    // decrese timer in Fifo struct 
    else
    {
        self->timer--;
    }
}

/**
 * @brief Set the default divider value for fifo struct
 * 
 * @param self fifo struct
 */
void setDefDivider(Fifo *self, uint16_t value)
{
    self->def_divider = value;
}

/**
 * @brief Set pause mode for Fifo output, will be overide by set_divider
 * 
 * @param self 
 */
void pauseMode(Fifo *self)
{
    self->def_divider = 0;
}

/**
 * @brief Set the divider value according to timer state
 * 
 * @param canbuffer pointer to Fifo struct,
 * @note static func
 */
static void setDivider(Fifo *canbuffer)
{
    // check pointer
    if (canbuffer == NULL)
        return;
    // check timer, if stop counting  
    if (canbuffer->timer == 0)
    {
        // set to default divider (slower|off)
        canbuffer->divider = canbuffer->def_divider;
    }
    // else set to min divider (faster)
    else 
    {
        canbuffer->divider = MIN_DIVIDER;
    }
}