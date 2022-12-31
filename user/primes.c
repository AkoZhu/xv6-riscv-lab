//
// Created by Xuanbiao Zhu on 12/28/22.
//

#include "kernel/types.h"
#include "user/user.h"

#define RD 0
#define WR 1

const uint INT_LEN = sizeof(int);

 /**
  * @brief find the prime number
  * @param lpipe left pipe
  *
  * */
 void primes(int lpipe[2])
 {
      close(lpipe[WR]);
      int first;
      int data;
      if(read(lpipe[RD], &first, sizeof(int)) == sizeof(int))
      {
          printf("prime %d\n", first);

          int rpipe[2];
          pipe(rpipe);

          while(read(lpipe[RD], &data, sizeof(int)) == sizeof(int)){
              // if the data can't be divided by the first data, write it to the right neighbor.
              if(data % first != 0){
                  write(rpipe[WR], &data, sizeof(int));
              }
          }

          if(fork() == 0){
              close(lpipe[RD]);
              primes(rpipe);
          }else{
              close(rpipe[RD]);


//              while(read(lpipe[RD], &data, sizeof(int)) == sizeof(int)){
//                  // if the data can't be divided by the first data, write it to the right neighbor.
//                  if(data % first != 0){
//                      write(rpipe[WR], &data, sizeof(int));
//                  }
//              }


              close(rpipe[WR]);
              wait(0);
          }
      }
      return;
 }

 // Separate the reading/sending process and forking process.

 int main(int argc, const char *argv[])
 {
      close(RD);
      int p[2];
      pipe(p);

     for(int i = 2; i < 36; i++){
         write(p[WR], &i, INT_LEN);
     }

      if(fork() == 0){
          primes(p);
      }else{
          close(p[RD]);
//          for(int i = 2; i < 36; i++){
//              write(p[WR], &i, INT_LEN);
//          }
          close(p[WR]);

          wait(0);
      }

      exit(0);
 }

