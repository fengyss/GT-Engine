#pragma once

#include "efsw/efsw.hpp"
#include <functional>
#include <string>
#include <filesystem>
#include <unordered_map>

namespace GT {

    enum class FileAction {
        Added = efsw::Actions::Add,
        Removed = efsw::Actions::Delete,
        Modified = efsw::Actions::Modified,
        Renamed = efsw::Actions::Moved
    };

    using FileCallback = std::function<void(const std::filesystem::path&, FileAction)>;

    class FileWatcher {
    public:
        FileWatcher();
        ~FileWatcher();

        void Start();
        void Stop();

		void RemoveWatch(const std::filesystem::path& path);
        void Watch(const std::filesystem::path& path, FileCallback callback);

    private:
        class Listener : public efsw::FileWatchListener {
        public:
            Listener(FileWatcher& watcher) : m_Watcher(watcher) {}

            void handleFileAction(
                efsw::WatchID watchid, const std::string& dir,
                const std::string& filename, efsw::Action action,
                const std::string& oldFilename = ""
            ) override;

        private:
            FileWatcher& m_Watcher;
        };

    private:
        void Notify(const std::filesystem::path& path, FileAction action);

    private:
		std::string m_Path;
        efsw::FileWatcher m_Watcher;
        Listener* m_Listener;
		std::unordered_map<std::filesystem::path, efsw::WatchID> m_WatchIDs;
        std::unordered_map<std::string, FileCallback> m_Callbacks;
    };

}