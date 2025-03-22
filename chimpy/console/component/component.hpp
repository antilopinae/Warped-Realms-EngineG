#pragma once


#include <string>

#include "../include/config_imgui.hpp"


namespace Core
{

/**
 * @class Component
 * @brief Abstract base class for all components in the Core namespace.
 *
 * The `Component` class provides a common interface for all components.
 * Each component has an `render` method that must be implemented in derived classes.
 * The `onUpdate` function member allows for dynamic customization of the render logic.
 *
 * CustomComponent comp;
 * comp.render(16); // Call the render method with a time delta of 16 ms
 */
class Component
{
public:
  std::string render_name_;
  float pos_left_, pos_top_, size_w_, size_h_;


  /**
   * @brief Protected constructor to prevent direct instantiation of `Component`.
   *
   * This constructor is accessible only to derived classes, ensuring that
   * `Component` can only be instantiated as part of a subclass.
   */
protected:
  Component(const std::string& render_name, float pos_left = 0, float pos_top = 0, float size_w = 0, float size_h = 0)
      : render_name_(render_name),
        pos_left_(pos_left),
        pos_top_(pos_top),
        size_w_(size_w),
        size_h_(size_h)
  {
  }


  /**
   * @brief Pure virtual method for updating the component.
   *
   * This method must be implemented in all derived classes. It defines
   * the behavior of the component during the render cycle.
   *
   * @param ms Time elapsed in milliseconds since the last render.
   */
public:
  virtual void render(float display_w, float display_h) = 0;


public:
  std::string getRenderName() const
  {
    return render_name_;
  }


protected:
  void resizeLeftButton(float display_w, float display_h)
  {
    ImVec2 current_size = ImGui::GetWindowSize();
    ImGui::SetWindowSize(current_size);

    size_w_ = current_size.x / display_w;
    size_h_ = current_size.y / display_h;

    pos_left_ = 1 - size_w_;
  }


protected:
  void resizeRightButton(float display_w, float display_h)
  {
    ImVec2 current_size = ImGui::GetWindowSize();
    ImGui::SetWindowSize(current_size);

    size_w_ = current_size.x / display_w;
    size_h_ = current_size.y / display_h;
  }


protected:
  void resizeCenter(float display_w, float display_h)
  {
    //  TODO:
  }


  /**
   * @brief Virtual destructor for safe polymorphic deletion.
   */
public:
  virtual ~Component() = default;
};


}  // namespace Core
