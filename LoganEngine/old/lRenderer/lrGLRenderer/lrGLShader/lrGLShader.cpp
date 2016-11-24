
#include <iostream>

#include "lrGLShader.h"

using namespace std;

bool lLoadText(const std::string &filename,std::string &destination)
{
	std::ifstream In;
	In.open(filename.c_str());

	if(!In.is_open())
	{
        #ifdef L_TESTING
            std::cerr << "Couldn't open " << filename << "\n";
		#endif

		return false;
	}

	while(!In.eof())
	{
		std::string Tmp;
		getline(In,Tmp);

		destination += Tmp;
		destination += "\n";
	}

	return true;
}

GLuint lrGLShader::LoadShader(const string &Src,GLenum ShaderType)
{
	GLuint Id;
	Id = glCreateShader(ShaderType);

	const char *C_Src = Src.c_str();

	glShaderSource(Id,1,&C_Src,NULL);
	glCompileShader(Id);

    #ifdef L_TESTING
        char Error[1024];

        glGetShaderInfoLog(Id,1024,NULL,Error);
        cout << "Shader compile status:\n" << Error << endl;
	#endif

	return Id;
}

lrGLShader::lrGLShader()
{

}

lrGLShader::~lrGLShader()
{

}
