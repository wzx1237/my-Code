enum pin_name  
{ 
  A1 = 1,
  A2 = 2,
  A3,
  A4,
  A5,
  A6
};

enum io_put
{
    INPUT = 1,
    OUTPUT
};

const int HIGH = 1;
const int LOW = 0;

void analogWrite(const int a, int b){};
void digitalWrite(const int a, int b){};
void delay(int a){};
void pinMode(const int a, int b){};
bool digitalRead(const int a){};