#include <iostream>

int
main ()
{
  long long n;
  int s = 0;
  std::cin >> n;
  if (n > 0)
    while (n != 0)
      s += n & 1, n = n >> 1;
  std::cout << (s <= 1 ? "TAK" : "NIE") << std::endl;
}
