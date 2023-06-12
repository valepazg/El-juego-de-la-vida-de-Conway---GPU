__kernel void mod1LifeKernel(__global uint* lifeData, __global uint* resultLifeData, const size_t worldWidth, const size_t worldHeight){
    int cellId = get_global_id(0);  
    size_t worldSize = worldWidth * worldHeight;   
   
    size_t x = cellId % worldWidth;
    size_t yAbs = cellId - x;
    size_t xLeft = (x + worldWidth - 1) % worldWidth;
    size_t xRight = (x + 1) % worldWidth;
    size_t yAbsUp = (yAbs + worldSize - worldWidth) % worldSize;
    size_t yAbsDown = (yAbs + worldWidth) % worldSize;
    
    size_t aliveCells = 0;

    if(lifeData[xLeft + yAbsUp] == 1){
        aliveCells++;
    }

    if(lifeData[x + yAbsUp] == 1){
        aliveCells++;
    }

    if(lifeData[xRight + yAbsUp] == 1){
        aliveCells++;
    }

    if(lifeData[xLeft + yAbs] == 1){
        aliveCells++;
    }

    if(lifeData[xRight + yAbs] == 1){
        aliveCells++;
    }

    if(lifeData[xLeft + yAbsDown] == 1){
        aliveCells++;
    }

    if(lifeData[x + yAbsDown] == 1){
        aliveCells++;
    }

    if(lifeData[xRight + yAbsDown] == 1){
        aliveCells++;
    }

    resultLifeData[x + yAbs] = (aliveCells == 3 || (aliveCells == 2 && lifeData[x + yAbs])) ? 1 : 0;
}    