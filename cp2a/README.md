# CP2a: Correlation Problem with Instruction-Level Parallelism

This implementation optimizes the correlation calculation from CP1 by exploiting instruction-level parallelism (ILP). The goal is to make the CPU work more efficiently by reducing pipeline stalls and increasing instruction throughput.

## Key Optimizations

### Loop Unrolling
The main computational loops are unrolled by a factor of 4. Instead of processing one element at a time, we process four elements per iteration. This reduces the loop overhead and gives the CPU more instructions to work with simultaneously.

```cpp
// Instead of:
for (int x = 0; x < nx; x++) {
    sum += data[x + i * nx];
}

// We do:
for (; x + 3 < nx; x += 4) {
    sum1 += data[x + i * nx];
    sum2 += data[x + 1 + i * nx];
    sum3 += data[x + 2 + i * nx];
    sum4 += data[x + 3 + i * nx];
}
```

### Multiple Accumulators
Using separate accumulator variables eliminates data dependencies. When you have `sum += a; sum += b;`, the second operation must wait for the first to complete. With separate accumulators, both operations can run in parallel.

### Double-Precision Calculations
All intermediate calculations use double precision for better numerical accuracy. We only convert back to float at the very end when storing results.

### Memory Access Patterns
The unrolled loops maintain sequential memory access, which plays nicely with the CPU cache. We're not jumping around in memory - just reading consecutive elements.

## Performance Impact

These optimizations target the CPU's ability to execute multiple instructions per clock cycle. Modern processors can often execute 4+ operations simultaneously if there are no dependencies between them.

The unrolling exposes more parallelism to the CPU's out-of-order execution engine, while the separate accumulators remove the artificial dependencies that would otherwise serialize the computation.

## Algorithm Structure

1. **Mean calculation**: Unrolled loop with 4 accumulators
2. **Standard deviation**: Unrolled loop with 4 accumulators  
3. **Correlation computation**: Unrolled covariance calculation with 4 accumulators

Each phase processes the data in chunks of 4 elements, with remainder handling for cases where the data size isn't divisible by 4.

The basic correlation formula remains the same - we're just computing it more efficiently.