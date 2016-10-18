#pragma once

#ifndef IMGUIBACKEND_H
#define IMGUIBACKEND_H

#include "imgui.h"
#include <GLFW/glfw3.h>

namespace imgui_backend
{
    /**
     * Initializes ImGui and creates Graphics Devices. Must be called after initializing Platform and RenderBackends.
     */
	extern void initialize();
    /**
     * Shuts down ImGui and frees Graphics resources.
     */
	extern void shutdown();
    /**
     * Prepares ImGui for a New Frame of the gameloop. Must be called at the beginning of the gameloop.
     */
	extern void new_frame();
    /**
     * Executes ImGui Render Callback.
     */
	extern void render();
    /**
     * Mouse Button Callback. Must be called within the Platform Backend's corresponding Callback.
     * @param window GLFWwindow pointer
     * @param button Button ID
     * @param action Action ID
     * @param mods   Modification
     */
    extern void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    /**
     * Mouse Scroll Callback. Must be called within the Platform Backend's corresponding Callback.
     * @param window GLFWwindow pointer
     * @param xoffset Mouse X Position Offset
     * @param yoffset Mouse Y Position Offset
     * @param mods   Modification
     */
	extern void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    /**
     * Key Callback. Must be called within the Platform Backend's corresponding Callback.
     * @param window GLFWwindow pointer
     * @param key Key ID
     * @param param Unknown for now
     * @param action Action ID
     * @param mods   Modification
     */
	extern void key_callback(GLFWwindow* window, int key, int param, int action, int mods);
    /**
     * Char Input Callback. Must be called within the Platform Backend's corresponding Callback.
     * @param window GLFWwindow pointer
     * @param c      Character ASCII Value
     */
	extern void char_callback(GLFWwindow* window, unsigned int c);
}

#endif

