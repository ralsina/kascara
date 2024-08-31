
#include <kapp.h>

#include "mainwin.h"

KConfig *conf;
KLocale *nls;

int main( int argc, char **argv )
{
    KApplication a( argc, argv, "Kascara" );

    if (argc < 2)
    {
        printf ("\nKascara version 0.0.1, proof of concept.\n");
        printf ("(c) 1997 Roberto Alsina <ralsina@unl.edu.ar>\n\n");
        printf ("Usage: Kascara program arg1 arg2 arg3 ... argn\n\n");
        printf ("Hope you find it useful ;-)\n\n");
        exit (1);
    }
    
    conf=a.getConfig();
    nls=a.getLocale();

    KShellWin w(argc-1,argv+1);
    a.setMainWidget (&w);
    w.show();
    a.exec();
}