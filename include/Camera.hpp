#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera {
private:
	glm::vec3 m_cameraPosition; // absolute position of the camera
	glm::vec3 m_cameraRight;	// the right direction
	glm::vec3 m_cameraUp;		// the up direction of the camera
	glm::vec3 m_cameraFront;	// also the direction foreward
	glm::vec3 m_cameraWatching; // where the camera is watching relative to itself

	// vars
	float		m_yaw			   = -90.0f; // rotation along the y axys
	float		m_pitch			   = 0.0f;	 // rotation aling the x axys
	float		m_FOV			   = 45.0f;	 // Fiel of view 1 -> 90
	const float m_cameraSpeedConst = 2.0f;	 // camera movement base speed

public:
	Camera(glm::vec3 position);

	glm::mat4 move(glm::vec3 offset);
	glm::mat4 point(float posx, float posy);
	void	  zoom(float diff_FOV);
	glm::vec3 getFrontSpeed();
	glm::vec3 getRightSpeed();
	glm::vec3 getUpSpeed();
	glm::mat4 getPerspective(float width, float height);
};