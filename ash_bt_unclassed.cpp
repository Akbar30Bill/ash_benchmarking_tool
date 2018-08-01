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

double total = 0;
double good_taste = 0;
double singlecore_score = 99999999;
double multicore_score = 99999999;

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
  if ((good_taste/total > 0.5 - ratio)&&(good_taste/total < 0.5 + ratio))
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
    system("clear");
    std::cout << "in thread 0x1 ratio: " << good_taste/total << std::endl;

  }
  clock_t end = clock();
  singlecore_score = (double)(end - begin)/CLOCKS_PER_SEC;
  return;
}

// void singlethread_calculate()
// {
//   for(int i = 0 ; i < first_calculations ; i++)
//   {
//     make_ash();
//   }
//   while(!check_good_taste_ash_ratio())
//   {
//     make_ash();
//     std::cout << "in thread 0x1 ratio: " << good_taste/total << std::endl;
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
    // system("clear");
    std::cout << "in thread " << threadid << " ratio: " << good_taste/total << std::endl;
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
  }
  pthread_exit(NULL);
  clock_t end = clock();
  multicore_score = (double)(end - begin)/CLOCKS_PER_SEC;
  return;
}

void benchmark()
{
  std::cout << "running benchmark..." << std::endl;
  singlecore_calculate();
  std::cout << "Single core Score: " << singlecore_score << std::endl;
  // multicore_calculate();
  std::cout << "MultiCore Score with " << concurentThreadsSupported << " threads: " << multicore_score << std::endl;
  std::cout << "CPU Clock speed: " << CLOCKS_PER_SEC << std::endl;
  std::cout << "OS name: " << getOsName() << std::endl;
}

int main(int argc, char const *argv[]) {
  benchmark();
  return 0;
}
