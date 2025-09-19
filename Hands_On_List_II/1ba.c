void timer_handler(int sig){
        printf("Timer expired: 10 seconds interval\n");
}

int main(){
        struct itimerval timer ;

        signal(SIGVTALRM,timer_handler);

        timer.it_value.tv_sec = 10 ;
        timer.it_value.tv_usec = 0 ;

        timer.it_interval.tv_sec = 10;
        timer.it_interval.tv_usec = 0;

        if(setitimer(ITIMER_VIRTUAL,&timer,NULL)==-1){
                perror("Error setting timer");
                exit(1);
        }

        while(1){

        }
}
