
#include "gtpch.h"
#include <glad/glad.h>
void GLClearError()
{
	int count = 0;
	while (glGetError() != GL_NO_ERROR)
	{
		count++;
		if(count>50) GT_CORE_ASSERT(-1,"OpenGL Keep Clearing Errors!!!");
	}
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		/*std::cout << "[OpenGL Error] (" << error << "): " << function << "\n"
			<< file << ": line of " << line
			<< std::endl;*/
		GT_CORE_ERROR("[OpenGL Error] ({0}): {1}\n{2}: line of {3}", error, function, file, line);
		return false;
	}
	return true;
}