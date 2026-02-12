#ifndef FIFO_H_
#define FIFO_H_

//#include "Can/Can.h"
#include <stdint.h>

/* Fifo can frames queue max size  */ 
#define MAX_CANFIFO 50
/* max divider */
#define MAX_DIVIDER 10
/* min divider */
#define MIN_DIVIDER 1
/* timer max set value */
#define TIMER_SET_V 10



/**
 * can frame struct
 */
typedef struct
{
    uint32_t  id;
    uint16_t  dlc;
    uint32_t  can_base;
    uint16_t  data[4];
} CAN_MSG_t;

/**
* @struct Fifo
* @brief basic Fifo buffer for CAN_MSG frames
*/
typedef struct Fifo 
{
    CAN_MSG_t frames[MAX_CANFIFO];  /*< CAN_MSG frames array */
    uint32_t size;  /*< Fifo buffer actual size */
    uint16_t divider; /*< divider value */
    uint16_t def_divider; /*< default divider value */
    uint16_t divider_idx; /*< Fifo buff divider index */
    uint16_t timer; /*< timer for Fifo*/
    int (*pushFrame)(struct Fifo*, CAN_MSG_t* a, CAN_MSG_t* b); /*< pushFrame method pointer */
    void (*timerSet)(struct Fifo*); /*< timerSet method pointer */
    void (*timerTick)(struct Fifo*); /*< timer tick method pointer */
    void(*setDefDivider)(struct Fifo*, uint16_t a); /*< set default divide method pointer */
    void(*pauseMode)(struct Fifo*); /*< pause_mode method pointer*/
} Fifo;

/**
 * @brief Constructor for Fifo class
 * 
 * @param canbuffer self
 */
void ctorFifo(Fifo *self);
//
/**
 *  @brief Push can frame to Fifo,
 *  @param *canmsg - pointer to new frame,
 *  @param *canmsgTx - pointer to output frame,
 *  @param *canbuffer - pointer to Fifo struct,
 *  @param divider -  buffer send divider
*   @return  int, 1 if Fifo is overflow and last frame is as canmsgTx or 0
 */ 
int pushFrame(Fifo *self, CAN_MSG_t *canmsg, CAN_MSG_t *canmsgTx);
//
/**
 * @brief Set the timer to start counting
 * 
 * @param canbuffer pointer to Fifo struct,
 */
void timerSet(Fifo *self);
//
/**
 * @brief tick the Fifo timer, down
 * 
 * @param canbuffer pointer to Fifo struct,
 */
void timerTick(Fifo *self);

/**
 * @brief Set the default divider value for fifo struct
 * 
 * @param self fifo struct
 */
void setDefDivider(Fifo *self, uint16_t value);

/**
 * @brief Pause Fifo output, will be overide by set_divider
 * 
 * @param self 
 */
void pauseMode(Fifo *self);

/**
 * @brief Set the divider value according to timer state
 * 
 * @param canbuffer pointer to Fifo struct,
 * @note static func
 */
static void setDivider(Fifo *canbuffer);
//
#endif // FIFO_H_
