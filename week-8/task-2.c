#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

int main() {
    setreuid(geteuid(), geteuid());
    system("id");
    // sudo chmod u+s filename
    // sudo chown user2:user2 filename
}