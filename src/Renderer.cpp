#include "Renderer.h"
#include <iostream>



void GLClearError()
{
    //while we have an error back from openGL call glGetError
    while (glGetError() != GL_NO_ERROR);
}



bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError()) // this will run as long as glGetError doesn't return 0 (or is not false)
    {
        //while we have errors print out the error code
        std::cout << "[OpenGL Error] (" << error << ") : " << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}



void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    shader.Bind();
    va.Bind();                                                                                     
    ib.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}