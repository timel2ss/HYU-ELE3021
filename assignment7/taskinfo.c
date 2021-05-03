#include <stdio.h>
#include <linux/unistd.h>

int main(int argc, char* argv[])
{
    syscall(323);
    return 0;
}
