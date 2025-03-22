#define STB_IMAGE_IMPLEMENTATION

#include "include/cui.hpp"

#include <atomic>


void cui(const std::atomic<bool>& flag)
{
  Core::Window window = {"Window", 800, 600};

  while (!glfwWindowShouldClose(window.getGLFWwindow()))
  {
    if (!flag.load())
    {
      glfwSetWindowShouldClose(window.getGLFWwindow(), true);
      // glfwDestroyWindow(window.getGLFWwindow());
      // glfwTerminate();
      break;
    }
    window.render();
  }
}
