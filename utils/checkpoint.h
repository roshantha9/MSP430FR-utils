/*
 * checkpoint.h
 *
 *  Created on: Sep 4, 2018
 *      Author: Rosh
 */

#ifndef UTILS_CHECKPOINT_H_
#define UTILS_CHECKPOINT_H_

#define CHECKPOINT_SRAM_SIZE 0x2000

void Checkpoint_backup(void);
void Checkpoint_restore(void);

#endif /* UTILS_CHECKPOINT_H_ */
