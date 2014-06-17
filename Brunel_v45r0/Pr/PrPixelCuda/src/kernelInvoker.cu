
#include "kernelInvoker.cuh"
#include "kernel.cuh"


extern int* h_no_sensors;
extern int* h_no_hits;
extern int* h_sensor_Zs;
extern int* h_sensor_hitStarts;
extern int* h_sensor_hitNums;
extern int* h_hit_IDs;
extern float* h_hit_Xs;
extern float* h_hit_Ys;
extern int* h_hit_Zs;

#define cudaCheck(stmt) do {										\
        cudaError_t err = stmt;										\
        if (err != cudaSuccess) {									\
            std::cerr << "Failed to run " << #stmt << std::endl;    \
            return err;										        \
        }															\
    } while(0)

// Helper function for using CUDA to add vectors in parallel.
cudaError_t invokeParallelSearch(dim3 numBlocks, dim3 numThreads,
	char* input, int size, Track*& tracks, int*& num_tracks, int*& h_track_indexes){
    
	// int* h_prevs, *h_nexts;
	// Histo histo;

	char *dev_input = 0;
	int* dev_num_tracks = 0;
	int* dev_track_indexes = 0;
	Track *dev_tracks = 0;
	bool* dev_track_holders = 0;
	int* dev_prevs = 0;
	int* dev_nexts = 0;
	int* dev_tracks_to_process = 0;
    cudaError_t cudaStatus = cudaSuccess;

    // Choose which GPU to run on, change this on a multi-GPU system.
    cudaCheck(cudaSetDevice(0));
    
	// Allocate memory
	// Allocate CPU buffers
	tracks = (Track*) malloc(MAX_TRACKS * sizeof(Track));
	num_tracks = (int*) malloc(sizeof(int));

	int* h_prevs = (int*) malloc(h_no_hits[0] * sizeof(int));
	int* h_nexts = (int*) malloc(h_no_hits[0] * sizeof(int));
	bool* h_track_holders = (bool*) malloc(MAX_TRACKS * sizeof(bool));
	h_track_indexes = (int*) malloc(MAX_TRACKS * sizeof(int));

    // Allocate GPU buffers
    cudaCheck(cudaMalloc((void**)&dev_tracks, MAX_TRACKS * sizeof(Track)));
	cudaCheck(cudaMalloc((void**)&dev_track_holders, MAX_TRACKS * sizeof(bool)));
	cudaCheck(cudaMalloc((void**)&dev_track_indexes, MAX_TRACKS * sizeof(int)));
	cudaCheck(cudaMalloc((void**)&dev_tracks_to_process, MAX_TRACKS * sizeof(int)));

	cudaCheck(cudaMalloc((void**)&dev_prevs, h_no_hits[0] * sizeof(int)));
	cudaCheck(cudaMalloc((void**)&dev_nexts, h_no_hits[0] * sizeof(int)));
    
    // Copy input file from host memory to GPU buffers
    cudaCheck(cudaMalloc((void**)&dev_input, size));
    cudaCheck(cudaMalloc((void**)&dev_num_tracks, sizeof(int)));
    
	// memcpys
    cudaCheck(cudaMemcpy(dev_input, input, size, cudaMemcpyHostToDevice));

	// Launch a kernel on the GPU with one thread for each element.
	prepareData<<<1, 1>>>(dev_input, dev_prevs, dev_nexts, dev_track_holders);

	// gpuKalman
	std::cout << "gpuKalman" << std::endl;
	cudaEvent_t start_kalman, start_postprocess, stop;
	float t0, t1, t2;

	cudaEventCreate(&start_kalman);
	cudaEventCreate(&start_postprocess);
	cudaEventCreate(&stop);

	cudaEventRecord(start_kalman, 0 );

	gpuKalman<<<46, 32>>>(dev_tracks, dev_track_holders);

	cudaEventRecord(start_postprocess);
	
	/*cudaCheck(cudaMemcpy(h_track_holders, dev_track_holders, MAX_TRACKS * sizeof(bool), cudaMemcpyDeviceToHost));
	cudaCheck(cudaMemcpy(tracks, dev_tracks, MAX_TRACKS * sizeof(Track), cudaMemcpyDeviceToHost));
	int j = 0;
	for(int i=0; i<h_no_hits[0]; ++i){
		if(h_track_holders[i]){
			++j;
	// 		printTrack(tracks, i);
		}
	}
	std::cout <<  "gpuKalman: " << j << " tracks" << std::endl;
	// histo.plotChi2("after-kalman.root", h_track_holders, tracks, h_no_hits[0]);
	*/

	std::cout << "postProcess" << std::endl;
	postProcess<<<1, 32>>>(dev_tracks, dev_track_holders, dev_track_indexes, dev_num_tracks, dev_tracks_to_process);
	
	cudaEventRecord( stop, 0 );
	cudaEventSynchronize( stop );

	cudaEventElapsedTime( &t0, start_kalman, start_postprocess );
	cudaEventElapsedTime( &t1, start_postprocess, stop );
	cudaEventElapsedTime( &t2, start_kalman, stop );
	cudaEventDestroy( start_kalman );
	cudaEventDestroy( start_postprocess );
	cudaEventDestroy( stop );

	// Get results
    cudaCheck(cudaMemcpy(h_track_holders, dev_track_holders, MAX_TRACKS * sizeof(bool), cudaMemcpyDeviceToHost));
    cudaCheck(cudaMemcpy(h_track_indexes, dev_track_indexes, MAX_TRACKS * sizeof(int), cudaMemcpyDeviceToHost));
	cudaCheck(cudaMemcpy(tracks, dev_tracks, MAX_TRACKS * sizeof(Track), cudaMemcpyDeviceToHost));
    cudaCheck(cudaMemcpy(num_tracks, dev_num_tracks, sizeof(int), cudaMemcpyDeviceToHost));
	
	// Amount of tracks after stage#1
	int no_tracks_stage1 = 0;
	for(int i=0; i<h_no_hits[0]; ++i)
               if(h_track_holders[i])
                       ++no_tracks_stage1;

	// Append to results
	// std::ofstream ofs;
	// ofs.open ("results_gpu.csv", std::ofstream::out | std::ofstream::app);
	// ofs << toString(h_no_hits[0]) << ", " << toString(no_tracks_stage1) << ", " << toString(num_tracks[0]) << ", " << toString(t0) << ", " << toString(t1) << ", " << toString(t2) << "\n";
	// ofs.close();

	// Produce some valuable statistics
        // Append to results
        //std::ofstream statf;
        //statf.open ("hits_per_sensor.csv", std::ofstream::out | std::ofstream::app);
        //statf << h_no_hits[0];
	//for(int i=0; i<48; ++i)
	//	statf << ", " << h_sensor_hitNums[i];
	//statf << "\n";
        //statf.close();


	for(int i=0; i<num_tracks[0]; ++i){
		printTrack(tracks, h_track_indexes[i]);
	}
	// std::cout << "postProcess: " << num_tracks[0] << " tracks" << std::endl << std::endl;
	std::cout << "Processed " << num_tracks[0] << " tracks" << std::endl;

	free(h_prevs);
	free(h_nexts);
	free(h_track_holders);

	// histo.plotChi2("after-post-processing.root", h_track_indexes, tracks, num_tracks[0]);

    // neighboursFinder<<<numBlocks, numThreads>>>();

	// Visualize results
	// cudaCheck(cudaMemcpy(h_prevs, dev_prevs, h_no_hits[0] * sizeof(int), cudaMemcpyDeviceToHost));
	// cudaCheck(cudaMemcpy(h_nexts, dev_nexts, h_no_hits[0] * sizeof(int), cudaMemcpyDeviceToHost));
	// printOutSensorHits(2, h_prevs, h_nexts);

	/*
	out = std::ofstream("prevnexts.out");
	out.write((char*) &h_prevs[0], h_no_hits[0] * sizeof(int));
	out.write((char*) &h_nexts[0], h_no_hits[0] * sizeof(int));
	out.close();
	*/

	// neighboursCleaner<<<numBlocks, numThreads>>>();
	
	// Visualize results
	// cudaCheck(cudaMemcpy(h_prevs, dev_prevs, h_no_hits[0] * sizeof(int), cudaMemcpyDeviceToHost));
	// cudaCheck(cudaMemcpy(h_nexts, dev_nexts, h_no_hits[0] * sizeof(int), cudaMemcpyDeviceToHost));
	// printOutSensorHits(2, h_prevs, h_nexts);
	// printOutAllSensorHits(h_prevs, h_nexts);
	
	// cudaDeviceSynchronize waits for the kernel to finish, and returns
    // any errors encountered during the launch.
 //    cudaCheck(cudaDeviceSynchronize());
    
	// // cuda copy back
	// cudaCheck(cudaMemcpy(num_tracks, dev_num_tracks, sizeof(int), cudaMemcpyDeviceToHost));
	
 //    // Copy output vector from GPU buffer to host memory.
 //    cudaCheck(cudaMemcpy(tracks, dev_tracks, num_tracks[0] * sizeof(Track), cudaMemcpyDeviceToHost));
    
    return cudaStatus;
}

// #track, h0, h1, h2, h3, ..., hn, length, chi2
void printTrack(Track* tracks, int track_no){
	std::cout << track_no << ": ";

	Track t = tracks[track_no];
	for(int i=0; i<t.hitsNum; ++i){
		std::cout << h_hit_IDs[t.hits[i]] << ", ";
	}

	std::cout << "length: " << (int) t.hitsNum << std::endl;
}

/*
float f_chi2(Track& t)
{
	float ch = 0.0;
	int nDoF  = -4;
	int hitNumber;
	for (int i=0; i<t.hitsNum; ++i){
		hitNumber = t.hits[i];
		ch += f_chi2Track(t, hitNumber);
		nDoF += 2;
	}
	return ch/nDoF;
}
*/

void printOutAllSensorHits(int* prevs, int* nexts){
	std::cout << "All valid sensor hits: " << std::endl;
	for(int i=0; i<h_no_sensors[0]; ++i){
		for(int j=0; j<h_sensor_hitNums[i]; ++j){
			int hit = h_sensor_hitStarts[i] + j;
			
			if(nexts[hit] != -1){
				std::cout << hit << ", " << nexts[hit] << std::endl;
			}
		}
	}
}

void printOutSensorHits(int sensorNumber, int* prevs, int* nexts){
	for(int i=0; i<h_sensor_hitNums[sensorNumber]; ++i){
		int hstart = h_sensor_hitStarts[sensorNumber];

		std::cout << hstart + i << ": " << prevs[hstart + i] << ", " << nexts[hstart + i] << std::endl;
	}
}

void getMaxNumberOfHits(char*& input, int& maxHits){
	int* l_no_sensors = (int*) &input[0];
    int* l_no_hits = (int*) (l_no_sensors + 1);
    int* l_sensor_Zs = (int*) (l_no_hits + 1);
    int* l_sensor_hitStarts = (int*) (l_sensor_Zs + l_no_sensors[0]);
    int* l_sensor_hitNums = (int*) (l_sensor_hitStarts + l_no_sensors[0]);

	maxHits = 0;
	for(int i=0; i<l_no_sensors[0]; ++i){
		if(l_sensor_hitNums[i] > maxHits)
			maxHits = l_sensor_hitNums[i];
	}
}
