#include "UIManager.h"

UIManager::UIManager(Player* player)
{
	this->ui		= ui;
	this->player	= player;

	//std::map<int, Spell*> spells_model = player->getSpellsModel();

	std::cout << "UIManager: Nombre de spells à set: " << player->getSpellsModel().size() << std::endl;

	int i = 0;

	//--- On parcourt tous les sorts du joueur (spellsModel) et on les assigne à la barre de sort ---//
	for (auto it = player->getSpellsModel().begin(); it != player->getSpellsModel().end(); ++it)
	{
		ui.setSpellButton(0, i, it->second);
		++i;
	}

	i = 0;

	//--- On parcourt tous les sorts du joueur (spellsModel) et on les assigne à la barre de sort ---//
	for (auto it = player->getTowersModel().begin(); it != player->getTowersModel().end(); ++it)
	{
		ui.setSpellButton(1, i, it->second);
		++i;
	}
}

void UIManager::setSpellButton(int i, short button, Spell* spell)
{
	ui.setSpellButton(i, button, spell);
}

void UIManager::renderUI()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    renderSpellBar();

    // Créer une fenêtre invisible pour éviter que la progress bar ne soit dans la fenêtre débogage
    ImGui::SetNextWindowSize(ImVec2(0, 0));  // Taille zéro pour rendre la fenêtre invisible
    ImGui::SetNextWindowPos(ImVec2(100, 50)); // Position où vous voulez dessiner la progress bar
    ImGui::Begin("InvisibleWindow", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoInputs);

    // Dessiner la progress bar
    float progress = 0.75f; // Progression (0.0 à 1.0)
    ImVec2 progressBarSize = ImVec2(200, 20); // Taille de la barre
    ImGui::ProgressBar(progress, progressBarSize, nullptr); // Dessiner la barre sans texte

    ImGui::End(); // Terminer la fenêtre

    // Fin du rendu ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UIManager::renderSpellBar()
{
    SpellBar* bar = ui.getBar(0);
    if (!bar) return;

    std::vector<Button> buttons = bar->getButtons();
    size_t size = buttons.size();
    if (size == 0) return;

    std::string name = bar->getName();
    short x = bar->x, y = bar->y, buttons_width = bar->buttons_width, buttons_height = bar->buttons_height, offsetX = bar->offsetX, offsetY = bar->buttons_height;

    // Démarrer une nouvelle fenêtre ImGui
    ImGui::Begin(name.data(), nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove);

    // Positionner la fenêtre (par exemple, en bas au centre de la fenêtre principale)
    ImGui::SetWindowPos(ImVec2(x, y));  // Ajuste selon la résolution de ta fenêtre
    ImGui::SetWindowSize(ImVec2(size * (buttons_width + offsetX) + 10, buttons_height + offsetY)); // Largeur de la barre et hauteur minimale //moche le + 10

    ImGui::PushStyleColor(ImGuiCol_Button       , ImVec4(0.3f, 0.3f, 0.3f, 0.3f)); // Couleur normale
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 0.6f)); // Au survol
    ImGui::PushStyleColor(ImGuiCol_ButtonActive , ImVec4(0.3f, 0.3f, 0.3f, 0.9f)); // Quand le bouton est cliqué

    // Afficher x emplacements sous forme de boutons
    for (unsigned int i = 0; i < size; i++)
    {
        Texture* texture = buttons[i].getTexture();
        //if (ImGui::Button(("Slot " + std::to_string(i + 1)).c_str(), ImVec2(buttons_width, buttons_height)))
        if (texture)
        {   //Si on a une texture c'est qu'on a un Element
            Element* element = buttons[i].getElement();

            if (!dynamic_cast<Spell*>(element)) continue;

            Spell* spell = dynamic_cast<Spell*>(element);

            if(!spell) continue;
            
            if (!spell->isCd() && spell->getCost() < player->getCharacter()->getResources())//si sort pas en CD et assez de mana
            {
                drawTextureButton(texture, buttons_width, buttons_height);
            }
            else//si CD ou oom
            {
                drawTextureButton(texture, buttons_width, buttons_height, 0.5);

                //si cd on écrit le timer
                if (spell->isCd())   drawTimer(spell->getCdLeft());
            }
        }
        else 
        {
            drawEmptyButton("Slot " + std::to_string(i + 1), buttons_width, buttons_height);
        }

        if (buttons[i].showShortcut && buttons[i].shortcut != 0)
        {
            drawShortcut(buttons[i].shortcut);
        }

        // Ajouter un espace entre les boutons sauf après le dernier
        if (i < size) ImGui::SameLine(0, offsetX);
    }


    // Rétablir la couleur par défaut
    ImGui::PopStyleColor(3); // Retire les trois couleurs ajoutées

    // Fin de la fenêtre
    ImGui::End();
}

void UIManager::drawShortcut(char key)
{
    // Dessiner le texte (par exemple, le raccourci clavier) par-dessus la texture
    ImVec2 buttonPos = ImGui::GetItemRectMin();
    ImVec2 buttonSize = ImGui::GetItemRectSize();
    ImVec2 textPos = ImVec2(buttonPos.x + buttonSize.x - 15.0f, buttonPos.y + 2);  // Centrer le texte

    std::string shortcut = std::string(1, key);

    // Utiliser la fonction de dessin d'ImGui pour ajouter du texte par-dessus le bouton
    ImGui::GetWindowDrawList()->AddText(textPos, IM_COL32(255, 255, 255, 255), shortcut.data());  // Affiche raccourci en blanc
}

void UIManager::drawEmptyButton(std::string name, short buttons_width, short buttons_height)
{
    if (ImGui::Button(name.c_str(), ImVec2(buttons_width, buttons_height)))
    {
        // Action déclenchée en cliquant sur le bouton
        std::cout << name << " clicked!" << std::endl;
    }
}

void UIManager::drawTimer(short timeLeft)
{
    // Dessiner le texte (par exemple, le raccourci clavier) par-dessus la texture
    ImVec2 buttonPos = ImGui::GetItemRectMin();
    ImVec2 buttonSize = ImGui::GetItemRectSize();
    ImVec2 textPos = ImVec2(buttonPos.x + buttonSize.x / 2.2, buttonPos.y + buttonSize.y / 2.5);  // Centrer le texte

    std::string cdLeft = std::to_string((timeLeft / 1000) + 1);

    // Utiliser la fonction de dessin d'ImGui pour ajouter du texte par-dessus le bouton
    ImGui::GetWindowDrawList()->AddText(textPos, IM_COL32(255, 255, 255, 255), cdLeft.data());  // Affiche raccourci en blanc
}

void UIManager::drawTextureButton(Texture* texture, short buttons_width, short buttons_height, GLfloat opacity)
{
    if (!texture) return;

    // Couleur de fond (si besoin) et de teinte
    ImVec4 backgroundColor = ImVec4(0.0f, 0.0f, 0.0f, 0.0f); // Transparent
    ImVec4 tintColor = ImVec4(1.0f, 1.0f, 1.0f, opacity); // Couleur blanche avec opacité

    if (ImGui::ImageButton("Slot", (ImTextureID)(intptr_t)texture->getTextureID(), ImVec2(buttons_width, buttons_height), ImVec2(0, 0), ImVec2(1, 1), backgroundColor, tintColor))
    {
        // Action déclenchée en cliquant sur le bouton
        std::cout << "Slot" << " clicked!" << std::endl;
    }
}
