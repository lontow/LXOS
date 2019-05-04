#ifndef BIO_H
#define BIO_H
#include "types.h"
#include "buf.h"
void            binit(void);
struct buf*     bread(uint, uint);
void            brelse(struct buf*);
void            bwrite(struct buf*);
#endif
