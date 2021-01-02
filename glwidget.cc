// Author: Marc Comino 2019

#include <glwidget.h>

#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#include "./volume.h"
#include "./volume_io.h"

namespace {

const float kFieldOfView = 60;
const float kZNear = 0.1;
const float kZFar = 10;

const char kVertexShaderFile[] = "../../ViewerSV/shaders/raycast.vert";
const char kFragmentShaderFile[] = "../../ViewerSV/shaders/raycast.frag";

const int kVertexAttributeIdx = 0;
const int kNormalAttributeIdx = 1;

bool ReadFile(const std::string filename, std::string *shader_source) {
  std::ifstream infile(filename.c_str());

  if (!infile.is_open() || !infile.good()) {
    std::cerr << "Error " + filename + " not found." << std::endl;
    return false;
  }

  std::stringstream stream;
  stream << infile.rdbuf();
  infile.close();

  *shader_source = stream.str();
  return true;
}

}  // namespace

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent), initialized_(false), width_(0.0), height_(0.0) {
  setFocusPolicy(Qt::StrongFocus);
  qttyText = 0;
}

GLWidget::~GLWidget() {}

bool GLWidget::LoadVolume(const QString &path) {
  std::unique_ptr<data_representation::Volume> vol =
      std::make_unique<data_representation::Volume>();
  qttyText = data_representation::ReadFromDicom(path.toUtf8().constData(), vol.get());
  if (qttyText > -1) {
    vol_.reset(vol.release());
    camera_.UpdateModel(cube_->min_, cube_->max_);
    return true;
  }

  return false;
}

void GLWidget::initializeGL() {
  glewInit();

  glEnable(GL_NORMALIZE);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);

  std::string vertex_shader, fragment_shader;
  bool res = ReadFile(kVertexShaderFile, &vertex_shader) &&
             ReadFile(kFragmentShaderFile, &fragment_shader);

  if (!res) exit(0);

  cube_ = std::make_unique<data_representation::Cube>();
  program_ = std::make_unique<QOpenGLShaderProgram>();
  program_->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                    vertex_shader.c_str());
  program_->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                    fragment_shader.c_str());
  program_->bindAttributeLocation("vertex", kVertexAttributeIdx);
  program_->bindAttributeLocation("normal", kNormalAttributeIdx);
  program_->link();

  initialized_ = true;
}

void GLWidget::resizeGL(int w, int h) {
  if (h == 0) h = 1;
  width_ = w;
  height_ = h;

  camera_.SetViewport(0, 0, w, h);
  camera_.SetProjection(kFieldOfView, kZNear, kZFar);
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    camera_.StartRotating(event->x(), event->y());
  }
  if (event->button() == Qt::RightButton) {
    camera_.StartZooming(event->x(), event->y());
  }
  updateGL();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
  camera_.SetRotationX(event->y());
  camera_.SetRotationY(event->x());
  camera_.SafeZoom(event->y());
  updateGL();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    camera_.StopRotating(event->x(), event->y());
  }
  if (event->button() == Qt::RightButton) {
    camera_.StopZooming(event->x(), event->y());
  }
  updateGL();
}

void GLWidget::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Up) camera_.Zoom(-1);
  if (event->key() == Qt::Key_Down) camera_.Zoom(1);

  if (event->key() == Qt::Key_Left) camera_.Rotate(-1);
  if (event->key() == Qt::Key_Right) camera_.Rotate(1);

  if (event->key() == Qt::Key_W) camera_.Zoom(-1);
  if (event->key() == Qt::Key_S) camera_.Zoom(1);

  if (event->key() == Qt::Key_A) camera_.Rotate(-1);
  if (event->key() == Qt::Key_D) camera_.Rotate(1);

  if (event->key() == Qt::Key_R) {
    std::string vertex_shader, fragment_shader;
    bool res = ReadFile(kVertexShaderFile, &vertex_shader) &&
               ReadFile(kFragmentShaderFile, &fragment_shader);

    if (!res) exit(0);

    program_ = std::make_unique<QOpenGLShaderProgram>();
    program_->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                      vertex_shader.c_str());
    program_->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                      fragment_shader.c_str());
    program_->bindAttributeLocation("vertex", kVertexAttributeIdx);
    program_->bindAttributeLocation("normal", kNormalAttributeIdx);
    program_->link();
  }

  updateGL();
}

void GLWidget::paintGL() {
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (initialized_) {
    camera_.SetViewport();

    Eigen::Matrix4f projection = camera_.SetProjection();
    Eigen::Matrix4f view = camera_.SetView();
    Eigen::Matrix4f model = camera_.SetModel();

    program_->bind();
    GLuint projection_location = program_->uniformLocation("projection");
    glUniformMatrix4fv(projection_location, 1, GL_FALSE, projection.data());

    GLuint view_location = program_->uniformLocation("view");
    glUniformMatrix4fv(view_location, 1, GL_FALSE, view.data());

    GLuint model_location = program_->uniformLocation("model");
    glUniformMatrix4fv(model_location, 1, GL_FALSE, model.data());

    if (vol_ != nullptr) {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_3D, vol_->GetTextureId());

      GLint volume = program_->uniformLocation("volume");
      glUniform1i(volume, 0);
    }

    GLuint color = program_->uniformLocation("color");
    glUniform4fv(color, plot->getSize(), glm::value_ptr(plot->getData()[0]));

    GLuint size = program_->uniformLocation("size");
    glUniform1i(size, plot->getSize());

    GLuint loc_qttyText = program_->uniformLocation("qttyText");
    glUniform1i(loc_qttyText, qttyText);

    cube_->Render();

    glDisable(GL_BLEND);
  }
}

void GLWidget::setPlotRGBA(PlotsRGBA *plot)
{
    this->plot = plot;
}
