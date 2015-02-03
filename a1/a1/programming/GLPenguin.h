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
        , m_left_leg_angle(-25)
        , m_left_foot_angle(0)
        , m_right_leg_angle(25)
        , m_right_foot_angle(0)
        , m_x(0)
        , m_y(0)
        , m_beak_height(0)
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

    void setX(const float x)
    { m_x = x; }
    void setY(const float y)
    { m_y = y; }

    void setBeakHeight(const float beak_height)
    { m_beak_height = beak_height; }

    void setHeadAngle(const float angle)
    { m_head_angle = angle; };
    void setArmAngle(const float angle)
    { m_arm_angle = angle; };
    void setLeftLegAngle(const float angle)
    { m_left_leg_angle = angle; };
    void setLeftFootAngle(const float angle)
    { m_left_foot_angle = angle; };
    void setRightLegAngle(const float angle)
    { m_right_leg_angle = angle; };
    void setRightFootAngle(const float angle)
    { m_right_foot_angle = angle; };

    float getX() const
    { return m_x; }
    float getY() const
    { return m_y; }

    float getBeakHeight() const
    { return m_beak_height; }

    float getHeadAngle() const
    { return m_head_angle; };
    float getArmAngle() const
    { return m_arm_angle; };
    float getLeftLegAngle() const
    { return m_left_leg_angle; };
    float getLeftFootAngle() const
    { return m_left_foot_angle; };
    float getRightLegAngle() const
    { return m_right_leg_angle; };
    float getRightFootAngle() const
    { return m_right_foot_angle; };

private:
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

    double m_head_angle;
    double m_arm_angle;
    double m_left_leg_angle;
    double m_left_foot_angle;
    double m_right_leg_angle;
    double m_right_foot_angle;

    double m_x;
    double m_y;

    double m_beak_height;
};

#endif
