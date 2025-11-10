/*file name: abstract_class.h*/

# include <iostream>
# include <typeinfo>
# include <cstring>

#ifndef CHARACTERS_H
#define CHARACTERS_H

using namespace std;
const int TOTAL_DIS = 10000;

enum buff_type
{
    SPEED = 1,
    ATTACK
};

struct BUFF
{
    buff_type TYPE;
    int data;
};

struct Buff_node
{
    BUFF buff;
    int remain_turn;    // 表示还有多少时间buff就失效了
    Buff_node* next;
};


class character
{
    private:
        static int total_count;
    protected:
        character* target;
        double time;            // 表示还需多少时间，character才能走到轴顶
    public:
        character(): target(nullptr), time(0.0){
            // cout << "character constructor" << endl;
            total_count++;
        }

        virtual ~character(){
            // cout << "character destructor" << endl;
            total_count--;
        }

        // 他们是pure virtual function
        virtual int get_blood() const = 0;
        // person的ID永远为-1, monster的ID永远大于0
        virtual int get_ID() const = 0;
        virtual int get_speed(bool = true) const = 0;
        virtual int get_attack() const = 0;
        virtual Buff_node* get_head() const = 0;
        virtual const char* get_name() const = 0;

        // 找出角色造成的伤害，并将这个伤害打到target身上
        // 如果target已经死亡，或者不存在，什么都不要做
        // target不可以是自己
        // 规定普攻是a, 战技是e
        virtual int get_damage(char type = 'a') const = 0;
        
        // static function，统计一共有多少character
        static int get_count() {return total_count;}

        // 找出我们还需多少时间才能走完一轮，再次到达轴顶
        // 这个函数只会在到达轴顶时call
        virtual double get_turn_time() const = 0;

        // 得到我们当前到达轴顶所需的时间
        double get_time() const {return time;}

        // mutator

        // 设置所需到达轴顶的时间
        void set_time(double t){
            time = t;
        }

        // 使用这个函数将伤害“打到”target身上
        // 注意：当change为正时，血量减少
        virtual void set_blood(int change) = 0;

        // 锁定目标
        void set_target(character* target){
            this->target = target;
        }

        // 添加buff，如果添加成功return true
        // 否则，return false
        // 当 new_buff == nullptr
        //    target == this
        //    target == monster
        // 时，
        // 我们不可以添加buff
        // 注意：新加的buff会添加到第一个
        virtual bool add_buff(Buff_node* new_buff = nullptr) = 0;

        // 回合开始时，调用这个函数，刷新自己身上的buff、血量
        // 如果自己的血量为0，return false
        // person要更新buff的剩余回合数, monster不用
        virtual bool start_action() = 0;
};

class person : public character
{
    private:
        char* name;
        int blood;
        int speed;
        int attack;
        int skill_multiply_n;   // 普攻倍率
        int skill_multiply_s;   // 战技倍率, default = 0
        Buff_node* head;        // 初始化为nullptr
    public:
        // 小心一下conversion constructor
        explicit person(const char* name, int blood = 0, int speed = 100, 
                        int attack = 0, int skill_multiply_n = 1, int skill_multiply_s = 0):
        blood(blood), speed(speed), attack(attack), skill_multiply_n(skill_multiply_n),
        skill_multiply_s(skill_multiply_s)
        {
            // cout << "person constructor" << endl;
            this->name = new char[strlen(name) + 1];
            strcpy(this->name, name);
            head = nullptr;

            // update time:
            time = TOTAL_DIS/speed;
        }

        // 禁用copy constructor
        person(const person* p) = delete;

        // destructor
        virtual ~person();

        // 实现我们的virtual function
        virtual int get_blood() const override {return blood;}
        virtual int get_ID() const override {return -1;}
        virtual int get_speed(bool = true) const override;
        virtual int get_attack() const override;
        virtual Buff_node* get_head() const override {return head;}
        virtual const char* get_name() const override {return name;}

        virtual int get_damage(char type) const override;
        virtual double get_turn_time() const override;

        // mutator:
        virtual void set_blood(int change) override{blood -= change;}

        // helper function: check vaild buff
        // 看buff是否过了有效期；如果是，那么就移除这个buff
        void check_vaild_buff();

        // 添加buff，如果添加成功return true
        // 否则，return false
        // 当 new_buff == nullptr
        //    target == this
        //    target == monster
        // 时，
        // 我们不可以添加buff
        virtual bool add_buff(Buff_node* new_buff) override;

        virtual bool start_action() override;
};

class monster : public character
{
    private:
        // monster是没有buff的
        short ID;
        int blood;
        int speed;
        int attack;
    public:
        monster(short ID, int blood, int speed, int attack):
        ID(ID), blood(blood), speed(speed), attack(attack){
            // update time:
            time = TOTAL_DIS/speed;
            
            // cout << "monster constructor" << endl;
        }
        virtual ~monster(){
            cout << "moster destructor" << endl;
        }
        virtual int get_blood() const override {return blood;}
        virtual int get_ID() const override {return ID;}
        virtual int get_speed(bool = true) const override {return speed;}
        virtual Buff_node* get_head() const override {return nullptr;}
        virtual const char* get_name() const override {cout << "monster ID is: " << ID << ' '; return "monster";}
        virtual int get_attack() const override {return attack;}

        virtual int get_damage(char type) const override;
        virtual double get_turn_time() const override{
            // 找出我们还需多少时间才能再次到达轴顶
            return TOTAL_DIS/speed;
        }

        // mutator:
        virtual void set_blood(int change) override{
            blood -= change;
        }
        virtual bool add_buff(Buff_node* new_buff = nullptr){return false;};
        virtual bool start_action() override{
            return (blood > 0);
        }
};

#endif