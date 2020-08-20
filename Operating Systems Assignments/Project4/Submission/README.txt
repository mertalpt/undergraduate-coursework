1) Run 'make' to compile all modules and the test application.
2) hello-1 module is the introductory module that were done as part of
   the "Learning Kernel Module Programming" part of the project.
   It does not do much.
3) print_module is the module that is for the second part of the project.
4) app is the test program to show that print_module is working.
   One thing to note here is that it must be called as 'sudo ./app.o'
   because it calls privileged commands such as 'insmod' inside it.
5) After its execution, module output can be seen by 'dmesg'.
   Module prints its own name, 'print_module' at the begining and
   at the end of its execution. That can be used to search for
   the exact location.
6) app creates, writes to, closes and deletes 10 text files during its
   execution. This may take a while as already shown in its output.
7) This project was developed with kernel version 4.20.16.
   We have seen that people had problems stemming from working with
   differing kernel versions. If our project does not work as expected
   we would be happy to do a demonstration on our own computers.
