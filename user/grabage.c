// uint8_t* options[] = {"Bin Image", "Pro Image", "Threshold", "Forward", "Rotate", "CurOffset", "PreOffset", "SumOffset"};
// int32_t* values[] = {&threshold, &forwardSpeed, &rotateSpeed, &curError, &preError, &sumError};
// uint8_t optionsCount = sizeof(options)/sizeof(char*);
// uint8_t pagesCount = 2;

// int8_t optionIndex = 0;         // The index of option, from 0 to max-1
// uint8_t imgOptionsCount = 2;    // The number image display options
// uint8_t page = 0;               // Current page index
// uint8_t option = 0;             // Current option index in one page
// uint8_t pageType = MENU_PAGE;   // Current page type
// uint8_t detailType;             // Current detailed page type
// uint8_t menuRow = 1;            /* 记录当前是第几排 值可以为1、2、3、4、5  修改了排数的除外 */


// void DispMenuPage(void)
// {
//     uint8_t startOption = page*MAX_OPTION_COUNT;
//     uint8_t i;
//     char buffer[9];
//     sprintf(buffer, "Page %d/%d", page+1, pagesCount);
//     OLED_DispString_1206(13, row_pos[0], buffer, 1, ALIGN_LEFT);
//     for (i = 0; i < MAX_OPTION_COUNT; ++i)
//     {
//         if (startOption + i >= optionsCount)
//         {
//             break;
//         }
//         else
//         {
//             OLED_DispString_1206(13, row_pos[i+1], options[startOption+i], 1, ALIGN_LEFT);
//         }
//     }
//     OLED_DispString_1206(3, row_pos[option+1], "$", 1, ALIGN_LEFT);
// }

// void DispDetailPage(void)
// {
//     uint8_t index;
//     OLED_DispString_1206(13, row_pos[0], options[optionIndex], 1, ALIGN_LEFT);
//     if (optionIndex < imgOptionsCount)
//     {
//         detailType = IMAGE_PAGE;
//         index = optionIndex;
//         switch (index)
//         {
//         case 0:
//         {
//             DispBinImg();
//             break;
//         }
//         case 1:
//         {
//             DispProcessedImg(true);
//         }
//         default:
//         {
//             break;
//         }
//         }
//     }
//     else
//     {
//         detailType = VALUE_PAGE;
//         index = optionIndex - imgOptionsCount;
//         OLED_DispInt_1206(13, row_pos[2], *values[index], 1, ALIGN_RIGHT);
//     }
    
// }

// /**
//  * @brief 
//  * 
//  */
// void menu()
// {
//     if (pageType == MENU_PAGE)
//     {
//         DispMenuPage();
//     }
//     else if (pageType == DETAIL_PAGE)
//     {
//         DispDetailPage();
//     }
//     else
//     {

//     }
//     OLED_Refresh_Gram();
//     keyState = keyCheck();
//     if (keyState) OLED_Clear();
    
//     SignMove();
//     // keyState=0;
// }

// /**
//  * @brief 
//  * 
//  */
// void SignMove()
// {
//     switch (keyState)
//     {
//     case KEY_UP:
//     {
//         GPIO_ResetBit(HW_GPIOB, 11);
//         GPIO_ResetBit(HW_GPIOB, 17);
//         if (pageType == MENU_PAGE)
//         {
//             --optionIndex;
//             if (optionIndex < 0) optionIndex += optionsCount;
//             page = optionIndex / MAX_OPTION_COUNT;
//             option = optionIndex % MAX_OPTION_COUNT;
//         }
//         else if (pageType == DETAIL_PAGE)
//         {

//         }
//         else
//         {
            
//         }
//         break;
//     }
//     case KEY_DOWN:
//     {
//         GPIO_ResetBit(HW_GPIOB, 11);
//         GPIO_SetBit(HW_GPIOB, 17);
//         if (pageType == MENU_PAGE)
//         {
//             ++optionIndex;
//             if (optionIndex >= optionsCount) optionIndex -= optionsCount;
//             page = optionIndex / MAX_OPTION_COUNT;
//             option = optionIndex % MAX_OPTION_COUNT;
//         }
//         else if (pageType == DETAIL_PAGE)
//         {

//         }
//         else
//         {
            
//         }
//         break;
//     }
//     case KEY_LEFT:
//     {
//         GPIO_SetBit(HW_GPIOB, 11);
//         GPIO_ResetBit(HW_GPIOB, 17);
//         if (pageType == MENU_PAGE)
//         {
//             if (optionIndex == 0) optionIndex = optionsCount - 1;
//             else
//             {
//                 optionIndex -= MAX_OPTION_COUNT;
//                 if (optionIndex < 0) optionIndex = 0;
//             }
//             page = optionIndex / MAX_OPTION_COUNT;
//             option = optionIndex % MAX_OPTION_COUNT;
//         }
//         else if (pageType == DETAIL_PAGE)
//         {

//         }
//         else
//         {
            
//         }
//         break;
//     }
//     case KEY_RIGHT:
//     {
//         GPIO_SetBit(HW_GPIOB, 11);
//         GPIO_SetBit(HW_GPIOB, 17);
//         if (pageType == MENU_PAGE)
//         {
//             if (optionIndex == optionsCount - 1) optionIndex = 0;
//             else
//             {
//                 optionIndex += MAX_OPTION_COUNT;
//                 if (optionIndex >= optionsCount) optionIndex = optionsCount - 1;
//             }
//             page = optionIndex / MAX_OPTION_COUNT;
//             option = optionIndex % MAX_OPTION_COUNT;
//         }
//         else if (pageType == DETAIL_PAGE)
//         {

//         }
//         else
//         {
            
//         }
//         break;
//     }
//     case KEY_MID:
//     {
//         if (pageType == MENU_PAGE)
//         {
//             pageType = DETAIL_PAGE;
//         }
//         else if (pageType == DETAIL_PAGE)
//         {
//             pageType = MENU_PAGE;
//         }
//         else
//         {
            
//         }
//     }
//     default:
//     {
//         break;
//     }
//     }
// }

// /**
//  * @brief 
//  * 
//  */
// void FlashValueOperate()
// {
//     /* 修改每个变量的值 */

//     if (keyState != 0)
//         OLED_Clear();
//     if (page == 2) /* 菜单第一页 */
//     {
//         switch (menuRow) /* 判断行数 */
//         {                /* 妹有第零行 */
//         case 0:
//         {
//         }
//         break;
//         /* 从case1开始  第一行 */
//         case 1:
//         {
//             if (keyState == KEY_LEFT)
//             {
//                 flashData[0]--;
//             }
//             else if (keyState == KEY_RIGHT)
//             {
//                 flashData[0]++;
//             }
//         }
//         break;
//         /*  第二行 */
//         case 2:
//         {
//             if (keyState == KEY_LEFT)
//             {
//                 flashData[1]--;
//             }
//             else if (keyState == KEY_RIGHT)
//             {
//                 flashData[1]++;
//             }
//         }
//         break;
//         case 3:
//         {
//             if (keyState == KEY_LEFT)
//             {
//             }
//             else if (keyState == KEY_RIGHT)
//             {
//             }
//         }
//         break;
//         case 4:
//         {
//             if (keyState == KEY_LEFT)
//             {
//             }
//             else if (keyState == KEY_RIGHT)
//             {
//             }
//         }
//         break;
//         }
//     }
// }

// /*
//  * 以下为flash操作的说明
//  * 首先在init了以后一定要
//  *        FLASH_Init();   flash初始化
//  *       flashDataRead();
//  * flash数据是存在内存的一个扇区当中。原理掠过直接将操作
//  * 我们的要存取的数据存在flashData[flashInfoNum]里面。可以把它当作一个中转站。比如A是我们想要进行flash存取的数据每次开机后，
//  * flashDataRead()函数执行过后。我们再把flashData[0]赋值给A，这是读取。在写入时，比如A的值在程序运行过程中发生了变化，先把A赋值给flashData[0]，
//  * 再执行flashDataSave()函数。那么我们就把我们想要存取的数据写进了对应扇区。每次我们想要存取的数据都必须有一一对应。
//  * 对于初次进行flash操作一定要先写入一遍在进行存取，即先不执行flashDataRead();，而直接执行flashDataSave()
//  * 之后在每次开机先进行flashDataRead();
//  * flash寿命有限，严禁在循环，中断中使用！
//  * flash寿命有限，严禁在循环，中断中使用！
//  * flash寿命有限，严禁在循环，中断中使用！
//  * flash寿命有限，严禁在循环，中断中使用！
//  * 一般是按下某个按键，进行写入存储。实现断电存数据
//  * 一般现在程序开始main()函数开始时执行flashDataRead()函数读取断电前的数据。再设置一个按钮，当按下按钮时执行flashDataSave()函数
//  */

// /* flash写入 */
// void flashDataSave()
// {
//     FLASH_EraseSector(245);
//     FLASH_EraseSector(244);
//     FLASH_EraseSector(243);                                          /* 先擦除扇区 ，一个扇区2kb擦三个够了吧，不够自己再开*/
//     FLASH_WriteSector(245, (uint8_t *)flashData, sizeof(flashData)); /* 存数据 */
// }

// /* flash存取 */
// void flashDataRead()
// {
//     Flash_read(245, 0, sizeof(flashData), (uint8_t *)flashData);
// }
