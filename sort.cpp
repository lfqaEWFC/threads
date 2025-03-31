#include "Threadpool.hpp"
#include <iostream>
#include <unistd.h>
#include <algorithm> 

pthread_mutex_t lock;
#define NUMSIZE 1000000
#define THREADSHOLD 50000
typedef struct sorttask{
    int left;
    int right;
    int *data;
}sorttask;
pool addpool(10);

void* creat(void* args){
    sorttask* creat = (sorttask*)args;
    creat->data = new int[NUMSIZE];
    srand(time(NULL));
    for(int i = 0;i < NUMSIZE;i++){
        creat->data[i] = rand() % NUMSIZE;
    }
    creat->left = 0;
    creat->right = NUMSIZE-1;

    return nullptr;
}

void* pthsort(void* args)
{
    sorttask*sort = (sorttask*)args;
    if(sort->left>=sort->right){
        return nullptr;
    }
    if(sort->right - sort->left < THREADSHOLD) {
        std::sort(sort->data + sort->left, sort->data + sort->right + 1);
        delete sort;
        return nullptr;
    }
    int begin = sort->left,end = sort->right,pivot = begin;
    int tmp = sort->data[pivot];
    while(begin<end){   
        while(sort->data[end]>=tmp && begin<end){
            end--;
        }
        sort->data[pivot]=sort->data[end];
        pivot = end;
        while(sort->data[begin]<=tmp && begin<end){
            begin++;
        }
        sort->data[pivot]=sort->data[begin];
        pivot=begin;
    }
    pivot = begin;
    sort->data[pivot]=tmp;

    sorttask*leftsort = new sorttask;
    sorttask*rightsort = new sorttask;
    leftsort->data=sort->data; leftsort->left=sort->left; leftsort->right=pivot-1;
    rightsort->data=sort->data; rightsort->left=pivot+1; rightsort->right = sort->right;
    
    addpool.addtask(pthsort,(void*)leftsort);
    addpool.addtask(pthsort,(void*)rightsort);
    return nullptr;

}

int main(){
    sorttask* args = new sorttask;
    creat((void*)args);
    addpool.addtask(pthsort,(void*)args);
    addpool.wait();
    for(int i=0;i<NUMSIZE;i++){
        std::cout << args->data[i] << std::endl;
    }
}