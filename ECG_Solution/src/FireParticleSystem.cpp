#include "FireParticleSystem.h"

void FireParticleSystem::emitParticle(glm::vec3 origin)
{
	glm::vec3 velocity = glm::vec3(0.0f);
	velocity = generateRandomUnitVectorWithError(direction, directionDeviation);
	velocity = glm::normalize(velocity);
	velocity *= generateValue(averageSpeed, speedError);
	float scale = generateValue(averageScale, scaleError);
	float lifeLength = generateValue(averageLifeLength, lifeError);

	new Particle(glm::vec3(origin), velocity, particleWeight, lifeLength, 0, scale);
}

float FireParticleSystem::generateValue(float average, float error)
{
	//std::srand(std::time(nullptr));
	float offset = (((float)std::rand()) / ((float)RAND_MAX) - 0.5f) * 2.0f * error;
	return average + offset;
}

glm::vec3 FireParticleSystem::generateRandomUnitVectorWithError(glm::vec3 coneDirection, float angle)
{
	float cosAngle = std::cos(angle);
	//std::srand(std::time(nullptr));

	float theta = ((float)std::rand()) / ((float)RAND_MAX) * 2.0f * glm::pi<float>();
	
	float z = cosAngle + (((float)std::rand()) / ((float)RAND_MAX) * (1.0f - cosAngle));
	float rootOneMinusZSquared = std::sqrtf(1 - z * z);
	float x = rootOneMinusZSquared * std::cos(theta);
	float y = rootOneMinusZSquared * std::sin(theta);

	glm::vec4 direction = glm::vec4(x, y, z, 1.0f);

	

	return glm::vec3(x,y,z);
}

FireParticleSystem::FireParticleSystem()
{

	//TODO CREATE PS WITH THE CODE FROM FIREPARTICLES DEMO
	pps = 100.0f;
	averageSpeed = 4.0f;
	particleWeight = 0.01f;
	averageLifeLength = 4.0f;
	averageScale = 0.1f;

	setDirection(glm::vec3(0.0f,1.0f, 0.0f), 0.0f);
	setSpeedError(0.5f);
	setScaleError(0.5f);
	setLifeError(0.5f);
}

void FireParticleSystem::setDirection(glm::vec3 direction, float deviation)
{
	this->direction = glm::vec3(direction);
	this->directionDeviation = deviation * glm::pi<float>();
}

void FireParticleSystem::setSpeedError(float error)
{
	this->speedError = error * averageSpeed;
}

void FireParticleSystem::setLifeError(float error)
{
	this->lifeError = error * averageLifeLength;
}

void FireParticleSystem::setScaleError(float error)
{
	this->scaleError = error * averageScale;
}

void FireParticleSystem::generateParticles(glm::vec3 systemOrigin, float dt)
{
	float particlesToCreate = pps * dt;
	int particleCount = (int) std::floorf(particlesToCreate);
	float partialParticle =std::fmodf(particlesToCreate,1.0f);
	std::srand(std::time(nullptr));
	for (int i = 0; i < particleCount; i++) {
		emitParticle(systemOrigin);
	}
	if (((float)std::rand()) / ((float)RAND_MAX) < partialParticle) {
		emitParticle(systemOrigin);
	}
}

/*
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <glaux.h>
#include <glut.h>
#include <math.h>
#include <iostream>
#include "define.h"

using namespace std;

#define	MAX_PARTICLES	10000		// Number Of Particles To Create

GLuint g_mainWnd;
GLuint g_nWinWidth  = G308_WIN_WIDTH;
GLuint g_nWinHeight = G308_WIN_HEIGHT;

int windowWidth=800, windowHeight=800;

int numKeys = 0;
bool keys[4];

float	xspeed;						// Base X Speed (To Allow Keyboard Direction Of Tail)
float	yspeed;						// Base Y Speed (To Allow Keyboard Direction Of Tail)
float	zoom=-40.0f;				// Used To Zoom Out

GLuint	loop;						// Misc Loop Variable
GLuint	texture[1];					// Storage For Our Particle Texture

static GLfloat white[3] =	{1.0f,1.0f,1.0f};
static GLfloat blue[3] =	{0.0f,0.0f,1.0f};
static GLfloat yellow[3] =	{1.0f,1.0f,0.0f};
static GLfloat orange[3] =	{1.0f,0.5f,0.0f};
static GLfloat red[3] =		{1.0f,0.1f,0.0f};

float posX = 0.0f;
float posY = -5.0f;
float posZ = 0.0f;

float gravX = 0.0f;
float gravY = 0.0f;
float gravZ = 0.0f;

float camX = 0.0f;
float camY = 2.5f;
float camZ = 50.0f;

typedef struct						// Create A Structure For Particle
{
	bool	active;					// Active (Yes/No)
	float	life;					// Particle Life
	float	fade;					// Fade Speed
	float	r;						// Red Value
	float	g;						// Green Value
	float	b;						// Blue Value
	float	x;						// X Position
	float	y;						// Y Position
	float	z;						// Z Position
	float	xi;						// X Direction
	float	yi;						// Y Direction
	float	zi;						// Z Direction
	float	xg;						// X Gravity
	float	yg;						// Y Gravity
	float	zg;						// Z Gravity
}
particles;							// Particles Structure

particles particle[MAX_PARTICLES];	// Particle Array (Room For Particle Info)

AUX_RGBImageRec *LoadBMP(char *Filename)				// Loads A Bitmap Image
{
		FILE *File=NULL;								// File Handle
		if (!Filename)									// Make Sure A Filename Was Given
		{
				return NULL;							// If Not Return NULL
		}
		File=fopen(Filename,"r");						// Check To See If The File Exists
		if (File)										// Does The File Exist?
		{
			fclose(File);								// Close The Handle
			return auxDIBImageLoad(Filename);			// Load The Bitmap And Return A Pointer
		}
		return NULL;									// If Load Failed Return NULL
}

int LoadGLTextures()									// Load Bitmap And Convert To A Texture
{
		int Status=FALSE;								// Status Indicator
		AUX_RGBImageRec *TextureImage[1];				// Create Storage Space For The Textures
		memset(TextureImage,0,sizeof(void *)*1);		// Set The Pointer To NULL

		if (TextureImage[0]=LoadBMP("Data/Particle.bmp"))	// Load Particle Texture
		{
			Status=TRUE;								// Set The Status To TRUE
			glGenTextures(1, &texture[0]);				// Create One Texture

			glBindTexture(GL_TEXTURE_2D, texture[0]);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		}

		if (TextureImage[0])							// If Texture Exists
		{
			if (TextureImage[0]->data)					// If Texture Image Exists
			{
				free(TextureImage[0]->data);			// Free The Texture Image Memory
			}
			free(TextureImage[0]);						// Free The Image Structure
		}
		return Status;									// Return The Status
}

void InitGL()										// All Setup For OpenGL Goes Here
{
	LoadGLTextures();								// Jump To Texture Loading Routine

	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f,0.0f,0.0f,0.0f);					// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup

	glDisable(GL_DEPTH_TEST);							// Disable the depth test for 2D

	glEnable(GL_BLEND);									// Enable Blending
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);					// Type Of Blending To Perform
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);	// Really Nice Perspective Calculations
	glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);				// Really Nice Point Smoothing
	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	glBindTexture(GL_TEXTURE_2D,texture[0]);			// Select Our Texture

	for (loop=0;loop<MAX_PARTICLES;loop++)				// Initials All The Textures
	{
		particle[loop].active=true;								// Make All The Particles Active
		particle[loop].life=1.0f;								// Give All The Particles Full Life
		particle[loop].fade=float(rand()%100)/1000.0f+0.003f;	// Random Fade Speed
		particle[loop].r=white[0];
		particle[loop].g=white[1];
		particle[loop].b=white[2];
		particle[loop].xi=float((rand()%50)-25.0f)*10.0f;		// Random Speed On X Axis
		particle[loop].yi=float((rand()%50)-25.0f)*10.0f;		// Random Speed On Y Axis
		particle[loop].zi=float((rand()%50)-25.0f)*10.0f;		// Random Speed On Z Axis
		particle[loop].xg=0.0f;									// Set Horizontal Pull
		particle[loop].yg=0.8f;									// Set Vertical Pull
		particle[loop].zg=0.0f;									// Set Pull On Z Axis To Zero
	}
}

// Reshape function
void G308_Reshape(int w, int h)
{
	if (h == 0) h = 1;

	g_nWinWidth = w;
	g_nWinHeight = h;

	glViewport(0, 0, g_nWinWidth, g_nWinHeight);
}

// Set Light
void SetLight()
{
	float direction[]	  = {0.0f, 0.0f, 1.0f, 0.0f};
	float diffintensity[] = {0.7f, 0.7f, 0.7f, 1.0f};
	float ambient[]       = {0.2f, 0.2f, 0.2f, 1.0f};

	glLightfv(GL_LIGHT0, GL_POSITION, direction);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffintensity);
	glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient);


	glEnable(GL_LIGHT0);
}

// Set Camera Position
void SetCamera()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(G308_FOVY, (double) g_nWinWidth / (double) g_nWinHeight, G308_ZNEAR_3D, G308_ZFAR_3D);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(camX, camY, camZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

}

void DrawGLScene()										// Here's Where We Do All The Drawing
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer
	glLoadIdentity();										// Reset The ModelView Matrix
	for (loop=0;loop<MAX_PARTICLES;loop++)					// Loop Through All The Particles
	{
		if (particle[loop].active)							// If The Particle Is Active
		{

			float x=particle[loop].x;						// Grab Our Particle X Position
			float y=particle[loop].y;						// Grab Our Particle Y Position
			float z=particle[loop].z+zoom;					// Particle Z Pos + Zoom

			// Draw The Particle Using Our RGB Values, Fade The Particle Based On It's Life
			glColor4f(particle[loop].r,particle[loop].g,particle[loop].b,particle[loop].life);

			glBegin(GL_TRIANGLE_STRIP);						// Build Quad From A Triangle Strip
				glTexCoord2d(1,1); glVertex3f(x+0.5f,y+0.5f,z); // Top Right
				glTexCoord2d(0,1); glVertex3f(x-0.5f,y+0.5f,z); // Top Left
				glTexCoord2d(1,0); glVertex3f(x+0.5f,y-0.5f,z); // Bottom Right
				glTexCoord2d(0,0); glVertex3f(x-0.5f,y-0.5f,z); // Bottom Left
			glEnd();										// Done Building Triangle Strip

			particle[loop].x+=particle[loop].xi/(750);// Move On The X Axis By X Speed
			particle[loop].y+=particle[loop].yi/(1000);// Move On The Y Axis By Y Speed
			particle[loop].z+=particle[loop].zi/(750);// Move On The Z Axis By Z Speed

			if((particle[loop].x>posX)&&(particle[loop].y>(0.1+posY))){
				particle[loop].xg=-0.3f;
			} else if((particle[loop].x<posX)&&(particle[loop].y>(0.1+posY))){
				particle[loop].xg=0.3f;
			} else {
				particle[loop].xg=0.0f;
			}

			particle[loop].xi+=(particle[loop].xg + gravX);			// Take Pull On X Axis Into Account
			particle[loop].yi+=(particle[loop].yg + gravY);			// Take Pull On Y Axis Into Account
			particle[loop].zi+=(particle[loop].zg + gravZ);			// Take Pull On Z Axis Into Account
			particle[loop].life-=particle[loop].fade;		// Reduce Particles Life By 'Fade'

			if (particle[loop].life<0.0f)					// If Particle Is Burned Out
			{
				particle[loop].life=1.0f;					// Give It New Life
				particle[loop].fade=float(rand()%100)/1000.0f+0.003f;	// Random Fade Value
				particle[loop].x=posX;						// Center On X Axis
				particle[loop].y=posY;						// Center On Y Axis
				particle[loop].z=posZ;						// Center On Z Axis
				particle[loop].xi=xspeed+float((rand()%60)-30.0f);	// X Axis Speed And Direction
				particle[loop].yi=yspeed+float((rand()%60)-30.0f);	// Y Axis Speed And Direction
				particle[loop].zi=float((rand()%60)-30.0f);	// Z Axis Speed And Direction
				particle[loop].r=white[0];
				particle[loop].g=white[1];
				particle[loop].b=white[2];
			}
			else if (particle[loop].life<0.4f)
			{
				particle[loop].r=red[0];
				particle[loop].g=red[1];
				particle[loop].b=red[2];
			}
			else if (particle[loop].life<0.6f)
			{
				particle[loop].r=orange[0];
				particle[loop].g=orange[1];
				particle[loop].b=orange[2];
			}
			else if (particle[loop].life<0.75f)
			{
				particle[loop].r=yellow[0];
				particle[loop].g=yellow[1];
				particle[loop].b=yellow[2];
			}
			else if (particle[loop].life<0.85f)
			{
				particle[loop].r=blue[0];
				particle[loop].g=blue[1];
				particle[loop].b=blue[2];
			}
		}
	}
	glutPostRedisplay();
	glutSwapBuffers();
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(g_nWinWidth, g_nWinHeight);
	g_mainWnd = glutCreateWindow("Fire Particles");

	glutDisplayFunc(DrawGLScene);
	glutReshapeFunc(G308_Reshape);

	InitGL();

	SetLight();
	SetCamera();

	glutMainLoop();

	return 0;
}

*/