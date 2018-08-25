// We are writing a class for our OpenGL renderer
// The class acts as a container for our renderer so that we can easily initialize OpenGL
// The renderer has a Node that can talk to the data bus
// It has an event handler
// It has an init function
// It has an update function
// Functions are definied in renderer.cpp

class renderer
{
private:
	node* Node;
	GLFWwindow* Window;
	std::string curEvent;
	command Command;
	void event();
public:
	void init(bus*);
	void update();
};