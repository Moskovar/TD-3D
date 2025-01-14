#include "UIManager.h"

UIManager::UIManager(Player* player, std::map<char, bool>* keyPressed)
{
	this->ui		 = ui;
	this->player	 = player;
    this->keyPressed = keyPressed;

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


    //--- Récupération des raccourcis ---//
    for (SpellBar* bar : ui.getBars())
    {
        for (Button& button : bar->getButtons())
        {
            if (button.shortcut == 0) continue;

            shortcuts[&button.shortcut] = button.getElement();
        }
    }
}

void UIManager::setSpellButton(int i, short button, Spell* spell)
{
    char* shortcut = ui.setSpellButton(i, button, spell);//si l'attribution échoue on récupère nullptr

    std::cout << "SHORTCUT: " << shortcut << " : " << *shortcut << std::endl;

    if (!shortcut) return;//si on a récupéré nullptr c'est que l'attribution a échoué

    shortcuts[shortcut] = spell;
}

void UIManager::renderUI()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Démarrer une nouvelle fenêtre ImGui
    ImGui::Begin("UI", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove);

    // Positionner la fenêtre (par exemple, en bas au centre de la fenêtre principale)
    ImGui::SetWindowPos(ImVec2(0, 0));  // Ajuste selon la résolution de ta fenêtre
    ImGui::SetWindowSize(ImVec2(1920, 1080)); // Largeur de la barre et hauteur minimale //moche le + 10

    renderSpellBar(ui.getBar(0));

    float progress = (float)player->getCharacter()->getResources() / (float)player->getCharacter()->getMaxResources(); // Progression en pourcentage
    std::string overlay = std::to_string(player->getCharacter()->getResources()) + " / " + std::to_string(player->getCharacter()->getMaxResources());

    drawProgressBar(100, 50, progress, ImVec4(0.2f, 0.2f, 1.0f, 1.0f), overlay);

    // Fin de la fenêtre
    ImGui::End();


    // Fin du rendu ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UIManager::renderSpellBar(SpellBar* bar)
{
    if (!bar) return;

    ImGui::SetCursorScreenPos(ImVec2(*(bar->x), *(bar->y))); // Déplacer le curseur

    std::vector<Button> buttons = bar->getButtons();
    size_t size = buttons.size();
    if (size == 0) return;

    std::string name = bar->getName();
    short buttons_width = bar->buttons_width, buttons_height = bar->buttons_height, offsetX = bar->offsetX, offsetY = bar->buttons_height;

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
                drawTextureButton(spell, buttons_width, buttons_height);
            }
            else//si CD ou oom
            {
                drawTextureButton(spell, buttons_width, buttons_height, 0.5);

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
}

//Fait la liaison entre les raccourcis des barres de sorts et les entrées utilisateur dans la fonction main
void UIManager::processKeyboard()
{
    for (auto it = shortcuts.begin(); it != shortcuts.end(); ++it)
    {
        char shortcut = *(it->first);
        //std::cout << "SHORTCUT: " << shortcut << " : " << (*keyPressed)[shortcut] << std::endl;
        if ((*keyPressed)[shortcut])
        {
            player->useSpell(it->second->getElementID());
            (*keyPressed)[shortcut] = false;//on met à false pour pas faire plusieurs fois (car dans la boucle main)
        }
    }
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

void UIManager::drawProgressBar(int x, int y, GLfloat progress, ImVec4 color, std::string overlay)
{
    ImVec2 progressBarPos = ImVec2(100, 50);  // Position de la barre
    ImVec2 progressBarSize = ImVec2(200, 20); // Taille de la barre
    ImGui::SetCursorScreenPos(progressBarPos); // Positionner la barre

    // Calculer la taille du texte pour centrer correctement
    ImVec2 textSize = ImGui::CalcTextSize(overlay.c_str());

    // Calculer la position pour centrer le texte
    ImVec2 textPos = ImVec2(progressBarPos.x + (progressBarSize.x - textSize.x) * 0.5f, progressBarPos.y + (progressBarSize.y - textSize.y) * 0.5f);

    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, color);
    ImGui::ProgressBar(progress, progressBarSize, ""); // Dessiner la barre sans texte

    // Restaure la position du curseur à la position du texte
    ImGui::SetCursorScreenPos(textPos);

    // Dessiner le texte centré sur la barre
    ImGui::Text("%s", overlay.c_str());

    // Restaure la couleur originale
    ImGui::PopStyleColor();
}

void UIManager::drawTextureButton(Element* element, short buttons_width, short buttons_height, GLfloat opacity)
{
    if (!element || !element->getTexture()) return;

    // Couleur de fond (si besoin) et de teinte
    ImVec4 backgroundColor = ImVec4(0.0f, 0.0f, 0.0f, 0.0f); // Transparent
    ImVec4 tintColor = ImVec4(1.0f, 1.0f, 1.0f, opacity); // Couleur blanche avec opacité

    if (ImGui::ImageButton("Slot", (ImTextureID)(intptr_t)element->getTexture()->getTextureID(), ImVec2(buttons_width, buttons_height), ImVec2(0, 0), ImVec2(1, 1), backgroundColor, tintColor))
    {
        // Action déclenchée en cliquant sur le bouton
        player->useSpell(element->getElementID());
        std::cout << "Slot" << " clicked!" << std::endl;
    }
}
