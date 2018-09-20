/*
*
*
* Copyright (C) 2017 by Primoz Lavric
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef BASE_EXTENSION_OBJECT_H
#define BASE_EXTENSION_OBJECT_H
#include <map>
#include <memory>
#include <typeindex>
#include <functional>
#include "base/UniqueWrapper.h"

namespace logi {

class Extension {
public:
	/**
     * @brief	Create a copy of the object and return a unique pointer that points to it.
     *
     * @return	Pointer to the object copy.
     */
	virtual std::unique_ptr<Extension> clone() const = 0;

	/**
     * @brief	Default virtual destructor.
     */
	virtual ~Extension() = 0;
};

/**
 * @brief	Base class for Logi object that can be extended with ExtensionsObjects.
 */
class Extendable {
public:
	virtual ~Extendable() = default;
	/**
	 * @brief	Default constructor.
	 */
	Extendable() = default;

	/**
	 * @brief	Compare extensions of two extendable objects.
	 *
	 * @param	rhs	Right hand side Extendable.
	 * @return	True if all the extensions contain equal configuration
	 */
	bool operator==(const Extendable& rhs) const;

	/**
	 * @brief	Check if the object has an extension of the given type.
	 *
	 * @tparam	ExtType	Extension type.
	 * @return	True if the extension is present.
	 */
	template <typename ExtType>
	bool hasExtension() const;

	/**
	 * @brief	Retrieve pointer to the extension of the given type.
	 *
	 * @tparam	ExtType	Extension type.
	 * @return	Pointer to the extension or null pointer if the extension is not present.
	 */
	template <typename ExtType>
	const ExtType* getExtension() const;

	/**
	 * @brief	Add Extension to the Extendable.
	 *
	 * @tparam	ExtType		Type of the extension.
	 * @param	extension	Extension to be added.
	 */
	template <typename ExtType>
	void addExtension(const ExtType& extension);

private:
	std::map<std::type_index, UniqueWrapper<Extension>> extensions_;
};

/**
 * @brief	Base class for Logi extension objects.
 */
class BuildableExtension {
public:
	/**
	 * @brief	Builds extension configuration and return void pointer to it.
	 *
	 * @return	Void pointer to the configuration structure.
	 */
	virtual void* build(void *next = nullptr) = 0;

	/**
	 * @brief	Create a copy of the object and return a unique pointer that points to it.
	 * 
	 * @return	Pointer to the object copy.
	 */
	virtual std::unique_ptr<BuildableExtension> clone() const = 0;

    /**
	 * @brief	Handles call with no extensions.
	 *
	 * @return	Null pointer.
	 */
	static void* buildExtensions() {
		return nullptr;
	}

    /**
     * @brief   Build the given BuildableExtension.
     *
     * @param	extension	BuildableExtension.
     * @return	Pointer to the built extension.
     */
    static void* buildExtensions(BuildableExtension& extension);

    /**
	 * @brief   Build the given BuildableExtension, link them and return the pointer to the first extension in linked list.
	 *
	 * @tparam	Args	    BuildableExtension type.
	 * @param	extension	Extension that will be built in this call.
	 * @param	extensions	Remaining extensions.
	 * @return	Pointer to the built extensions.
	 */
	template<typename... Args>
    static void* buildExtensions(BuildableExtension& extension, Args ... extensions);

	/**
	 * @brief	Default virtual destructor.
	 */
	virtual ~BuildableExtension() = default;
};

/**
 * @brief	Base class for Logi object that can be extended with ExtensionsObjects.
 */
class BuildableExtendable {
public:
    virtual ~BuildableExtendable() = default;
    /**
	 * @brief	Default constructor.
	 */
	BuildableExtendable() = default;

	/**
	 * @brief	Compare extensions of two extendable objects.
	 *
	 * @param	rhs	Right hand side BuildableExtendable.
	 * @return	True if all the extensions contain equal configuration
	 */
	bool operator==(const BuildableExtendable& rhs) const;

	/**
	 * @brief	Check if the object has an extension of the given type.
	 *
	 * @tparam	ExtType	Extension type.
	 * @return	True if the extension is present.
	 */
	template <typename ExtType>
	bool hasExtension() const;

	/**
	 * @brief	Retrieve pointer to the extension of the given type.
	 *
	 * @tparam	ExtType	Extension type.
	 * @return	Pointer to the extension or null pointer if the extension is not present.
	 */
	template <typename ExtType>
	ExtType* getExtension();

	/**
	 * @brief	Add BuildableExtension to the BuildableExtendable.	
	 *
	 * @tparam	ExtType		Type of the extension.
	 * @param	extension	Extension to be added.
	 */
	template <typename ExtType>
	void addExtension(const ExtType& extension);

protected:
	/**
	 * @brief	Builds all extensions and links them via next pointers.
	 *
	 * @return	Pointer to the first extension in the chain.
	 */
	void* buildExtensions() const;

private:
	std::map<std::type_index, UniqueWrapper<BuildableExtension>> extensions_;
};

template <typename ExtType>
bool Extendable::hasExtension() const {
	return extensions_.find(typeid(ExtType)) != extensions_.end();
}

template <typename ExtType>
const ExtType* Extendable::getExtension() const {
	auto it = extensions_.find(typeid(ExtType));

	if (it == extensions_.end()) {
		return nullptr;
	}

	return dynamic_cast<const ExtType*>(it->second.get());
}

template <typename ExtType>
void Extendable::addExtension(const ExtType& extension) {
	extensions_[typeid(ExtType)] = UniqueWrapper<Extension>::create(extension);
}

template <typename ... Args>
void* BuildableExtension::buildExtensions(BuildableExtension& extension, Args ... extensions) {
    return extension.build(buildExtensions(extensions...));
}

template <typename ExtType>
bool BuildableExtendable::hasExtension() const {
	return extensions_.find(typeid(ExtType)) != extensions_.end();
}

template <typename ExtType>
ExtType* BuildableExtendable::getExtension() {
	auto it = extensions_.find(typeid(ExtType));
	
	if (it == extensions_.end()) {
		return nullptr;
	}

	return dynamic_cast<ExtType*>(it->second.get());
}

template <typename ExtType>
void BuildableExtendable::addExtension(const ExtType& extension) {
	extensions_[typeid(ExtType)] = UniqueWrapper<BuildableExtension>::create(extension);
}

}

#endif