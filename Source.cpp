#include "imgui.h"
#include "imgui-SFML.h"
#include "util/Obf.hpp"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>

#include "menu/Fonts.hpp"
#include "menu/Menu.hpp"
#include "menu/Console.hpp"
#include "ModuleManager.hpp"

#include <Windows.h>
#include "menu/imgui_notify.h"

#include <windows.h>
#include "cheatfile/cheat.hpp"

bool show_demo_window = true;

void keyCheck() {
	if (GetAsyncKeyState(VK_INSERT) & 1) {
		Menu::isGUIVisible = !Menu::isGUIVisible;
	}

	for (Module* mod : ModuleManager::i().modules) {
		if (GetAsyncKeyState(mod->getKey()) & 1) {
			mod->toggle();
			ImGuiToast toast(ImGuiToastType_Info, 400);
			toast.set_title((obf("Toggled ") + mod->getName()).c_str());
			ImGui::InsertNotification(toast);
		}
	}
}

void runGraphicsInterface() {
	auto desktopMode = sf::VideoMode::getDesktopMode();
	double screenWidth3 = GetSystemMetrics(SM_CXSCREEN);
	// 获取屏幕高度
	double screenHeight3 = GetSystemMetrics(SM_CYSCREEN);
	// 创建一个无边框的窗口，尺寸等于屏幕分辨率
	sf::RenderWindow window(sf::VideoMode(screenWidth3, screenHeight3), "TheFinals", sf::Style::None);
	//std::cout << "desktopMode" << desktopMode.width << desktopMode.height << std::endl;
	window.setFramerateLimit(144);
	ImGui::SFML::Init(window);
	//cheat();
	// Setup our Menu Theme and font
	Menu::loadTheme();

	// 其他初始化代码...

	sf::Clock deltaClock;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(window, event);

			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		ImGui::SFML::Update(window, deltaClock.restart());
		keyCheck();

		window.clear(sf::Color::Black);

		//sf::RectangleShape rectangle(sf::Vector2f(100.f, 50.f));
		//rectangle.setFillColor(sf::Color::Green);
		//rectangle.setPosition(200, 200);
		//window.draw(rectangle);
		// imgui render here
		cheat();
		Menu::render();
		if (Menu::isGUIVisible)
			Console::i().render();

		// render Notifications
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(43.f / 255.f, 43.f / 255.f, 43.f / 255.f, 100.f / 255.f));
		ImGui::RenderNotifications();
		ImGui::PopStyleVar(1);
		ImGui::PopStyleColor(1);

		ImGui::SFML::Render(window);

		window.display();
	}

	ImGui::SFML::Shutdown();
}

int main() {
	// 可以在这里执行其他初始化或命令行相关的操作

	//runGraphicsInterface(); // 运行图形界面

	// 在图形界面关闭后执行的代码
	// ...

	   // std::thread dataThread(dma._DMA());
	dma._DMA();
	//cheat();
	//getAllDataloop();

	if (!kmboxx.open(PortNumber, 115200)) {
		std::cout << "can not connect to kmbox " << PortNumber << std::endl;
	}
	else {
		std::cout << "successful connect kmbox " << PortNumber << std::endl;

	}

	//std::thread dataThread(getAllDataloop);
	//std::thread matrixThread(getMatrix);
	//std::thread getkeystate(keydown_Loop);
	//std::thread getAim(Aimbot);
	//std::thread guiThread(guiLoop);

	std::thread t1(getAllDataloop);
	t1.detach();
	std::thread t2(getMatrix);
	t2.detach();
	std::thread t3(keydown_Loop);
	t3.detach();
	std::thread t4(Aimbot);
	t4.detach();
	//std::thread t5(gettingplayername);
	//t5.detach();
	std::thread t6(runGraphicsInterface);
	t6.join();
	//std::thread t6(cheat);
	//t6.join();

	return 0;
}