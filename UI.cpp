#include "UI.h"

UI::UI()
{
	spellBar = new SpellBar(50, 50);
}

UI::~UI()
{
	if (spellBar)
	{
		delete spellBar;
		spellBar = nullptr;
	}

    std::cout << "<<--- UI cleared --->>" << std::endl;
}

void UI::render()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Appeler ta barre de sorts
    spellBar->render();

    // Fin du rendu ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
