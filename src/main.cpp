#include <csignal>    // std::signal
#include <iostream>   // std::cout, std::cerr
#include <random>     // std::random_device, std::mt19937, std::uniform_real_distribution
#include <thread>     // std::this_thread::sleep_for
#include <memory>     // std::shared_ptr
#include <chrono>     // std::chrono ...

#include <unistd.h>   // fork, getpid
#include <sys/mman.h> // mmap, munmap

void ChildTerm(int);
void ChildProcess();

double timer = -1;            // -1 - default, can be modified
                              // via second argoment in console
const int kPageSize = 4096;   // for shared memory
std::shared_ptr<int> counter; // RAII mechanism

const double kRandomLowerBound = 0.001; // seconds, lower bound for random
const double kRandomUpperBound = 1;     // seconds, upper bound for random

int main(int argc,  char** argv)
{
    // Preparing for RAII mechanism
    void *allocated_ptr = mmap(NULL, kPageSize,
                               PROT_READ | PROT_WRITE,
                               MAP_SHARED | MAP_ANONYMOUS,
                               -1, 0);
    auto deleter = [](void *ptr){
        munmap(ptr, kPageSize);
    };
    // Put allocated pointer and assign deleter
    counter.reset(static_cast<int *>(allocated_ptr), deleter);

    *counter = 0; // reset counter

    if(argc == 2) // set timer via command line
    {
        try // try to convert second argument to integer
        { timer = std::stoi(argv[1]); }
        catch(const std::exception& e)
        { std::cerr << "Argument is invalid, using default timer\n"; }
    }

    std::signal(SIGCHLD, ChildTerm); // Bind restart mechanism
    if (fork() == 0) // make child process
    { ChildProcess(); }
    while(true); // Exit with Ctrl-Z :)

    return 0;
}

void ChildProcess()
{
    using namespace std::chrono;

    std::cout << "Starting child process (PID = " << getpid()
        << ", counter = " << *counter << ")\n";
    // Generate random timer if not set via command line
    // use kRandomLowerBound and kRandomUpperBound
    if(timer == -1)
    {
        // Make random mechanism
        std::random_device rd;  //Will be used to obtain a seed for the random number engine
        std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
        std::uniform_real_distribution<> distrib(kRandomLowerBound, kRandomUpperBound);

        timer = distrib(gen);
        std::cout << "Generated timer for child process: " << timer << "s\n";
    }
    else
    { std::cout << "Timer for child process: " << timer << "s\n"; }

    while(true)
    {
        std::this_thread::sleep_for(duration<double>(timer));
        ++(*counter);
        std::cout << "Child (PID:" << getpid() << ") increasing counter: " << *counter << std::endl;
    }
}

void ChildTerm(int)
{
    std::signal(SIGCHLD, ChildTerm); // reset signal
    std::cout << "Child dead, restarting child process\n";
    if(fork() == 0)
    { ChildProcess(); }
}
