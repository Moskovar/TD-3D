#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(std::string filePath)
{
	this->filePath = filePath.c_str();
    // Cr�e une variable pour stocker l'ID de la texture

    // G�n�re une texture et stocke son ID dans la variable texture
    glGenTextures(1, &textureID);

    // Lie la texture � la cible GL_TEXTURE_2D
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Utilise stb_image pour charger l'image de la texture depuis le chemin sp�cifi�
    unsigned char* data = stbi_load(this->filePath, &width, &height, &nrChannels, 0);

    // V�rifie si l'image a �t� charg�e avec succ�s
    if (data)
    {
        printf("||--- Texture loaded ---||\n");
        // D�clare une variable pour stocker le format de l'image
        GLenum format;

        // D�termine le format de l'image en fonction du nombre de canaux
        if (nrChannels == 1)
            format = GL_RED;        // L'image a 1 canal (par exemple, une image en niveaux de gris)
        else if (nrChannels == 3)
            format = GL_RGB;        // L'image a 3 canaux (rouge, vert, bleu)
        else if (nrChannels == 4)
            format = GL_RGBA;       // L'image a 4 canaux (rouge, vert, bleu, alpha)

        //std::cout << "FORMAT: " << format << " : " << GL_RED << " : " << GL_RGB << " : " << GL_RGBA << std::endl;

        // Cr�e une texture en 2D avec les donn�es de l'image
        // Les param�tres sont :
        // - Niveau de d�tail de la texture (0 = base level)
        // - Format interne de la texture
        // - Largeur et hauteur de l'image
        // - Toujours 0 (bordure, doit �tre 0)
        // - Format des pixels de l'image
        // - Type des donn�es des pixels
        // - Pointeur vers les donn�es de l'image
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        // G�n�re des mipmaps pour la texture
        glGenerateMipmap(GL_TEXTURE_2D);

        // D�finit les param�tres de wrapping (enveloppement) pour la coordonn�e S (horizontale)
        // GL_TEXTURE_WRAP_S indique comment la texture est �chantillonn�e lorsqu'une coordonn�e de texture d�passe la plage [0.0, 1.0]
        // GL_REPEAT r�p�te la texture en cas de d�passement (comportement par d�faut)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

        // D�finit les param�tres de wrapping (enveloppement) pour la coordonn�e T (verticale)
        // GL_TEXTURE_WRAP_T indique comment la texture est �chantillonn�e lorsqu'une coordonn�e de texture d�passe la plage [0.0, 1.0]
        // GL_REPEAT r�p�te la texture en cas de d�passement (comportement par d�faut)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // D�finit les param�tres de filtrage pour le minification (r�duction) de la texture
        // GL_TEXTURE_MIN_FILTER indique quel type de filtrage est utilis� lorsque la texture est r�duite
        // GL_LINEAR_MIPMAP_LINEAR utilise le filtrage lin�aire avec les mipmaps pour une transition plus lisse entre les niveaux de d�tail
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        // D�finit les param�tres de filtrage pour la magnification (agrandissement) de la texture
        // GL_TEXTURE_MAG_FILTER indique quel type de filtrage est utilis� lorsque la texture est agrandie
        // GL_LINEAR utilise le filtrage lin�aire, ce qui signifie que la valeur de chaque pixel est interpol�e lin�airement entre les pixels voisins
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        // Affiche un message d'erreur si l'image n'a pas pu �tre charg�e
        std::cout << "Failed to load texture" << std::endl;
    }

    // Lib�re la m�moire utilis�e par les donn�es de l'image
    stbi_image_free(data);
}

Texture::~Texture()
{
    filePath = (char*)"";
}

void Texture::useTexture()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
}
