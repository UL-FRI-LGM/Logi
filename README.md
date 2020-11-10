# Logi

Logi is light-weight Vulkan abstraction framework which attempts to bridge the gap between previous graphics APIs and Vulkan by making API easier to use while still preserving Vulkan semantics.

Goals:
* Provide an object-oriented Vulkan solution.
* Enforce correct construction and destruction order of Vulkan resources.
* Integrate and hide the dynamic dispatching of Vulkan functions.
* Reduce the amount of work that the developer is required to do while still maintaining Vulkan’s flexibility.
* Provide Vulkan configuration structures that use standard library (STL) containers.
* Provide shader reflection functionality.
* Simplify management of memory allocations and memory bindings.
* Most importantly, introduce as little overhead as possible.



