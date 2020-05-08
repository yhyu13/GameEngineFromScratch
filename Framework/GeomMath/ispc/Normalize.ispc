export void Normalize(uniform float v[], uniform float sqlength, const uniform size_t count)
{
    sqlength = rsqrt(sqlength);
    foreach (index = 0 ... count) {
        v[index] *= sqlength;
    }
}
