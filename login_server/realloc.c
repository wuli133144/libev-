

//realloc 
//return int 
//args 

realloc可以将ptr指向的内存调整为size大小。这个功能看上去很明确，其实则不然，其一共有三种
不同的行为：
·参数ptr为NULL，而size不为0，则等同于malloc（size）。
·参数ptr不为NULL，而size为0，则等同于free（ptr）。
·参数ptr和size均不为0，其行为类似于free（ptr）；malloc（size）。


//ptr is null 
//args
//return 



int *ptr=(int *)realloc(ptr,size);
if(ptr==null){
    //<==> malloc(size)    	
}else if(ptr!=null && size==0){
    //<==>free(ptr);	
}else{
	 //<==>free(ptr) malloc(size);
}


linux提供wait()函数获取子进程的退出状态


#include  <sys/wait.h>
pid_t wait(int *status )


errno     说明
ECHLD     调用进程时发现并没有子进程需要等待



等待所有子进程退出

while((childPid = wait(NULL)) != -1)
	continue;
if(errno!=ECHILD)
   errExit("wait");

一个进程可能包含多个线程，传统意义上的进程，不过是多线程的一种特例，即该进程只包含一个线程。


// pthread_self
// pthread_create
// pthread_detach

1）已经退出的线程，其空间没有被释放，仍然在进程的地址空间之内。
2）新创建的线程，没有复用刚才退出的线程的地址空间。



互斥量的使用

#include <pthread.h>
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_create()
pthread_detach()
pthread_self()


互斥量的加锁何解锁

//pthread_mutex_lock
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_trylock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);


 在等待的过程中，如果互斥量持有线程解锁互斥量，可能会发生如下事件：
·函数调用线程是唯一等待者，获得互斥量，成功返回。
·函数调用线程不是唯一等待者，但成功获得互斥量，返回。
·函数调用线程不是唯一等待者，没能获得互斥量，继续阻塞，等待下一轮


//pthread_mutex_lock
pthread_mutex_lock(&mutex);
global_cnt++;
pthread_mutex_lock(&mutex);


//一
条件变量的使用


pthread_cond_t  cond=PTHREAD_COND_INITIALIZER



//int ,char *
//return
int main(int ,char *){
	 
	printf("hello world! ok");
	
	return 0;
}


 



