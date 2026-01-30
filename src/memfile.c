#define _GNU_SOURCE

#include <assert.h>
#include <err.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

const char content[] = {
#embed "hello.c" if_empty('M', 'i', 's', 's', 'i', 'n', 'g', '\n')
    , 0};

int main() {
  int fd = memfd_create("tempfile", 0);

  if (fd == -1) {
    perror("memfd_create");
    return EXIT_FAILURE;
  }

  // if (ftruncate(fd, size) == -1)
  //   err(EXIT_FAILURE, "truncate");

  char fddir[1024] = {0};
  snprintf(fddir, sizeof(fddir), "/proc/%jd/fd", (intmax_t)getpid());

  char tempfile[1024] = {0};
  snprintf(tempfile, sizeof(tempfile), "%s/%d", fddir, fd);

  printf("PID: %jd; fd: %d; %s\n", (intmax_t)getpid(), fd, tempfile);
  assert(write(fd, content, strlen(content)) > 0);

  // syncfs(fd);

  // Optionally, map the file into memory
  // void *addr = mmap(NULL, sizeof(content), PROT_READ, MAP_SHARED, fd, 0);
  // printf("%s\n", (char *)addr);

  if (fork() == 0) {
    // NOTE: Child process
    const char *command[] = {"ls", "-lhA", fddir, NULL};

    for (char **arg = (char **)command; *arg != NULL; ++arg) {
      printf("%s ", *arg);
    }
    printf("\n");

    if (execvp(command[0], (char **)command) == -1) {
      perror("execvp failed");
      return EXIT_FAILURE;
    }
  }

  wait(NULL);

  if (fork() == 0) {
    // NOTE: Child process
    const char *command[] = {"cc", "-x", "c", tempfile, NULL};

    for (char **arg = (char **)command; *arg != NULL; ++arg) {
      printf("%s ", *arg);
    }
    printf("\n");

    if (execvp(command[0], (char **)command) == -1) {
      perror("execvp failed");
      return EXIT_FAILURE;
    }
  }

  wait(NULL);

  getchar();

  // Clean up
  // munmap(addr, sizeof(content));
  close(fd);

  return EXIT_SUCCESS;
}
