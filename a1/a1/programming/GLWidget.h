/*************************************************************************
    CSC418/2504, Winter 20l5
    Assignment 1

    Instructions:
        See main.cpp for more instructions.

        This file contains the OpenGL portion of the main window.
**************************************************************************/

#ifndef __GLWidget_h__
#define __GLWidget_h__

#include "include_gl.h"
#include "GLState.h"
#include "GLBaseShapes.h"
#include "GLPenguin.h"
#include <QtOpenGL/QtOpenGL>

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    // These values control the bounds to display on the joint angle sliders.
    //////////////////////////////////////////////////////////////////////////
    // TODO:
    //   Add different ranges for the different joints.  Use these ranges
    //   when constructing sliders and when animating joints.
    //////////////////////////////////////////////////////////////////////////
    static const int HEAD_MIN = -15;
    static const int HEAD_MAX = 15;

    static const int BEAK_MIN = -8;
    static const int BEAK_MAX = 0;

    static const int ARM_MIN = -45;
    static const int ARM_MAX = 45;

    static const int LEFT_LEG_MIN = -45;
    static const int LEFT_LEG_MAX = 45;

    static const int LEFT_FOOT_MIN = -45;
    static const int LEFT_FOOT_MAX = 45;

    static const int RIGHT_LEG_MIN = -45;
    static const int RIGHT_LEG_MAX = 45;

    static const int RIGHT_FOOT_MIN = -45;
    static const int RIGHT_FOOT_MAX = 45;

    static const int X_MIN = -300;
    static const int X_MAX = 300;

    static const int Y_MIN = -100;
    static const int Y_MAX = 100;

    explicit GLWidget(QWidget *parent=NULL);

public slots:
    void onSetHeadAngle(int angle)
    { m_penguin.setHeadAngle(angle); update(); }
    void onSetArmAngle(int angle)
    { m_penguin.setArmAngle(angle); update(); }
    void onSetLeftLegAngle(int angle)
    { m_penguin.setLeftLegAngle(angle); update(); }
    void onSetLeftFootAngle(int angle)
    { m_penguin.setLeftFootAngle(angle); update(); }
    void onSetRightLegAngle(int angle)
    { m_penguin.setRightLegAngle(angle); update(); }
    void onSetRightFootAngle(int angle)
    { m_penguin.setRightFootAngle(angle); update(); }
    void onSetX(int x)
    { m_penguin.setX(x); update(); }
    void onSetY(int y)
    { m_penguin.setY(-y); update(); }
    void onSetBeakHeight(int height)
    { m_penguin.setBeakHeight(height); update(); }

    void onPressAnimate(int is_animating)
    {
        // This method is called when the user changes the animation checkbox.
        m_is_animating = (bool)is_animating;
        m_animation_frame = 0;
        update();
    }

    float getX() const
    { return m_penguin.getX(); }
    float getY() const
    { return m_penguin.getY(); }
    float getBeakHeight() const
    { return m_penguin.getBeakHeight(); }

    float getHeadAngle() const
    { return m_penguin.getHeadAngle(); };
    float getArmAngle() const
    { return m_penguin.getArmAngle(); };
    float getLeftLegAngle() const
    { return m_penguin.getLeftLegAngle(); };
    float getLeftFootAngle() const
    { return m_penguin.getLeftFootAngle(); };
    float getRightLegAngle() const
    { return m_penguin.getRightLegAngle(); };
    float getRightFootAngle() const
    { return m_penguin.getRightFootAngle(); };

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void timerEvent(QTimerEvent *event);

private:
    GLTransformStack &transformStack()
    { return m_gl_state.transformStack(); }

private:
    GLState m_gl_state;
    bool m_is_animating;
    int m_animation_frame;

    UnitSquare m_unit_square;
    UnitCircle m_unit_circle;
    //////////////////////////////////////////////////////////////////////////
    // TODO: Add additional joint parameters.
    //////////////////////////////////////////////////////////////////////////
    GLPenguin m_penguin;
};

#endif
