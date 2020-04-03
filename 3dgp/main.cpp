#include <iostream>
#include "GL/glew.h"
#include "GL/3dgl.h"
#include "GL/glut.h"
#include "GL/freeglut_ext.h"

// Include GLM core features
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <fstream>
#include <sstream>

#include "SpaceStation.h"
#include "SpaceRobot.h"

#pragma comment (lib, "glew32.lib")

using namespace std;
using namespace _3dgl;
using namespace glm;

// 3D Models
CSpaceStation station;
CSpaceRobot robot;

// GLSL Objects (Shader Program)
C3dglProgram Program;

// camera position (for first person type camera navigation)
mat4 matrixView;			// The View Matrix
float angleTilt = 0.f;		// Tilt Angle
vec3 cam(0);				// Camera movement values

bool init()
{
	// rendering states
	glEnable(GL_DEPTH_TEST);	// depth test is necessary for most 3D scenes
	glEnable(GL_NORMALIZE);		// normalization is needed by AssImp library models
	glShadeModel(GL_SMOOTH);	// smooth shading mode is the default one; try GL_FLAT here!
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	// this is the default one; try GL_LINE!

	// Initialise Shaders
	C3dglShader VertexShader;
	C3dglShader FragmentShader;

	if (!VertexShader.Create(GL_VERTEX_SHADER)) return false;
	if (!VertexShader.LoadFromFile("shaders/basic.vert")) return false;
	if (!VertexShader.Compile()) return false;

	if (!FragmentShader.Create(GL_FRAGMENT_SHADER)) return false;
	if (!FragmentShader.LoadFromFile("shaders/basic.frag")) return false;
	if (!FragmentShader.Compile()) return false;

	if (!Program.Create()) return false;
	if (!Program.Attach(VertexShader)) return false;
	if (!Program.Attach(FragmentShader)) return false;
	if (!Program.Link()) return false;
	if (!Program.Use(true)) return false;

	// glut additional setup
	glutSetVertexAttribCoord3(Program.GetAttribLocation("aVertex"));
	glutSetVertexAttribNormal(Program.GetAttribLocation("aNormal"));

	// load your 3D models here!
	// We have loaded all the models and textures you will need
	// so there is no need to extend this section of cide

	// load the space station models and materials
	station.loadModels("models\\SciFi\\Models");
	station.loadMaterials("models\\SciFi\\Textures");

	// load the robot model, materials and animations
	if (!robot.load("models\\robot\\frspbt.obj")) return false;
	robot.loadMaterials("");
	robot.loadAnimations("models\\robot\\frspbt.anim");
	robot.AnimateIdle();

	// Textures
	glActiveTexture(GL_TEXTURE0);
	Program.SendUniform("texture0", 0);

	// setup lights:
	Program.SendUniform("lightAmbient.color", 0.4, 0.4, 0.4);
	Program.SendUniform("lightDir.direction", 1.0, 0.5, 0.33);
	Program.SendUniform("lightDir.diffuse", 0.5, 0.5, 0.5);
	
	// setup materials
	Program.SendUniform("materialAmbient", 1.0, 1.0, 1.0);
	Program.SendUniform("materialDiffuse", 1.0, 1.0, 1.0);
	
	// Initialise the View Matrix (initial position of the camera)
	matrixView = rotate(mat4(1.f), radians(angleTilt), vec3(1.f, 0.f, 0.f));
	matrixView *= lookAt(
		vec3(0.0, 0.0, 0.0),
		vec3(0.0, 0.0, -300.0),
		vec3(0.0, 1.0, 0.0));

	// setup the screen background colour
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // night sky background

	cout << endl;
	cout << "Use:" << endl;
	cout << "  WASD or arrow key to navigate" << endl;
	cout << "  QE or PgUp/Dn to move the camera up and down" << endl;
	cout << "  Drag the mouse to look around" << endl;
	cout << endl;

	return true;
}

void done()
{
}

// this is a helper function displaying the yellow text in the bottom of the screen
void displayCoords(int x, int z)
{
	stringstream ss;
	ss << "x = " << x << "    z = " << z;
	string s = ss.str();
	Program.SendUniform("Text", 1);
	glWindowPos2i(10, 10);  // move in 10 pixels from the left and bottom edges
	for each (char ch in ss.str())
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ch);
	Program.SendUniform("Text", 0);
}

void render()
{
	// this global variable controls the animation
	float theta = glutGet(GLUT_ELAPSED_TIME) * 0.01f;

	// clear screen and buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// setup the View Matrix (camera)
	mat4 m = rotate(mat4(1.f), radians(angleTilt), vec3(1.f, 0.f, 0.f));// switch tilt off
	m = translate(m, cam);												// animate camera motion (controlled by WASD keys)
	m = rotate(m, radians(-angleTilt), vec3(1.f, 0.f, 0.f));			// switch tilt on
	m = m * matrixView;
	matrixView = m;

	// setup View Matrix
	Program.SendUniform("matrixView", matrixView);

	glActiveTexture(GL_TEXTURE0);

	
	// Display debug information: your current coordinates (x, z)
	// These coordinates are available as inv[3].x, inv[3].z
	mat4 inv = inverse(matrixView);
	displayCoords((int)inv[3].x, (int)inv[3].z);


	// To display the Space Station,
	// try to uncomment the following lines
	// (one at a time: otherwise the segments will display on top of the other)
	// To place your station segments, use the usual GLM functions
	   	  	
	m = matrixView;
	// station.renderSegment(m);
	// station.renderIntersection(m);
	// station.renderEnd(m);

	
	// To display the Robot,
	// uncomment the line below.
	m = matrixView;
	// robot.render(m);


	// To animate the robot, use following functions to START the animation cycle:
	// AnimateIdle(), AnimateActivated() and AnimateAlarmed.
	// Each of these functions starts the animation (or re-starts it if already initiated)
	// so you should avoid calling them every frame!
	// See the onKeyDown function below for example of use (lines 215 - 217)



	// essential for double-buffering technique
	glutSwapBuffers();

	// proceed the animation
	glutPostRedisplay();
}

// called before window opened or resized - to setup the Projection Matrix
void reshape(int w, int h)
{
	float ratio = w * 1.0f / h;      // we hope that h is not zero
	glViewport(0, 0, w, h);
	Program.SendUniform("matrixProjection", perspective(radians(60.f), ratio, 0.1f, 10000.f));
}

// Handle WASDQE keys
void onKeyDown(unsigned char key, int x, int y)
{
	switch (tolower(key))
	{
	case 'w': cam.z = std::max(cam.z * 150.f, 1.f); break;
	case 's': cam.z = std::min(cam.z * 150.f, -1.f); break;
	case 'a': cam.x = std::max(cam.x * 150.f, 1.f); break;
	case 'd': cam.x = std::min(cam.x * 150.f, -1.f); break;
	case 'e': cam.y = std::max(cam.y * 150.f, 1.f); break;
	case 'q': cam.y = std::min(cam.y * 150.f, -1.f); break;
	case '1': robot.AnimateIdle(); break;
	case '2': robot.AnimateActivated(); break;
	case '3': robot.AnimateAlarmed(); break;
	}
	// speed limit
	cam.x = std::max(-5.f, std::min(5.f, cam.x));
	cam.y = std::max(-5.f, std::min(5.f, cam.y));
	cam.z = std::max(-5.f, std::min(5.f, cam.z));
}

// Handle WASDQE keys (key up)
void onKeyUp(unsigned char key, int x, int y)
{
	switch (tolower(key))
	{
	case 'w':
	case 's': cam.z = 0; break;
	case 'a':
	case 'd': cam.x = 0; break;
	case 'q':
	case 'e': cam.y = 0; break;
	}
}

// Handle arrow keys and Alt+F4
void onSpecDown(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_F4:		if ((glutGetModifiers() & GLUT_ACTIVE_ALT) != 0) exit(0); break;
	case GLUT_KEY_UP:		onKeyDown('w', x, y); break;
	case GLUT_KEY_DOWN:		onKeyDown('s', x, y); break;
	case GLUT_KEY_LEFT:		onKeyDown('a', x, y); break;
	case GLUT_KEY_RIGHT:	onKeyDown('d', x, y); break;
	case GLUT_KEY_PAGE_UP:	onKeyDown('q', x, y); break;
	case GLUT_KEY_PAGE_DOWN:onKeyDown('e', x, y); break;
	case GLUT_KEY_F11:		glutFullScreenToggle();
	}
}

// Handle arrow keys (key up)
void onSpecUp(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:		onKeyUp('w', x, y); break;
	case GLUT_KEY_DOWN:		onKeyUp('s', x, y); break;
	case GLUT_KEY_LEFT:		onKeyUp('a', x, y); break;
	case GLUT_KEY_RIGHT:	onKeyUp('d', x, y); break;
	case GLUT_KEY_PAGE_UP:	onKeyUp('q', x, y); break;
	case GLUT_KEY_PAGE_DOWN:onKeyUp('e', x, y); break;
	}
}

// Handle mouse click
bool bJustClicked = false;
void onMouse(int button, int state, int x, int y)
{
	bJustClicked = (state == GLUT_DOWN);
	glutSetCursor(bJustClicked ? GLUT_CURSOR_CROSSHAIR : GLUT_CURSOR_INHERIT);
	glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
}

// handle mouse move
void onMotion(int x, int y)
{
	if (bJustClicked)
		bJustClicked = false;
	else
	{
		glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);

		// find delta (change to) pan & tilt
		float deltaPan = 0.25f * (x - glutGet(GLUT_WINDOW_WIDTH) / 2);
		float deltaTilt = 0.25f * (y - glutGet(GLUT_WINDOW_HEIGHT) / 2);

		// View = Tilt * DeltaPan * Tilt^-1 * DeltaTilt * View;
		angleTilt += deltaTilt;
		mat4 m = mat4(1.f);
		m = rotate(m, radians(angleTilt), vec3(1.f, 0.f, 0.f));
		m = rotate(m, radians(deltaPan), vec3(0.f, 1.f, 0.f));
		m = rotate(m, radians(-angleTilt), vec3(1.f, 0.f, 0.f));
		m = rotate(m, radians(deltaTilt), vec3(1.f, 0.f, 0.f));
		matrixView = m * matrixView;
	}
}

int main(int argc, char** argv)
{
	// init GLUT and create Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("CI5520 3D Graphics Programming");

	// init glew
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		cerr << "GLEW Error: " << glewGetErrorString(err) << endl;
		return 0;
	}
	cout << "Using GLEW " << glewGetString(GLEW_VERSION) << endl;

	// register callbacks
	glutDisplayFunc(render);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecDown);
	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecUp);
	glutMouseFunc(onMouse);
	glutMotionFunc(onMotion);

	cout << "Vendor: " << glGetString(GL_VENDOR) << endl;
	cout << "Renderer: " << glGetString(GL_RENDERER) << endl;
	cout << "Version: " << glGetString(GL_VERSION) << endl;

	// init light and everything – not a GLUT or callback function!
	if (!init())
	{
		cerr << "Application failed to initialise" << endl;
		return 0;
	}

	// enter GLUT event processing cycle
	glutMainLoop();

	done();

	return 1;
}

