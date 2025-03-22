#include "window.hpp"
#include "component/text_cmp.hpp"


namespace Core
{


Window::Window(const std::string& name, int width, int height) : component_managers_(1)
{
  init(name, width, height);

  initComponents();

  setfnCallback(
    [&](int e) -> void
    {
      // if (e.getType() == Event::EventType::WINDOW_CLOSED_EVENT)
      // {
      //   // dispatching
      //   glfwSetWindowShouldClose(window_, GL_TRUE);
      // }
    });
}


void Window::init(const std::string& name, int width, int height)
{
  name_ = name;
  width_ = width;
  height_ = height;

  if (!glfwInit())
  {
    __asm__("int3");
  }

  window_ = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);

  if (!window_)
  {
    __asm__("int3");
  }

  glfwMakeContextCurrent(window_);  // open gl state machine contexts for thread

  glfwSetWindowUserPointer(window_, this);
  glfwSetCursorPosCallback(window_, mouseMovedCallback);
  glfwSetScrollCallback(window_, mouseScrollCallback);
  glfwSetMouseButtonCallback(window_, mouseButtonCallback);
  glfwSetKeyCallback(window_, keyCallback);
  glfwSetWindowSizeCallback(window_, windowResizeCallback);
  glfwSetWindowCloseCallback(window_, windowCloseCallback);

  const char* glsl_version = "#version 130";
  glfwSwapInterval(1);

  IMGUI_CHECKVERSION();
  auto context = ImGui::CreateContext();
  // ImGui::SetCurrentContext(context);

  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window_, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
}


void Window::initComponents()
{
  component_managers_[0].addCmps(std::make_shared<Core::TextCmp>("Text Component"));
}


void Window::render()
{
  int display_w, display_h;
  glfwGetFramebufferSize(window_, &display_w, &display_h);

  glfwPollEvents();

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::SetNextWindowSize(ImVec2(display_w, display_h));

  ImGui::Begin("Laboratory Tabs", NULL,
    ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

  float window_w = display_w;
  float window_h = display_h * 0.9;

  component_managers_[0].render(window_w, window_h, 0.1f);

  ImGui::End();

  ImGui::Render();
  glViewport(0, 0, display_w, display_h);
  glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  glfwSwapBuffers(window_);
}


void Window::mouseMovedCallback(GLFWwindow* window, double x, double y)
{
  auto& handle = *(Window*)glfwGetWindowUserPointer(window);
  // MouseMovedEvent e(x, y);
  handle.fnCallback1_(1);
}


void Window::mouseScrollCallback(GLFWwindow* window, double x, double y)
{
  auto& handle = *(Window*)glfwGetWindowUserPointer(window);
  // MouseScrolledEvent e(y);
  handle.fnCallback1_(2);
}


void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
  auto& handle = *(Window*)glfwGetWindowUserPointer(window);

  if (action == GLFW_PRESS)
  {
    // MouseButtonPressedEvent e(button);
    handle.fnCallback1_(3);
  }
  else
  {
    // MouseButtonReleasedEvent e(button);
    handle.fnCallback1_(4);
  }
}


void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  auto& handle = *(Window*)glfwGetWindowUserPointer(window);

  if (action == GLFW_PRESS)
  {
    // KeyPressedEvent e(key);
    handle.fnCallback1_(5);
  }
  else if (action == GLFW_RELEASE)
  {
    // KeyReleasedEvent e(key);
    handle.fnCallback1_(6);
  }
}


void Window::windowResizeCallback(GLFWwindow* window, int width, int height)
{
  auto& handle = *(Window*)glfwGetWindowUserPointer(window);
  // WindowResizedEvent e(width, height);
  handle.width_ = width;
  handle.height_ = height;
  handle.fnCallback1_(7);
}


void Window::windowCloseCallback(GLFWwindow* window)
{
  auto& handle = *(Window*)glfwGetWindowUserPointer(window);
  // WindowClosedEvent e;
  handle.fnCallback1_(8);
}


void Window::renderComponent(const Component& target) {}


Window::~Window()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwWindowShouldClose(window_);
  glfwDestroyWindow(window_);
  glfwTerminate();
}


}  // namespace Core
