void timer_handler(int sig) {
    printf("Timer expired: 10 seconds of user+system CPU time\n");
}

int main() {
    struct itimerval timer;

    signal(SIGPROF, timer_handler);

    timer.it_value.tv_sec = 10;
    timer.it_value.tv_usec = 0;

    timer.it_interval.tv_sec = 10;
    timer.it_interval.tv_usec = 0;

    if (setitimer(ITIMER_PROF, &timer, NULL) == -1) {
        perror("Error setting timer");
        exit(1);
    }

    while (1) {

    }
}
