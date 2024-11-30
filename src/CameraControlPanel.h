#pragma once

#include "Camera/Camera.h"
#include "ElaScrollPage.h"

class CameraControlPanel :  public ElaScrollPage
{
    Q_OBJECT
public:
    explicit CameraControlPanel(QWidget *parent = nullptr);
private:
};