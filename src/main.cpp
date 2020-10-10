/*
* Developed by Kareem Skinner - (https://github.com/KareemSkinner/The_Cellular_Automata)
* This program generates elementary cellular automata and Conway's Game of Life.
* Inspired by Stephen Wolfram's book - "A New Kind of Science"
*/

#include <vector>
#include <chrono>
#include <thread>
#include <memory>

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

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Window state
	const ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

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
		static bool show_basic_drawing_grid = false;
		static bool show_about_window = false;
		static bool show_rules_window = false;

		{
			{
				ImGui::BeginMenuBar();

				if (ImGui::BeginMenu("Menu"))
				{
					ImGui::MenuItem("Show ImGui Demo Window", NULL, &show_demo_window);
					ImGui::MenuItem("Show Basic Drawing Grid", NULL, &show_basic_drawing_grid);
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("About"))
				{
					ImGui::MenuItem("Rules", NULL, &show_rules_window);
					ImGui::MenuItem("About...", NULL, &show_about_window);	
					ImGui::EndMenu();
				}

				{
					// Metrics
					ImGui::Separator();
					ImGui::Text("Dear ImGui Version: %s", ImGui::GetVersion());
					ImGui::SameLine();
					ImGui::Text("Framerate = %.1f FPS", io.Framerate);
				}
				
				if (show_demo_window)
				{
					ImGui::ShowDemoWindow(&show_demo_window);
				}

				if (show_rules_window)
				{
					ImGui::Begin("Rules", &show_rules_window, ImGuiWindowFlags_AlwaysAutoResize);

					ImGui::Text("John Conway's Game of Life:");
					const std::string gameOfLifeRules =
						"The universe of the Game of Life is an infinite, two-dimensional orthogonal grid of square cells, each of which is in one of two possible states, alive or dead.\n"
						"Every cell interacts with its eight neighbours, which are the cells that are horizontally, vertically, or diagonally adjacent.\n"
						"At each step in time, the following transitions occur:\n"
						"1. Any live cell with fewer than two live neighbours dies, as if by underpopulation.\n"
						"2. Any live cell with two or three live neighbours lives on to the next generation.\n"
						"3. Any live cell with more than three live neighbours dies, as if by overpopulation.\n"
						"4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.\n";
					ImGui::Text(gameOfLifeRules.c_str());

					ImGui::Separator();

					ImGui::Text("Elementary Cellular Automata:");
					const std::string elementaryCellularAutomataRules =
						"The simplest class of aone-dimensional cellular automata.\n"
						"Elementary cellular automata have two possible values for each cell (0 or 1), and rules that depend only on previous neighbor values.\n"
						"As a result, the evolution of an elementary cellular automaton can completely be described by a table specifying the state a given cell will have in the next generation.\n"
						"These states are based on the value of the cell itself, and the values of the cells to the left and right.\n"
						"Since there are 2^3 = 8 possible binary states for the neighbour cells, there are a total of 2^8 = 256 elementary cellular automata, indexted by an 8-bit binary number.\n"
						"One such example is the default example given in this program, known as rule 90: 01011010.\n"
						"- Stephen Wolfram - Wolfram MathWorld (https://mathworld.wolfram.com/ElementaryCellularAutomaton.html)"
						"";
					ImGui::Text(elementaryCellularAutomataRules.c_str());

					ImGui::End();
				}

				if (show_about_window)
				{
					ImGui::Begin("About this program", &show_about_window, ImGuiWindowFlags_AlwaysAutoResize);

					const std::string aboutText =
						"Developed by Kareem Skinner - (https://github.com/KareemSkinner/The_Cellular_Automata).\n"
						"This program generates elementary cellular automata and Conway's Game of Life.\n"
						"Inspired by Stephen Wolfram's book - 'A New Kind of Science'.\n"
						"Please enjoy :)\n";

					ImGui::Text(aboutText.c_str());
					ImGui::End();
				}

				ImGui::EndMenuBar();
			}

			ImGui::BeginTabBar("Main Tab");

			if(show_basic_drawing_grid)
			{
				if (ImGui::Begin("Basic Grid", &show_basic_drawing_grid))
				{
					static std::unique_ptr<Grid> basicGrid = std::make_unique<Grid>();

					static bool gridSwitch = true;
					ImGui::Checkbox("Enable Grid", &gridSwitch);
					basicGrid->EnableGrid(gridSwitch);

					static int gridSteps = 40;
					ImGui::SetNextItemWidth(100);
					ImGui::SliderInt("Grid Step Size", &gridSteps, 1, 100);
					basicGrid->SetGridSteps(gridSteps);

					static int basic_rect_x = 0;
					static int basic_rect_y = 0;

					ImGui::SetNextItemWidth(100);
					ImGui::InputInt("x-coordinate", &basic_rect_x);
					ImGui::SameLine(); ImGui::SetNextItemWidth(100);
					ImGui::InputInt("y-coordinate", &basic_rect_y);

					ImGui::SetNextItemWidth(300);
					static ImVec4 colour_picker = { 1.0f, 0.0f, 0.0f, 1.0f };
					ImGui::ColorEdit4("Cell Colour", &colour_picker.x);
					basicGrid->SetMainCellColour(static_cast<ImColor>(colour_picker));

					ImGui::Separator();

					{
						ImGui::BeginTabBar("Grid Tab");
						ImGui::BeginTabItem("Grid");
						if (basic_rect_x < 0) basic_rect_x = 0;
						if (basic_rect_y < 0) basic_rect_y = 0;

						basicGrid->DrawGrid();

						basicGrid->m_cells_to_draw = { ImVec2(static_cast<float>(basic_rect_x), static_cast<float>(basic_rect_y)) };
						basicGrid->DrawCells();
						ImGui::EndTabItem();
						ImGui::EndTabBar();
					}

					ImGui::End();
				}
			}
			
			if (ImGui::BeginTabItem("Game of Life"))
			{
				static std::unique_ptr<GameOfLife> ConwaysGameOfLife = std::make_unique<GameOfLife>();

				static bool GoLgridSwitch = false;
				ImGui::Checkbox("Enable Grid", &GoLgridSwitch);
				ConwaysGameOfLife->EnableGrid(GoLgridSwitch);

				auto [width, height] = ConwaysGameOfLife->GetGameDimensions();
				static float gameWidth = width;
				static float gameHeight = height;

				if (gameWidth < 0) gameWidth = 0;
				if (gameHeight < 0) gameHeight = 0;

				ImGui::SetNextItemWidth(100);
				ImGui::InputFloat("Game Width", &gameWidth, 1.0f, 10.0f, "%.0f");
				ImGui::SameLine(); ImGui::SetNextItemWidth(100);
				ImGui::InputFloat("Game Height", &gameHeight, 1.0f, 10.0f, "%.0f");

				ConwaysGameOfLife->SetGameDimensions(ImVec2(gameWidth, gameHeight));

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
						ConwaysGameOfLife->GenerateRandomCells();
						break;
					}
					case 1:
					{
						ConwaysGameOfLife->GeneratePattern(Pattern::R_Pentomino);
						break;
					}
					case 2:
					{
						ConwaysGameOfLife->GeneratePattern(Pattern::Glider_Gun);
						break;
					}
					case 3:
					{
						ConwaysGameOfLife->GeneratePattern(Pattern::Infinite_Growth);
						break;
					}
					}
				}
				
				static int gridSteps = 5;
				ImGui::SetNextItemWidth(100);
				ImGui::SliderInt("Zoom", &gridSteps, 1, 100);
				ConwaysGameOfLife->SetGridSteps(gridSteps);

				ConwaysGameOfLife->GenerateGameOfLife();

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Elementary Cellular Automata"))
			{
				std::unique_ptr<Elementary> elementaryAutomata = std::make_unique<Elementary>();
				auto& ElementaryCellularAutomataRuleset = elementaryAutomata->SetRuleset();

				static int nCellsPerGeneration = elementaryAutomata->GetNumberOfCellsPerGeneration();
				static int nGenerations = elementaryAutomata->GetNumberOfGenerations();

				if (nCellsPerGeneration < 0) nCellsPerGeneration = 0;
				if (nGenerations < 0) nGenerations = 0;

				ImGui::SetNextItemWidth(100);
				ImGui::InputInt("Cells Per Generation", &nCellsPerGeneration);
				ImGui::SameLine(); ImGui::SetNextItemWidth(100);
				ImGui::InputInt("Number of Generations", &nGenerations);

				elementaryAutomata->SetNumberOfCellsPerGeneration(nCellsPerGeneration);
				elementaryAutomata->SetNumberOfGenerations(nGenerations);

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

				const std::string rulesetText = "Current Ruleset = " + rulesetReversedString;

				ImGui::Text(rulesetText.c_str());

				if (ImGui::Button("Generate"))
				{
					elementaryAutomata->GenerateElementaryAutomata();
				}

				elementaryAutomata->DrawGrid();
				elementaryAutomata->DrawCells();

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
