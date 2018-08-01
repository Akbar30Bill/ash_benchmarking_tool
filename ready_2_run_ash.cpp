#include <iostream>
#include <ctime>
#include <string>
#include <stdlib.h>

const double ratio = 0.00001;
const int first_calculations = 5;

std::string getOsName()
{
  #ifdef _WIN32
  return "Windows 32-bit";
  #elif _WIN64
  return "Windows 64-bit";
  #elif __unix || __unix__
  return "Unix";
  #elif __APPLE__ || __MACH__
  return "Mac OSX";
  #elif __linux__
  return "Linux";
  #elif __FreeBSD__
  return "FreeBSD";
  #else
  return "Other";
  #endif
}

class ash_taste
{
private:
  int total;
  int good_taste;
public:
  bool is_ash_ok()
  {
    if ( rand()%2 && rand()%2 )
      return 1;
    return 0;
  }
  ash_taste()
  {
    total = 0;
    good_taste = 0;
  }
  void make_ash()
  {
    total++;
    if (is_ash_ok())
      good_taste++;
  }
  bool check_good_taste_ash_ratio()
  {
    if (  ( (double)good_taste/(double)total )  <  ratio   )
      return 1;
    return 0;
  }
  double singlecore_calculate()
  {
    clock_t begin = clock();
    for(int i = 0 ; i < first_calculations ; i++)
    {
      make_ash();
    }
    while(!check_good_taste_ash_ratio())
    {
      make_ash();
    }
    clock_t end = clock();
    return (double)(end - begin)/CLOCKS_PER_SEC;
  }
  void singlecore_benchmark()
  {
    std::cout << "running singlecore benchmark..." << std::endl;
    std::cout << "Single core Score: " << singlecore_calculate() << std::endl;
    std::cout << "OS name: " << getOsName() << std::endl;
  }
};

int main(int argc, char const *argv[]) {
  ash_taste ash;
  ash.singlecore_benchmark();
  return 0;
}
