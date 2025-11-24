#include <cmath>

/*
This is the function you need to implement. Quick reference:
- input rows: 0 <= y < ny
- input columns: 0 <= x < nx
- element at row y and column x is stored in data[x + y*nx]
- correlation between rows i and row j has to be stored in result[i + j*ny]
- only parts with 0 <= j <= i < ny need to be filled
*/
void correlate(int ny, int nx, const float *data, float *result) {
    // Pre-calculate all row means and standard deviations using double precision
    double *means = new double[ny];
    double *std_devs = new double[ny];
    
    // Calculate means for all rows with loop unrolling for ILP
    for (int i = 0; i < ny; i++) {
        double sum1 = 0.0, sum2 = 0.0, sum3 = 0.0, sum4 = 0.0;
        int x = 0;
        
        // Unroll by 4 to expose instruction-level parallelism
        for (; x + 3 < nx; x += 4) {
            sum1 += (double)data[x + i * nx];
            sum2 += (double)data[x + 1 + i * nx];
            sum3 += (double)data[x + 2 + i * nx];
            sum4 += (double)data[x + 3 + i * nx];
        }
        
        // Handle remaining elements
        double sum_remainder = 0.0;
        for (; x < nx; x++) {
            sum_remainder += (double)data[x + i * nx];
        }
        
        means[i] = (sum1 + sum2 + sum3 + sum4 + sum_remainder) / nx;
    }
    
    // Calculate standard deviations for all rows with loop unrolling
    for (int i = 0; i < ny; i++) {
        double sum_sq1 = 0.0, sum_sq2 = 0.0, sum_sq3 = 0.0, sum_sq4 = 0.0;
        double mean = means[i];
        int x = 0;
        
        // Unroll by 4 to expose instruction-level parallelism
        for (; x + 3 < nx; x += 4) {
            double diff1 = (double)data[x + i * nx] - mean;
            double diff2 = (double)data[x + 1 + i * nx] - mean;
            double diff3 = (double)data[x + 2 + i * nx] - mean;
            double diff4 = (double)data[x + 3 + i * nx] - mean;
            
            sum_sq1 += diff1 * diff1;
            sum_sq2 += diff2 * diff2;
            sum_sq3 += diff3 * diff3;
            sum_sq4 += diff4 * diff4;
        }
        
        // Handle remaining elements
        double sum_sq_remainder = 0.0;
        for (; x < nx; x++) {
            double diff = (double)data[x + i * nx] - mean;
            sum_sq_remainder += diff * diff;
        }
        
        std_devs[i] = sqrt(sum_sq1 + sum_sq2 + sum_sq3 + sum_sq4 + sum_sq_remainder);
    }
    
    // Calculate correlations for upper triangle with optimized inner loop
    for (int i = 0; i < ny; i++) {
        double mean_i = means[i];
        double std_i = std_devs[i];
        
        for (int j = 0; j <= i; j++) {
            double mean_j = means[j];
            double std_j = std_devs[j];
            
            // Use multiple accumulators to reduce dependencies
            double numerator1 = 0.0, numerator2 = 0.0, numerator3 = 0.0, numerator4 = 0.0;
            int x = 0;
            
            // Unroll the covariance calculation by 4
            for (; x + 3 < nx; x += 4) {
                double diff_i1 = (double)data[x + i * nx] - mean_i;
                double diff_j1 = (double)data[x + j * nx] - mean_j;
                double diff_i2 = (double)data[x + 1 + i * nx] - mean_i;
                double diff_j2 = (double)data[x + 1 + j * nx] - mean_j;
                double diff_i3 = (double)data[x + 2 + i * nx] - mean_i;
                double diff_j3 = (double)data[x + 2 + j * nx] - mean_j;
                double diff_i4 = (double)data[x + 3 + i * nx] - mean_i;
                double diff_j4 = (double)data[x + 3 + j * nx] - mean_j;
                
                numerator1 += diff_i1 * diff_j1;
                numerator2 += diff_i2 * diff_j2;
                numerator3 += diff_i3 * diff_j3;
                numerator4 += diff_i4 * diff_j4;
            }
            
            // Handle remaining elements
            double numerator_remainder = 0.0;
            for (; x < nx; x++) {
                double diff_i = (double)data[x + i * nx] - mean_i;
                double diff_j = (double)data[x + j * nx] - mean_j;
                numerator_remainder += diff_i * diff_j;
            }
            
            double numerator = numerator1 + numerator2 + numerator3 + numerator4 + numerator_remainder;
            
            // Calculate correlation coefficient
            double correlation;
            double denominator = std_i * std_j;
            
            if (denominator == 0.0) {
                correlation = 0.0;
            } else {
                correlation = numerator / denominator;
            }
            
            result[i + j * ny] = (float)correlation;
        }
    }
    
    // Clean up
    delete[] means;
    delete[] std_devs;
}
