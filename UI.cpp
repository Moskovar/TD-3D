#include "UI.h"

UI::UI()
{
	//--- SpellBar 1 ---//
	bars.push_back(new SpellBar("Spellbar", 50, 50, 800, 925));
	bars[0]->setShortcut(0, GLFW_KEY_Q);

	//--- BuildBar 1 ---//
	bars.push_back(new SpellBar("BuildBar", 50, 50, 800, 1000));
}

UI::~UI()
{
	for (SpellBar* sp : bars)
	{
		if (sp)
		{
			delete sp;
			sp = nullptr;
		}
	}

    std::cout << "<<--- UI cleared --->>" << std::endl;
}

SpellBar* UI::getBar(int i)
{
	if (i < 0 || i >= bars.size()) return nullptr; 
	return bars[i];
}

char* UI::setSpellButton(int i, short button, Element* element)
{
	if (i < 0 || i >= bars.size()) return nullptr;
	if (!bars[i]) return nullptr;

	return bars[i]->setElement(button, element);
}

void UI::setShortcut(short i_bar, short i, char shortcut)
{
	if (i_bar < 0 || i_bar >= bars.size()) return;

	bars[i_bar]->setShortcut(i, shortcut);
}

void UI::render()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

 //   spellBar->render();
	//buildBar->render();

	bars[0]->render();

    // Fin du rendu ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
