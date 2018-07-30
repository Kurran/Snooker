#include "Renderer.h"

Renderer::Renderer(Window &parent) : OGLRenderer(parent)	{

	glEnable(GL_DEPTH_TEST);
}

Renderer::~Renderer(void)	{
	 
}

void	Renderer::RenderScene() {
	for(vector<RenderObject*>::iterator i = renderObjects.begin(); i != renderObjects.end(); ++i ) {
		Render(*(*i));
	}
}

void	Renderer::Render(const RenderObject &o) {
	modelMatrix = o.GetWorldTransform();

	if(o.GetShader() && o.GetMesh()) {
		GLuint program = o.GetShader()->GetShaderProgram();
		
		glUseProgram(program);

		UpdateShaderMatrices(program);

		o.Draw();
	}

	for(vector<RenderObject*>::const_iterator i = o.GetChildren().begin(); i != o.GetChildren().end(); ++i ) {
		Render(*(*i));
	}
}

void	Renderer::UpdateScene(float msec) {
	for(vector<RenderObject*>::iterator i = renderObjects.begin(); i != renderObjects.end(); ++i ) {
		(*i)->Update(msec);
	}
}







//class SFMLRenderer
//{
//public:
//	SFMLRenderer();
//	~SFMLRenderer(void);
//
//	virtual void RenderScene();
//
//
//	virtual void Render(const RenderObject &o);
//
//	virtual void UpdateScene(float msec);
//
//	void AddRenderObject(RenderObject& r) {
//		renderObjects.push_back(&r);
//	}
//
//	void UpdateShaderMatrices(GLuint program);
//
//	void			SetProjectionMatrix(Matrix4 m) {
//		projMatrix = m;
//	}
//
//	void			SetViewMatrix(Matrix4 m) {
//		viewMatrix = m;
//	}
//
//private:
//	vector<RenderObject*> renderObjects;
//	Matrix4 projMatrix;
//	Matrix4 modelMatrix;
//	Matrix4 viewMatrix;
//	Matrix4 textureMatrix;
//
//	bool	init;			//Did the renderer initialise properly?
//
//	static int		width;
//	static int		height;
//
//};

