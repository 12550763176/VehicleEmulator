#include "mainwindow.h"

#include <QApplication>
#include <QTextCodec>
#include <qtextcodec.h>
#include <signal.h>

void hanlder(int sig)
{
   if(sig==SIGPIPE){
       printf("socket error SIGPIPE");
   }
}
void  setHandler()
{
   struct sigaction sigact;
   memset(&sigact, 0, sizeof(sigact));
   sigact.sa_handler = hanlder;
   sigaction(SIGPIPE, &sigact, NULL);
}

int main(int argc, char *argv[])
{
    setHandler();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //w.setWindowFlags(w.windowFlags()& ~Qt::WindowMaximizeButtonHint& ~Qt::WindowMinimizeButtonHint);
    w.showMaximized();



    return a.exec();
}
