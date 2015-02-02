#ifndef __GL_PENGUIN__
#define __GL_PENGUIN__

#include "GLBaseShapes.h"
#include "GLState.h"

class GLPenguin
{
public:
    GLPenguin(GLTransformStack &stack, GLState &glState,
    UnitSquare &unitSquare, UnitCircle &unitCircle)
        : transformStack(stack)
        , m_gl_state(glState)
        , m_unit_square(unitSquare)
        , m_unit_circle(unitCircle)
        {}
    void drawBody(void);

private:
    GLState &m_gl_state;
    GLTransformStack &transformStack;
    UnitSquare &m_unit_square;
    UnitCircle &m_unit_circle;

    double m_head_angle;
    double m_arm_angle;
    double m_left_leg_angle;
    double m_left_knee_angle;
    double m_right_leg_angle;
    double m_right_knee_angle;
};

#endif
