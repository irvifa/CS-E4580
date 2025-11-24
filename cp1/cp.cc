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
    // Pre-calculate all row means and standard deviations
    double *means = new double[ny];
    double *std_devs = new double[ny];
    
    // Calculate means for all rows
    for (int i = 0; i < ny; i++) {
        double sum = 0.0;
        for (int x = 0; x < nx; x++) {
            sum += data[x + i * nx];
        }
        means[i] = sum / nx;
    }
    
    // Calculate standard deviations for all rows
    for (int i = 0; i < ny; i++) {
        double sum_sq = 0.0;
        double mean = means[i];
        for (int x = 0; x < nx; x++) {
            double diff = data[x + i * nx] - mean;
            sum_sq += diff * diff;
        }
        std_devs[i] = sqrt(sum_sq);
    }
    
    // Calculate correlations for upper triangle
    for (int i = 0; i < ny; i++) {
        for (int j = 0; j <= i; j++) {
            double numerator = 0.0;
            double mean_i = means[i];
            double mean_j = means[j];
            
            // Calculate covariance
            for (int x = 0; x < nx; x++) {
                double diff_i = data[x + i * nx] - mean_i;
                double diff_j = data[x + j * nx] - mean_j;
                numerator += diff_i * diff_j;
            }
            
            // Calculate correlation coefficient
            double correlation;
            double denominator = std_devs[i] * std_devs[j];
            
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
