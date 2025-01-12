#include "SpellBar.h"

SpellBar::SpellBar(short buttons_width, short buttons_height)
{
    this->buttons_width     = buttons_width;
    this->buttons_height    = buttons_height;

	for (short x = 0; x < 5; x++)
	{
		buttons.push_back(Button(0, true));
	}

    buttons[0].shortcut = 'A';
    buttons[1].shortcut = 'Z';
}

void SpellBar::render()
{
    size_t size = buttons.size();
    if (size == 0) return;

    // Démarrer une nouvelle fenêtre ImGui
    ImGui::Begin("Spell Bar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove);

    // Positionner la fenêtre (par exemple, en bas au centre de la fenêtre principale)
    ImGui::SetWindowPos(ImVec2(x, y));  // Ajuste selon la résolution de ta fenêtre
    ImGui::SetWindowSize(ImVec2(size * (buttons_width + offsetX) + 10 , buttons_height + offsetY)); // Largeur de la barre et hauteur minimale //moche le + 10

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.3f, 0.3f, 0.3f)); // Couleur normale
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 0.6f)); // Au survol
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.3f, 0.3f, 0.9f)); // Quand le bouton est cliqué

    // Afficher 5 emplacements sous forme de boutons
    for (unsigned int i = 0; i < size; i++)
    {
        //if (ImGui::Button(("Slot " + std::to_string(i + 1)).c_str(), ImVec2(buttons_width, buttons_height)))
        if (i == 0)
        {
            if (ImGui::ImageButton("Slot", (ImTextureID)(intptr_t)text->getTextureID(), ImVec2(buttons_width, buttons_height)))
            {
                // Action déclenchée en cliquant sur le bouton
                std::cout << "Slot " << (i + 1) << " clicked!" << std::endl;
            }
        }
        else if (ImGui::Button(" ", ImVec2(buttons_width, buttons_height)))
        {
            // Action déclenchée en cliquant sur le bouton
            std::cout << "Slot " << (i + 1) << " clicked!" << std::endl;
        }

        if (buttons[i].showShortcut && buttons[i].shortcut != 0)
        {
            // Dessiner le texte (par exemple, le raccourci clavier) par-dessus la texture
            ImVec2 buttonPos = ImGui::GetItemRectMin();
            ImVec2 buttonSize = ImGui::GetItemRectSize();
            ImVec2 textPos = ImVec2(buttonPos.x + buttonSize.x - 15.0f, buttonPos.y + 2);  // Centrer le texte

            std::string shortcut = std::string(1, buttons[i].shortcut);

            // Utiliser la fonction de dessin d'ImGui pour ajouter du texte par-dessus le bouton
            ImGui::GetWindowDrawList()->AddText(textPos, IM_COL32(255, 255, 255, 255), shortcut.data());  // Affiche raccourci en blanc
        }

        // Ajouter un espace entre les boutons sauf après le dernier
        if (i < size) ImGui::SameLine(0, offsetX);
    }


    // Rétablir la couleur par défaut
    ImGui::PopStyleColor(3); // Retire les trois couleurs ajoutées

    // Fin de la fenêtre
    ImGui::End();
}
