#include <iostream>
#include <ctime>
#include <string>
#include <stdlib.h>
#include <pthread.h>
#include <omp.h>
#include <thread>

const double ratio = 0.00001;
const int first_calculations = 5;
unsigned concurentThreadsSupported = std::thread::hardware_concurrency();

std::string getOsName()
{
  #ifdef _WIN32
  return "Windows 32-bit";
  #elif _WIN64
  return "Windows 64-bit";
  #elif __linux__
  return "Linux";
  #elif __unix || __unix__
  return "Unix";
  #elif __APPLE__ || __MACH__
  return "Mac OSX";
  #elif __FreeBSD__
  return "FreeBSD";
  #else
  return "Other";
  #endif
}
int total = 0;
int good_taste = 0;
int singlecore_score = 99999999;
int multicore_score = 99999999;


bool is_ash_ok()
{
  if ( rand()%2 && rand()%2 )
    return 1;
  return 0;
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
void singlecore_calculate()
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
  singlecore_score = (double)(end - begin)/CLOCKS_PER_SEC;
  return;
}
// void *singlethread_calculate()
// {
//   for(int i = 0 ; i < first_calculations ; i++)
//   {
//     make_ash();
//   }
//   while(!check_good_taste_ash_ratio())
//   {
//     make_ash();
//   }
//   pthread_exit(NULL);
// }
static void *singlethread_calculate_v2(void *threadid)
{
  for(int i = 0 ; i < first_calculations ; i++)
  {
    make_ash();
  }
  while(!check_good_taste_ash_ratio())
  {
    make_ash();
  }
  pthread_exit(NULL);
}
void multicore_calculate()
{
  clock_t begin = clock();
  pthread_t threads[concurentThreadsSupported];
  for ( int i = 0 ; i < concurentThreadsSupported ; i++ )
  {
    pthread_create(&threads[i], NULL, singlethread_calculate_v2, (void *)i);
    // pthread_create(&threads[i] , NULL , singlethread_calculate , NULL );
  }
  pthread_exit(NULL);
  clock_t end = clock();
  multicore_score = (double)(end - begin)/CLOCKS_PER_SEC;
  return;
}
void benchmark()
{
  singlecore_calculate();
  multicore_calculate();
  std::cout << "running singlecore benchmark..." << std::endl;
  std::cout << "Single core Score: " << singlecore_score << std::endl;
  std::cout << "MultiCore Score with " << concurentThreadsSupported << " threads: " << multicore_score << std::endl;
  std::cout << "OS name: " << getOsName() << std::endl;
}

int main(int argc, char const *argv[]) {
  benchmark();
  return 0;
}
