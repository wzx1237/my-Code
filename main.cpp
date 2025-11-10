#include "route.h"
#include <cstdlib>
#include <ctime>

character_node* create_character_node(character* ptr){
    character_node* new_one = new character_node;
    new_one->cptr = ptr;
    new_one->next = nullptr;
    return new_one;
}

Buff_node* create_buff_node(int d, buff_type t, int remain = 1){
    Buff_node* new_node = new Buff_node;

    // initialize our data:
    new_node->next = nullptr;
    new_node->remain_turn = remain;
    new_node->buff.data = d;
    new_node->buff.TYPE = t;
    return new_node;
}

int character::total_count = 0;

int main()
{
    character_stack track;
    character* march_seven = new person{"march seven", 4500, 145, 2000, 2, 3};
    character* nick_name = new person{"nick_name", 3500, 120, 1800, 1, 6};
    character* bronya = new person{"bronya", 4200, 160, 1200, 2, 0};

    monster* A = new monster{1, 5000, 201, 700};
    monster* B = new monster{2, 10000, 165, 700};
    monster* C = new monster{3, 12000, 165, 700};
    monster* boss = new monster{4, 40000, 90, 300};
    monster* D = new monster{5, 7000, 201, 650};

    character_node* march_seven_node = create_character_node(march_seven);
    character_node* nick_name_node = create_character_node(nick_name);
    character_node* bronya_node = create_character_node(bronya);

    character_node* A_node = create_character_node(A);
    character_node* B_node = create_character_node(B);
    character_node* C_node = create_character_node(C);
    character_node* D_node = create_character_node(D);
    character_node* boss_node = create_character_node(boss);

    track.insert(march_seven_node);
    track.insert(nick_name_node);
    track.insert(bronya_node);

    track.insert(A_node);
    track.insert(B_node);
    track.insert(C_node);
    track.insert(D_node);
    track.insert(boss_node);

    bool win = false;
    bool lose = false;

    cout << "now, start our game" << endl;
    std::srand(std::time(0));           // 将这个放在loop外面，避免因为loop跑的太快而导致的问题

    while(win == false && lose == false){
        cout << "\na new turn start" << endl;
        track.update_time();
        track.print_info();
        
        character* action_one = track.top();
        if(action_one->get_ID() > 0)
        {
            cout << "it is monster action" << endl;
            // 随机造成伤害
            int index = std::rand();
            // cout << "index is: " << index << endl;

            action_one->set_target(track.selected_one(index, 'p')->cptr);
            cout << "it result " << action_one->get_damage() << " damage" << endl;
            track.update();
        }
        else
        {
            cout << "it is your turn" << endl;
            cout << "now, is: " << action_one->get_name() << " turn. please select skill" << endl;
            cout << "enter 'a/e' to indicate it. if you want to stop the game, tap 's' to stop it." << endl;
            char t;
            cin >> t;
            if(t == 'a'){(track.skill_point < 6)? track.skill_point += 1 : track.skill_point = 6 ;}
            else if(t == 'e' && track.skill_point > 0){track.skill_point -= 1;}
            else if(t == 's'){break;}
            else{cerr << "invaild input" << endl;}

            cout << "now, select the target, enter the monster's ID to indicate which one your want to attack" << endl;
            cout << "however, if you use bronya, be careful. Enter where your target locate in" << endl;
            
            int i;
            cin >> i;

            action_one->start_action();
            if(strcmp(action_one->get_name(), "bronya") == 0 && t == 'e'){
                i = (i % 2) + 2;
                character_node* cur_target = track.selected_one(i, 'p');
                // cout << "current target is: " << cur_target->cptr->get_name() << endl;
                action_one->set_target(cur_target->cptr);
                Buff_node* new_node_1 = create_buff_node(300, buff_type::SPEED, 1);
                if(!action_one->add_buff(new_node_1)){
                    cerr << "error" << endl;
                    delete new_node_1;
                }
                Buff_node* new_node_2 = create_buff_node(1800, buff_type::ATTACK, 1);
                if(!action_one->add_buff(new_node_2)){
                    cerr << "error" << endl;
                    delete new_node_2;
                }
                track.update(true, cur_target, cur_target->cptr->get_speed(false));

            }else{
                action_one->set_target(track.selected_one(i, 'm')->cptr);
                cout << "it result: " << action_one->get_damage(t) << " damage" << endl;
                track.update();
            }
            // end of our action
        }
        track.pop();
        // action one重新开始了新一个轮回
        double new_time = action_one->get_turn_time();
        action_one->set_time(new_time);
        track.insert(create_character_node(action_one));

        track.win_lose(win, lose);
    }

    cout << "game finish" << endl;
    if(win)
        cout << "you win";
    else
        cout << "you lose";

    while(!track.empty()){
        track.pop();
    }

    // node已经使用pop删完了，这里不用再删除了
    delete march_seven;
    delete nick_name;
    delete bronya;

    delete A; delete B; delete C; delete D; delete boss;

    return 0;
}
