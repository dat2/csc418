#include "GLPenguin.h"

#define RGB_TO_OPENGL(x,y,z) x/255.0, y/255.0, z/255.0
#define BLACK RGB_TO_OPENGL(0, 0, 0)
#define RED RGB_TO_OPENGL(255, 0, 0)
#define WHITE RGB_TO_OPENGL(255, 255, 255)
#define YELLOW RGB_TO_OPENGL(255, 255, 0)

const float HEAD_WIDTH = 40.0f;
const float HEAD_HEIGHT = 40.0f;

const float BEAK_WIDTH = 20.0f;
const float BEAK_HEIGHT = 10.0f;

const float ARM_WIDTH = 35.0f;
const float ARM_LENGTH = 100.0f;

const float BODY_WIDTH = 80.0f;
const float BODY_LENGTH = 100.0f;

const float LEG_WIDTH = 20.0f;
const float LEG_LENGTH = 100.0f;

const float FOOT_WIDTH = 20.0f;
const float FOOT_LENGTH = 50.0f;

const float JOINT_RADIUS = 3.0f;
const float EYE_RADIUS = 5.0f;

void GLPenguin::draw(void)
{
    // Note that successive transformations are applied *before* the previous
    // ones.

    // Push the current transformation matrix on the stack
    transformStack().pushMatrix();
        transformStack().translate(m_x, m_y);

        // draw the first level of elements in the kinematic tree
        drawTorso();
        drawArm();
        drawLeftLeg();
        drawRightLeg();
        drawHead();

    // Retrieve the previous state of the transformation stack
    transformStack().popMatrix();
}

// draw the simple hexagonal body
void GLPenguin::drawTorso(void)
{
    transformStack().pushMatrix();
        // draw black torso
        transformStack().scale(BODY_WIDTH, BODY_LENGTH);
        m_gl_state.setColor(BLACK);
        m_torso.draw();

        // draw smaller white torso above it
        transformStack().scale(0.9, 0.9);
        m_gl_state.setColor(WHITE);
        m_torso.draw();
    transformStack().popMatrix();
}

void GLPenguin::drawArm(void)
{
    transformStack().pushMatrix();
        // move the whole arm
        transformStack().translate(0, ARM_LENGTH/2);

        // draw the arm
        transformStack().pushMatrix();
            // Move the arm to the joint hinge
            transformStack().rotateInDegrees(m_arm_angle);

            //scale the arm
            transformStack().scale(ARM_WIDTH, ARM_LENGTH);

            // Move to center location of arm, under previous rotation
            transformStack().translate(0.0, -0.35);

            // Draw the square for the arm
            m_gl_state.setColor(BLACK);
            m_unit_square.draw();
        transformStack().popMatrix();

        drawHinge();
    transformStack().popMatrix();
}

void GLPenguin::drawHead(void)
{
    transformStack().pushMatrix();
        // move the head
        transformStack().translate(0, BODY_LENGTH);

        // rotate the head
        transformStack().rotateInDegrees(m_head_angle);

        // draw beak
        transformStack().pushMatrix();
            // move the beak to the left
            transformStack().translate(-HEAD_WIDTH - 6, HEAD_HEIGHT/2 + BEAK_HEIGHT);

            // draw the upper beak
            transformStack().pushMatrix();
                transformStack().translate(0, m_beak_height);

                // move the hinge point
                transformStack().scale(BEAK_WIDTH, BEAK_HEIGHT);
                transformStack().translate(-0.5, 0);

                m_gl_state.setColor(YELLOW);
                m_beak.draw();
            transformStack().popMatrix();

            // draw the lower beak
            transformStack().translate(-BEAK_WIDTH/2, -2*BEAK_HEIGHT);
            transformStack().scale(BEAK_WIDTH * 2, BEAK_HEIGHT / 2);
            m_unit_square.draw();
        transformStack().popMatrix();

        // draw head
        transformStack().pushMatrix();
            //scale the head
            transformStack().scale(HEAD_WIDTH, HEAD_HEIGHT);
            // move the head a little bit up
            transformStack().translate(0, 0.9);

            m_gl_state.setColor(BLACK);
            m_head.draw();
        transformStack().popMatrix();

        // draw eye
        transformStack().pushMatrix();
            // move to the left side of the head
            transformStack().translate(-HEAD_WIDTH / 2, HEAD_HEIGHT);

            // draw the eyeball
            transformStack().scale(EYE_RADIUS, EYE_RADIUS);
            m_gl_state.setColor(WHITE);
            m_unit_circle.draw();

            // draw the iris
            transformStack().scale(0.5, 0.5);
            m_gl_state.setColor(BLACK);
            m_unit_circle.draw();
        transformStack().popMatrix();

        drawHinge();
    transformStack().popMatrix();
}

void GLPenguin::drawHinge()
{
    transformStack().pushMatrix();
        transformStack().scale(JOINT_RADIUS, JOINT_RADIUS);
        m_gl_state.setColor(WHITE);
        m_unit_circle.draw();
    transformStack().popMatrix();
}

void GLPenguin::drawLeg(
    const float x, const float y,
    const float leg_angle, const float foot_angle)
{
    transformStack().pushMatrix();
        // move the whole leg
        transformStack().translate(x, y);

        // move the leg to the hinge position
        transformStack().rotateInDegrees(leg_angle);
        transformStack().translate(0, -0.4 * LEG_LENGTH);

        // draw the leg
        transformStack().pushMatrix();
            // move the leg to the hinge
            transformStack().translate(0, 0.5);
            transformStack().scale(LEG_WIDTH, LEG_LENGTH);

            m_gl_state.setColor(BLACK);
            m_unit_square.draw();
        transformStack().popMatrix();

        // draw the foot
        transformStack().pushMatrix();
            // translate the foot
            transformStack().translate(0, -LEG_LENGTH / 2 + FOOT_WIDTH / 2);

            // draw the actual foot
            transformStack().pushMatrix();
                // rotate the hinge
                transformStack().rotateInDegrees(-90 -foot_angle);

                // move the foot to the hinge
                transformStack().scale(FOOT_WIDTH, FOOT_LENGTH);
                transformStack().translate(0, -0.4);

                m_gl_state.setColor(BLACK);
                m_unit_square.draw();
            transformStack().popMatrix();

            // draw the hinge
            drawHinge();
        transformStack().popMatrix();

        // draw the leg hinge
        transformStack().translate(0, LEG_LENGTH/2 - FOOT_WIDTH/2);
        drawHinge();
    transformStack().popMatrix();
}

void GLPenguin::drawLeftLeg(void)
{
    drawLeg(
        -BODY_WIDTH/2,
        -BODY_LENGTH + LEG_LENGTH * 0.10,
        m_left_leg_angle, m_left_foot_angle);
}

void GLPenguin::drawRightLeg(void)
{
    drawLeg(
        BODY_WIDTH/2,
        -BODY_LENGTH + LEG_LENGTH * 0.10,
        m_right_leg_angle, m_right_foot_angle);
}
