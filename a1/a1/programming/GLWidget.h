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
    static const int HEAD_MIN = -45;
    static const int HEAD_MAX = 45;

    static const int ARM_MIN = -45;
    static const int ARM_MAX = 45;

    static const int LEFT_LEG_MIN = -45;
    static const int LEFT_LEG_MAX = 45;

    static const int LEFT_KNEE_MIN = -45;
    static const int LEFT_KNEE_MAX = 45;

    static const int RIGHT_KNEE_MIN = -45;
    static const int RIGHT_KNEE_MAX = 45;

    static const int RIGHT_LEG_MIN = -45;
    static const int RIGHT_LEG_MAX = 45;

    explicit GLWidget(QWidget *parent=NULL);

public slots:
    // This method is called when the user changes the joint angle slider.
    //////////////////////////////////////////////////////////////////////////
    // TODO:
    //   There is currently only one joint, but you need to add more.
    //////////////////////////////////////////////////////////////////////////
    void setHeadAngle(int angle)
    {
        // This method is called when the user changes the slider value.
        m_penguin.setHeadAngle(angle);

        // Call update() to trigger a redraw.
        update();
    }

    void setArmAngle(int angle)
    {
        // This method is called when the user changes the slider value.
        m_penguin.setArmAngle(angle);

        // Call update() to trigger a redraw.
        update();
    }

    void onPressAnimate(int is_animating)
    {
        // This method is called when the user changes the animation checkbox.
        m_is_animating = (bool)is_animating;
        m_animation_frame = 0;
        update();
    }

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void timerEvent(QTimerEvent *event);
    void drawBody();

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
