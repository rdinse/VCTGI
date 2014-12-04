VCTGI
=====

Real-time global illumination renderer using voxel cone tracing.

![Screenshot](Screenshot.png)

### Compilation

The main parts of the renderer are located in `src/executables/VCTGI` and `src/shaders/VCTGI`.  Some parts of `src/libraries/CVK_Framework` are modified, in particular the shader classes.  Empty space skipping, conservative rasterization and soft shadows can be enabled in `Voxelization.geom`, `DirectIllumination.frag` and `Globalllumination.frag` respectively.  The code is tested on Linux and should compile with `cmake` on Windows too.

### References and Resources

* https://research.nvidia.com/publication/interactive-indirect-illumination-using-voxel-cone-tracing
* https://research.nvidia.com/publication/octree-based-sparse-voxelization-using-gpu-hardware-rasterizer
* http://maverick.inria.fr/Membres/Cyril.Crassin/thesis/
* http://http.developer.nvidia.com/GPUGems2/gpugems2_chapter42.html
* https://github.com/domme/VoxelConeTracing
* http://www.naixela.com/alex/
* http://tf3dm.com/3d-model/cars-66597.html
