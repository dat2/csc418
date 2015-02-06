#ifndef __GL_PENGUIN__
#define __GL_PENGUIN__

#include "GLBaseShapes.h"
#include "GLState.h"

// this class manages the angles of the penguin, and the shapes it must draw
class GLPenguin
{
public:
    GLPenguin(GLState &glState)
        : m_head_angle(0)
        , m_arm_angle(0)
        , m_left_leg_angle(-25)
        , m_left_foot_angle(0)
        , m_right_leg_angle(25)
        , m_right_foot_angle(0)
        , m_x(0)
        , m_y(0)
        , m_beak_height(0)
        , m_gl_state(glState)
        {}

    void initialize() {
        // Copy the data for the shapes we'll draw into the video card's memory.
        m_unit_square.initialize(m_gl_state.VERTEX_POSITION_SHADER_LOCATION);
        m_unit_circle.initialize(
        m_gl_state.VERTEX_POSITION_SHADER_LOCATION,
        /*num_circle_segments=*/100);
        m_torso.initialize(m_gl_state.VERTEX_POSITION_SHADER_LOCATION);
        m_head.initialize(m_gl_state.VERTEX_POSITION_SHADER_LOCATION);
        m_beak.initialize(m_gl_state.VERTEX_POSITION_SHADER_LOCATION);
    }

    void draw(void);

    // glwidget owns this, so its fine to make these public
    double m_head_angle;
    double m_arm_angle;
    double m_left_leg_angle;
    double m_left_foot_angle;
    double m_right_leg_angle;
    double m_right_foot_angle;

    double m_x;
    double m_y;

    double m_beak_height;

private:
    // copied to keep the code the same
    GLTransformStack &transformStack()
    { return m_gl_state.transformStack(); }

    void drawTorso(void);
    void drawArm(void);
    void drawLeftLeg(void);
    void drawRightLeg(void);
    void drawHead(void);

    void drawLeg(const float x, const float y,
        const float leg_angle, const float foot_angle);
    void drawHinge(void);

    GLState &m_gl_state;
    UnitSquare m_unit_square;
    UnitCircle m_unit_circle;

    PenguinTorso m_torso;
    PenguinHead m_head;
    PenguinBeak m_beak;
};

#endif
