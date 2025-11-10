/*file name: route.h*/

#include "characters.h"

#ifndef ROUTE_H
#define ROUTE_H

struct character_node
{
    character* cptr;            // 指向character object的pointer
    character_node* next;       // 指向next character node的pointer
};


// 我们要使用一个stack，来模拟行动条
class character_stack
{
    private:
        character_node* head;       // 链表的开头
        int total_num;              // 表示现在这里面一共有多少character
        int person_num;             // 表示现在stack里面有多少person
    public:
        int skill_point;            // 战技点
        // constructor
        character_stack():head(nullptr), total_num(0), person_num(0), skill_point(3){};
        // destructor
        ~character_stack();

        // ACCESSOR function
        // 判断stack是不是空的
        bool empty() const{
            if(head == nullptr && total_num == 0){
                return true;
            }return false;
        }

        // 返回stack中有多少character
        int size() const{return total_num;}
        // 返回stack中有多少person
        int person_count() const{return person_num;}

        // 返回轴顶的character
        character* top() const{return head->cptr;}

        // 返回我们挑选好的目标
        // t == m表示我们选择了一个monster
        // t == p表示我们选择了一个person
        // monster的选择是看的ID
        // person的选择是看的从上到下数的第几个person
        // 第一个人的就是index == 1
        character_node* selected_one(int index, char t) const;

        // 输出内容，实现交互
        void print_info() const;

        // 胜利、失败条件判定
        void win_lose(bool& win, bool& lose) const;

        // mutator function
        // 将一个character插入我们的stack中
        // 要求，到轴顶距离小的人在前
        // 即：get_time数字小的人在前
        // 注：如果两个数字一样，新加的自动放到原有的object后面
        //     这个函数只有在初始化track或者让character重新插入行动条时才会被调用
        void insert(character_node*);

        // 轴顶的character行动完后，pop掉它
        // 同时update我们当前所有object的time
        void pop();

        // update所有character的状态
        // 如果character死了，直接移除他
        // 如果character得到了加速效果，给他赋予一个新的remain_time, 并且重新排我们的stack
        // target: 释放技能的目标，而不是释放技能者
        // 注意：这是加完buff后再update
        void update(bool speed_up = false, character_node* target = nullptr, int = 100);

        /**
         * 以base指向的character的时间为基准update所有character的时间
         * update之后，base的time会变成0
         * 函数拥有default value， nullptr
         * 在default情况下，我们以head作为我们的base
         */
        void update_time(character_node* base = nullptr);
};

#endif