extern void MulByElement(uniform const float a[], uniform const float b[], uniform float result[], uniform const size_t count);
extern uniform float Sum(const uniform float array[], uniform const size_t count);

export void DotProduct(uniform const float a[], uniform const float b[], uniform float * uniform result, uniform const size_t count) {
	uniform float * uniform r = uniform new uniform float [count];
	MulByElement(a, b, r, count);
	*result = Sum(r, count);
	delete[] r;
}

