#include <sys/types.h>
#include <unistd.h>

#include <csignal>
#include <iostream>

void ChildTerm(int);
void ChildProcess();

void ParentProcess();

int main()
{
    signal(SIGCHLD, ChildTerm);

    if (fork() == 0)
    {
        ChildProcess();
    }
    std::cout << "Parent PID: " << getpid() << std::endl;
    while(true);

    return 0;
}

void ChildProcess()
{
    std::cout << "Child PID: " << getpid() << std::endl;
    while(true);
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