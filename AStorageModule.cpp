#include "AStorageModule.hpp"

namespace woodBox {
	namespace module {
		AStorageModule::AStorageModule(storage::IStorage *storage):
			_storage(storage)
		{}

		AStorageModule::AStorageModule(AStorageModule &other):
			_storage(other._storage)
		{
			other._storage = nullptr;
		}

		AStorageModule &AStorageModule::operator=(AStorageModule &other) {
			if (_storage != nullptr)
				delete _storage;
			_storage = other._storage;
			other._storage = nullptr;
			return *this;
		}

		AStorageModule::~AStorageModule() {
			if (_storage != nullptr)
				delete _storage;
		}

		const storage::IStorage *AStorageModule::getStorage() {
			return _storage;
		}

		void AStorageModule::setStorage(storage::IStorage *storage) {
			if (_storage != nullptr)
				delete _storage;
			_storage = storage;
		}
	}
}