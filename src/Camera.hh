#ifndef CAMERA_HH_
#define CAMERA_HH_

#include "glm.hh"

class Camera
{
public:
  Camera();
  virtual ~Camera();
  
  void              SetPosition(const glm::vec3 & position);
  void              SetTargetPosition(const glm::vec3 & position);
  void              SetFOV(double fov);
  void              SetClippingPlanes(double near, double far);
  void              SetUp(const glm::vec3 & up);
  
  virtual void      MoveForward(double amount);
  virtual void      MoveRight(double amount);
  virtual void      MoveUp(double amount);

  const glm::mat4 & GetViewProjection() const;
  const glm::mat4 & GetView() const;
  const glm::mat4 & GetProjection() const;
  void              GetDirection(glm::vec3 & direction) const;

  void              UpdateProjection();
  void              UpdateView(); 

private:
  double    _fov;
  double    _clippingplane_near;
  double    _clippingplane_far;
  glm::vec3 _up;
  glm::vec3 _position;
  glm::vec3 _target_position;
  glm::mat4 _view_projection;
  glm::mat4 _projection;
  glm::mat4 _view;
};

#endif
