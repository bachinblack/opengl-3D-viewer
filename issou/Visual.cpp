#include "Visual.h"


Visual::~Visual() { }

glm::vec3 Visual::getColor(void) const { return color; }
void Visual::setColor(const glm::vec3& value) { color = value; }

float Visual::getShininess(void) const { return shininess; }
void Visual::setShininess(const float value) { shininess = value; }

float Visual::getAlpha(void) const { return alpha; }
void Visual::setAlpha(const float value) { alpha = value; }