#ifndef __GL_PENGUIN__
#define __GL_PENGUIN__

#include "GLBaseShapes.h"
#include "GLState.h"

class GLPenguin
{
public:
    GLPenguin(GLState &glState)
        : m_gl_state(glState)
        , m_head_angle(0)
        , m_arm_angle(0)
        , m_left_leg_angle(0)
        , m_left_knee_angle(0)
        , m_right_leg_angle(45)
        , m_right_knee_angle(0)
        {}

    void initialize() {
        // Copy the data for the shapes we'll draw into the video card's memory.
        m_unit_square.initialize(m_gl_state.VERTEX_POSITION_SHADER_LOCATION);
        m_unit_circle.initialize(
        m_gl_state.VERTEX_POSITION_SHADER_LOCATION,
        /*num_circle_segments=*/100);
        m_unit_hexagon.initialize(m_gl_state.VERTEX_POSITION_SHADER_LOCATION);
    }

    void draw(void);
private:
    void drawBody(void);
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
