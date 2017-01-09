# Example Program: kpi_pirand_mult

## Code Description:
  The kpi_pirand example program uses the KPI Host Runtime C++ wrapper library to
  estimate a value for pi via a Monte Carlo method.

  The host application runs through several iterations with different numbers
  of (x, y) coordinates which will be set as an argument for the piRandRation
  kernel, found in the source file *pirand.knucpp*.

  Each tDSP kernel (or rank) will initialize the hardware random number generator
  using seed values that are dependent on the specific rank. The tDSP will
  then generate pairs of random values, and, using a technique known as
  rejection sampling, estimate a value for pi based on determining whether
  or not a point with those values in (x,y) coordinates is located within
  a unit circle.

## Expected output:
  Output can be found in *example_output.csv*.

  > Operating on 512 tDSPs.

  > Iteration, World Size, coordsPerRank, # Samples,  Iteration RunTime (Sec),  Pi Value

  > 1, 512, 10,  5120,  0.004429,  3.15156

  > Total Time: 0.164694

## Build and run:
  1) Provide this git URL into KWS and click 'Run'.
