/******************************************************************************
  Copyright 2016-2017
  All rights reserved.
  KNUPATH Proprietary and Confidential

  File: kpi_pirand.cpp

  Description: Host code for KPI pi estimation example using Monte Carlo method

******************************************************************************/

#include "kpi/kpi_runtime.hpp"
#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>

int main(int argc, char* argv[])
{
  // Get start of entire runtime
  std::clock_t begin_run = std::clock();

  // Create a context for complex 0 (the default)
  kpi::Context        ctx;

  // Create a command queue to submit tasks to the complex
  kpi::CommandQueue   cq = ctx.CreateCommandQueue();

  // Allocate all available clusters within the complex (SDB = 64)
  kpi::ClusterBlock   cb = ctx.AllocClusterBlock();

  // Write to an output file
  std::ofstream output;
  output.open("example_output.csv");

  // Output Info
  output << "Operating on " << cb.Size() << " clusters." << std::endl << std::endl;
  output << "Iteration, World Size,  coordsPerRank,        # Samples, Iteration RunTime (Sec), Pi Value" << std::endl;

  // Run Limit (9 = 1 billion per tdsp)
  // **Note: KWS v0.1 does not support multiple Run Limits
  int runLimit = 1;

  // Set first coordsPerRank to 1
  int coordsPerRank = 1;

  // Run in increments of the power of 2
  for (int i=1; i<=runLimit; i+=1)
  {
    // Get start of each iteration
    std::clock_t begin = std::clock();

    // Calculates # of coordinates per rank
    coordsPerRank *= 10;

    // Load a kernel and set the argument to the coordsPerRank
    kpi::KernelWithArgs kwa = ctx.CreateKernelWithArgs("piRandRatio", coordsPerRank);

    // Submit the launch configuration to the queue. Use the returned
    // host connection to communicate with the kernels during while
    // the launch is running.
    kpi::HostConn hc =  cq.SubmitWithHostConn(
                          ctx.CreateLaunchConfig(
                            cb.Size(),
				            kpi::ProcGroup(cb),
                            kwa),
                           cb);

	// Packet format
	struct receive_format
	{
		float pi_estimate;
		uint32_t world_size;
	} packet;

    // Receive the estimate
	std::vector<receive_format> packets(1);
    hc.ReceiveCount(packets);

    // Output Run Info
    output << std::setw(9) << i
    	<< ", " << std::setw(10) << packets[0].world_size
		<< ", " << std::setw(14) << coordsPerRank
		<< ", " << std::setw(16) << long(coordsPerRank) * cb.Size() * 8
		<< ", " << std::setw(16) << double(std::clock() - begin) / CLOCKS_PER_SEC
		<< ", " << std::setw(14) << packets[0].pi_estimate
		<< std::endl;
  }

  // Write total time it took to run program
  output << std::endl << "Total Time (sec): " << double(std::clock() - begin_run)/CLOCKS_PER_SEC << std::endl;

  // Close file and exit
  output.close();
  return 0;
}
