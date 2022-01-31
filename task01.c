#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Hello, your uid: %u gid: %u. \n", getuid(), getgid());
    return 0;
}
