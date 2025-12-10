/**
 * @file list.h
 * @author Enzo
 * @brief  List base header file
 * @version 0.1
 * @date 2025-12-11
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef _LIST_H_
#define _LIST_H_

#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"

typedef void (*FuncHandler)(void *param);

typedef struct FuncParam_t
{
    uint8_t *input;
    uint8_t inputLen;
    uint8_t *output;
    uint8_t *outputLen;
} FuncParam_t;

// 示例2：需要一个结构体指针
typedef struct
{
    uint8_t id;
    uint8_t *buffer;
    uint32_t length;
} MyStruct;

// 初始化内存池，将每个节点连接成空闲链表
void ListNodePool_Init(void);

// 在链表尾部插入一个新节点，数据为value
bool insert_at_tail(int value, FuncHandler func_cb);

// 删除第一个数据为 ID 的节点
bool delete_node(int id);

// 打印整个链表
void print_list();

/**
 * @brief 覆盖指定ID节点的函数指针
 * @param id 要修改的节点ID
 * @param new_func 新的函数指针
 * @return bool 操作是否成功
 */
bool override_function(int id, FuncHandler new_func);

/**
 * @brief 根据ID执行对应的函数
 * @param id 要执行函数的节点ID
 * @return bool 操作是否成功
 */
bool execute_function_by_id(int id, FuncParam_t *func_cb_param);

/**
 * @brief 根据ID查找节点索引
 * @param id 要查找的节点ID
 * @return int 节点索引，找不到返回-1
 */
int find_node_index_by_id(int id);

#endif // _LIST_H_
