#pragma once
#include "AItem.h"
class LineSegment :
	public AItem
{
public:
	LineSegment(const Transform& trans, const Shader &pr, const glm::vec4& st, const glm::vec4& en);
	~LineSegment();

	void setPoints(const glm::vec4& start, const glm::vec4& end);
	void setColor(const glm::vec3& col);

private:

	glm::vec4 start;
	glm::vec4 end;
	glm::vec3 color;

	void update();
	void setup() override;
	void perform_draw() override;
};

