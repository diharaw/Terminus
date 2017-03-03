cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project("imgui")

set(IMGUI_HEADERS "imgui.h"
				  "imconfig.h"
				  "imgui_internal.h"
				  "stb_rect_pack.h"
				  "stb_textedit.h"
				  "imgui_dock.h"
				  "imgui_user.h"
				  "stb_truetype.h"
				  "ImGuizmo.h")

set(IMGUI_SOURCES "imgui.cpp"
				  "imgui_demo.cpp"
				  "imgui_draw.cpp"
				  "imgui_dock.cpp"
				  "imgui_user.inl"
				  "ImGuizmo.cpp")

add_library(imgui ${IMGUI_SOURCES} ${IMGUI_HEADERS})
set_target_properties(imgui PROPERTIES
                      FOLDER "Third Party")
