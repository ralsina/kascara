#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#include <expect.h>

main(int argc,char **argv)
{
    if (argc < 2)
    {
        printf ("\nttyize version 0.0.1, and hopefully last.\n");
        printf ("(c) 1997 Roberto Alsina <ralsina@unl.edu.ar>\n\n");
        printf ("Usage: ttyize program arg1 arg2 arg3 ... argn\n\n");
        printf ("This program sucks, and that's just because of my ignorance.\n");
	printf ("Also, its utterly useless except as part of Kascara");
        printf ("Hope you like it ;-)\n\n");
        exit (1);
    }
    
    exp_stty_init=" -echo echonl ";
    
    int pipe=exp_spawnv(argv[1],argv+1);
    fcntl(pipe,F_SETFL,O_NONBLOCK);
    fcntl(0,F_SETFL,O_NONBLOCK);
    fcntl(1,F_SETFL,O_NONBLOCK);
    fcntl(2,F_SETFL,O_NONBLOCK);
    
    FILE *fpipe=fdopen(pipe,"r+");
    setbuffer (fpipe,NULL,_IONBF);
    setbuffer (stdin,NULL,_IONBF);
    setbuffer (stdout,NULL,_IONBF);
    setbuffer (stderr,NULL,_IONBF);
    
    char outBuf[1024];
    char inBuf[1024];
    
    int c1=0;
    int c2=0;
    
    while (1)
    {
        
        c1=fread (outBuf,1,1022,fpipe);
        if (c1>=0)
        {
            fwrite (outBuf,1,c1,stdout);
            fflush (stdout);
        }
        
        c2=fread (inBuf,1,1022,stdin);
        if (c2>=0)
        {
            fwrite (inBuf,1,c2,fpipe);
            fflush (fpipe);
        }
    }
}
