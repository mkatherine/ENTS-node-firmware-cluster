#ifndef STM32_LIB_STORAGE_INCLUDE_EPSDUMMY_H_
#define STM32_LIB_STORAGE_INCLUDE_EPSDUMMY_H_

/**
 * @brief Loads the last saved page state
 * 
 */
void PageInit(void);

/**
 * @brief Get the front page
 * 
 */
void PageFront(void);

/**
 * @brief Get the back page
 * 
 */
void PageBack(void);

/**
 * @brief Push a page to the front of the linked list
 */
void PagePushFront(void);

/**
 * @brief Pop a page from the front of the linked list
 * 
 * @param 
 */
void PagePopFront(void);

/**
 * @brief Push a page to the back of the linked list
 * 
 */
void PagePushBack(void);

/**
 * @brief Pop a page from the back of the linked list
 * 
 */
void PagePopBack(void);

/**
 * @brief Get the number of elements in the linked list
 * 
 * @return Number of elements in the linked list
 */
unsigned int PageSize(void);

/**
 * @brief Check if linked list is empty
 * 
 * @return true if linked list is empty, false otherwise
 */
bool PageIsEmpty(void);

/**
 * @brief Saves the current page state 
 * 
 */
void PageSave(void);


#endif // STM32_LIB_STORAGE_INCLUDE_EPSDUMMY_H_