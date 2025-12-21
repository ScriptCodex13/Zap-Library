
#include "Cube.h"


Cube :: Cube() : zap::Mesh (zap::standard_cube::standardcubevertices) {
	SetVertexShaderSource(vertexShaderSourcecube);
	SetFragmentShaderSource(fragmentShaderSourcecube);

	SetAttribPointer(0, 3, 6, 0);
	SetAttribPointer(1, 3, 6, 3);

	Finish();

	object_color_location = glGetUniformLocation(GetProgram(), "objectColor");
	light_color_location = glGetUniformLocation(GetProgram(), "lightColor");
	light_position_location = glGetUniformLocation(GetProgram(), "lightPos");
	view_position_location = glGetUniformLocation(GetProgram(), "viewPos");
}
void Cube::setColorUniform3f(float r, float g, float b) const {
	glUniform3f(object_color_location, r, g, b);
}
void Cube::setLightColorUniform3f(float r, float g, float b)  const {
	glUniform3f(light_color_location, r, g, b);
}
void Cube::setLightPositionUniform3fv(glm::vec3 pos)  const {
	glUniform3fv(light_position_location, 1, &pos[0]);
}
void Cube::setViewPositionUniform3fv(glm::vec3 pos)  const {
	glUniform3fv(view_position_location, 1, &pos[0]);
}
void Cube::MoveUpdate(const zap::SceneCamera& camera, glm::vec3 lightPos, double time)
{
	setLightPositionUniform3fv(lightPos);
	setViewPositionUniform3fv(camera.GetPosition());
	UpdateProjection(camera.GetProjection());
	UpdateView(camera.GetView());
	UpdateModel(glm::rotate(glm::mat4(1.0), (float)time, glm::vec3(0.0f, 1.0f, 0.0f)));

}
