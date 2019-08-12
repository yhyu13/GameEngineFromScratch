uniform float Sum(const uniform float array[], uniform const size_t count) {
    float sum = 0;
    foreach (index = 0 ... count)
        sum += array[index];
    return reduce_add(sum);
}