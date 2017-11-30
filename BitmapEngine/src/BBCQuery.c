#include <stdio.h>
#include <stdlib.h>
#include "Core.h"
#include "ActiveRun.h"

int OR_BBC(byte * ret, byte * col1, unsigned int size1, byte * col2, unsigned int size2)
{
    printf("hey\n");
    //The header position in col1 of run1
    int pos1 = 0;

    //The header position in col2 of run2
    int pos2 = 0;

    //the position we are in in our output array
    int out_pos = 0;


    //The first active run
    activeRun *run1 = initDefRun();
    /*run1 = initActiveRun(col1, size1, pos1);
    pos1++;*/

    //The second active run
    activeRun *run2 = initDefRun();
    /*run2 = initActiveRun(col2, size2, pos2);
    pos2++;*/

    int k = 0;
    while(pos1 < size1 && pos2 < size2)
    {
        printf("inside loop\n");
        //If run1 is empty than initiate the next run from col1
        if(run1->tail_len == 0 && run1->fill_len == 0)
        {
            run1 = initActiveRun(col1, size1, pos1);
            printActiveRun(run1);
            pos1++;
        }
        //If run2 is empty thatn intiate the next run form col2
        if(run2->tail_len == 0 && run2->fill_len == 0)
        {
            run2 = initActiveRun(col2, size2, pos2);
            printActiveRun(run2);
            pos2++;
        }
        //While run1 and run2 are not empty keep operating
        while((run1->tail_len != 0 || run1->fill_len != 0) && (run2->tail_len != 0 || run2->fill_len != 0))
        {
            //fill OR fill
            if(run1->fill_len > 0 && run2->fill_len > 0){
            
                int lookAhead = 0;
                if(run1->fill_len > run2->fill_len)
                {
                    lookAhead = run2->fill_len;
                }
                else
                {
                    lookAhead = run1->fill_len;
                }

                int i;
                //0 fill OR 0 fill
                if(run1->fill_bit == 0 && run2->fill_bit == 0){
                   

                    
                    for(i = 0; i < lookAhead; i++)
                    {
                        //ERROR here, need to mallloc the *ret variable to the size
                        //of the column(s) we are compressing. This should be done 
                        //a tthe top of this file. 
                        ret[out_pos + i] = 0b00000000;
                    }
                }
                //1 fill OR 0 fill, as well as 1 fill OR 1 fill
                else
                {
                    printf("1 and 1 fill \n");
                    for(i = 0; i < lookAhead; i++)
                    {
                        ret[out_pos + i] = 0b11111111;
                    }
                }
                out_pos += i;
                pos1 += i;
                pos2 += i;
                run1->fill_len -= lookAhead;
                run2->fill_len -= lookAhead;
            }
            //fill OR messy
            else if(run1->fill_len > 0 && run2->fill_len == 0){
                //0 fill and messy
                if(run1->fill_bit == 0){
                    ret[out_pos] = 0b00000000 | run2->tail_store[pos2];                 
                }
                //1 fill and messy
                else{
                    ret[out_pos] = 0b11111111 | run2->tail_store[pos2];
                }
                out_pos++;
                pos1++;
                pos2++;
                run1->fill_len--;
                run2->tail_len--;
            }
            //messy OR fill
            else if(run2->fill_len > 0 && run1->fill_len == 0){

                //0 fill and messy
                if(run1->fill_bit == 0){
                    ret[out_pos] = 0b00000000 | run1->tail_store[pos1];                 
                }
                //1 fill and messy
                else{
                    ret[out_pos] = 0b11111111 | run1->tail_store[pos1];
                }
                out_pos++;
                pos1++;
                pos2++;
                run2->fill_len--;
                run1->tail_len--;
            }
            //messy OR messy
            else{

                ret[out_pos] = run1->tail_store[pos1] | run2->tail_store[pos2];
                out_pos++;
                pos1++;
                pos2++;
                run1->tail_len--;
                run2->tail_len--;
            }
        }

        //If run1 is empty than update pos1 to the header postion of the next run
        if(run1->tail_len == 0 && run1->fill_len == 0)
        {
            free(run1);
        }
        
        //If run2 is empty than update pos2 to the header positon of the next run

        if(run2->tail_len == 0 && run2->fill_len == 0)
        {
            free(run2);
        }
        
    //If on success than return 1
    }

    printf("about to return\n");
    printf("size of ret is %u\n", out_pos);
    printf("ret at 0: %u\n", ret[0]);
    printf("ret at 1: %u\n", ret[1]);
    printf("ret at 2: %u\n", ret[2]);
    printf("ret at 3: %u\n", ret[3]);
    return out_pos;
}

/*int AND_BBC(byte * ret, byte * col1, unsigned int size1, byte * col2, unsigned int size2)
{
    return 1;
}*/
