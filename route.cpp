#include "route.h"

struct character_node;

character_stack::~character_stack()
{
    if(head == nullptr){
                return ;
    }else if(head->next == nullptr){
        delete head;
    }else{
        character_node* cur = head;
        character_node* ptr = head->next;
        while(ptr != nullptr){
            delete cur;
            cur = ptr;
            ptr = ptr->next;
        }   // 不要忘记删除最后一个
        delete cur;
    }
}

void character_stack::insert(character_node* new_node)
{
    double needed_time = new_node->cptr->get_turn_time();
    if(head == nullptr){
        head = new_node;
    }
    else if(head->cptr->get_time() > needed_time){
        // 插入在head
        new_node->next = head;
        head = new_node;
    }
    else{
        character_node* prev = head;
        character_node* p = head->next;
        while(p != nullptr && p->cptr->get_time() <= needed_time){
            prev = p;
            p = p->next;
        }
        new_node->next = p;
        prev->next = new_node;
    }
    // update number...
    total_num++;
    if(new_node->cptr->get_ID() == -1){
        person_num += 1;
    }
}

void character_stack::pop()
{
    // step 1: handle boundary case
    // if num == 0, 无需任何操作
    if(total_num == 0 || head == nullptr){
        return ;
    }
    else
    {
        // step 2: update person number
        character_node* p = head;
        if(p->cptr->get_ID() == -1)
        {
            // update person number
            person_num -= 1;
        }
        // cout << "pop: " << p->cptr->get_name() << endl;
        // step 3: remove the top one
        head = head->next;
        delete p;
        
        // step 4: update num
        total_num--;
    }
}

void character_stack::update(bool speed_up, character_node* target, int prev_v){
    // step 1: 移除死掉的
    // step 1.1: 处理boundary case
    while(head != nullptr && head->cptr->get_blood() <= 0){
        character_node* temp = head;
        head = head->next;
        // 记住要update our total_num and person_num
        if(typeid(*(head->cptr)) == typeid(person)){person_num -= 1;}
        total_num -= 1;
        delete temp; temp = nullptr;
    }
    if(head == nullptr){return;}

    // step 1.2: 遍历整个链表
    character_node* prev = head;
    character_node* p = head->next;
    while(p != nullptr){
        if(p->cptr->get_blood() <= 0){
            prev->next = p->next;
            // 记住要update our total_num and person_num
            if(typeid(*(p->cptr)) == typeid(person)){person_num -= 1;}
            total_num -= 1;
            // 删除他
            delete p;
        }else{
            prev = p;
        }
        p = prev->next;
    }
    p = nullptr;

    // step 2: 处理加速
    if(speed_up){
        if(target == nullptr){
            return ;
        }
        // step 2.1: 更新time
        int cur_speed = target->cptr->get_speed();
        double new_time = prev_v / cur_speed* target->cptr->get_time();
        target->cptr->set_time(new_time);

        // step 2.2: 重排stack
        p = head;
        while(p != nullptr && p->next != target){
            p = p->next;
        }

        if(p == nullptr){
            cerr << "not find this target" << endl;
        }

        // step 2.3: 将这个target移出链表，并重新插入
        p->next = target->next;
        target->next = nullptr;
        // 这里不调用insert函数了
        // 因为insert里面调用的时间是get_turn_time, 并不是我们想要的time
        // 所以这里不用
        character_node* prev = head;
        p = head->next;
        if(head->cptr->get_time() > new_time){
            // 插入在head
            target->next = head;
            head = target;
        }
        else{
            while(p != nullptr && p->cptr->get_time() <= new_time){
                prev = p;
                p = p->next;
            }
            target->next = p;
            prev->next = target;
        }
        // end of function
    }
}

void character_stack::print_info() const
{
    cout << "now, " << this->top()->get_name() << " start action" << endl;
    character_node* p = head;
    for(int i = 0; i < total_num && p != nullptr; i++, p = p->next){
        cout << p->cptr->get_name() << " blood: " << p->cptr->get_blood()
        << " speed: " << p->cptr->get_speed() << " attack: " << p->cptr->get_attack() <<
        " \nneeded time to achieve top: " << p->cptr->get_time() << endl;
    }
    cout << "now you have " << skill_point << " skill_point left" << endl;
}

character_node* character_stack:: selected_one(int index, char t) const
{
    if(t == 'm'){
        character_node* p = head;
        
        while(p != nullptr){
            if(p->cptr->get_ID() == index){
                // find the monster
                return p;
            }else{p = p->next;}
        }
        if(p == nullptr){
            cerr << "not find monster" << endl;
            return nullptr;
        }else{
            cerr << "invalid index" << endl;
            return nullptr;
        }
    }
    else if(t == 'p'){
        character_node* p = head;
        index = 1 + (index % person_num);

        cout << "person number is: " << person_num <<endl;
        int count = 0;
        while(p != nullptr){
            if(p->cptr->get_ID() == -1){
                count++;
            }
            if(count == index){
                // find the person
                return p;
            }
            p = p->next;
        }
        if(p == nullptr){
            cerr << "not find person" << endl;
            return nullptr;
        }else{
            cerr << "invalid index" << endl;
            return nullptr;
        }
    }else{
        cerr << "invalid input" << endl;
        return nullptr;
    }
}

void character_stack::win_lose(bool& win, bool& lose) const
{
    win = true;
    lose = true;
    character_node* p = head;
    while(p != nullptr){
        if(p->cptr->get_ID() == -1){
            lose = false;
        }
        if(p->cptr->get_ID() >= 0){
            win = false;
        }
        p = p->next;
    }
    if(win == true && lose == true){
        cerr << "invaild state" << endl;
    }
}

void character_stack::update_time(character_node* base)
{
    if(base){
        cout << "sorry, currently we do not support this function" << endl;
    }
    else{
        character_node* p = head;
        double elapsed_time = p->cptr->get_time();
        for(int i = 0; i < total_num; i++){
            p->cptr->set_time(p->cptr->get_time() - elapsed_time);
            p = p->next;
        }
    }
}
