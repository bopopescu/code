#include <stdio.h>
#include <stdint.h>

// fifo
// macro to create a pointer FIFO 
#define AddPointerFifo(NAME,SIZE,TYPE,SUCCESS,FAIL)     \
    TYPE volatile *NAME ## PutPt;                       \
    TYPE volatile *NAME ## GetPt;                       \
    TYPE static NAME ## Fifo [SIZE];                    \
    void NAME ## Fifo_Init(void){                       \
    NAME ## PutPt = NAME ## GetPt = &NAME ## Fifo[0];   \
    }                                                   \
    int NAME ## Fifo_Put (TYPE data){                   \
        TYPE volatile *nextPutPt;                       \
            nextPutPt = NAME ## PutPt + 1;              \
            if(nextPutPt == &NAME ## Fifo[SIZE]){       \
                nextPutPt = &NAME ## Fifo[0];           \
            }                                           \
        if(nextPutPt == NAME ## GetPt ){                \
            return(FAIL);                               \
        }                                               \
        else{                                           \
            *( NAME ## PutPt ) = data;                  \
                NAME ## PutPt = nextPutPt;              \
                return(SUCCESS);                        \
        }                                               \
    }                                                   \
    int NAME ## Fifo_Get (TYPE *datapt){                \
        if( NAME ## PutPt == NAME ## GetPt ){           \
            return(FAIL);                               \
        }                                               \
        *datapt = *( NAME ## GetPt ## ++);              \
        if( NAME ## GetPt == &NAME ## Fifo[SIZE]){      \
            NAME ## GetPt = &NAME ## Fifo[0];           \
        }                                               \
        return(SUCCESS);                                \
    }


#define AddIndexFifo(NAME,SIZE,TYPE,SUCCESS,FAIL)       \
    uint32_t volatile NAME ## PutI;                     \
    uint32_t volatile NAME ## GetI;                     \
    TYPE static NAME ## Fifo [SIZE];                    \
    void NAME ## Fifo_Init(void){                       \
        NAME ## PutI = NAME ## GetI = 0;                \
    }                                                   \
    int NAME ## Fifo_Put (TYPE data){                   \
        if(( NAME ## PutI NAME ## GetI ) & ~(SIZE-1)){  \
            return(FAIL);                               \
        }                                               \
        NAME ## Fifo[ NAME ## PutI &(SIZE-1)] = data;   \
        NAME ## PutI ## ++;                             \
        return(SUCCESS);                                \
    }                                                   \
    int NAME ## Fifo_Get (TYPE *datapt){                \
        if( NAME ## PutI == NAME ## GetI ){             \
            return(FAIL);                               \
        }                                               \
        *datapt = NAME ## Fifo[ NAME ## GetI &(SIZE-1)];\
        NAME ## GetI ## ++;                             \
        return(SUCCESS);                                \
    }                                                   \
    uint16_t NAME ## Fifo_Size (void){                  \
        return ((uint16_t)( NAME ## PutI NAME ## GetI ));\
    } 


#define Collect() (Histogram[TxFifo_Size()]++;)

void TxFifo_Init();
int TxFifo_Get();
void TxFifo_Put();
int TxFifo_Size();


int main()
{
    //AddPointerFifo(Rx, 20, uint16_t, 1, 0)
    //AddIndexFifo(Tx, 32, int32_t, 0, 1) 

}
