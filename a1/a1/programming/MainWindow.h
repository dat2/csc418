/*************************************************************************
    CSC418/2504, Winter 20l5
    Assignment 1


    Instructions:
        See main.cpp for more instructions.

        This file contains the class for the main window of the program.
**************************************************************************/

#ifndef __MainWindow_h__
#define __MainWindow_h__

#include <QtGui/QtGui>
#include "GLWidget.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow()
    {
        // create the main layout
        m_main_layout = new QVBoxLayout();

        // Create a GLWidget to hold the OpenGL viewport.
        m_gl_widget = new GLWidget();
        m_main_layout->addWidget(m_gl_widget);

        // create the slider layouts
        QBoxLayout *left_sliders = new QVBoxLayout();
        QBoxLayout *right_sliders = new QVBoxLayout();
        QBoxLayout *slider_layout = new QHBoxLayout();

        create_beak_slider(m_main_layout);

        slider_layout->addLayout(left_sliders);
        slider_layout->addLayout(right_sliders);

        m_main_layout->addLayout(slider_layout);

        // attach the sliders to their respective layouts
        create_left_sliders(left_sliders);
        create_right_sliders(right_sliders);
        create_other_controls();

        // set the main layout
        setLayout(m_main_layout);

        setWindowTitle("CSC418/2504 Assignment 1");
    }

    void create_other_controls()
    {
        QBoxLayout *controls_layout = new QHBoxLayout();

        // Create a button to quit the program.
        m_quit_button = new QPushButton("Quit", this);
        connect(
            m_quit_button, SIGNAL(clicked(bool)),
            this, SLOT(onPressQuit(bool)));
        controls_layout->addWidget(m_quit_button);

        // Create a checkbox to turn animation on and off, and make it
        // call GLWidget::onPressAnimate when checked.
        m_animate_checkbox = new QCheckBox("Animate", this);
        connect(
            m_animate_checkbox, SIGNAL(stateChanged(int)),
            m_gl_widget, SLOT(onPressAnimate(int)));
        controls_layout->addWidget(m_animate_checkbox);

        m_main_layout->addLayout(controls_layout);
    }

    void create_beak_slider(QBoxLayout *layout)
    {
        m_beak_slider = create_joint_angle_slider(layout,
        "Beak height", GLWidget::BEAK_MIN, GLWidget::BEAK_MAX);
        connect(
            m_beak_slider, SIGNAL(valueChanged(int)),
            m_gl_widget, SLOT(onSetBeakHeight(int)));
        m_beak_slider->setValue(m_gl_widget->getBeakHeight());
    }

    void create_left_sliders(QBoxLayout *layout)
    {
        m_x_slider = create_joint_angle_slider(layout,
        "X", GLWidget::X_MIN, GLWidget::X_MAX);
        connect(
            m_x_slider, SIGNAL(valueChanged(int)),
            m_gl_widget, SLOT(onSetX(int)));
        m_x_slider->setValue(m_gl_widget->getX());

        m_head_slider = create_joint_angle_slider(layout,
        "Head Angle", GLWidget::HEAD_MIN, GLWidget::HEAD_MAX);
        connect(
            m_head_slider, SIGNAL(valueChanged(int)),
            m_gl_widget, SLOT(onSetHeadAngle(int)));
        m_head_slider->setValue(m_gl_widget->getHeadAngle());

        m_left_leg_slider = create_joint_angle_slider(layout,
        "Left Leg Angle", GLWidget::LEFT_LEG_MIN, GLWidget::LEFT_LEG_MAX);
        connect(
            m_left_leg_slider, SIGNAL(valueChanged(int)),
            m_gl_widget, SLOT(onSetLeftLegAngle(int)));
        m_left_leg_slider->setValue(m_gl_widget->getLeftLegAngle());

        m_left_foot_slider = create_joint_angle_slider(layout,
        "Left Foot Angle", GLWidget::LEFT_FOOT_MIN, GLWidget::LEFT_FOOT_MAX);
        connect(
            m_left_foot_slider, SIGNAL(valueChanged(int)),
            m_gl_widget, SLOT(onSetLeftFootAngle(int)));
        m_left_foot_slider->setValue(m_gl_widget->getLeftFootAngle());
    }

    void create_right_sliders(QBoxLayout *layout)
    {
        m_y_slider = create_joint_angle_slider(layout,
        "Y", GLWidget::Y_MIN, GLWidget::Y_MAX);
        connect(
            m_y_slider, SIGNAL(valueChanged(int)),
            m_gl_widget, SLOT(onSetY(int)));
        m_y_slider->setValue(m_gl_widget->getY());

        m_arm_slider = create_joint_angle_slider(layout,
        "Arm Angle", GLWidget::ARM_MIN, GLWidget::ARM_MAX);
        connect(
            m_arm_slider, SIGNAL(valueChanged(int)),
            m_gl_widget, SLOT(onSetArmAngle(int)));
        m_arm_slider->setValue(m_gl_widget->getArmAngle());

        m_right_leg_slider = create_joint_angle_slider(layout,
        "Right Leg Angle", GLWidget::RIGHT_LEG_MIN, GLWidget::RIGHT_LEG_MAX);
        connect(
            m_right_leg_slider, SIGNAL(valueChanged(int)),
            m_gl_widget, SLOT(onSetRightLegAngle(int)));
        m_right_leg_slider->setValue(m_gl_widget->getRightLegAngle());

        m_right_foot_slider = create_joint_angle_slider(layout,
        "Right Foot Angle", GLWidget::RIGHT_FOOT_MIN, GLWidget::RIGHT_FOOT_MAX);
        connect(
            m_right_foot_slider, SIGNAL(valueChanged(int)),
            m_gl_widget, SLOT(onSetRightFootAngle(int)));
        m_right_foot_slider->setValue(m_gl_widget->getRightFootAngle());
    }

public slots:
    void onPressQuit(bool)
    {
        exit(0);
    }

private:
    QSlider *create_joint_angle_slider(
        QBoxLayout *layout,
        const char *label, int min_angle, int max_angle)
    {
        QSlider *slider = new QSlider(Qt::Horizontal, this);
        slider->setRange(min_angle, max_angle);
        slider->setSingleStep(1);
        slider->setPageStep(5);
        slider->setTickInterval(5);
        slider->setTickPosition(QSlider::TicksBelow);

        QBoxLayout *sub_layout = new QHBoxLayout();
        sub_layout->addWidget(new QLabel(label));
        sub_layout->addWidget(slider);
        layout->addLayout(sub_layout);

        return slider;
    }

    GLWidget *m_gl_widget;
    QCheckBox *m_animate_checkbox;
    QPushButton *m_quit_button;

    // sliders
    QSlider *m_x_slider;
    QSlider *m_y_slider;

    QSlider *m_arm_slider;
    QSlider *m_head_slider;
    QSlider *m_beak_slider;

    QSlider *m_left_leg_slider;
    QSlider *m_left_foot_slider;
    QSlider *m_right_foot_slider;
    QSlider *m_right_leg_slider;

    QVBoxLayout *m_main_layout;
};

#endif
