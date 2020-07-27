Lab 2 

Write a small shell - called shhh - that has the 
following capabilities:

1. Can execute a command with the accompanying arguments.  
2. Recognize multiple pipe requests and handle them.  
3. Recognize redirection requests and handle them.  
4. Type "exit" to quit the shhh shell.  

Sample commands:

shhh>ls 

shhh>ls -t -al

shhh>cat file.txt   (file.txt is an existing file)

shhh>ls -al > output.txt

And then open output.txt to see if the content is correct or not

shhh> ls | more | wc

shhh>./pre < input.txt | ./sort > output.txt (./pre and ./sort are the executable from proj1. 
                                          input.txt is the file that provides the input
                                          and output.txt is the output file)

shhh> exit

The shell shhh should always wait for ALL the 
commands to finish. The topology of the forked processes 
should be linear children; e.g the shell should have 
as many children as there are processes needed - 
with pipes connecting adjacent children.You may assume 
that any redirection in the command 
is specified like the third example above.  E.g. 
"redirection in" ( < ) is always specified before 
the first pipe appears  and "redirection out" ( > ) 
is always after the last pipe specified. To make 
life easier for you, you may assume that only 
commands with correct syntax are typed in.  In other 
words don't worry about errors in the formation of 
the commands.

The partial program is available in TRACS, lab2.c.
The command parsing part is already done in the program. 
On your part, you need to implement the above functions.


Submission:

In order not to lose any files, you'd better zip all your files into a .zip file. 

Submit your project to TRACS before the deadline. Homework will NOT be accepted 
through emails. You should write a readme textfile telling the grader how to run 
your programs. Without this file, it is very likely that your project will not be 
run properly. 
