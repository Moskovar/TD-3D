#include "UI.h"

UI::UI()
{
	bars.push_back(new SpellBar("Spellbar", 50, 50));
	bars.push_back(new SpellBar("BuildBar", 50, 50, 25));
}

UI::~UI()
{
	//if (spellBar)
	//{
	//	delete spellBar;
	//	spellBar = nullptr;
	//}

	//if (buildBar)
	//{
	//	delete buildBar;
	//	buildBar = nullptr;
	//}

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

void UI::setSpellButton(int i, short button, Element* element)
{
	if (i < 0 || i >= bars.size()) return;
	if (!bars[i]) return;

	bars[i]->setElement(button, element);
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
