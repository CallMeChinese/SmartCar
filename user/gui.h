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

#define KEY_UP              1       // Upper
#define KEY_DOWN            2       // Down
#define KEY_LEFT            3       // Left
#define KEY_RIGHT           4       // Right
#define KEY_MID             5       // Select

#define MENU_PAGE           0
#define DETAIL_PAGE         1

#define IMAGE_PAGE          0
#define VALUE_PAGE          1

#define MAX_OPTION_COUNT    4

int keyCheck(void);

void DispMenuPage(void);
void menu();
void SignMove();
void MenuPage();
void DetailPage();


extern void FlashValueOperate();
extern void flashDataSave();
extern void flashDataRead();
