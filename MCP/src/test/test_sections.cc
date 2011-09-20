static const int a = 1;
static int b = 2;
const int c = 3;

int main(void)
{
  static const int d = 4 ;
  static int e = 5;
  const int f = 6;

  e = c + f + a;
  b = a + c + d + e + f;

  return 0;
}
