#ifndef SINGLETON_H
#define SINGLETON_H

template <typename T>
class Singleton
{
private:
    // Deleted to prevent copying/moving
    Singleton(const Singleton& rhs) = delete;
    Singleton& operator=(const Singleton& rhs) = delete;
    Singleton(Singleton&& rhs) = delete;
    Singleton& operator=(Singleton&& rhs) = delete;

protected:
    Singleton(void)
    {
        /* Nothing to do. */
    }

    virtual ~Singleton(void)
    {
        /* Nothing to do. */
    }

public:
    static T* Instance(void)
    {
        static T instance;
        return &instance;
    }
};

#endif
