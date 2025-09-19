void timer_handler(int sig) {
    static int count = 0;
    count++;
    printf("Timer expired: 10 microseconds of user+system CPU time (count=%d)\n", count);

    if (count >= 10) {
        exit(0);
    }
}

int main() {
    struct itimerval timer;

    signal(SIGPROF, timer_handler);

    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 10;

    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 10;

    if (setitimer(ITIMER_PROF, &timer, NULL) == -1) {
        perror("Error setting timer");
        exit(1);
    }

    while (1) {

    }
}
