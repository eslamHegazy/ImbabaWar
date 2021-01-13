#include "Camera.h"
#include <math.h>
#include "Vector3f.h"
#include <glut.h>

#define DEG2RAD(a) (a * 0.0174532925)
#define FIRST_PERSON_VIEW 0
#define THIRD_PERSON_VIEW 1

	Camera::Camera(float eyeX = 1.0f, float eyeY = 1.0f, float eyeZ = 1.0f, float centerX = 0.0f, float centerY = 0.0f, float centerZ = 0.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f) 
		:eye(eyeX, eyeY, eyeZ),
		center(centerX, centerY, centerZ),
		up(upX, upY, upZ)
	{
	}

	void Camera::moveX(float d) {
		Vector3f right = up.cross(center - eye).unit();
		eye = eye + right * d;
		center = center + right * d;
	}

	void Camera::moveY(float d) {
		eye = eye + up.unit() * d;
		center = center + up.unit() * d;
	}

	void Camera::moveZ(float d) {
		Vector3f view = (center - eye).unit();
		eye = eye + view * d;
		center = center + view * d;
	}

	void Camera::rotateX(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + up * sin(DEG2RAD(a));
		up = view.cross(right);
		center = eye + view;
	}

	void Camera::rotateY(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
		right = view.cross(up);
		center = eye + view;
	}

	void Camera::look() {

		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			up.x, up.y, up.z
		);
	}
	
	//void Camera::setView(int view, int x, int z) {
	//	if (view == FIRST_PERSON_VIEW) {
	//		//camera = Camera(0.5f + PlayerForward, 2.3f, lanes[player_lane], 1.0f + PlayerForward, 2.3f, lanes[player_lane], 0.0f, 1.0f, 0.0f);
	//		eye.x = 0.5f + x;
	//		eye.y = 2.3f;
	//		eye.z = z;
	//		center.x = 1.0f + x;
	//		center.y = 2.3f;
	//		center.z = z;
	//		up.x = 0.0f;
	//		up.y = 1.0f;
	//		up.z = 0.0f;
	//	}
	//	else if (view==THIRD_PERSON_VIEW){
	//		//camera = Camera(-5.6f + PlayerForward, 2.77f, lanes[player_lane], 1.4f + PlayerForward, 2.84f, lanes[player_lane], 0.0f, 1.0f, 0.0f);
	//		eye.x = -5.6f + x;
	//		eye.y = 2.77f;
	//		eye.z = z;
	//		center.x = 1.4f + x;
	//		center.y = 2.84f;
	//		center.z = z;
	//		up.x = 0.0f;
	//		up.y = 1.0f;
	//		up.z = 0.0f;
	//	}
	//}
