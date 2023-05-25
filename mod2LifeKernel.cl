__kernel void mod2LifeKernel(__global uint* lifeData, __global uint* resultLifeData, const size_t worldWidth, const size_t worldHeight) {               
                                          
    size_t cellId_x = get_global_id(0);  
    size_t cellId_y = get_global_id(1);  
       
    size_t left = (cellId_y + worldWidth - 1) % worldWidth;
    size_t right = (cellId_y + 1) % worldWidth;
    size_t top = (cellId_x + worldHeight - 1) % worldHeight;
    size_t bottom = (cellId_x + 1) % worldHeight; 

    size_t aliveCells = lifeData[top * worldWidth + left] + lifeData[top * worldWidth + cellId_y] + 
                             lifeData[top * worldWidth + right] + lifeData[cellId_x * worldWidth + left] + 
                             lifeData[cellId_x * worldWidth + right] + lifeData[bottom * worldWidth + left] + 
                             lifeData[bottom * worldWidth + cellId_y] + lifeData[bottom * worldWidth + right];
            
    resultLifeData[cellId_x * worldWidth + cellId_y] = (aliveCells == 3 || (aliveCells == 2 && lifeData[cellId_x * worldWidth + cellId_y])) ? 1 : 0;
    
}
