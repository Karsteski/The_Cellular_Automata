
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

// Application 
#include "Application.h"
#include "Grid.h"
#include "Elementary.h"
#include "GameOfLife.h"

// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_stdlib.h"

// Desktop OpenGL function loader library.
// Helper library to load OpenGL function pointers.
// Initialize with gl3wInit()
#include <GL/gl3w.h> 

// Always include glfw3.h after OpenGL
#include <GLFW/glfw3.h>

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char**)
{
	// Setup window
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;

	// Decide GL+GLSL versions
#if __APPLE__
	// GL 3.2 + GLSL 150
	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

	// Create window with graphics context
	GLFWwindow* window = glfwCreateWindow(1280, 720, "The Cellular Automata", NULL, NULL);
	if (window == NULL)
		return 1;
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	// Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
	bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
	bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
	bool err = gladLoadGL() == 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
	bool err = gladLoadGL(glfwGetProcAddress) == 0; // glad2 recommend using the windowing library loader instead of the (optionally) bundled one.
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
	bool err = false;
	glbinding::Binding::initialize();
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
	bool err = false;
	glbinding::initialize([](const char* name) { return (glbinding::ProcAddress)glfwGetProcAddress(name); });
#else
	bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif

	if (err)
	{
		fprintf(stderr, "Failed to initialize OpenGL loader!\n");
		return 1;
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Window state
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	while (!glfwWindowShouldClose(window))
	{
		// Poll and handle events (inputs, window resize, etc.)
		// Read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to main application.
		// Generally may always pass all inputs to dear imgui, and hide them from the application based on those two flags.
		glfwPollEvents();

		// Feed inputs to imgui, starts a new frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame(); 

		// Settings to get the ImGui window to adapt to the OS window size.
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(io.DisplaySize);
		ImGui::Begin("The Cellular Automata", 0, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		// Application Flags
		static bool show_demo_window = false;

		{
			{
				ImGui::BeginMenuBar();

				//Far more menu items to add.
				if (ImGui::BeginMenu("Menu"))
				{
					ImGui::MenuItem("Show ImGui Demo Window", NULL, &show_demo_window);
					ImGui::EndMenu();
				}

				if (show_demo_window)
				{
					ImGui::ShowDemoWindow(&show_demo_window);
				}

				ImGui::EndMenuBar();
			}

			ImGui::BeginTabBar("Main Tab");
			if (ImGui::BeginTabItem("Basic Drawing Grid"))
			{
				static Grid basic_grid;
				static bool gridSwitch = true;
				ImGui::Checkbox("Enable Grid", &gridSwitch);
				basic_grid.EnableGrid(gridSwitch);

				static int gridSteps = 10;
				ImGui::SetNextItemWidth(100);
				ImGui::SliderInt("Grid Step Size", &gridSteps, 1, 100);
				basic_grid.SetGridSteps(gridSteps);

				static int basic_rect_x = 0;
				static int basic_rect_y = 0;

				ImGui::SetNextItemWidth(100);
				ImGui::InputInt("x-coordinate", &basic_rect_x);
				ImGui::SameLine(); ImGui::SetNextItemWidth(100);
				ImGui::InputInt("y-coordinate", &basic_rect_y);

				ImGui::SetNextItemWidth(300);
				static ImVec4 colour_picker = { 0.0f, 0.0f, 0.0f, 0.33f };
				ImGui::ColorEdit4("Cell Colour", &colour_picker.x);
				basic_grid.SetMainCellColour((ImColor)colour_picker);

				ImGui::Separator();
				
				{
					ImGui::BeginTabBar("Grid Tab");
					ImGui::BeginTabItem("Grid");
					if (basic_rect_x < 0) basic_rect_x = 0;
					if (basic_rect_y < 0) basic_rect_y = 0;

					basic_grid.DrawGrid();

					basic_grid.m_cells_to_draw = { ImVec2(static_cast<float>(basic_rect_x), static_cast<float>(basic_rect_y)) };
					basic_grid.DrawCells();
					ImGui::EndTabItem();
					ImGui::EndTabBar();
				}
				ImGui::EndTabItem();
			}

			// Needs to be its own function
			if (ImGui::BeginTabItem("Elementary Cellular Automata"))
			{
				static Elementary elementaryAutomata;
				auto& ElementaryCellularAutomataRuleset = elementaryAutomata.SetRuleset();
				
				// Can't pass individual bits by reference, therefore the following repeated code is a necessary evil,
				// unless a workaround is implemented to make a vector<bool> act like a regular STL container.
				// Note vector<bool> is not individually addressable because of its special implementation. (https://en.cppreference.com/w/cpp/container/vector_bool)
				// Not worth the trouble in my opinion...
				static bool rulesetPosition_0 = ElementaryCellularAutomataRuleset[0];
				static bool rulesetPosition_1 = ElementaryCellularAutomataRuleset[1];
				static bool rulesetPosition_2 = ElementaryCellularAutomataRuleset[2];
				static bool rulesetPosition_3 = ElementaryCellularAutomataRuleset[3];
				static bool rulesetPosition_4 = ElementaryCellularAutomataRuleset[4];
				static bool rulesetPosition_5 = ElementaryCellularAutomataRuleset[5];
				static bool rulesetPosition_6 = ElementaryCellularAutomataRuleset[6];
				static bool rulesetPosition_7 = ElementaryCellularAutomataRuleset[7];

				ImGui::Checkbox("##0", &rulesetPosition_0); ImGui::SameLine();
				ImGui::Checkbox("##1", &rulesetPosition_1); ImGui::SameLine();
				ImGui::Checkbox("##2", &rulesetPosition_2); ImGui::SameLine();
				ImGui::Checkbox("##3", &rulesetPosition_3); ImGui::SameLine();
				ImGui::Checkbox("##4", &rulesetPosition_4); ImGui::SameLine();
				ImGui::Checkbox("##5", &rulesetPosition_5); ImGui::SameLine();
				ImGui::Checkbox("##6", &rulesetPosition_6); ImGui::SameLine();
				ImGui::Checkbox("##7", &rulesetPosition_7); 

				ElementaryCellularAutomataRuleset[0] = rulesetPosition_0;
				ElementaryCellularAutomataRuleset[1] = rulesetPosition_1;
				ElementaryCellularAutomataRuleset[2] = rulesetPosition_2;
				ElementaryCellularAutomataRuleset[3] = rulesetPosition_3;
				ElementaryCellularAutomataRuleset[4] = rulesetPosition_4;
				ElementaryCellularAutomataRuleset[5] = rulesetPosition_5;
				ElementaryCellularAutomataRuleset[6] = rulesetPosition_6;
				ElementaryCellularAutomataRuleset[7] = rulesetPosition_7;
				
				// Bitset is read in reverse order
				std::string rulesetReversedString = ElementaryCellularAutomataRuleset.to_string();
				std::reverse(rulesetReversedString.begin(), rulesetReversedString.end());

				std::string rulesetText = "Current Ruleset = " + rulesetReversedString;

				ImGui::Text(rulesetText.c_str());
				
				if (ImGui::Button("Generate"))
				{	
					try
					{
						elementaryAutomata.GenerateElementaryAutomata();
					}
					catch (const std::out_of_range&)
					{
						ImGui::SetTooltip("There was an out of range error!");
					}
				}
				
				elementaryAutomata.DrawGrid();
				elementaryAutomata.DrawCells();

				ImGui::EndTabItem();
			}
			
			if (ImGui::BeginTabItem("Game of Life"))
			{
				static GameOfLife ConwaysGameOfLife;

				auto [width, height] = ConwaysGameOfLife.GetGameDimensions();
				static int gameWidth = width;
				static int gameHeight = height;

				ImGui::SetNextItemWidth(100);
				ImGui::InputInt("Game Width", &gameWidth);
				ImGui::SameLine(); ImGui::SetNextItemWidth(100);
				ImGui::InputInt("Game Height", &gameHeight);

				ConwaysGameOfLife.SetGameDimensions(ImVec2(gameWidth, gameHeight));

				static int radioButtonSwitch = 0;
				ImGui::RadioButton("Conway's Game of Life", &radioButtonSwitch, 0); ImGui::SameLine();
				ImGui::RadioButton("R-Pentomino", &radioButtonSwitch, 1); ImGui::SameLine();
				ImGui::RadioButton("Glider Gun", &radioButtonSwitch, 2); ImGui::SameLine();
				ImGui::RadioButton("Infinite Growth", &radioButtonSwitch, 3);

				if (ImGui::Button("Generate"))
				{
					switch (radioButtonSwitch)
					{
					case 0:
					{
						ConwaysGameOfLife.GenerateRandomCells();
						break;
					}
					case 1:
					{
						ConwaysGameOfLife.GeneratePattern(Pattern::R_Pentomino);
						break;
					}
					case 2:
					{
						ConwaysGameOfLife.GeneratePattern(Pattern::Glider_Gun);
						break;
					}
					case 3:
					{
						ConwaysGameOfLife.GeneratePattern(Pattern::Infinite_Growth);
						break;
					}
					}
				}
				
				static int gridSteps = 5;
				ImGui::SetNextItemWidth(100);
				ImGui::SliderInt("Zoom", &gridSteps, 1, 100);
				ConwaysGameOfLife.SetGridSteps(gridSteps);

				ConwaysGameOfLife.SetAllCellStates();
				ConwaysGameOfLife.DrawGrid();
				ConwaysGameOfLife.DrawCells();

				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
			ImGui::End();
		}

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);

	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
