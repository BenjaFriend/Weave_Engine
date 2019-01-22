#pragma once

/// <summary>
/// Singleton data structure to be used for objects
/// that need to have multiple implementations
/// </summary>
template <typename T>
class Singleton
{
public:

    Singleton( const Singleton<T>& ) = delete;
    Singleton& operator=( const Singleton<T>& ) = delete;

    /// <summary>
    /// Get this singleton instance
    /// </summary>
    /// <returns>Static pointer to the instance of this object</returns>
    static T * GetInstance();

    /// <summary>
    /// Release this singleton instance
    /// </summary>
    static void ReleaseInstance();

protected:

    static T* Instance;

    Singleton();

};

template <typename T>
Singleton<T>::Singleton()
{
    printf( "Singleton creat" );
}

template <typename T>
T* Singleton<T>::GetInstance()
{
    if ( Singleton::Instance == nullptr )
    {
        Singleton::Instance = new T();
    }
    return Singleton::Instance;
}

template <typename T>
void Singleton<T>::ReleaseInstance()
{
    if ( Singleton::Instance != nullptr )
    {
        delete Singleton::Instance;
    }
    Singleton::Instance = nullptr;
}

// Static initialization
template <typename T> T* Singleton<T>::Instance = nullptr;