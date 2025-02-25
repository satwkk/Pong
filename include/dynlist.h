#pragma once

#include "pch.h"

/// =============================================================================
/// ============================== DEFINES ======================================
/// =============================================================================

#define DEBUG 0

/// =============================================================================
/// ============================== STRUCTS ======================================
/// =============================================================================

typedef struct 
{
    uint8_t* data;
    uint8_t* itr;
    size_t element_size;
    size_t size;
    size_t count;
} __dynamic_list;

typedef __dynamic_list dlist;

/// =============================================================================
/// ============================== MACROS =======================================
/// =============================================================================

#define DLIST_PUSH(p_dlist, data_type, value) \
{\
    data_type tmp = value; \
    __push_dlist(p_dlist, &tmp); \
}

#define DLIST_PUSH_PTR(p_dlist, data_type_ptr, value) \
{\
    data_type_ptr tmp = value; \
    __push_dlist(p_dlist, tmp); \
}

#define DLIST_CREATE(data_type, count) \
__make_dlist(sizeof(data_type) * count, sizeof(data_type));

#define DLIST_GET(p_dlist, index) \
__get_data(p_dlist, index);\

#if DEBUG == 1
#define LOG_INFO(x, ...) \
    __dlist_log(stdout, x, ##__VA_ARGS__)
#else
#define LOG_INFO(x, ...)
#endif

#if DEBUG == 1
#define LOG_ERROR(x, ...) \
    __dlist_log(stderr, x, ##__VA_ARGS__)
#else
#define LOG_ERROR(x, ...)
#endif

/// =============================================================================
/// ============================== DECLARATIONS =================================
/// =============================================================================

/// @brief Frees the dynamic list memory allocations and safely sets it to unused
void 
free_dlist(dlist* list);

/// @brief Creates a stack allocated dynamic list
/// @param size size in bytes for amount of memory you want to allocate
/// @param element_size size of each element that will be pushed
/// @note Use DLIST_CREATE macro instead
/// @return a stack allocated list
dlist 
__make_dlist(size_t size, size_t element_size);

/// @brief Checks if the dynamic list is full
/// @return 0 if true else -1
int 
__is_full(dlist* list);

/// @brief Grows the list's size dynamically to allocate more items
/// @return 0 is succeeded else -1
int
__resize_list(dlist* list);

/// @brief Copies the contents of memory into the list
/// @param list The list to copy to
/// @param data The destination memory region from where data will be copied
/// @note Use DLIST_PUSH / DLIST_PUSH_PTR macros instead
/// @return 0 if succeeds else -1
int 
__push_dlist(dlist* list, void* data);

/// @brief Gets the data at specified index into the dlist
/// @param list The list from where data will be fetched
/// @param index The index number of the data we want to get from the list
/// @note Use DLIST_GET macro instead
/// @return Data from the list (this needs to be casted back to the type you want)
void* 
__get_data(dlist* list, int index);

/// =============================================================================
/// ============================== PUBLIC =======================================
/// =============================================================================

static void 
free_dlist(dlist* list) 
{
    if (list != NULL && list->data != NULL) 
    {
        free(list->data);
        list->data = NULL;
        list->size = 0;
        list->count = 0;
    }
}

/// =============================================================================
/// ============================== PRIVATE ======================================
/// =============================================================================

static void
__dlist_log(FILE* const stream, char* log, ...)
{
#if DEBUG == 1
    if (stream == NULL) { return; }
    va_list list;
    char buffer[5000];
    va_start(list, log);
    vsnprintf(buffer, 5000, log, list);
    va_end(list);
    fprintf(stream, "%s", buffer);
#endif
}

static void
__print_state(dlist* list)
{
#if DEBUG == 1
    printf("=================================================\n");
    LOG_INFO("Current count: %d\n", list->count);
    LOG_INFO("Current memory size: %d\n", list->size);
    LOG_INFO("Each element size: %d\n", list->element_size);
    LOG_INFO("Is memory valid: %d\n", (list->data != NULL) ? 0 : -1);
    LOG_INFO("=================================================\n");
#endif
}

static dlist 
__make_dlist(size_t size, size_t element_size) 
{
    dlist list;
    list.data = malloc(size);
    memset(list.data, 0, size);
    list.itr = list.data;
    list.size = size;
    list.element_size = element_size;
    list.count = 0;
    return list;
}

static int 
__is_full(dlist* list) 
{
    return (list->count >= (list->size / list->element_size)) ? 0 : -1;
}

static int
__resize_list(dlist* list) 
{
    if (list == NULL || list->data == NULL) 
    { 
        LOG_ERROR("List is not allocated. Call DLIST_CREATE first to allocate memory for dlist.");
        return -1; 
    }
    size_t new_element_count = list->count * 2;
    size_t new_mem_size = list->element_size * new_element_count;
    LOG_INFO("Resizing array from size %d to size %d\n", list->size, new_mem_size);
    uint8_t* new_mem_reg = (uint8_t*)realloc(list->data, new_mem_size);
    list->size = new_mem_size;
    list->data = new_mem_reg;
    list->itr = list->data + (list->count * list->element_size);

    return 0;
}

static int 
__push_dlist(dlist* list, void* data) 
{
    if (__is_full(list) == 0)
    {
        if (__resize_list(list) < 0)
        {
            LOG_ERROR("Resizing list failed due to some reason");
            return -1;
        }
    }

    if (list->element_size == (sizeof(void*))) {
        *(void**)list->itr = data;
    } else {
        memcpy(list->itr, data, list->element_size);
    }
    list->itr += list->element_size;
    list->count++;

    __print_state(list);
    return 0;
}

static void* 
__get_data(dlist* list, int index) 
{
    return list->data + (list->element_size * index);
}