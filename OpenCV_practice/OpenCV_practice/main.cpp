#include "ImageProcessor.h"

int main() {
	ImageProcessor ip;
	String title = "test2.jpg";
	vector<Block> blocks = ip.findChessboardBlocks(title);
	vector<Block> objects = ip.findChessObject(blocks, title);

	return 0;
}
