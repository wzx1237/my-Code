/*
 * ELEC1100 Your Lab#06 & Project Template
 * 
 * To program the car tracking thw white line on a dark mat
 * 
 * Group No. 90
 * Group Member 1 (name & SID): WANG, Zhixin 21150652
 * Group Menber 2 (name & SID): HUANG, Yiqiao 21152351
 */
/*
一些特别注意事项：
1.  far_rightSensor == BLACK;
    记住：如果不需要读另一侧sensor的数据，要提前设好它；
    避免因为无法update它而造成事故
2.  注意：我们这里使用A1；A6不知道为什么无法read sensor的信号了
3.  在第一个掉头的地方不需要使用最外面的两个sensor，
    因为他们离得太远了，没办法同时看到白色
4.  巡线的sensor同时看到白色的时候可以有不同的反应，这个每个case都要调
*/

const int WHITE = 0;
const int BLACK = 1;

const int pinL_Sensor = A5;     // pin A5: left sensor
const int pinB_Sensor = A4;     // pin A4: bumper sensor
const int pinR_Sensor = A3;     // pin A3: right sensor
const int pin_farL_Sensor = A1; // pinA1: far left sensor
const int pin_farR_Sensor = A2; // pinA2: far right sensor

const int pinL_PWM = 9;  // pin D9: left motor speed
const int pinL_DIR = 10; // pin D10: left motor direction

const int pinR_PWM = 11; // pin D11: right motor speed
const int pinR_DIR = 12; // pin D12: right motor direction

// define variables to be used in script

bool bumperSensor = 1;    // not sensing white
bool leftSensor = 1;      // not sensing white
bool rightSensor = 1;     // not sensing white
bool far_leftSensor = 1;  // not sensing white
bool far_rightSensor = 1; // not sensing white

int countBumper = 0; // bumper sensor not triggered yet
int process = 1;

bool debug = false;
// the setup function runs once when you press reset or power the board

void forward()
{
  analogWrite(pinL_PWM, 200);
  analogWrite(pinR_PWM, 200);
  digitalWrite(pinL_DIR, 1);
  digitalWrite(pinR_DIR, 1);
}

void turn_left(const int &delay_time)
{
  analogWrite(pinL_PWM, 200);
  analogWrite(pinR_PWM, 200);
  digitalWrite(pinL_DIR, 0);
  digitalWrite(pinR_DIR, 1);
  delay(delay_time);
}
void turn_right(const int &delay_time)
{
  analogWrite(pinL_PWM, 200);
  analogWrite(pinR_PWM, 200);
  digitalWrite(pinL_DIR, 1);
  digitalWrite(pinR_DIR, 0);
  delay(delay_time);
}

void rotate(const int& delay_time)
{
  analogWrite(pinL_PWM, 200);
  analogWrite(pinR_PWM, 200);
  digitalWrite(pinL_DIR, 0);
  digitalWrite(pinR_DIR, 1);
  delay(delay_time);
  // 掉一格电：550-- 180
  // 满电：delay(1100)--360
  // 掉一格电 delay(1200)--360
  // 掉两格电：delay(1200)--360
}

void setup()
{
  // define pins as input and output
  pinMode(pinB_Sensor, INPUT);
  pinMode(pinL_Sensor, INPUT);
  pinMode(pinR_Sensor, INPUT);

  pinMode(pinL_DIR, OUTPUT);
  pinMode(pinR_DIR, OUTPUT);

  pinMode(pinL_PWM, OUTPUT);
  pinMode(pinR_PWM, OUTPUT);

  // initialize output pins
  digitalWrite(pinL_DIR, HIGH); // forward direction
  digitalWrite(pinR_DIR, HIGH); // forward direction
  analogWrite(pinL_PWM, 0);     // stop at the start position
  analogWrite(pinR_PWM, 0);     // stop at the start position
}

// the loop function runs over and over again forever
void loop()
{
  bumperSensor = digitalRead(pinB_Sensor);

  // car stops at the start position when bumper sensor no trigger
  if (bumperSensor && countBumper == 0)
  {
    analogWrite(pinL_PWM, 0);
    analogWrite(pinR_PWM, 0);
  }

  // bumper sensor is triggered at the start position for the 1st time
  else if (!bumperSensor && countBumper == 0)
  {
    analogWrite(pinL_PWM, 200);
    analogWrite(pinR_PWM, 200);
    countBumper = countBumper + 1;
    delay(350); // to let the car leave the start position with no miscount
  }
  else if (bumperSensor && countBumper == 1)
  {
    switch (process)
    {
//
//
    // 启动，并转过第一个弯，向左转
    // 这是一个T字型分叉
    case 1:
      far_leftSensor = BLACK;
      far_rightSensor = BLACK;
      // 强制进入巡线
      while (far_leftSensor == BLACK || far_rightSensor == BLACK)
      {
        leftSensor = digitalRead(pinL_Sensor);
        rightSensor = digitalRead(pinR_Sensor);
        far_leftSensor = digitalRead(pin_farL_Sensor);
        far_rightSensor = digitalRead(pin_farR_Sensor);

        // 向左转
        if (leftSensor == WHITE && rightSensor == BLACK)
        {
          analogWrite(pinL_PWM, 180);
          analogWrite(pinR_PWM, 180);
          digitalWrite(pinL_DIR, 0);
          digitalWrite(pinR_DIR, 1);
        }
        // 向右转
        else if (leftSensor == BLACK && rightSensor == WHITE)
        {
          analogWrite(pinL_PWM, 180);
          analogWrite(pinR_PWM, 180);
          digitalWrite(pinL_DIR, 1);
          digitalWrite(pinR_DIR, 0);
        }
        // 两边都是黑的时候，前进
        else if(leftSensor == BLACK && rightSensor == BLACK)
        {
          analogWrite(pinL_PWM, 200);
          analogWrite(pinR_PWM, 200);
          digitalWrite(pinL_DIR, 1);
          digitalWrite(pinR_DIR, 1);
        }
        // 两边都是白的时候，停下
        else
        {
          analogWrite(pinL_PWM, 0);
          analogWrite(pinR_PWM, 0);
          digitalWrite(pinL_DIR, 0);
          digitalWrite(pinR_DIR, 0);
        }
      }
      // 说明此时，最左侧的和最右侧sensor都是白色的
      turn_left(210);
      // 正常：210
      forward();
      delay(200);
      process += 1;
      break;
//
//
    // 转过第二个弯,向左转
    // 这是一个普通岔路口
    case 2:
      far_leftSensor = BLACK;
      // 强制进入巡线
      while (far_leftSensor == BLACK)
      {
        leftSensor = digitalRead(pinL_Sensor);
        rightSensor = digitalRead(pinR_Sensor);
        far_leftSensor = digitalRead(pin_farL_Sensor);

        // 向左转
        if (leftSensor == WHITE && rightSensor == BLACK)
        {
          analogWrite(pinL_PWM, 170);
          analogWrite(pinR_PWM, 170);
          digitalWrite(pinL_DIR, 0);
          digitalWrite(pinR_DIR, 1);
        }
        // 向右转
        else if (leftSensor == BLACK && rightSensor == WHITE)
        {
          analogWrite(pinL_PWM, 170);
          analogWrite(pinR_PWM, 170);
          digitalWrite(pinL_DIR, 1);
          digitalWrite(pinR_DIR, 0);
        }
        // 前进，两边都是白的时候，前进 \ 两边都是黑的时候，前进
        else
        {
          analogWrite(pinL_PWM, 160);
          analogWrite(pinR_PWM, 160);
          digitalWrite(pinL_DIR, 1);
          digitalWrite(pinR_DIR, 1);
        }
      }
      // 说明此时最左侧的sensor是白色的
      turn_left(250);
      // 满电：250
      forward();
      delay(100);
      process += 1;
      break;
//
//
    // 掉头
    case 3:
      leftSensor = BLACK;
      rightSensor = BLACK;
      // 强制进入巡线
      while (leftSensor == BLACK || rightSensor == BLACK)
      {
        leftSensor = digitalRead(pinL_Sensor);
        rightSensor = digitalRead(pinR_Sensor);

        // 向左转
        if (leftSensor == WHITE && rightSensor == BLACK)
        {
          analogWrite(pinL_PWM, 180);
          analogWrite(pinR_PWM, 180);
          digitalWrite(pinL_DIR, 0);
          digitalWrite(pinR_DIR, 1);
        }
        // 向右转
        else if (leftSensor == BLACK && rightSensor == WHITE)
        {
          analogWrite(pinL_PWM, 180);
          analogWrite(pinR_PWM, 180);
          digitalWrite(pinL_DIR, 1);
          digitalWrite(pinR_DIR, 0);
        }
        // 两边都是黑的时候，前进
        else
        {
          analogWrite(pinL_PWM, 160);
          analogWrite(pinR_PWM, 160);
          digitalWrite(pinL_DIR, 1);
          digitalWrite(pinR_DIR, 1);
        }
      }
      // 说明此时我们碰到了T
      rotate(580);
      forward();
      delay(100);
      process += 1;
      break;
//
//
    // 转过第四个弯，左转
    case 4:
      far_leftSensor = BLACK;
      far_rightSensor = BLACK;
      // 强制进入巡线
      while (far_leftSensor == BLACK || far_rightSensor == BLACK)
      {
        leftSensor = digitalRead(pinL_Sensor);
        rightSensor = digitalRead(pinR_Sensor);
        far_leftSensor = digitalRead(pin_farL_Sensor);
        far_rightSensor = digitalRead(pin_farR_Sensor);

        // 向左转
        if (leftSensor == WHITE && rightSensor == BLACK)
        {
          analogWrite(pinL_PWM, 180);
          analogWrite(pinR_PWM, 180);
          digitalWrite(pinL_DIR, 0);
          digitalWrite(pinR_DIR, 1);
        }
        // 向右转
        else if (leftSensor == BLACK && rightSensor == WHITE)
        {
          analogWrite(pinL_PWM, 180);
          analogWrite(pinR_PWM, 180);
          digitalWrite(pinL_DIR, 1);
          digitalWrite(pinR_DIR, 0);
        }
        // 前进，两边都是白的时候，前进 \ 两边都是黑的时候，前进
        else
        {
          analogWrite(pinL_PWM, 170);
          analogWrite(pinR_PWM, 170);
          digitalWrite(pinL_DIR, 1);
          digitalWrite(pinR_DIR, 1);
        }
      }
      // 说明此时遇到了T
      turn_left(210);
      // 满电：210
      // 掉一格电：240
      forward();
      delay(50);
      process += 1;
      break;
//
//
    // 转过第五个弯，向右转
    case 5:
      far_rightSensor = BLACK;
      // 强制进入巡线
      while (far_rightSensor == BLACK)
      {
        leftSensor = digitalRead(pinL_Sensor);
        rightSensor = digitalRead(pinR_Sensor);
        far_rightSensor = digitalRead(pin_farR_Sensor);

        // 向左转
        if (leftSensor == WHITE && rightSensor == BLACK)
        {
          analogWrite(pinL_PWM, 180);
          analogWrite(pinR_PWM, 180);
          digitalWrite(pinL_DIR, 0);
          digitalWrite(pinR_DIR, 1);
        }
        // 向右转
        else if (leftSensor == BLACK && rightSensor == WHITE)
        {
          analogWrite(pinL_PWM, 180);
          analogWrite(pinR_PWM, 180);
          digitalWrite(pinL_DIR, 1);
          digitalWrite(pinR_DIR, 0);
        }
        // 前进，两边都是白的时候，前进 \ 两边都是黑的时候，前进
        else
        {
          analogWrite(pinL_PWM, 160);
          analogWrite(pinR_PWM, 160);
          digitalWrite(pinL_DIR, 1);
          digitalWrite(pinR_DIR, 1);
        }
      }
      // 说明此时最右侧的sensor是白色的
      turn_right(210);
      // 满电：210
      // 掉一格电：230
      forward();
      delay(50);
      process += 1;
      break;
//
//
    // 走过gentle curve, 并且转过第一个弯和第二个弯
    case 6:
      far_leftSensor = BLACK;
      // 强制进入巡线
      while (far_leftSensor == BLACK)
      {
        leftSensor = digitalRead(pinL_Sensor);
        rightSensor = digitalRead(pinR_Sensor);
        far_leftSensor = digitalRead(pin_farL_Sensor);
        // 向左转
        if (leftSensor == WHITE && rightSensor == BLACK)
        {
          analogWrite(pinL_PWM, 180);
          analogWrite(pinR_PWM, 180);
          digitalWrite(pinL_DIR, 0);
          digitalWrite(pinR_DIR, 1);
        }
        // 向右转
        else if (leftSensor == BLACK && rightSensor == WHITE)
        {
          analogWrite(pinL_PWM, 180);
          analogWrite(pinR_PWM, 180);
          digitalWrite(pinL_DIR, 1);
          digitalWrite(pinR_DIR, 0);
        }
        // 前进，两边都是白的时候，前进 \ 两边都是黑的时候，前进
        else
        {
          analogWrite(pinL_PWM, 180);
          analogWrite(pinR_PWM, 180);
          digitalWrite(pinL_DIR, 1);
          digitalWrite(pinR_DIR, 1);
        }
      }
      turn_left(200); // 转过第一个弯
      forward();
      delay(200);
      turn_left(300); // 转过第二个弯
      // 满电：300
      // 超级满电、塑料毯：200
      // 满电、塑料毯：time 220
      // 掉一格电：230
      // time: 300
      forward();
      delay(300);
      process += 1;
      break;
//
//  
    // 走过口字型的第一个弯
    case 7:
      far_leftSensor = BLACK;
      // 强制进入巡线
      while (far_leftSensor == BLACK)
      {
        leftSensor = digitalRead(pinL_Sensor);
        rightSensor = digitalRead(pinR_Sensor);
        far_leftSensor = digitalRead(pin_farL_Sensor);

        // 向左转
        if (leftSensor == WHITE && rightSensor == BLACK)
        {
          analogWrite(pinL_PWM, 180);
          analogWrite(pinR_PWM, 180);
          digitalWrite(pinL_DIR, 0);
          digitalWrite(pinR_DIR, 1);
        }
        // 向右转
        else if (leftSensor == BLACK && rightSensor == WHITE)
        {
          analogWrite(pinL_PWM, 180);
          analogWrite(pinR_PWM, 180);
          digitalWrite(pinL_DIR, 1);
          digitalWrite(pinR_DIR, 0);
        }
        // 前进，两边都是白的时候，前进 \ 两边都是黑的时候，前进
        else
        {
          analogWrite(pinL_PWM, 160);
          analogWrite(pinR_PWM, 160);
          digitalWrite(pinL_DIR, 1);
          digitalWrite(pinR_DIR, 1);
        }
      }
      turn_left(170);
      forward();
      delay(50);
      process += 1;
      break;
//
//
    // 转过口字形的第二个弯
    case 8:
      leftSensor = BLACK;
      // 强制进入巡线
      while (leftSensor == BLACK || rightSensor == BLACK)
      {
        leftSensor = digitalRead(pinL_Sensor);
        rightSensor = digitalRead(pinR_Sensor);

        // 向左转
        if (leftSensor == WHITE && rightSensor == BLACK)
        {
          analogWrite(pinL_PWM, 170);
          analogWrite(pinR_PWM, 170);
          digitalWrite(pinL_DIR, 0);
          digitalWrite(pinR_DIR, 1);
        }
        // 向右转
        else if (leftSensor == BLACK && rightSensor == WHITE)
        {
          analogWrite(pinL_PWM, 170);
          analogWrite(pinR_PWM, 170);
          digitalWrite(pinL_DIR, 1);
          digitalWrite(pinR_DIR, 0);
        }
        // 两边都是黑的时候，前进
        else if(leftSensor == BLACK && rightSensor == BLACK)
        {
          analogWrite(pinL_PWM, 140);
          analogWrite(pinR_PWM, 160);
          digitalWrite(pinL_DIR, 1);
          digitalWrite(pinR_DIR, 1);
        }
        // 两边都是白的时候，停下
        else
        {
          analogWrite(pinL_PWM, 0);
          analogWrite(pinR_PWM, 0);
          digitalWrite(pinL_DIR, 1);
          digitalWrite(pinR_DIR, 1);
        }
      }
      turn_left(180);
      // 满电、塑料：170
      // 掉一格电：190
      forward();
      delay(100);
      process += 1;
      break;
//
//
    // 转过出口字形后的两个直角弯
    case 9:
    case 10:
      far_leftSensor = BLACK;
      while (far_leftSensor == BLACK)
      {
        leftSensor = digitalRead(pinL_Sensor);
        rightSensor = digitalRead(pinR_Sensor);
        far_leftSensor = digitalRead(pin_farL_Sensor);

        // 向左转
        if (leftSensor == WHITE && rightSensor == BLACK)
        {
          analogWrite(pinL_PWM, 180);
          analogWrite(pinR_PWM, 180);
          digitalWrite(pinL_DIR, 0);
          digitalWrite(pinR_DIR, 1);
        }
        // 向右转
        else if (leftSensor == BLACK && rightSensor == WHITE)
        {
          analogWrite(pinL_PWM, 180);
          analogWrite(pinR_PWM, 180);
          digitalWrite(pinL_DIR, 1);
          digitalWrite(pinR_DIR, 0);
        }
        // 两边都是黑的时候，前进
        else if(leftSensor == BLACK && rightSensor == BLACK)
        {
          analogWrite(pinL_PWM, 160);
          analogWrite(pinR_PWM, 170);   // 这里右侧加10，让他贴着左侧走
          digitalWrite(pinL_DIR, 1);
          digitalWrite(pinR_DIR, 1);
        }
        // 两边都是白的时候，停下
        else
        {
          analogWrite(pinL_PWM, 0);
          analogWrite(pinR_PWM, 0);
          digitalWrite(pinL_DIR, 1);
          digitalWrite(pinR_DIR, 1);
        }
      }
      turn_left(190);
      forward();
      delay(50);
      process += 1;
      break;
    // 走过那个几字型弯
    case 11:
      far_rightSensor = BLACK;
      // 强制进入巡线
      while (far_rightSensor == BLACK)
      {
        leftSensor = digitalRead(pinL_Sensor);
        rightSensor = digitalRead(pinR_Sensor);
        far_rightSensor = digitalRead(pin_farR_Sensor);
        // 向左转
        if (leftSensor == WHITE && rightSensor == BLACK)
        {
          analogWrite(pinL_PWM, 180);
          analogWrite(pinR_PWM, 180);
          digitalWrite(pinL_DIR, 0);
          digitalWrite(pinR_DIR, 1);
        }
        // 向右转
        else if (leftSensor == BLACK && rightSensor == WHITE)
        {
          analogWrite(pinL_PWM, 180);
          analogWrite(pinR_PWM, 180);
          digitalWrite(pinL_DIR, 1);
          digitalWrite(pinR_DIR, 0);
        }
        // 两边都是黑的时候，前进
        else if(leftSensor == BLACK && rightSensor == BLACK)
        {
          analogWrite(pinL_PWM, 150);
          analogWrite(pinR_PWM, 150);
          digitalWrite(pinL_DIR, 1);
          digitalWrite(pinR_DIR, 1);
        }
        // 前进，两边都是白的时候，停下
        else
        {
          analogWrite(pinL_PWM, 0);
          analogWrite(pinR_PWM, 0);
          digitalWrite(pinL_DIR, 0);
          digitalWrite(pinR_DIR, 0);
        }
      }
      turn_right(200); // 转过第一个弯
      forward();
      delay(300);
      // time: 300
      // 掉一格电：350
      turn_right(300); // 转过第二个弯
      // 满电：270
      // 掉一格电：300
      forward();
      delay(200);
      process += 1;
      break;
//
//
    // 转过几字型弯后的第一个弯
    // 转过旋转前的最后一格弯
    case 12:
    case 13:
      far_leftSensor = BLACK;
      while (far_leftSensor == BLACK)
      {
        leftSensor = digitalRead(pinL_Sensor);
        rightSensor = digitalRead(pinR_Sensor);
        far_leftSensor = digitalRead(pin_farL_Sensor);

        // 向左转
        if (leftSensor == WHITE && rightSensor == BLACK)
        {
          analogWrite(pinL_PWM, 180);
          analogWrite(pinR_PWM, 180);
          digitalWrite(pinL_DIR, 0);
          digitalWrite(pinR_DIR, 1);
        }
        // 向右转
        else if (leftSensor == BLACK && rightSensor == WHITE)
        {
          analogWrite(pinL_PWM, 180);
          analogWrite(pinR_PWM, 180);
          digitalWrite(pinL_DIR, 1);
          digitalWrite(pinR_DIR, 0);
        }
        // 两边都是黑的时候，前进
        else if(leftSensor == BLACK && rightSensor == BLACK)
        {
          analogWrite(pinL_PWM, 160);
          analogWrite(pinR_PWM, 160);
          digitalWrite(pinL_DIR, 1);
          digitalWrite(pinR_DIR, 1);
        }
        // 两边都是白的时候，停下
        else
        {
          analogWrite(pinL_PWM, 0);
          analogWrite(pinR_PWM, 0);
          digitalWrite(pinL_DIR, 1);
          digitalWrite(pinR_DIR, 1);
        }
      }
      turn_left(210);
      // delay:200
      forward();
      delay(50);
      process += 1;
      break;
//
//
    // 旋转360°，而后直行，再倒车
    case 14:
      far_leftSensor = BLACK;
      far_rightSensor = BLACK;
      // 强制进入巡线
      while (far_leftSensor == BLACK || far_rightSensor == BLACK)
      {
        leftSensor = digitalRead(pinL_Sensor);
        rightSensor = digitalRead(pinR_Sensor);
        far_leftSensor = digitalRead(pin_farL_Sensor);
        far_rightSensor = digitalRead(pin_farR_Sensor);

        // 向左转
        if (leftSensor == WHITE && rightSensor == BLACK)
        {
          analogWrite(pinL_PWM, 180);
          analogWrite(pinR_PWM, 180);
          digitalWrite(pinL_DIR, 0);
          digitalWrite(pinR_DIR, 1);
        }
        // 向右转
        else if (leftSensor == BLACK && rightSensor == WHITE)
        {
          analogWrite(pinL_PWM, 180);
          analogWrite(pinR_PWM, 180);
          digitalWrite(pinL_DIR, 1);
          digitalWrite(pinR_DIR, 0);
        }
        // 两边都是黑的时候，前进
        else if(leftSensor == BLACK && rightSensor == BLACK)
        {
          analogWrite(pinL_PWM, 150);
          analogWrite(pinR_PWM, 150);
          digitalWrite(pinL_DIR, 1);
          digitalWrite(pinR_DIR, 1);
        }
        // 两边都是白的时候,停下 
        else
        {
          analogWrite(pinL_PWM, 0);
          analogWrite(pinR_PWM, 0);
          digitalWrite(pinL_DIR, 0);
          digitalWrite(pinR_DIR, 0);
        }
      }
      rotate(1200);
      // 满电：1200
      // 超级满电、塑料：1000
      forward();
      delay(700);
      // 这里少弄一点，以免撞上
      analogWrite(pinL_PWM, 200);
      analogWrite(pinR_PWM, 200);
      digitalWrite(pinL_DIR, 0);
      digitalWrite(pinR_DIR, 0);
      delay(500);
      process += 1;
      break;
//
//
    // 默认，停下我们的小车
    default:
      analogWrite(pinL_PWM, 0);
      analogWrite(pinR_PWM, 0);
      digitalWrite(pinL_DIR, 1);
      digitalWrite(pinR_DIR, 1);
      delay(20000);
    }
    if(debug)
    {
      analogWrite(pinL_PWM, 0);
      analogWrite(pinR_PWM, 0);
      digitalWrite(pinL_DIR, 1);
      digitalWrite(pinR_DIR, 1);
      delay(3000);
    }
  }
}
