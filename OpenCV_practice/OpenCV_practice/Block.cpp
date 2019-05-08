#include "Block.h"

Block::Block() {}
Block::Block(int index, vector<Point2f> points) {
	this->index = index;
	this->points = points;
}
Block::Block(int index, vector<Point2f> points, bool flag) {
	this->index = index;
	this->points = points;
	this->isInObject = flag;
}

void Block::setPoints(vector<Point2f> points) { this->points = points; }
void Block::setIndex(int index) { this->index = index; }
void Block::setIsInObject(bool flag) { this->isInObject = flag; }

int Block::getIndex() { return this->index; }
vector<Point2f> Block::getPoints() { return this->points; }
bool Block::isHaveObject() { return this->isInObject; }