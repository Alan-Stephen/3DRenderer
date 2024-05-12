#include "Grass.h"

Grass::Grass(int height, int width, glm::vec3 scale, glm::vec3 translate)
{
	Texture ambient = Texture("", true, RGBA(255, 255, 255, 255));
	Texture specular = Texture("", true, RGBA(255, 255, 255, 255));
	Texture diffuse = Texture("objs/grass/grass_billboard.png", false, RGBA(255, 255, 255, 255));

	_mat = Material(ambient, specular, diffuse, 8, 1.0, "GRASS_BILLBOARD");

}
