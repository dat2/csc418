/*************************************************************************
    CSC418/2504, Winter 20l5
    Assignment 1

    Instructions:
        See main.cpp for more instructions.

        This file contains the class OpenGL portion of the main window.
**************************************************************************/

#include "GLWidget.h"
#include "GLPenguin.h"
#include <iostream>

static QGLFormat createQGLFormat()
{
    // Return a QGLFormat object that tells Qt information about what
    // OpenGL context we would like.
    QGLFormat format(QGL::SampleBuffers);
    if (supportsNewOpenGL())
    {
	// Choose a version of OpenGL that supports vertex array objects and
	// tell it that we do not want support for deprecated functions.
        format.setVersion(3, 3);
        format.setProfile(QGLFormat::CoreProfile);
    }
    return format;
}

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(createQGLFormat(), parent)
    , m_is_animating(false)
    , m_animation_frame(0)
    , m_penguin(m_gl_state)
{
    // Start a timer that will call the timerEvent method every 50ms.
    startTimer(/*milliseconds=*/50);
}

void GLWidget::initializeGL()
{
    m_gl_state.initializeGL();

    // To aid with troubleshooting, print out which version of OpenGL we've
    // told the driver to use.
    std::cout << "Using OpenGL: " << glGetString(GL_VERSION) << std::endl;

    m_penguin.initialize();

    // Tell OpenGL what color to fill the background to when clearing.
    glClearColor(/*red=*/0.7f, /*green=*/0.7f, /*blue=*/0.9f, /*alpha=*/1.0f);

}

void GLWidget::resizeGL(int width, int height)
{
    // Respond to the window being resized by updating the viewport and
    // projection matrices.

    checkForGLErrors();

    // Setup projection matrix for new window
    m_gl_state.setOrthographicProjectionFromWidthAndHeight(width, height);

    // Update OpenGL viewport and internal variables
    glViewport(0, 0, width, height);
    checkForGLErrors();
}

void GLWidget::timerEvent(QTimerEvent *)
{
    // Respond to a timer going off telling us to update the animation.
    if (!m_is_animating)
        return;

    // increment the frame number.
    m_animation_frame++;

    // Update joint angles.

    //////////////////////////////////////////////////////////////////////////
    // TODO:
    //   Modify this function to animate the character's joints
    //   Note: Nothing should be drawn in this function!
    //////////////////////////////////////////////////////////////////////////

    // rotate the arm
    const double arm_rot_speed = 0.06;
    double arm_rot_t =
        (sin(m_animation_frame * arm_rot_speed) + 1.0) / 2.0;
    m_penguin.m_arm_angle = arm_rot_t * ARM_MIN + (1 - arm_rot_t) * ARM_MAX;

    // rotate the head
    const double head_rot_speed = 0.1;
    double head_rot_t =
        (sin(m_animation_frame * head_rot_speed) + 1.0) / 2.0;
    m_penguin.m_head_angle = head_rot_t * HEAD_MIN + (1 - head_rot_t) * HEAD_MAX;

    // rotate the legs
    const double leg_rot_speed = 0.05;
    double leg_rot_t =
        (sin(m_animation_frame * leg_rot_speed) + 1.0) / 2.0;
    m_penguin.m_left_leg_angle = leg_rot_t * LEFT_LEG_MIN + (1 - leg_rot_t) * LEFT_LEG_MAX;
    m_penguin.m_right_leg_angle = -(leg_rot_t * RIGHT_LEG_MIN + (1 - leg_rot_t) * RIGHT_LEG_MAX);

    // rotate the feet
    const double foot_rot_speed = 0.1;
    double foot_rot_t =
        (sin(m_animation_frame * foot_rot_speed) + 1.0) / 2.0;
    m_penguin.m_left_foot_angle = foot_rot_t * LEFT_FOOT_MIN + (1 - foot_rot_t) * LEFT_FOOT_MAX;
    m_penguin.m_right_foot_angle = foot_rot_t * RIGHT_FOOT_MIN + (1 - foot_rot_t) * RIGHT_FOOT_MAX;

    // move the beak up and down
    const double beak_movement_speed = 0.1;
    double beak_move_t =
        (sin(m_animation_frame * beak_movement_speed) + 1.0) / 2.0;
    m_penguin.m_beak_height = beak_move_t * BEAK_MIN + (1 - beak_move_t) * BEAK_MAX;

    // move x and y
    const double x_speed = 0.02;
    const double y_speed = 0.05;
    double x_move_t =
        (sin(m_animation_frame * x_speed) + 1.0) / 2.0;
    double y_move_t =
        (sin(m_animation_frame * y_speed) + 1.0) / 2.0;

    m_penguin.m_x = x_move_t * X_MIN + (1 - x_move_t) * X_MAX;
    m_penguin.m_y = y_move_t * Y_MIN + (1 - y_move_t) * Y_MAX;
    // Tell this widget to redraw itself.
    update();
}

void GLWidget::paintGL()
{
    // This method gets called by the event handler to draw the scene, so
    // this is where you need to build your scene -- make your changes and
    // additions here.  All rendering happens in this function.

    checkForGLErrors();

    // Clear the screen with the background colour.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Setup the model-view transformation matrix stack.
    transformStack().loadIdentity();
    checkForGLErrors();

    //////////////////////////////////////////////////////////////////////////
    // TODO:
    //   Modify this function draw the scene.  This should include function
    //   calls to pieces that apply the appropriate transformation matrices
    //   and render the individual body parts.
    //////////////////////////////////////////////////////////////////////////

    // Draw our hinged object
    m_penguin.draw();

    // Execute any GL functions that are in the queue just to be safe
    glFlush();
    checkForGLErrors();
}
