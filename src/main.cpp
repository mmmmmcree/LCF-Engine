#include "Window.h"

#include <QApplication>
#include "ElaTheme.h"
#include "ElaApplication.h"
#pragma comment(lib, "user32.lib")

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    eApp->init();
    eApp->setIsEnableMica(true);
    Window w;
    w.show();
    eTheme->setThemeMode(ElaThemeType::Dark); 
    return a.exec();
}