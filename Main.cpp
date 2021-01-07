#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <glut.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include "Vector3f.h"
#include "Camera.h" 
#include <time.h>       

#define GLUT_KEY_ESCAPE 27
#define LEFT_LANE -2
#define CENTER_LANE 0
#define RIGHT_LANE 2
#define GROUND_LENGTH 10000
#define RESPAWN_POSITION 100
#define RESPAWN1_POSITION 61

using namespace std;

int lanes[3] = { LEFT_LANE,CENTER_LANE,RIGHT_LANE };
int start = 0;
int obs = 0;
int curlane=0;
int destroyed = 0;
struct Shape;
const int SKYBOX_BOUNDARY = 40;
const float GAME_SPEED = 0.8;
bool vis[100];
int WIDTH = 1280;
int HEIGHT = 720;
GLuint tex;
char title[] = "Imbaba War";

float groundTransform = 0;

int coin_rotation_angle;
bool l0,l1,l2;
int light = 0;
int player_lane = 1;
int score = 0;
int virtual_score = 0;
int maxScore = 10;
int score_pos = -30;
int stop = 1;
double PlayerForward = 0;
vector<Shape> obstacles;
vector<Shape> coins;
double trans = 0;
int idx = 0;
struct Shape {
	float x;
	int lane;

	Shape(float x, int lane) {
		this->x = x, this->lane = lane;
	};
};
vector<Shape> weo;

int cameraZoom = 0;

Camera camera = Camera(0.5f, 2.0f, 0.0f, 1.0f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f);
//*******************************************************************************************//
// EYE (ex, ey, ez): defines the location of the camera.									 //
// AT (ax, ay, az):	 denotes the direction where the camera is aiming at.					 //
// UP (ux, uy, uz):  denotes the upward orientation of the camera.							 //
//*******************************************************************************************//

void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 1280 / 720, 0.001, 200);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.look();
}

// Model Variables
Model_3DS model_house;
Model_3DS model_car;
Model_3DS model_bomb;

Model_3DS coin_model;
Model_3DS model_bridge;
// Textures
GLTexture tex_ground;

GLTexture tex_surface;
GLTexture tex_wood;

void print(int x, int y, char *string)
{
	int len, i;

	//set the position of the text in the window using the x and y coordinates
	glRasterPos2f(x, y);

	//get the length of the string to display
	len = (int)strlen(string);

	//loop to display character by character
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}

//=======================================================================
// Lighting Configuration Function
//=======================================================================
void InitLightSource()
{







	glEnable(GL_LIGHTING);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);

	

	GLfloat lmodel_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

	GLfloat l0Diffuse[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat l0Spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat l0Ambient[] = { .1f, 0.1f, 0.1f, 1.f };
	GLfloat l0Position[] = { 10.0f, 0.0f, 0.0f, l0};
	GLfloat l0Direction[] = { -1.0, 0.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, l0Diffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, l0Ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, l0Spec);
	glLightfv(GL_LIGHT0, GL_POSITION, l0Position);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 30.0);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 90.0);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, l0Direction);

	GLfloat l1Diffuse[] = { 0.0f, 1.0f, 0.0f, 1.0f };
	GLfloat l1Ambient[] = { 0.1f, .1f, 0.1f, 1.0f };
	GLfloat l1Spec[] = { 1.0f, 0.0f, 1.0f, 1.0f };
	GLfloat l1Position[] = { 0.0f, 10.0f, 0.0f, l1 };
	GLfloat l1Direction[] = { 0.0, -1.0, 0.0 };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, l1Diffuse);
	glLightfv(GL_LIGHT1, GL_AMBIENT, l1Ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, l1Spec);
	glLightfv(GL_LIGHT1, GL_POSITION, l1Position);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 90.0);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, l1Direction);

	GLfloat l2Diffuse[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	GLfloat l2Ambient[] = { 0.0f, 0.0f, .1f, 1.0f };
	GLfloat l2Spec[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	GLfloat l2Position[] = { 0.0f, 0.0f, 10.0f, l2 };
	GLfloat l2Direction[] = { 0.0, 0.0, -1.0 };
	glLightfv(GL_LIGHT2, GL_DIFFUSE, l2Diffuse);
	glLightfv(GL_LIGHT2, GL_AMBIENT, l2Ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, l2Spec);
	glLightfv(GL_LIGHT2, GL_POSITION, l2Position);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 30.0);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 90.0);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, l2Direction);

	

}

//=======================================================================
// Material Configuration Function
//======================================================================
void InitMaterial()
{
	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);

	// Sich will be assigneet Material Properties whd by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Set Material's Specular Color
	// Will be applied to all objects
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 96.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}



//=======================================================================
// Render Ground Function
//=======================================================================
void RenderGround()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_ground.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with tescorexture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-20, 0, -3);
	glTexCoord2f(200, 0);
	glVertex3f(GROUND_LENGTH, 0, -3);
	glTexCoord2f(200, 3);
	glVertex3f(GROUND_LENGTH, 0, 3);
	glTexCoord2f(0, 3);
	glVertex3f(-20, 0, 3);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}

void RenderSurface()
{
	glDisable(GL_LIGHTING);	// Disable lighting

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_surface.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-200, 0, -200);
	glTexCoord2f(100, 0);
	glVertex3f(GROUND_LENGTH, 0, -200);
	glTexCoord2f(100, 10);
	glVertex3f(GROUND_LENGTH, 0, 200);
	glTexCoord2f(0, 10);
	glVertex3f(-200, 0, 200);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}



// Draws a unit quad
void renderFace(Vector3f normal)
{
	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(normal.x, normal.y, normal.z);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-1, 0, -1);
	glTexCoord2f(1, 0);
	glVertex3f(1, 0, -1);
	glTexCoord2f(1, 1);
	glVertex3f(1, 0, 1);
	glTexCoord2f(0, 1);
	glVertex3f(-1, 0, 1);
	glEnd();
	glPopMatrix();
}

void renderCoin(float x, float lane) {

	glDisable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.
	//Draw Coins
	glPushMatrix();
	glTranslatef(x + 5,  1.5, lane);
	glScalef(4.01, 4.01, 4.01);
	glRotatef(coin_rotation_angle, 0, 1, 0);
	coin_model.Draw();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.
	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.

}
int counter = 1;
int tot[100];
void shiftAll() {
	if (counter<60) {
		double total = 1.5 + counter * 25;
		//total -= trans;
		//printf("ta %f \n", trans);
		//printf("ta %f \n", total);
		tot[counter++]=total;
	}
}
void renderObstacle(float x, float lane,int i)
{
	if (x > 1000) {
		
		glDisable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.
	//Draw Coins
		glPushMatrix();
//		double total = 1.5 + counter * 10;
	//	total -= trans;
		//printf("ta %f \n", trans);
		//printf("ta %f \n", total);
		double z = tot[i] - trans>1.5?tot[i]-trans:1.5;

		glTranslatef(x, z, lane);
		glScalef(4.01, 4.01, 4.01);
		glRotatef(coin_rotation_angle, 0, 1, 0);
		model_bomb.Draw();
		
		glPopMatrix();
		//counter++;
		glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.
		glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
		
	}
	else {
		glDisable(GL_LIGHTING);	// Disable lighting 

		glColor3f(1, 1, 1);

		glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

		glBindTexture(GL_TEXTURE_2D, tex_wood.texture[0]);	// Bind the ground texture

		glPushMatrix();

		glTranslated(x, 1.7, lane);
		// Top Face
		glPushMatrix();
		glTranslated(0, 1, 0);
		renderFace(Vector3f(0, 1, 0));
		glPopMatrix();

		// Bottom Face
		glPushMatrix();
		glTranslated(0, -1, 0);
		renderFace(Vector3f(0, -1, 0));
		glPopMatrix();

		// Left Face
		glPushMatrix();
		glRotated(90, 0, 0, 1);
		glTranslated(0, 1, 0);
		renderFace(Vector3f(1, 0, 0));
		glPopMatrix();

		// Right Face
		glPushMatrix();
		glRotated(90, 0, 0, 1);
		glTranslated(0, -1, 0);
		renderFace(Vector3f(-1, 0, 0));
		glPopMatrix();

		// Front Face
		glPushMatrix();
		glRotated(90, 0, 0, 1);
		glRotated(90, 1, 0, 0);
		glTranslated(0, 1, 0);
		renderFace(Vector3f(1, 0, 0));
		glPopMatrix();


		// Back Face
		glPushMatrix();
		glRotated(90, 0, 0, 1);
		glRotated(90, 1, 0, 0);
		glTranslated(0, -1, 0);
		renderFace(Vector3f(1, 0, 0));
		glPopMatrix();



		glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

		glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.

		glPopMatrix();
	}
}

// adds an obstacle behind the skybox
void addObstacle(int lane)
{
	
		weo.push_back(Shape(RESPAWN1_POSITION * obs, lane));
		obstacles.push_back(Shape(RESPAWN1_POSITION * obs, lane));
	
}

void addCoin(int lane)
{
	if(RESPAWN_POSITION*start<980|| RESPAWN_POSITION * start>1110)
	coins.push_back(Shape(RESPAWN_POSITION*start, lane));
}



void destroyAtIndex(int index, vector<Shape> &shapes)
{
	// Swap this element with the last one to pop from the vector
	Shape tmp = shapes[shapes.size() - 1];
	shapes[shapes.size() - 1] = shapes[index];
	shapes[index] = tmp;
	shapes.pop_back();
}

// TODO implement
void onObstacleCollision(int max)
{
	glFlush();
	if (PlayerForward > 1110) {
		score = 0;
		printf("%d obs: \n", obs);
		//counter = 0;
		trans=222.5*2;
		
		//obs = 19;
		start = 10;
		coins.clear();
		//shiftAll();
		while (PlayerForward > 1110) {
			PlayerForward -= 0.5;
			camera.eye.x -= 0.5;
			camera.center.x -= 0.5;			
		}
		
	}
	if (PlayerForward<1008||PlayerForward>1110) {
		PlayerForward -= 0.5;
		camera.eye.x -= 0.5;
		camera.center.x -= 0.5;
	}
	
}

void onCoinCollision(int i)
{
	glFlush();
	glutSwapBuffers();
	printf("%d", score);

	printf("%d", virtual_score);
	virtual_score++;
	score++;

	if (virtual_score == 10) {
		glutSwapBuffers();
		tex_ground.Load("Textures/wood.bmp");
		tex_surface.Load("Textures/ground0.bmp");
		tex_wood.Load("Textures/marple.bmp");


		score = 0;
		maxScore = 20;
		
	}

	else if (virtual_score == 30) {

		stop = 0;
		exit(EXIT_SUCCESS);

	}
}

int random(int lower, int upper)
{
	return (rand() % (upper - lower + 1)) + lower;
}

//=======================================================================
// Display Function
//=======================================================================
void myDisplay(void)
{
	shiftAll();


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	setupCamera();
	InitLightSource();
	InitMaterial();

	/*GLfloat lightIntensity[] = { 0.7, 0.7, 0.7, 1.0f };
	GLfloat lightPosition[] = { 0.0f, 100.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);*/

	// Display Score
	char* strScore[20];

	glPushMatrix();
	if (player_lane == 0)
	{
		glTranslatef(-10+PlayerForward, score_pos, 10);
	}
	else if (player_lane == 1)
	{

		glTranslatef(-10 + PlayerForward, score_pos, 12);

	}
	else {
		glTranslatef(-10 + PlayerForward, score_pos, 14);

	}
	glColor3f(0, 0, 0);	// Dim the ground texture a bit
	sprintf((char *)strScore, "Score = %d/%d", score, maxScore);
	print(50, 50, (char *)strScore);
	glPopMatrix();

	// Display Level

	glPushMatrix();
	glTranslated(groundTransform * 0.2, 0, 0);
	//glScaled(20, 1, 1);
	RenderSurface();
	glPopMatrix();

	// Draw Ground
	glPushMatrix();
	glTranslated(groundTransform, 0.5, 0);
	//glScaled(20.0f, 20.0f, 1.0f);
	RenderGround();
	glPopMatrix();

	// Draw Player
	glPushMatrix();

	glTranslatef(PlayerForward, 0.5, lanes[player_lane]);
	glScalef(0.5, 0.5, 0.5);
	glRotatef(-90.f, 0, 1, 0);
	model_car.Draw();

	glPopMatrix();

	// bridge
	glPushMatrix();
	glTranslatef(1000,0,0);
	glScalef(0.001, 0.001, 0.001);
	glRotatef(-90.f, 0, 1, 0);
	model_bridge.Draw();
	glPopMatrix();

	//Draw all Coins
	for (unsigned i = 0; i < coins.size(); i++)
	{
		renderCoin(coins[i].x, lanes[coins[i].lane]);
	}


	// Draw all obstacles
	for (unsigned i = 0; i < obstacles.size(); i++)
	{
		if(obstacles[i].x<980||obstacles[i].x>1110&&i<=1000)
		renderObstacle(obstacles[i].x, lanes[obstacles[i].lane], i);
	}

	//sky box4
	glDisable(GL_LIGHTING);	// Disable lighting 
	glPushMatrix();

	GLUquadricObj * qobj;
	qobj = gluNewQuadric();
	glTranslated(50+PlayerForward, 0, 0);
	glRotated(90, 1, 0, 1);
	glBindTexture(GL_TEXTURE_2D, tex);
	gluQuadricTexture(qobj, true);
	gluQuadricNormals(qobj, GL_SMOOTH);
	gluSphere(qobj, 100, 100, 100);
	gluDeleteQuadric(qobj);
	glPopMatrix();
	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.


	//glutSwapBuffers();
	glFlush();
}

//=======================================================================
// Assets Loading Function
//=======================================================================
void LoadAssets()
{
	// Loading Model files
	model_car.Load("Models/car/ausfb.3ds");
	model_bridge.Load("Models/bridge/Bridge.3ds");
	coin_model.Load("Models/coin/Coin Block2.3ds");
	model_bomb.Load("Models/Obstacle/Bomb.3ds");

	// Loading texture files
	if (score <= 2) {
		tex_ground.Load("Textures/ground.bmp");
	}
	tex_surface.Load("Textures/surface.bmp");

	tex_wood.Load("Textures/wall.bmp");

	loadBMP(&tex, "Textures/blu-sky-3.bmp", true);
}

//===
//====================================================================
// Animation Function
//=======================================================================
void anime()
{
	coin_rotation_angle += 5 * stop;
	for (int i = 0; i < obstacles.size(); i++)
	{
		//obstacles[i].x -= GAME_SPEED * stop;

		// If player collided with obstacle
		if (obstacles[i].lane == player_lane &&
			obstacles[i].x <=PlayerForward+4 && obstacles[i].x >=PlayerForward-3)
		{
			printf("kolayd was here %d \n ", obstacles[i].lane );
			printf("kolayd was here %f \n ", obstacles[i].x);

			onObstacleCollision(obstacles[i].x);
		}
		else if (obstacles[i].lane != player_lane &&
			obstacles[i].x <= PlayerForward + 4 && obstacles[i].x >= 0) {
		
			
		}
		if (obstacles[i].x <= PlayerForward - 3&&PlayerForward<1110) {
			destroyAtIndex(i--, obstacles);
			destroyed++;
		}


		// If the obstacle is way behind the player
	}



	for (int i = 0; i < coins.size(); i++)
	{
	//	coins[i].x -= 1 * stop;

		// If player collided with coin
		if (coins[i].lane == player_lane &&
			coins[i].x <= PlayerForward && coins[i].x >= PlayerForward-7)
		{
			//printf("%f \n", coins[i].x);

			//printf("%f \n", PlayerForward);

			onCoinCollision(i);
			destroyAtIndex(i--, coins);
		}
	}

	for (int i = 0; i < coins.size(); i++)
	{
		// If the coin is way behind the player
		if (coins[i].x < -20 && coins.size() > 0)
			destroyAtIndex(i--, coins);
	}

	//groundTransform -= GAME_SPEED * stop;

	for (int i = 0; i < 1e7; i++);
	glutPostRedisplay();
}







void Keyboard(unsigned char key, int x, int y) {
	float d = 0.8;
	float x_truck_cam = 2;

	switch (key) {
	case 'm':
    	if (PlayerForward == 1008.5) {
			glutSwapBuffers();
			tex_ground.Load("Textures/wood.bmp");
			tex_surface.Load("Textures/ground0.bmp");
			tex_wood.Load("Textures/marple.bmp");
		}
		printf("%f \n", PlayerForward);
		printf("%f  dest \n ",trans);
		
		if (PlayerForward < 991.5 || PlayerForward>=1008||(PlayerForward>=1008&&PlayerForward<1110)) {
			PlayerForward += 0.5;
			camera.eye.x += 0.5;
			camera.center.x += 0.5;
			trans+=0.2;
		}
		else if (player_lane == 1) {
			PlayerForward += 0.5;
			camera.eye.x += 0.5;
			camera.center.x += 0.5;
			trans+=0.2;

		}
		break;
	case 'w':
		camera.moveY(d);
		break;
	case 's':
		camera.moveY(-d);
		break;
	case 'd':{
		
		//printf("%d \n dest", destroyed);
		int n = destroyed;
		//printf("%f \n", weo[n].x);

		if (player_lane < 2 && !(player_lane + 1 == weo[n].lane&&PlayerForward+4>=weo[n].x)||(player_lane > 0 && PlayerForward>1008&&PlayerForward<1110)||(player_lane > 0 && PlayerForward>1110))
			if (PlayerForward < 992) {
					player_lane++;
					camera.moveX(-x_truck_cam);
			}
			else if (PlayerForward >= 1008) {
				player_lane++;
				camera.moveX(-x_truck_cam);
			}
			break;
		}
	case 'a': {
		int n = destroyed;
	//	printf("%f \n", obstacles[n].x);

		if (player_lane > 0 && !(player_lane - 1 == weo[n].lane && PlayerForward + 4 >= weo[n].x) || ( player_lane > 0 && PlayerForward > 1008 && PlayerForward < 1110) || (player_lane > 0 && PlayerForward > 1110))
			if (PlayerForward < 992) {
				player_lane--;
				camera.moveX(x_truck_cam);
			}
			else if (PlayerForward >= 1008) {
				player_lane--;
				camera.moveX(x_truck_cam);
			}
		break;
	}
		case 'q':
		camera.moveZ(d);
		break;
	case 'e':
		camera.moveZ(-d);
		break;

	case 't':
		score_pos = -48.5;
		camera = Camera(-8.0f + PlayerForward, 7.0f, lanes[player_lane], -1.0f + PlayerForward, 2.7f, lanes[player_lane], 0.0f, 1.0f, 0.0f);
		break;

	case 'f':
		score_pos = -30;
		camera = Camera(0.5f + PlayerForward, 2.0f, lanes[player_lane], 1.0f + PlayerForward, 2.0f, lanes[player_lane], 0.0f, 1.0f, 0.0f);;
		break;
	case GLUT_KEY_ESCAPE:
		exit(EXIT_SUCCESS);
	}

	glutPostRedisplay();
}

void Special(int key, int x, int y) {
	float a = 1.0;

	switch (key) {
	case GLUT_KEY_UP:
		camera.rotateX(a);
		break;
	case GLUT_KEY_DOWN:
		camera.rotateX(-a);
		break;
	case GLUT_KEY_LEFT:
		camera.rotateY(a);
		break;
	case GLUT_KEY_RIGHT:
		camera.rotateY(-a);
		break;
	}

	glutPostRedisplay();
}
void dropCoin(int v)
{
	boolean dropAllowed = random(0, 100) < 100;
	start++;
	if (dropAllowed)
	{
		int lane = random(0, 2);
		addCoin(lane);
	}
	glutTimerFunc(500, dropCoin, 0);
}
void dropObstacle(int v)
{
	obs++;
	
		int lane = random(0, 2);
		addObstacle(lane);
	glutTimerFunc(1, dropObstacle, 0);
}

//=======================================================================
// Main Function
//=======================================================================
void main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);

	glutInitWindowPosition(100, 150);
	srand(time(NULL));

	glutCreateWindow(title);

	glutDisplayFunc(myDisplay);

	glutIdleFunc(anime);
	glutTimerFunc(0, dropObstacle, 0);
	glutTimerFunc(0, dropCoin, 0);
//	glutTimerFunc(0, lightAnim, 0);

	glutKeyboardFunc(Keyboard);

	glutSpecialFunc(Special);

	LoadAssets();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}


