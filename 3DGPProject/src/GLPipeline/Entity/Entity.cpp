#include "GLPipeline/Entity/Entity.h"
#include "GLPipeline/Entity/Model.h"
#include "GLPipeline/Entity/Texture.h"
#include "GLPipeline/Entity/ShaderProgram.h"
#include "GLPipeline/Entity/Collider/BoxCollider.h"
#include "GLPipeline/Scene/Camera.h"

#include <GL/glew.h>
#include <glm/ext.hpp>

Entity::Entity(const char* _modelPath, const char* _texturePath) :
	m_id(-1),
	m_Transform(),
	m_Model(),
	m_Texture(),
	m_Shader(),
	m_Collider()
{
	printf("Entity constructor\n");

	m_Model = std::make_shared<Model>(_modelPath);
	m_Texture = std::make_shared<Texture>(_texturePath);
	m_Shader = std::make_shared<ShaderProgram>("./resources/shaders/specular/vert.vs", "./resources/shaders/specular/frag.fs");
	m_Collider = std::make_shared<BoxCollider>(&m_Transform, glm::vec3(1.0f, 1.0f, 1.0f));
}

Entity::~Entity()
{

}

void Entity::Update(std::vector<Entity>* _entityList)
{
	printf("Updating entity %d\n", m_id);

	for(int i = 0; i < _entityList->size(); i++)
	{
		if(_entityList->at(i).GetID() != m_id)
		{
			printf("	Colliding with Entity %d\n", _entityList->at(i).GetID());
			m_Collider->CollisionResponse((Collider*)_entityList->at(i).m_Collider.get());
		}
	}
}

void Entity::Draw(Camera* _camera)
{
    glBindVertexArray(m_Model->GetVAO());
	glBindTexture(GL_TEXTURE_2D, m_Texture->GetID());

	glm::mat4 L_modelMatrix = m_Transform.GetModelMatrix();

	m_Shader->Use();
	_camera->Use(m_Shader.get());
	m_Shader->SetUniform("u_Model", L_modelMatrix);

	// Draw shape
	glDrawArrays(GL_TRIANGLES, 0, m_Model->GetVertexCount());

	// Reset the state
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Entity::Move(glm::vec3 _movement)
{
	m_Transform.Translate(_movement);
}

void Entity::Rotate(glm::vec3 _rotation)
{
	m_Transform.Rotate(_rotation);
}

void Entity::Scale(glm::vec3 _scale)
{
	m_Transform.Scale(_scale);
}

void Entity::SetID(int _id)
{
	printf("ID set to Entity as %d\n", _id);
	m_id = _id;
}

int Entity::GetID()
{
	return m_id;
}