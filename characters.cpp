/*file name: abstract_class.cpp*/

#include "characters.h"

using namespace std;

person::~person()
{
    // step 1：删除name
    cout << "person destructor" << endl;
    delete[] name;

    // step 2: 清空链表
    if (head == nullptr)
    {
        return;
    }
    else
    {
        Buff_node *cur = head;
        Buff_node *ptr = head->next;
        while (ptr != nullptr)
        {
            delete cur;
            cur = ptr;
            ptr = ptr->next;
        }
        // 不要忘记删除最后一个
        delete cur;
    }
}

/**
 * return: speed of character
 * @param bool enhenced: 表示我们的速度是基础速度还是加上buff之后的速度
 */
int person::get_speed(bool enhenced) const
{
    int cur_speed = speed;
    if(enhenced)
    {
        Buff_node *ptr = head;
        while (ptr != nullptr)
        {
            if (ptr->buff.TYPE == buff_type::SPEED)
            {
                cur_speed += ptr->buff.data;
            }
            ptr = ptr->next;
        }
    }
    return cur_speed;
}

int person::get_attack() const
{
    int enhanced_attack = attack;
    Buff_node *ptr = head;
    while (ptr != nullptr)
    {
        if (ptr->buff.TYPE == ATTACK)
        {
            enhanced_attack += ptr->buff.data;
        }
        ptr = ptr->next;
    }
    return enhanced_attack;
}

int person::get_damage(char type) const
// 这里不能有default value了，否则会出error
{
    int damage = 0;

    // 找出所有buff带来的加成
    int enhanced_attack = attack;
    Buff_node *ptr = head;
    while (ptr != nullptr)
    {
        if (ptr->buff.TYPE == ATTACK)
        {
            enhanced_attack += ptr->buff.data;
        }
        ptr = ptr->next;
    }
    if (type == 'a')
    {
        damage = enhanced_attack * skill_multiply_n;
    }
    else if (type == 'e')
    {
        damage = enhanced_attack * skill_multiply_s;
    }
    else
    {
        cout << "invaild input" << endl;
        return 0;
    }

    // 将这个伤害打到target身上
    if (target != this && target != nullptr)
    {
        target->set_blood(damage);
    }
    return damage;
}

double person::get_turn_time() const
{
    // 找出所有buff带来的加成
    int enhanced_speed = speed;
    Buff_node *ptr = head;
    while (ptr != nullptr)
    {
        if (ptr->buff.TYPE == SPEED)
        {
            enhanced_speed += ptr->buff.data;
        }
        ptr = ptr->next;
    }
    return TOTAL_DIS/enhanced_speed;
}

// helper function: check vaild buff
// 看buff是否过了有效期；如果是，那么就移除这个buff
void person::check_vaild_buff()
{
    // step 1: handle boundary case
    while (head != nullptr && head->remain_turn <= 0)
    {
        Buff_node *temp = head;
        head = head->next;
        delete temp;
        temp = nullptr;
    }
    if (head == nullptr)
    {
        return;
    }

    // step 2: 遍历整个链表
    Buff_node *prev = head;
    Buff_node *p = head->next;
    while (p != nullptr)
    {
        if (p->remain_turn <= 0)
        {
            prev->next = p->next;
            delete p;
        }
        else
        {
            prev = p;
        }
        p = prev->next;
    }
}

bool person::add_buff(Buff_node *new_buff)
{
    // 如果给我的是nullptr,不进行任何操作
    if (new_buff != nullptr)
    {
        // step 1: 我们不可以给monster加buff
        // 我们也不可以给自己加buff
        if (typeid(*target) == typeid(monster))
        {
            cout << "we cannot add buff to monster" << endl;
            return false;
        }
        if (target == this)
        {
            cout << "we cannot add buff to ourself" << endl;
            return false;
        }

        // step 2: 处理boundary case
        if (target->get_head() == nullptr)
        {
            dynamic_cast<person*>(target)->head = new_buff;
            cout << "add buff OK" << endl;
            return true;
        }
        
        // step 3: 新加的node添加到第一个即可
        new_buff->next = target->get_head();
        // 这里直接调用head, 否则使用get_head()无法update我们的head
        // 需要添加dynamic_cast, 否则会有compilation error
        // 这里要小心
        dynamic_cast<person*>(target)->head = new_buff;
        return true;
    }
    else{
        return false;
    }
}

bool person::start_action()
{
    Buff_node *p = head;
    // 更新buff持续时间
    while (p != nullptr)
    {
        p->remain_turn--;
        p = p->next;
    }
    // 移除失效的buff
    check_vaild_buff();
    if (blood <= 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

int monster::get_damage(char type) const
{
    // 将这个伤害打到target身上
    if (target != nullptr && target != this)
    {
        target->set_blood(attack);
    }
    return attack;
}
