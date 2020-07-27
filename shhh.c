//Author: Jason Yuan
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<fcntl.h>

int main(){
   
   int end = 0;
   int count;
   int Pcount;
   int *start;
   int *temp;
   int input;
   int output;
   int pid;
   int fdL[2];
   int fdR[2];
   int file;
   char ch;
   char buffer[80];
   char *ptr;
   char *command[20];
   char inputfile[80];
   char outputfile[80];
     
   while(!end){
      
      /*************************  Start Reading  *************************/

      strcpy(buffer, "");       //empty the buffer
      strcpy(inputfile, "");    //empty the inputfile buffer
      strcpy(outputfile, "");   //empty the outputfile buffer
      count = 0;                //zero the command counter
      Pcount = 0;               //zero the process counter
      input = 0;                //zero the input flag
      output = 0;               //zero the ouput flag
      ptr = buffer;             //assign the pointer to the head of the buffer
      command[count] = ptr;     //assign the address of the first command
      count++;                  //get command count to the next

      fdR[0] = 0;
      fdR[1] = 0;

      printf("shhh>");
      
      ch = getchar();           //read the first character
      *ptr++ = ch;              //assign the first character
 
      while(ch != '\n'){
      
         ch = getchar();

	 if(ch == ' '){
	    *ptr++ = '\0';
            command[count] = ptr;
	    count++;
	 }

	 else if(ch != '\n'){
            if(ch == '|') Pcount++;
            *ptr++ = ch;   
	 }
      }

      *ptr++ = '\0';         //end of reading

      /*****************  End Reading, Start Processing  ******************/
      
      //preprocess for '|', '<', and '>'command

      start = (int*) malloc((Pcount+ 1) * sizeof(int));
      temp = start; //pointer to hold the head of the array
      *start++ = 0; //the start of the command always at 0

      for(int i = 0; i < count; i++){
	 
	 if(!strcmp(command[i], "|")){
            *start++ = i + 1;
	    command[i] = 0;
	 }

	 else if(!strcmp(command[i], "<")){
            strcpy(inputfile, command[i + 1]);
	    command[i] = 0;
	    input = 1;
	 }

	 else if(!strcmp(command[i], ">")){
            strcpy(outputfile, command[i + 1]);
	    command[i] = 0;
	    output = 1;
	 }

         //if(command[i] != 0) printf("%s\n", command[i]);
      }

      start = temp;

      //Section can check all segments of commands 
      /*for(int j = 0; j < Pcount + 1; j++ ){
	 printf("%d\n", start[j]);
      }

      printf("there are total %d process.\n", Pcount + 1);
      printf("the input file name is: %s \n", inputfile);
      printf("the output file name is: %s \n", outputfile);
      */
      
      //processing command

      if(!strcmp(command[0], "exit") && count == 1) end = 1;
      
      else if(strcmp(command[0], "\n") && strcmp(command[0], "exit")){
      
	 for(int i = 0; i < Pcount + 1; i++){
	 
            if( i >= 0 && i < Pcount + 1){
	       //printf("fdR0 = %d, fdR1 = %d\n", fdR[0], fdR[1]);
	       pipe(fdR);
	       //printf("This is pipe %d\n", i);
	    }

	    pid = fork();

	    if(pid < 0) printf("ERROR! Fork failed!\n");

	    else if(pid > 0){
	    
	        //printf("This is parent process %d\n", i);

                if(Pcount > 0){
                   close(fdL[0]);
      	           close(fdL[1]);	   

	           fdL[0] = fdR[0];
	           fdL[1] = fdR[1];
		}
		
                //printf("fdR0 = %d, fdR1 = %d\n", fdR[0], fdR[1]);
	        //printf("fdL0 = %d, fdL1 = %d\n", fdL[0], fdL[1]);

	      	
		wait(NULL);
		//printf("Back to the parent process %d\n", i);
 
	    }

	    else{
	    
	        //printf("This is the child process %d\n", i);
         
                //write end of the pipe
		if(i == 0 && input == 1){

		   if((file = open(inputfile, O_RDONLY)) < 0){
		      printf("File %s does NOT exist\n", inputfile);
		      exit(-1);
		   }
		   
		   dup2(file, STDIN_FILENO);
		   close(file); 
		   
		}


		if(Pcount > 0 && i < Pcount){
		   close(1);
		   dup(fdR[1]);
		   close(fdR[0]);
		   close(fdR[1]);
		}

                //read end of the pipe
		if(i > 0 && i < Pcount + 1){
	           close(0);
		   dup(fdL[0]);
		   close(fdL[0]);
		   close(fdL[1]);
		   
		   close(fdR[0]);
		   close(fdR[1]);

		}

		if(i == Pcount && output == 1){
		  if((file = creat(outputfile, 0750)) < 0) exit(-1);
                  
		  printf("Output successful! check file %s\n", outputfile); 

                  close(1);
		  dup(file);
		  close(file);
		  close(fdR[0]);
		  close(fdR[1]);
		}

	
		//printf("%s\n", command[start[i]]);
                execvp(command[start[i]], command);
	        printf("command NOT recognized\n");
		exit(0); //if the command is not recognized, need to end the child process.  
		    
		
            
	    }
	 
	 }
         
      }
      
      //printf("process end!\n");
      free(start); //free up the memory allocation of the array 
   }   
	
   return 0;
}
