#pragma once

class Test
{
public:
	virtual void Model(glm::mat4* model) = 0;
	virtual std::vector<OldObject> ParseObj() = 0;
};

class Test0 : public Test
{
public:
	void Model(glm::mat4* model)
	{
	}
	virtual std::vector<OldObject> ParseObj()
	{
		std::vector<OldObject> objs;
		return objs;
	}
};

class Test1 : public Test
{
public:
	void Model(glm::mat4* model)
	{
		(*model) = glm::rotate((*model), (float)glfwGetTime() / 2, glm::vec3(0.f, 1.f, 0.f));
	}
	virtual std::vector<OldObject> ParseObj()
	{
		std::vector<OldObject> objs;
		obj_parse("objs/bird/textured_quad.obj", &objs);
		return objs;
	}
};


class Test2 : public Test
{
public:
	void Model(glm::mat4* model)
	{
		(*model) = glm::scale((*model), glm::vec3(20.5f, 20.5f, 20.5f));
	}
	virtual std::vector<OldObject> ParseObj()
	{
		std::vector<OldObject> objs;
		obj_parse("objs/doughnut2/doughnut2.obj", &objs);
		return objs;
	}
};
