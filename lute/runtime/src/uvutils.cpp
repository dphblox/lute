#include "lute/uvutils.h"

#include "uv.h"

namespace uvutils
{

UvError::UvError(int code)
    : code(code)
{
}

std::string UvError::toString() const
{
    return uv_strerror(code);
}

Luau::Variant<std::string, UvError> getStringFromUv(BufferWriter bufferWriter, size_t initialBufferSize)
{
    std::string buffer;
    size_t size = initialBufferSize;
    buffer.resize(size);

    int writeStatus = bufferWriter(buffer.data(), &size);
    if (writeStatus == UV_ENOBUFS)
    {
        // `size` now contains the required size
        buffer.resize(size);
        writeStatus = bufferWriter(buffer.data(), &size);
    }

    if (writeStatus < 0)
        return UvError{writeStatus};

    buffer.resize(size);
    return buffer;
}

std::optional<int> getEnvironmentVariables(std::map<std::string, std::string>& map)
{
    uv_env_item_t* currentEnvItems;
    int currentEnvCount;
    int err = uv_os_environ(&currentEnvItems, &currentEnvCount);
    if (err != 0)
    {
        uv_os_free_environ(currentEnvItems, currentEnvCount);
        return err;
    }

    for (int i = 0; i < currentEnvCount; i++)
    {
        auto var = currentEnvItems[i];
        if (var.name && var.value && map.find(var.name) == map.end())
            map[var.name] = var.value;
    }
    uv_os_free_environ(currentEnvItems, currentEnvCount);

    return std::nullopt;
}

std::vector<char*> getProcessEnvironmentString(const std::map<std::string, std::string>& map)
{
    std::vector<std::string> envStrings;
    std::vector<char*> envPtr;
    // Turn the new environment into a char** array
    envStrings.reserve(map.size());
    envPtr.reserve(map.size() + 1);
    for (const auto& pair : map)
    {
        envStrings.push_back(pair.first + "=" + pair.second);
    }

    for (auto& str : envStrings)
    {
        envPtr.push_back(&str[0]);
    }
    envPtr.push_back(nullptr);
    return envPtr;
}


} // namespace uvutils
