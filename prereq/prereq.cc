struct Result {
    float avg[3];
};

/*
This is the function you need to implement. Quick reference:
- x coordinates: 0 <= x < nx
- y coordinates: 0 <= y < ny
- horizontal position: 0 <= x0 < x1 <= nx
- vertical position: 0 <= y0 < y1 <= ny
- color components: 0 <= c < 3
- input: data[c + 3 * x + 3 * nx * y]
- output: avg[c]
*/
Result calculate(int ny, int nx, const float *data, int y0, int x0, int y1, int x1) {
    Result result{{0.0f, 0.0f, 0.0f}};
    
    // Calculate the number of pixels in the rectangle
    int width = x1 - x0;
    int height = y1 - y0;
    int total_pixels = width * height;
    
    // Use double precision for all calculations
    double sum[3] = {0.0, 0.0, 0.0};
    
    // Optimize memory access by calculating row base indices
    for (int y = y0; y < y1; y++) {
        // Calculate base index for this row
        int row_base = 3 * nx * y;
        
        for (int x = x0; x < x1; x++) {
            // Calculate pixel base index
            int pixel_base = row_base + 3 * x;
            
            // Process all three color components at once
            sum[0] += data[pixel_base];     // Red
            sum[1] += data[pixel_base + 1]; // Green
            sum[2] += data[pixel_base + 2]; // Blue
        }
    }
    
    // Calculate averages and convert back to single precision
    double inv_total = 1.0 / total_pixels;
    result.avg[0] = (float)(sum[0] * inv_total);
    result.avg[1] = (float)(sum[1] * inv_total);
    result.avg[2] = (float)(sum[2] * inv_total);
    
    return result;
}
