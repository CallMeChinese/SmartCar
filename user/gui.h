/**
 * @file gui.h
 * @author BadCodeBuilder
 * @brief 
 * @version 0.1
 * @date 2019-11-28
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#define KEY_UP          1       // Upper
#define KEY_DOWN        2       // Down
#define KEY_LEFT        3       // Left
#define KEY_RIGHT       4       // Right
#define KEY_MID         5       // Select
#define MAX_PAGE_COUNT  3

extern int keyState;
extern int menuRow;
extern int temp1;
extern int temp2;
extern int temp3;
extern int temp4;
extern int temp5;
extern int temp6;
extern int temp7;
extern int key_counter;
extern int key_temp;
extern const unsigned char row_pos[5];
extern int clearCount;

extern void menu();

extern void flashWrite();
extern void SignMove();

extern int keyCheck(void);

extern void FlashValueOperate();

extern void MenuShow3();

extern void MenuShow2();

extern void flashDataSave();
extern void flashDataRead();
