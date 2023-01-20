
# MultiProgramming Operating System 

This MOS project is divided into 3 phases.



## Assumptions of Phase-1

- Jobs entered without error in input file
- No physical separation between jobs
- Job outputs separated in output file by 2 blank lines
- Program loaded in memory starting at location 00 
- No multiprogramming, load and run one program at a time
- SI interrupt for service request

## Assumptions of Phase-2
-	Jobs may have program errors
-	PI interrupt for program errors introduced
-	No physical separation between jobs
-	Job outputs separated in output file by 2 blank lines
-	Paging introduced, page table stored in real memory
-	Program pages allocated one of 30 memory block using random number generator  
-	Load and run one program at a time
-	Time limit, line limit, out-of-data errors introduced
-	TI interrupt for time-out error introduced
-	2-line messages printed at termination


