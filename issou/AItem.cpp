#include "AItem.h"


AItem::AItem(const Transform& trans, const Shader &pr)
	: transform(trans), shader(pr), visible(true), model(0)
{
	//transform.rotationAxis = glm::vec3(0);
}


AItem::~AItem() {}


void AItem::setup() { }


void AItem::perform_draw() { }


void AItem::apply_transform() {

	float totalRotation = transform.rotation.degree;
	glm::vec3 tmpRotAx = transform.position - transform.rotationAxis;

	m_model.glTranslate(transform.position.x, transform.position.y, transform.position.z);

	if (transform.rotation.constantRotation) {
		transform.rotation.currentRotation += 1;
		if (transform.rotation.currentRotation >= 360 || transform.rotation.currentRotation <= -360) { transform.rotation.currentRotation = 0; }
		totalRotation += transform.rotation.currentRotation;
	}
	if (transform.rotationAxis != glm::vec3(0)) {
		m_model.glTranslate(-tmpRotAx.x, -tmpRotAx.y, -tmpRotAx.z);
		m_model.glRotate(totalRotation, transform.rotation.x, transform.rotation.y, transform.rotation.z);
		m_model.glTranslate(tmpRotAx.x, tmpRotAx.y, tmpRotAx.z);
	} else {
		m_model.glRotate(totalRotation, transform.rotation.x, transform.rotation.y, transform.rotation.z);
	}

	m_model.glScale(transform.scale.x, transform.scale.y, transform.scale.z);
}


void AItem::setModelView(const glm::mat4& view)
{

	model = m_model.getMatrix();

	glm::mat4 MVP = *shader.projection * *shader.view * model;
	glUniformMatrix4fv(shader.program->uniform("MVP"), 1, GL_FALSE, glm::value_ptr(MVP));

	glm::mat4 modelview = view * model;
	glm::mat4 inverseModelView = glm::inverse(modelview);
	glm::mat3 normalMatrix = glm::mat3(glm::transpose(inverseModelView));
}

void AItem::draw(const glm::mat4 &view)
{
	if (visible) {
		m_model.glPushMatrix();

		glBindVertexArray(this->vaoHandle);

		this->apply_transform();
		this->setModelView(view);
		this->perform_draw();

		m_model.glPopMatrix();
		
		glBindVertexArray(0);

	}
}

void AItem::setVisible(const bool vis) { visible = vis; }

