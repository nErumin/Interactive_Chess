#include "Block.h"

Block::Block() {}
Block::Block(int index, vector<Point2f> points) {
	this->index = index;
	this->points = points;
}

void Block::setPoints(vector<Point2f> points) { this->points = points; }
void Block::setIndex(int index) { this->index = index; }
void Block::setIsInObject(bool flag) { this->isInObject = flag; }

int Block::getIndex() { return this->index; }
vector<Point2f> Block::getPoints() { return this->points; }
bool Block::haveObject() { return this->isInObject; }