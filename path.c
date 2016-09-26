  #include<stdio.h>
  #include<stdlib.h>
  #include<unistd.h>
  #include<string.h>
  #include <sys/types.h>
  #include<sys/wait.h>

  #define TOK_DELIM " \t\r\n\a"

  int no_of_paths=0;
  int read_line(char **tokens,int *);
  void exe_cmds(char **tokens,int ,char **path);
  void createChild(char **,int pos);
  int checkPath(char **tokens,char **path);
  void changeDir(char **tokens,int pos);
  void addPath(char **tokens,int pos,char **);
  void print_err();
  void exit_func(int exit_code);



  int main(int argc, char const *argv[]) {
    char *path[128]={'\0'};
    char *tokens[128]={'\0'};
    int position=0;

    while(1)
    {
      printf("\n>> ");
      int ex=read_line(tokens,&position);
      if(!ex)
      {
        exit_func(1);
      }
      else if(ex==1)
      {
        exe_cmds(tokens,position,path);
      }

      memset(tokens, 0, sizeof tokens);
      position=0;
    } //while(1)
  }  //End of main()

  int read_line(char **tokens,int *position)
  {

    char cmdline[130]="";
    char *tokenize=NULL;
    char *e="exit\n";

    if(fgets(cmdline,130,stdin)==NULL || (strlen(cmdline)<=2) || (strlen(cmdline)>128))
    {
      print_err();
      return 2;
    }

    printf("len is : %ld %ld\n",strlen(cmdline),sizeof(cmdline));

    if(strcmp(cmdline,e)==0)
    {
      exit_func(0);
    }

    tokenize=strtok(cmdline,TOK_DELIM);
    while(tokenize!=NULL)
    {
      tokens[(*position)]=strdup(tokenize);
      tokenize=strtok(NULL,TOK_DELIM);
      (*position)++;
    }
    return 1;
  }

  void exe_cmds(char **tokens,int pos,char **path)
  {
    char ch[128];
    int path_index=0;


    if(strcmp(tokens[0],"pwd")==0)
    {
      if(pos>1)
      exit_func(1);
      char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        fprintf(stdout, "%s", cwd);
    else
        print_err();
    }
    else if(strcmp(tokens[0],"cd")==0)
    {
      changeDir(tokens,pos);
    }
    else if (strcmp(tokens[0],"path")==0)
    {
      addPath(tokens,pos,path);
    }
    else {
      int path_index=checkPath(tokens,path);
      if(path_index==-1)
      {
        print_err();
      }
      else
      {
        createChild(tokens,pos);
      }

    }
  }

  void changeDir(char **tokens,int pos)
  {
    if(pos==1)
    {
      char *cd=getenv("HOME");
      chdir(cd);
    }
    else
    if(pos==2)
    {
      int err=chdir(tokens[1]);
      if(err!=0)
      {
        print_err();
      }
    }
  }

  void addPath(char **tokens,int pos,char **path)
  {
    no_of_paths=0;
    int itr=0;
    if(pos>0)
    {
      while(itr<pos-1)
      {
        path[itr]=strdup(tokens[itr+1]);
        no_of_paths++;
        itr++;
      }
    }
  }

  int checkPath(char **tokens,char **path)
  {
    int itr=0;
    int returnStatus=-1;
    itr=0;
    while(itr<no_of_paths)
    {
      char temp[128]="";
      char *t1=strdup(path[itr]);
      strcat(temp,t1);
      strcat(temp,"/");
      char *t2=strdup(tokens[0]);
      char *ch=strcat(temp,t2);

      FILE *fp=fopen(ch,"r");
      if(fp)
      {
        tokens[0]=strdup(ch);
        returnStatus=itr;
        return returnStatus;
      }
      itr++;
    }
   return returnStatus;
  }

  void createChild(char **tokens,int pos)
  {
    pid_t  c_pid;
    c_pid = fork();
    if (c_pid == 0)
    {
      if(execv(tokens[0],tokens)==-1)
      {
        print_err();
      }
    }
    else
    if(c_pid<0)
    {
      print_err();
    }
    else
    if(c_pid>0)
    {
      waitpid (c_pid,0,0);
    }

  }

  void print_err()
  {
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
  }

  void exit_func(int exit_code)
  {
    exit(exit_code);
  }
