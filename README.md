# Logi

Logi is light-weight Vulkan abstraction framework, which attempts to bridge the gap between previous graphics APIs and Vulkan by making API easier to use while still preserving Vulkan semantics and introducing as little overhead as possible.

## Features

* Object-oriented Vulkan abstraction framework
* Correctly enforced construction and destruction order of Vulkan resources
* Integration of the dynamic dispatching of Vulkan functions
* Vulkan configuration structures that use standard library (STL) containers
* Shader reflection functionality (SPIR-V Cross)
* Simplified management of memory allocations and memory bindings (Vulkan Memory Allocator)

## Building
Logi has been tested on Windows and Linux. Use the provided CMakeLists.txt with [CMake](https://cmake.org) to generate a build configuration for your favorite IDE or compiler.


## Usage
For examples see [`./examples`](examples/).

## Documentation and examples
Generate documentation with [CMake](https://cmake.org) by running `doc_doxygen` target.  
```
cmake --build <dir> --target doc_doxygen 
```

For more detailed description of framework, please see [Primož Lavrič master thesis](https://repozitorij.uni-lj.si/Dokument.php?id=123261&lang=slv) or paper [Vulkan Abstraction Layer for Large Data Remote Rendering System](http://lgm.fri.uni-lj.si/wp-content/uploads/2018/07/1537841091.pdf).  



