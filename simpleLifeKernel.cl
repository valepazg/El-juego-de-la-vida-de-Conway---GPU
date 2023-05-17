__kernel void simpleLifeKernel(                             
   __global uint* lifeData,                      
   __global uint* resultLifeData,                      
   const size_t worldWidth,                      
   const size_t worldHeight)               
{                                          
   int cellId = get_global_id(0);  
   size_t worldSize = worldWidth * worldHeight;   
   size_t blockSize = get_local_size(0); 
   size_t gridSize = get_global_size(0);          
   
   for(cellId; cellId<worldSize; cellId+=blockSize*gridSize){
      size_t x = cellId % worldWidth;
		size_t yAbs = cellId - x;
		size_t xLeft = (x + worldWidth - 1) % worldWidth;
		size_t xRight = (x + 1) % worldWidth;
		size_t yAbsUp = (yAbs + worldSize - worldWidth) % worldSize;
		size_t yAbsDown = (yAbs + worldWidth) % worldSize;
		size_t aliveCells = lifeData[xLeft + yAbsUp] + lifeData[x + yAbsUp] + lifeData[xRight + yAbsUp]
				+ lifeData[xLeft + yAbs] + lifeData[xRight + yAbs]
				+ lifeData[xLeft + yAbsDown] + lifeData[x + yAbsDown] + lifeData[xRight + yAbsDown];

		resultLifeData[x + yAbs] = (aliveCells == 3 || (aliveCells == 2 && lifeData[x + yAbs])) ? 1 : 0;
	}
}     


