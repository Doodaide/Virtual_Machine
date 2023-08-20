Documentation: 

Header.h contains all the functions

getInstructionSet.c contains all of the functions related to extracting information from the binary file 

heapMemoryFuncSet.c contains all of the functions related to utilising the heap banks as a linked structure. 
Interpretation of the banks is still done in the main vm_riskxvii.c file 

OperationSet.c contains all of the operations (add, sub, etc.). 

vm_riskxvii.c contains all of the code related to reading and running the virtual machine. 

Error handling notes: 
- You are not allowed to read or write words at the edge of "virtual memory" that hang over. 
  - For example, loading a word at the second to last byte in VM will cause an error. 

My File is slightly too large (only just 8% over the size). But using gzip or other compression methods with 
makefiles was very tedious, and resulted in many files created. 

running make coverage will get a report of the code coverage of the testcases I have written. 
Tests are inside the tests directory. There is also the OGTests, which contains the original test c code.

Memory leaks were tested with valgrind. 

