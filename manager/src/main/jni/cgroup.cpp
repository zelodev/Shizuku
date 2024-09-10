#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>

namespace cgroup {

    static ssize_t fdgets(char *buf, const size_t size, int fd) {
        ssize_t len = 0;
        buf[0] = '\0';
        while (len < size - 1) {
            ssize_t ret = read(fd, buf + len, 1);
            if (ret < 0)
                return -1;
            if (ret == 0)
                break;
            if (buf[len] == '\0' || buf[len++] == '\n') {
                break;
            }
        }
        buf[len] = '\0';
        buf[size - 1] = '\0';
        return len;
    }

    int switch_cgroup(int pid) {
        char buf[PATH_MAX];

        // TODO: add more later on
        const char* cgroup_paths[] = {
                "/sys/fs/cgroup/uid_0/cgroup.procs",
                "/acct/uid_0/cgroup.procs",
                "/acct/cgroup.procs"
        };

        for (const char* path : cgroup_paths) {
            int fd = open(path, O_WRONLY | O_APPEND);
            if (fd != -1) {
                snprintf(buf, sizeof(buf), "%d\n", pid);
                if (write(fd, buf, strlen(buf)) != -1) {
                    close(fd);
                    return 0;
                }
                close(fd);
            }
        }

        return -1;
    }
}