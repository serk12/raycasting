// Author: Marc Comino 2019

#include <camera.h>

#include <GL/glew.h>

#define GLM_FORCE_RADIANS

#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>

namespace data_visualization {

namespace {

const Eigen::Vector3f vra(0.0, 1.0, 0.0);
const Eigen::Vector3f hra(1.0, 0.0, 0.0);

}  //  namespace

Camera::Camera()
    : distance_(2.0f),
      step_(0.05f),
      rotating_(false),
      zooming_(false),
      rotation_y_(0.0f),
      rotation_x_(0.0f),
      current_x_(-1.0f),
      current_y_(-1.0f),
      viewport_x_(0.0f),
      viewport_y_(0.0f),
      viewport_width_(0.0f),
      viewport_height_(0.0f),
      centering_x_(0.0f),
      centering_y_(0.0f),
      centering_z_(0.0f),
      scaling_(1.0f),
      field_of_view_(0.0f),
      z_near_(0.0f),
      z_far_(0.0f) {}

void Camera::SetViewport(double x, double y, double w, double h) {
  viewport_x_ = x;
  viewport_y_ = y;
  viewport_width_ = w;
  viewport_height_ = h;

  glViewport(viewport_x_, viewport_y_, viewport_width_, viewport_height_);
}

void Camera::SetViewport() const {
  glViewport(viewport_x_, viewport_y_, viewport_width_, viewport_height_);
}

Eigen::Matrix4f Camera::SetIdentity() const {
  Eigen::Matrix4f identity;
  identity << 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1;
  return identity;
}

Eigen::Matrix4f Camera::SetModel() const {
  const Eigen::Affine3f kScaling(
      Eigen::Scaling(Eigen::Vector3f(scaling_, scaling_, scaling_)));
  const Eigen::Affine3f kTranslation(Eigen::Translation3f(
      Eigen::Vector3f(centering_x_, centering_y_, centering_z_)));

  return kScaling.matrix() * kTranslation.matrix();
}

Eigen::Matrix4f Camera::SetView() const {
  const Eigen::Affine3f kTranslation(
      Eigen::Translation3f(Eigen::Vector3f(0, 0, -distance_)));
  const Eigen::Affine3f kRotationA(Eigen::AngleAxisf(rotation_x_, hra));
  const Eigen::Affine3f kRotationB(Eigen::AngleAxisf(rotation_y_, vra));

  return kTranslation.matrix() * kRotationA.matrix() * kRotationB.matrix();
}

Eigen::Matrix4f Camera::SetProjection(double fov, double znear, double zfar) {
  field_of_view_ = fov;
  z_near_ = znear;
  z_far_ = zfar;

  return SetProjection();
}

Eigen::Matrix4f Camera::SetProjection() const {
  glm::dmat4x4 glm_perspective =
      glm::perspective((field_of_view_ * M_PI / 180.0),
                       (viewport_width_ / viewport_height_), z_near_, z_far_);

  Eigen::Matrix4f eigen_perspective;
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j) eigen_perspective(i, j) = glm_perspective[j][i];

  return eigen_perspective;
}

void Camera::Zoom(double modifier) {
  distance_ += step_ * modifier;

  if (distance_ < kMinCameraDistance) distance_ = kMinCameraDistance;
  if (distance_ > kMaxCameraDistance) distance_ = kMaxCameraDistance;
}

void Camera::SafeZoom(double modifier) {
  if (zooming_) {
    Zoom(modifier - current_y_ < 0 ? -1 : 1);
    current_y_ = modifier;
  }
}

void Camera::Rotate(double modifier) {
  rotation_y_ += AngleIncrement * modifier;
}

void Camera::UpdateModel(Eigen::Vector3f min, Eigen::Vector3f max) {
  Eigen::Vector3f center = (min + max) / 2.0;
  centering_x_ = -center[0];
  centering_y_ = -center[1];
  centering_z_ = -center[2];

  double longest_edge =
      std::max(max[0] - min[0], std::max(max[1] - min[1], max[2] - min[2]));
  scaling_ = 1.0 / longest_edge;
}

void Camera::SetRotationX(double y) {
  if (rotating_) {
    rotation_x_ += (y - current_y_) * step_;
    current_y_ = y;
    if (rotation_x_ < kMinRotationX) rotation_x_ = kMinRotationX;
    if (rotation_x_ > MaxRotationX) rotation_x_ = MaxRotationX;
  }
}

void Camera::SetRotationY(double x) {
  if (rotating_) {
    rotation_y_ += (x - current_x_) * step_;
    current_x_ = x;
  }
}

void Camera::StartRotating(double x, double y) {
  current_x_ = x;
  current_y_ = y;
  rotating_ = true;
}

void Camera::StopRotating(double x, double y) {
  current_x_ = x;
  current_y_ = y;
  rotating_ = false;
}

void Camera::StartZooming(double x, double y) {
  current_x_ = x;
  current_y_ = y;
  zooming_ = true;
}

void Camera::StopZooming(double x, double y) {
  current_x_ = x;
  current_y_ = y;
  zooming_ = false;
}

}  //  namespace data_visualization
