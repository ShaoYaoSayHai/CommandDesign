/**
 * @file list.c
 * @author Enzo
 * @brief  List base source file
 * @version 0.1
 * @date 2025-12-11
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "list.h"

#define POOL_SIZE 100 // 定义“内存池”的大小

// 定义链表节点结构
typedef struct ListNode_t
{
    int id;              // 节点数据
    FuncHandler func_cb; // 节点处理函数指针
    int next;            // 游标，指向下一个节点的数组下标
} ListNode_t;

// 静态数组作为内存池
ListNode_t static_pool[POOL_SIZE];
int head = -1;     // 数据链表的头节点索引，-1表示空表
int free_head = 0; // 空闲链表的头节点索引

// 初始化内存池，将每个节点连接成空闲链表
void ListNodePool_Init(void)
{
    for (int i = 0; i < POOL_SIZE - 1; i++)
    {
        static_pool[i].next = i + 1;
        static_pool[i].id = 0; // 可选：初始化数据
    }
    static_pool[POOL_SIZE - 1].next = -1; // 最后一个节点指向-1，表示结束
    head = -1;                            // 初始化时数据链表为空
    free_head = 0;                        // 第一个空闲节点是数组的第0个元素
}

// 从空闲链表中分配一个节点，返回节点索引，失败返回-1
int my_alloc()
{
    if (free_head == -1)
    {
        printf("错误：内存池已满，无法分配新节点！\n");
        return -1;
    }
    // 从空闲链表头部取出节点
    int allocated_index = free_head;
    free_head = static_pool[free_head].next; // 更新空闲链表头
    // 初始化新分配节点的值
    static_pool[allocated_index].id = 0;    // 当前节点数据初始化
    static_pool[allocated_index].next = -1; // 新节点的next初始化为-1 下一个节点仍然是空
    return allocated_index;
}

// 将节点释放回空闲链表
void my_free(int index)
{
    if (index < 0 || index >= POOL_SIZE)
    {
        printf("错误：无效的节点索引，无法释放！\n");
        return;
    }
    // 将释放的节点插入到空闲链表头部
    static_pool[index].next = free_head;
    free_head = index;
}

// 在链表尾部插入一个新节点，数据为value
bool insert_at_tail(int value, FuncHandler func_cb)
{
    int new_node_index = my_alloc();
    if (new_node_index == -1)
    {
        return false; // 分配失败
    }
    static_pool[new_node_index].id = value;
    static_pool[new_node_index].func_cb = func_cb;

    if (head == -1)
    {
        // 如果链表为空，新节点成为头节点
        head = new_node_index;
    }
    else
    {
        // 找到当前链表的最后一个节点
        int current = head;
        while (static_pool[current].next != -1)
        {
            current = static_pool[current].next;
        }
        // 将最后一个节点的next指向新节点
        static_pool[current].next = new_node_index;
    }
    printf("数据 %d 已插入到链表尾部（节点索引：%d)\n", value, new_node_index);
    return true;
}

// 删除第一个数据为 ID 的节点
bool delete_node(int id)
{
    if (head == -1)
    {
        // printf("链表为空，无法删除！\n");
        return false;
    }

    int current = head;
    int previous = -1;

    // 遍历寻找目标节点 当表遍历完毕或者找到目标节点时停止
    while (current != -1 && static_pool[current].id != id)
    {
        previous = current;
        current = static_pool[current].next;
    }

    // 未找到目标节点
    if (current == -1)
    {
        // printf("未找到值为 %d 的节点！\n", value);
        return false;
    }

    // 调整链表指针，跳过要删除的节点
    if (previous == -1)
    {
        // 要删除的是头节点
        head = static_pool[current].next;
    }
    else
    {
        // 要删除的是中间或尾部节点
        // 将后一个节点的索引赋值给前一个节点的next
        static_pool[previous].next = static_pool[current].next;
    }

    // 释放节点
    my_free(current);
    printf("节点 %d 已被删除！\n", id);
    return true;
}

// 打印整个链表
void print_list()
{
    if (head == -1)
    {
        printf("链表为空！\n");
        return;
    }
    printf("当前链表: ");
    int current = head;
    while (current != -1)
    {
        printf("FID : %d -> ", static_pool[current].id);
			  static_pool[current].func_cb( NULL );
        current = static_pool[current].next;
    }
    printf("NULL\n");
}

/**
 * @brief 覆盖指定ID节点的函数指针
 * @param id 要修改的节点ID
 * @param new_func 新的函数指针
 * @return bool 操作是否成功
 */
bool override_function(int id, FuncHandler new_func)
{
    if (head == -1)
    {
        printf("链表为空，无法覆盖函数！\n");
        return false;
    }

    int current = head;
    while (current != -1)
    {
        if (static_pool[current].id == id)
        {
            // 找到目标节点，覆盖函数指针
            static_pool[current].func_cb = new_func;
            printf("已覆盖ID为 %d 节点的函数指针！\n", id);
            return true;
        }
        current = static_pool[current].next;
    }

    printf("未找到ID为 %d 的节点，无法覆盖函数！\n", id);
    return false;
}

/**
 * @brief 根据ID执行对应的函数
 * @param id 要执行函数的节点ID
 * @return bool 操作是否成功
 */
bool execute_function_by_id(int id, FuncParam_t *func_cb_param)
{
    if (head == -1)
    {
        printf("链表为空，无法执行函数！\n");
        return false;
    }

    int current = head;
    while (current != -1)
    {
        if (static_pool[current].id == id)
        {
            // 找到目标节点，执行函数
            if (static_pool[current].func_cb != NULL)
            {
                printf("正在执行ID为 %d 节点的函数...\n", id);
                // static_pool[current].func_cb((void *)&id);             // 执行函数 以地址的方式传参
                static_pool[current].func_cb((void *)func_cb_param); // 执行后将函数指针置空，防止重复执行
                return true;
            }
            else
            {
                printf("ID为 %d 节点的函数指针为空，无法执行！\n", id);
                return false;
            }
        }
        current = static_pool[current].next;
    }

    printf("未找到ID为 %d 的节点，无法执行函数！\n", id);
    return false;
}

/**
 * @brief 根据ID查找节点索引
 * @param id 要查找的节点ID
 * @return int 节点索引，找不到返回-1
 */
int find_node_index_by_id(int id)
{
    if (head == -1)
    {
        return -1;
    }

    int current = head;
    while (current != -1)
    {
        if (static_pool[current].id == id)
        {
            return current; // 返回节点索引
        }
        current = static_pool[current].next;
    }

    return -1; // 未找到
}

// 示例1：需要一个uint8_t类型的数据
void my_function_uint8(void *arg)
{
    // 将void*参数转换为uint8_t类型数据的指针，然后解引用
    uint8_t data = *((uint8_t *)arg);
    printf("Processing uint8_t data: %u\n", data);
    // ... 你的处理逻辑
}

void my_function_struct(void *arg)
{
    // 将void*参数直接转换为特定的结构体指针
    MyStruct *p_data = (MyStruct *)arg;
    printf("Processing struct, ID: %u, Buffer Length: %ul\n", p_data->id, p_data->length);
    //可以通过p_data->buffer访问数据
}

void my_function_funcparam(void *arg)
{
    // 将void*参数直接转换为FuncParam_t结构体指针
    FuncParam_t *p_data = (FuncParam_t *)arg;
    printf("Processing FuncParam_t, Input Length: %u\n", p_data->inputLen);
    //通过p_data->input和p_data->output访问数据
    
}

int func_demo(uint8_t *input, uint8_t inputLen, uint8_t *output, uint8_t *outputLen)
{
    printf("func_demo called with input length: %u\n", inputLen);
    // demo：简单地将输入复制到输出
    if (output != NULL && outputLen != NULL && *outputLen >= inputLen)
    {
        for (uint8_t i = 0; i < inputLen; i++)
        {
            output[i] = input[i];
        }
        *outputLen = inputLen; // 设置实际输出长度
        return 0;              // 成功
    }
    return -1; // 失败
}


