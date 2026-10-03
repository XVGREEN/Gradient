constexpr bool inRect(float x, float y, float rx, float ry, float rw, float rh) {
	return ((x > rx && y > ry) && (x < rw && y < rh));
}