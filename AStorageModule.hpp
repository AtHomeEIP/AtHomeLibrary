#ifndef ASTORAGEMODULE_HPP
# define ASTORAGEMODULE_HPP

# include "IModule.hpp"
# include "IStorage.hpp"

namespace woodBox {
	namespace module {
		class AStorageModule : public IModule {
			public:
				AStorageModule(storage::IStorage *);
				AStorageModule(AStorageModule &);
				AStorageModule &operator=(AStorageModule &);
				virtual ~AStorageModule();
				virtual void run() = 0;
				virtual void stop() = 0;
				virtual const storage::IStorage *getStorage();
				virtual void setStorage(storage::IStorage *);
			protected:
				virtual void onBackupOnStorage() = 0;
                virtual void onRestoreFromStorage() = 0;
			protected:
				storage::IStorage *_storage;
		};
	}
}

#endif /* ASTORAGEMODULE_HPP */