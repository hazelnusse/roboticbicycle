#include "myclasses.h"

static Root root(1000);

void cplusplustest(void);

int main(void)
{
  while(1) {
    cplusplustest();
  }

  return 0;
}


void cplusplustest(void)
{
  Root *pm1, *pm2, *pm;

  root.say_hello();
  pm1 = new Child1(2000);
  pm1->say_hello();
  pm2 = new Child2(3000);
  pm2->say_hello();
  pm1->say_hello();
  pm2->say_hello();
  pm = pm1;
  pm->say_hello();
  pm = pm2;
  pm->say_hello();
  delete pm1;
  delete pm2;
}
