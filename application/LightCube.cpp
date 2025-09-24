// Just a sample
#include <Zap.h>

#include "LightCube.h"


LightCube::LightCube() : zap::Mesh(zap::standard_cube::standardcubevertices) {
	SetVertexShaderSource(vertexShaderSource);
	SetFragmentShaderSource(fragmentShaderSource);
	SetAttribPointer(0, 3, 6, 0);
	Finish();
}
void LightCube::MoveUpdate(const zap::SceneCamera& camera, glm::vec3 lightPos, double time)
{
	SetProjection(camera.GetProjection());
	SetView(camera.GetView());
	glm::mat4 model = glm::translate(glm::mat4(1.0f), lightPos);
	UpdateModel(glm::scale(model, glm::vec3(0.2f)));
}
