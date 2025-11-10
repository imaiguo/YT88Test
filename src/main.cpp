
#include "DialogFunction.h"
#include <QApplication>

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    DialogFunction dialog;
    dialog.show();

    return a.exec();
}
