// CoastVista 3D - main.cpp
// Day 1 (Ramisa): window, render loop, perspective projection, default camera.
// Also set up: sky gradient, sun light and a layout preview of the scene, so the
// perspective can be checked straight away.
//
// Coordinates:  +X along the road, +Y up, +Z towards the sea  (see config.h)

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdio>
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/freeglut.h>

#include "config.h"
#include "scene.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE 0x809D   // not in the old Windows gl.h
#endif

namespace {

    // ---- Camera ---------------------------------------------------------------
    // Day 4 will add keyboard + mouse control on top of this struct.
    struct Camera {
        float x, y, z;
        float yaw;      // degrees, 0 = looking along +X, 90 = towards the sea (+Z)
        float pitch;    // degrees, + = look up
    };

    Camera g_cam = { CAM_START_X, CAM_START_Y, CAM_START_Z, CAM_START_YAW, CAM_START_PITCH };

    // ---- State ----------------------------------------------------------------
    int   g_lastMs = 0;
    float g_time = 0.0f;      // seconds since start (drives animation later)
    int   g_frames = 0;
    int   g_fpsClockMs = 0;

    inline float deg2rad(float d) { return d * 3.14159265f / 180.0f; }
    inline float mixf(float a, float b, float t) { return a + (b - a) * t; }
    inline float clamp01(float v) { return v < 0.0f ? 0.0f : (v > 1.0f ? 1.0f : v); }

    // ---- Sky ------------------------------------------------------------------
    // Gradient that stays attached to the horizon, even when the camera looks up or down.
    void drawSky()
    {
        float t = tanf(deg2rad(g_cam.pitch)) / tanf(deg2rad(FOV_Y * 0.5f));
        float hy = clamp01(0.5f - 0.5f * t);            // horizon height on screen (0 = bottom, 1 = top)
        float k = clamp01((1.0f - hy) / 0.65f);        // how far up the sky we can see

        float top[3];
        for (int i = 0; i < 3; ++i) top[i] = mixf(SKY_HORIZON[i], SKY_TOP[i], k);

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glBegin(GL_QUADS);
        glColor3fv(SKY_HORIZON);                        // below the horizon (ground will cover it)
        glVertex2f(0.0f, 0.0f); glVertex2f(1.0f, 0.0f);
        glVertex2f(1.0f, hy);   glVertex2f(0.0f, hy);
        glColor3fv(SKY_HORIZON);                        // horizon -> upper sky
        glVertex2f(0.0f, hy);   glVertex2f(1.0f, hy);
        glColor3fv(top);
        glVertex2f(1.0f, 1.0f); glVertex2f(0.0f, 1.0f);
        glEnd();

        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);

        glEnable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);
    }

    // ---- GLUT callbacks ---------------------------------------------------------
    void display()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        drawSky();

        // Default camera: gluLookAt from position + yaw/pitch.
        float yaw = deg2rad(g_cam.yaw), pit = deg2rad(g_cam.pitch);
        float dx = cosf(yaw) * cosf(pit);
        float dy = sinf(pit);
        float dz = sinf(yaw) * cosf(pit);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(g_cam.x, g_cam.y, g_cam.z,
            g_cam.x + dx, g_cam.y + dy, g_cam.z + dz,
            0.0, 1.0, 0.0);

        // Sun = directional light (w = 0). Set AFTER gluLookAt so it stays fixed in the world.
        const GLfloat sunPos[4] = { SUN_DIR[0], SUN_DIR[1], SUN_DIR[2], 0.0f };
        glLightfv(GL_LIGHT0, GL_POSITION, sunPos);

        // Scene (each module is in its own file)
        drawBeach();
        drawRoad();
        drawSea(g_time);
        drawMountains();

        glutSwapBuffers();
    }

    void reshape(int w, int h)
    {
        if (h == 0) h = 1;
        glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(FOV_Y, (double)w / (double)h, Z_NEAR, Z_FAR);
        glMatrixMode(GL_MODELVIEW);
    }

    // Render loop: ~60 times per second.
    void tick(int)
    {
        int now = glutGet(GLUT_ELAPSED_TIME);
        float dt = (now - g_lastMs) / 1000.0f;
        g_lastMs = now;
        g_time += dt;

        // FPS in the window title, refreshed once a second
        ++g_frames;
        if (now - g_fpsClockMs >= 1000) {
            char title[96];
            std::snprintf(title, sizeof(title), "%s  |  %d FPS", WINDOW_TITLE, g_frames);
            glutSetWindowTitle(title);
            g_frames = 0;
            g_fpsClockMs = now;
        }

        glutPostRedisplay();
        glutTimerFunc(16, tick, 0);
    }

    void keyboard(unsigned char key, int, int)
    {
        if (key == 27) glutLeaveMainLoop();     // Esc = exit
    }

    // ---- OpenGL state -----------------------------------------------------------
    void initGL()
    {
        glClearColor(SKY_HORIZON[0], SKY_HORIZON[1], SKY_HORIZON[2], 1.0f);
        glEnable(GL_DEPTH_TEST);
        glShadeModel(GL_SMOOTH);
        glEnable(GL_NORMALIZE);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        glEnable(GL_MULTISAMPLE);

        // Light 0 = the sun: warm direct light + cool sky-coloured ambient.
        const GLfloat sceneAmbient[4] = { 0.22f, 0.26f, 0.32f, 1.0f };
        const GLfloat sunDiffuse[4] = { 0.80f, 0.76f, 0.68f, 1.0f };
        const GLfloat sunAmbient[4] = { 0.00f, 0.00f, 0.00f, 1.0f };
        const GLfloat sunSpecular[4] = { 0.00f, 0.00f, 0.00f, 1.0f };
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, sceneAmbient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, sunDiffuse);
        glLightfv(GL_LIGHT0, GL_AMBIENT, sunAmbient);
        glLightfv(GL_LIGHT0, GL_SPECULAR, sunSpecular);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

        // glColor3f(...) sets the material colour (so we do not need glMaterial for every object)
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    }

} // namespace

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    unsigned int mode = GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH;
#if USE_MSAA
    mode |= GLUT_MULTISAMPLE;
    glutSetOption(GLUT_MULTISAMPLE, 4);
#endif
    glutInitDisplayMode(mode);
    glutInitWindowSize(WINDOW_W, WINDOW_H);
    glutInitWindowPosition(100, 60);
    glutCreateWindow(WINDOW_TITLE);

    initGL();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    g_lastMs = g_fpsClockMs = glutGet(GLUT_ELAPSED_TIME);
    glutTimerFunc(0, tick, 0);

    glutMainLoop();
    return 0;
}
