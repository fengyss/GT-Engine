
#include "gtpch.h"
#include "FileWatcher.h"

namespace GT {

    FileWatcher::FileWatcher()
    {
        m_Listener = new Listener(*this);
    }

    FileWatcher::~FileWatcher()
    {
        Stop();
    }

    void FileWatcher::Start()
    {
        m_Watcher.watch();
    }

    void FileWatcher::Stop()
    {
		for (auto& [path, watchID] : m_WatchIDs)
		    m_Watcher.removeWatch(watchID);
    }

    void FileWatcher::RemoveWatch(const std::filesystem::path& path)
    {
		auto it = m_WatchIDs.find(path);
        if (it != m_WatchIDs.end())
        {
            m_Watcher.removeWatch(it->second);
            m_WatchIDs.erase(it);
            m_Callbacks.erase(path.string());
        }
    }

    void FileWatcher::Watch(const std::filesystem::path& path, FileCallback callback)
    {
        m_Path = path.string();
        std::string pathStr = path.string();
        m_Callbacks[pathStr] = callback;

        efsw::WatchID ID = m_Watcher.addWatch(pathStr, m_Listener, true);
		m_WatchIDs.find(path) != m_WatchIDs.end() ? m_WatchIDs[path] = ID : m_WatchIDs[path]=ID;
    }

    void FileWatcher::Notify(const std::filesystem::path& path, FileAction action)
    {
        //std::string pathStr = path.string();
        //if (m_Callbacks.count(pathStr))
            m_Callbacks[m_Path](path, action);
    }

    void FileWatcher::Listener::handleFileAction(
        efsw::WatchID watchid,
        const std::string& dir,
        const std::string& filename,
        efsw::Action action,
        const std::string& oldFilename
    )
    {
        std::filesystem::path fullPath = std::filesystem::path(dir) / filename;
        m_Watcher.Notify(fullPath, (FileAction)action);
    }

}