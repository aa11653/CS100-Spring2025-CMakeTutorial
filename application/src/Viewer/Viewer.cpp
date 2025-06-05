#include <Corrade/Containers/ArrayViewStl.h>
#include <LorenzField.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Math/Angle.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/Shaders/PhongGL.h>
#include <Magnum/Timeline.h>
#include <Magnum/Trade/MeshData.h>
#include <vector>

using namespace Magnum;

class LorenzFieldViewer : public Platform::Application {

  // 立方体结构体，用于存储立方体的位置、旋转和缩放信息
  struct Cube {
    Vector3 position;
    Matrix4 rotation;
    Vector3 scale;
  };

public:
  explicit LorenzFieldViewer(const Arguments &arguments);

private:
  void drawEvent() override;
  void pointerMoveEvent(PointerMoveEvent &event) override;
  void keyPressEvent(KeyEvent &event) override;
  void updateCubes(Float dt);

  GL::Mesh _mesh;
  Shaders::PhongGL _shader;
  Matrix4 _view, _projection;
  Timeline _timeline;

  LorenzField::LorenzField _lorenz_field;
  size_t _num_cubes;
  std::vector<Cube> _cubes;
  Float _speed; // 模拟速率
  Float _min_speed; // 最小速率
  Float _max_speed; // 最大速率
};

LorenzFieldViewer::LorenzFieldViewer(const Arguments &arguments)
    : Platform::Application{
        arguments,
        Configuration{}.setTitle("Lorentz Field Viewer")
      },
      _shader{
        Shaders::PhongGL::Configuration{}.setFlags(
          Shaders::PhongGL::Flag::InstancedTransformation)
      },
      _num_cubes(1000),
      _speed(0.01f),
      _min_speed(0.01f),
      _max_speed(1.0f)
{
  using namespace Magnum::Math::Literals;

  // 初始化渲染器的一些选项
  {
    GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
    GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);
  }

  // 创建一个参考的立方体
  _mesh = MeshTools::compile(Primitives::cubeSolid());

  // 初始化相机参数
  {
    _view = Matrix4();
    _projection =
        Matrix4::perspectiveProjection(
            70.0_degf, Vector2{windowSize()}.aspectRatio(), 0.01f, 100.0f) *
        Matrix4::translation(Vector3(0.0f, -20.0f, -30.0f));
  }

  // 初始化立方体参数
  for (size_t i = 0; i < _num_cubes; ++i) {
    Vector3 position = Vector3(
        2.0f, (static_cast<Float>(i) / _num_cubes - 0.01f) * 0.10f + 1.0f,
        1.0f);
    _cubes.push_back({position, Matrix4(), Vector3(0.1f)});
  }

  _timeline.start();
}

void LorenzFieldViewer::updateCubes(Float dt) {
  const Float factor = _speed * dt;
  for (auto &cube : _cubes) {
    // 根据洛伦兹场的数值更新位置
    cube.position += _lorenz_field.valueAt(cube.position) * factor;

    // 根据洛伦兹场的旋度更新旋转，旋度的模长决定了旋转的速率
    const Vector3 curl = _lorenz_field.curlAt(cube.position) * factor;
    const Float len = curl.length();
    if (len < 0.01f)
      continue;
    cube.rotation = Matrix4::rotation(Deg{len}, curl / len) * cube.rotation;
  }
}

void LorenzFieldViewer::drawEvent() {
  using namespace Magnum::Math::Literals;

  // 清空默认帧缓冲区（也就是之前画上去的东西）
  GL::defaultFramebuffer.clearDepth(1.0f);
  GL::defaultFramebuffer.clearColor(0x202020_rgbf);

  // 更新立方体的位置和旋转
  if (_timeline.previousFrameTime() > 1.0f) {
    updateCubes(_timeline.previousFrameDuration());
  }

  // 根据存储的立方体信息计算一系列实例参数
  struct CubeInstanceData {
    Matrix4 transform;
    Matrix3x3 normal;
  };

  std::vector<CubeInstanceData> instance_data(_cubes.size());
  for (size_t i = 0; i < _cubes.size(); ++i) {
    const Cube &cube = _cubes[i];
    instance_data[i].transform = _view * Matrix4::translation(cube.position) *
                                 cube.rotation * Matrix4::scaling(cube.scale);
    instance_data[i].normal = instance_data[i].transform.normalMatrix();
  }

  GL::Buffer instance_buffer;
  instance_buffer.setData(instance_data);

  _mesh.setInstanceCount(_cubes.size())
      .addVertexBufferInstanced(instance_buffer, 1, 0,
                                Shaders::PhongGL::TransformationMatrix{},
                                Shaders::PhongGL::NormalMatrix{});

  _shader.setLightPositions({{10.0f, 10.0f, 0.0f, 0.0f}})
      .setDiffuseColor(Color3::fromHsv({240.0_degf, 0.6f, 0.8f}))
      .setAmbientColor(Color3::fromHsv({330.0_degf, 0.3f, 0.3f}))
      .setProjectionMatrix(_projection)
      .draw(_mesh);

  swapBuffers();
  redraw();
  _timeline.nextFrame();
}

void LorenzFieldViewer::keyPressEvent(KeyEvent &event) {
  // 控制模拟速率的键盘事件处理 Up = 增加速率, Down = 减少速率
  if (event.key() == Sdl2Application::Key::Up) {
    _speed = Math::min(_speed * 2.0f, _max_speed);
    event.setAccepted();
  } else if (event.key() == Sdl2Application::Key::Down) {
    _speed = Math::max(_speed / 2.0f, _min_speed);
    event.setAccepted();
  }
}

void LorenzFieldViewer::pointerMoveEvent(PointerMoveEvent &event) {
  // 鼠标移动事件处理，用于旋转视图
  if (!event.isPrimary() ||
      !(event.pointers() & (Pointer::MouseLeft | Pointer::Finger)))
    return;

  Vector2 delta =
      3.0f * Vector2{event.relativePosition()} / Vector2{windowSize()};

  _view = Matrix4::rotationX(Rad{delta.y()}) * _view *
          Matrix4::rotationY(Rad{delta.x()});

  event.setAccepted();
  redraw();
}

MAGNUM_APPLICATION_MAIN(LorenzFieldViewer)
