# Solution to "need help understanding something in my homework"

## Original Asker
[sp4celeader](https://www.reddit.com/user/sp4celeader/)

Posted in:
- https://www.reddit.com/r/C_Homework/comments/n0kdbm/need_help_understanding_something_in_my_homework/

## Original Question
hi guys,

i'm kinda new to the c

i have this homework where one of the tasks is like this :

i have a maximum of 20 steps :

the guys have to start from n[0] to n[19]

how it works fisrt step he should move 2 steps forward and then one backward

then 3 steps forward and one backward ( and everytime add one step to the forward steps and keeps the backward steps 1)

and then it should sum the backward steps including step n[0];

and if there was a steps left to do and it can't continue because the max is 20 it shouldn't add it to the sum.

well here is my code:

please help me understand what i explained to what i have to do in my code to make it work :

it only sums the odd arrays here in my code
```C
#define T 20
int steps(int a[])
 {
 	int i,j=1,sum=0;
    for(i=0; i<T;i++)
    {
i++;
           sum+=a[i];
// what I'm trying to do here is make the program start by two steps forward then
 backwards and then save the array that it have stopped on, after that it 
countinues from where it stopped and then moving 3 steps forward and then 1 
backward and (save the array that it stopped at) and continuing like this until
 it reaches the T Then sums all the arrays that it has stopped on
 
        }
     	return sum;
 }
int main ()
{
int,i,n[T],sum=0;  
    {
        scanf("%d",&n[i]);
    }
    sum=steps(n);
    printf("sum of array is :%d",sum);
    return 0;
}
```
