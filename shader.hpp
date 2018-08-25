// We are going to create a shader management class

class shader
{
private:
	GLuint programID;
	std::string loadFile(std::string);
	void compileShader(const char*, GLuint*);
	GLuint createShader(GLuint*, GLuint*);
public:
	void loadShader(std::string, std::string);
	void useShader();
};