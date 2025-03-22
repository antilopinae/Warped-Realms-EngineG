#pragma once

#include <algorithm>
#include <cstdint>
#include <functional>
#include <list>
#include <memory>
#include <string>
#include <utility>

#include "component/component_manager.hpp"
#include "config_imgui.hpp"


namespace Core
{


class Window final
{
private:
  std::vector<ComponentManager> component_managers_;

  std::string name_;
  int width_, height_;
  GLFWwindow* window_ = nullptr;
  std::function<void(int)> fnCallback1_;


  /**
   * @brief Private constructor to enforce the singleton design pattern.
   * @param name_ Name of the window.
   * @param width Width of the window in pixels.
   * @param height_ Height of the window in pixels.
   */
public:
  Window(const std::string& name, int width, int height);


public:
  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;


public:
  ~Window();


  /**
   * @brief Retrieves the singleton instance of the Window.
   * @return Reference to the single instance of the Window.
   */
private:
  static Window& getInstance()
  {
    static Window instance("Window", 800, 600);
    return instance;
  }


  /**
   * @brief Initializes the window with a name_, width, and height_.
   * @param name_ Name of the window.
   * @param width Width of the window in pixels.
   * @param height_ Height of the window in pixels.
   */
public:
  void init(const std::string& name, int width, int height);


public:
  void initComponents();


  /**
   * @brief Retrieves the GLFW window pointer.
   * @return Pointer to the GLFW window instance.
   */
  __attribute__((always_inline)) GLFWwindow* getGLFWwindow() const
  {
    return window_;
  }


  /**
   * @brief Sets a custom callback function for processing events.
   * @param fn Function to handle events.
   */
  void setfnCallback(const std::function<void(int)>& fn)
  {
    fnCallback1_ = fn;
  }


  /**
   * @brief Updates the window, including component renders and event handling.
   */
public:
  void render();


  /**
   * @brief Adds one or more components to the window.
   * @tparam Component Variadic template for multiple components.
   * @param cmps Components to add to the window.
   */
  template <typename... Component>
  void addCmps(std::shared_ptr<Component>&&... cmps, size_t index_manager)
  {
    component_managers_[index_manager].addCmps(std::forward<std::shared_ptr<Component>>(cmps)...);
  }


  /**
   * @brief Renders a given component.
   * @param target Reference to the component to render.
   */
  void renderComponent(const Component& target);


public:
  /**
   * @brief Callback for mouse movement events.
   * @param window Pointer to the GLFW window.
   * @param x New x-coordinate of the mouse.
   * @param y New y-coordinate of the mouse.
   */
  static void mouseMovedCallback(GLFWwindow* window, double x, double y);


  /**
   * @brief Callback for mouse scroll events.
   * @param window Pointer to the GLFW window.
   * @param x Scroll offset along the x-axis.
   * @param y Scroll offset along the y-axis.
   */
  static void mouseScrollCallback(GLFWwindow* window, double x, double y);


  /**
   * @brief Callback for mouse button press/release events.
   * @param window Pointer to the GLFW window.
   * @param button Mouse button that was pressed or released.
   * @param action Action (GLFW_PRESS, GLFW_RELEASE).
   * @param mods Modifier keys pressed during the event.
   */
  static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);


  /**
   * @brief Callback for key press/release events.
   * @param window Pointer to the GLFW window.
   * @param key Key that was pressed or released.
   * @param scancode System-specific scancode of the key.
   * @param action Action (GLFW_PRESS, GLFW_RELEASE, GLFW_REPEAT).
   * @param mods Modifier keys pressed during the event.
   */
  static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);


  /**
   * @brief Callback for window resize events.
   * @param window Pointer to the GLFW window.
   * @param width New width of the window.
   * @param height_ New height_ of the window.
   */
  static void windowResizeCallback(GLFWwindow* window, int width, int height_);


  /**
   * @brief Callback for window close events.
   * @param window Pointer to the GLFW window.
   */
  static void windowCloseCallback(GLFWwindow* window);
};


}  // namespace Core
