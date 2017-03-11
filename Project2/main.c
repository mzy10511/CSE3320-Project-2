#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX 1024

#include <pthread.h>

int total = 0 ;
int tNum = 2; //Thread Count, at least 2
int n1 , n2;
char *s1 , *s2;
FILE *fp ;

int readf(FILE * fp){
    /*
    if((fp=fopen("strings.txt","r"))==NULL){
        printf("ERROR: cant open string.txt!\n");
        return 0;
    }
    s1=(char *)malloc(sizeof(char)*MAX);
    if(s1==NULL){
        printf("ERROR: Out of memory\n");
        return -1;
    }
    s2=(char *)malloc(sizeof(char)*MAX);
    if(s1==NULL){
        printf("ERROR: Out of memory\n");
        return -1;
    }

    s1=fgets(s1, MAX, fp);
    s2=fgets(s2, MAX, fp);
    n1=strlen(s1);
    n2=strlen(s2)-1;
    */

    //test hardcode
    s1 = "aaa";
    s2 = "aa";
    n1=strlen(s1);
    n2=strlen(s2)-1;


    if(s1==NULL || s2==NULL || n1<n2 ){
        return -1;
    }
    return 1;
}

int num_substring (int tid){
    int i, j, k;
    int count;

    //Load Splitting variable
    int n1_split = (n1/tNum) * tid; //equal partition, sets n1_split to first index of the partition
    int n2_split = n1_split * 2 + (n2 - 1); //unsafely sets n2_split to ending index of the partition + tries to catch boundary substrings
    if(n2_split > (n1)){    //if n2_split is out of bounds force it inbounds
        n2_split = n1-n2;
    }

    for(i = n1_split;i <= (n2_split); i++){
        count = 0;
        for(j = i, k = 0; k < n2; j++, k++){ /* se a rc h f o r t h e nex t s t r i n g o f s i z e o f n2*/
            if(*(s1 + j) != *(s2 + k)){
                break;
            }
            else
                count++;
            if(count==n2)
                total++; /* f i n d a s u b s t r i n g in t h i s s t e p */
        }
    }
    return total;
}

int main(int argc, char * argv[]){
    int count;
    int i, rc;
    readf(fp);

    //Thread Setup
    pthread_t thread[tNum];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    //Create tNum number of threads
    for(i = 0; i < tNum; i++){
        rc = pthread_create(&thread[i], &attr, num_substring, i);
    }
    //Join threads
    for(i = 0; i < tNum; i++){
        rc = pthread_join(thread[i], NULL);
        if(rc){
            printf("ERROR: return code from pthread_join() is %d\n", rc);
            exit(-1);
        }
    }


    //count = num_substring();//remove and implement multithreaded solution
    printf("The number of sub strings is: %d\n", total);


    return 1;
}
