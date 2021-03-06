#ifndef _RWLockImpl_Header
#define _RWLockImpl_Header

#include <assert.h>
#include <iostream>
#include <Windows.h>
#include <process.h>

using namespace std;

/*
 读写锁允许当前的多个读用户访问保护资源，但只允许一个写读者访问保护资源
*/

//-----------------------------------------------------------------
class CRWLockImpl
{
protected:
    CRWLockImpl();
    ~CRWLockImpl();
    void ReadLockImpl();
    bool TryReadLockImpl();
    void WriteLockImpl();
    bool TryWriteLockImpl();
    void UnlockImpl();

private:
    void AddWriter();
    void RemoveWriter();
    DWORD TryReadLockOnce();

    HANDLE   m_mutex;
    HANDLE   m_readEvent;
    HANDLE   m_writeEvent;
    unsigned m_readers;
    unsigned m_writersWaiting;
    unsigned m_writers;
};

//-----------------------------------------------------------------

class CMyRWLock: private CRWLockImpl
{
public:

    //创建读/写锁
    CMyRWLock(){};

    //销毁读/写锁
    ~CMyRWLock(){};

    //获取读锁
    //如果其它一个线程占有写锁，则当前线程必须等待写锁被释放，才能对保护资源进行访问
    void ReadLock();

    //尝试获取一个读锁
    //如果获取成功，则立即返回true，否则当另一个线程占有写锁，则返回false
    bool TryReadLock();

    //获取写锁
    //如果一个或更多线程占有读锁，则必须等待所有锁被释放
    //如果相同的一个线程已经占有一个读锁或写锁，则返回结果不确定
    void WriteLock();

    //尝试获取一个写锁
    //如果获取成功，则立即返回true，否则当一个或更多其它线程占有读锁，返回false
    //如果相同的一个线程已经占有一个读锁或写锁，则返回结果不确定
    bool TryWriteLock();

    //释放一个读锁或写锁
    void Unlock();

private:
    CMyRWLock(const CMyRWLock&);
    CMyRWLock& operator = (const CMyRWLock&);
};

inline void CMyRWLock::ReadLock()
{
    ReadLockImpl();
}

inline bool CMyRWLock::TryReadLock()
{
    return TryReadLockImpl();
}

inline void CMyRWLock::WriteLock()
{
    WriteLockImpl();
}

inline bool CMyRWLock::TryWriteLock()
{
    return TryWriteLockImpl();
}

inline void CMyRWLock::Unlock()
{
    UnlockImpl();
}


#endif