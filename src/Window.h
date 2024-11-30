#pragma once
#include <QMainWindow>
#include "ElaWindow.h"
// #include <QWidget>

class Window : public ElaWindow {
    Q_OBJECT

public:
    Window(QWidget* parent = nullptr);
    ~Window();
};