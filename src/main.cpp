#include <csignal>
#include <iostream>
#include <random>
#include <thread>

#include <unistd.h>

void ChildTerm(int);
void ChildProcess();

void ParentProcess();

double timer = -1;

const double kRandomLowerBound = 0.001;
const double kRandomUpperBound = 1;

int main(int argc,  char** argv)
{
    if(argc == 2) // set timer via command line
    {
        try
        { timer = std::stoi(argv[1]); }
        catch(const std::exception& e)
        { std::cerr << "Argument is invalid, using default timer\n"; }
    }

    signal(SIGCHLD, ChildTerm); // Bind restart mechanism
    if (fork() == 0)
    {
        ChildProcess(); // make child process
    }
    while(true);

    return 0;
}

void ChildProcess()
{
    using namespace std::chrono;
    int counter = 0;

    std::cout << "Starting child process (PID = " << getpid()
        << ", counter = " << counter << ")\n";

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
        ++counter;
        std::cout << "Child (PID:" << getpid() << ") increasing counter: " << counter << std::endl;
    }
}


void ChildTerm(int)
{
    signal(SIGCHLD, ChildTerm); // reset signal to current func
    std::cout << "Child dead, restarting child process\n";
    if(fork() == 0)
    {
        ChildProcess();
    }
}