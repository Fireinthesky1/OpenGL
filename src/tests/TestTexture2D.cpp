#include "TestTexture2D.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"



namespace test {

	TestTexture2D::TestTexture2D()
		: m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)), 
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0)
	{
		float positions[] =																	//create a vertex array (this one holds positions and texture coordinates)
		{
			-50.0f, -50.0f,  0.0f,  0.0f,	//0
			 50.0f, -50.0f,  1.0f,  0.0f,	//1
			 50.0f,  50.0f,  1.0f,  1.0f,	//2
			-50.0f,  50.0f,  0.0f,  1.0f	//3
		};

		unsigned int indices[] =															//create an index array
		{
			0, 1, 2,
			2, 3, 0
		};

		GLCall(glEnable(GL_BLEND));															//enable blending
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));							//set the blending equation


		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * (sizeof(float)));//create the vertex buffer using the vertex array
		VertexBufferLayout layout;															//creat a layout
		layout.Push<float>(2);																//define the layout
		layout.Push<float>(2);																//define the layout
			
		m_VertexArrayObject = std::make_unique<VertexArray>();								//create a vertex array object
		m_VertexArrayObject->AddBuffer(*m_VertexBuffer, layout);							//add the vertexbuffer and the layout to the vertex array object
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);							//create the index buffer using the index array

		m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");					//load in the shader
		m_Shader->Bind();																	//bind the shader
		m_Shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);							//set the color uniform

		m_Texture = std::make_unique<Texture>("res/textures/wood.png");						//load in the texture
		m_Shader->SetUniform1i("u_Texture", 0);												//set the texture uniform

	}

	TestTexture2D::~TestTexture2D()
	{
	}

	void TestTexture2D::OnUpdate(float deltaTime)
	{
	}

	void TestTexture2D::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));										//define the color of a cleared screen
		GLCall(glClear(GL_COLOR_BUFFER_BIT));												//clear the screen

		Renderer renderer;																	//instantiate the renderer

		m_Texture->Bind();																	//bind the texture

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);				//set the model matrix
			glm::mat4 mvp = m_Proj * m_View * model;										//multiply the model matrix the view matrix and the projection matrix
			m_Shader->Bind();																//bind the shader
			m_Shader->SetUniformMat4f("u_MVP", mvp);										//set the model view projection uniform
			renderer.Draw(*m_VertexArrayObject, *m_IndexBuffer, *m_Shader);					//draw to the screen the first object
		}

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VertexArrayObject, *m_IndexBuffer, *m_Shader);
		}
	}

	void TestTexture2D::OnImGuiRender()
	{
		{
			ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 960.0f);
			ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 960.0f);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}
	}
}
