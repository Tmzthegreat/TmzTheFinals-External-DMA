// Serial.cpp : �������̨Ӧ�ó������ڵ㡣
/*
���ڻ�����⣨WIN32�� ver 0.1
  ������ �� BC++ 5; C++ BUILDER 4, 5, 6, X; VC++ 5, 6; VC.NET;  GCC;
class   _base_com : ����� �������ڽӿ�;
class   _sync_com : ͬ��I/O ������;
class   _asyn_com : �첽I/O ������;
class _thread_com : �첽I/O �����������߳� ��ת��������Ϣ ������(�ɼ̳��麯��on_receive���ڶ�����)��
class        _com : _thread_com ͬ��
*/
//#include "stdafx.h"
#ifndef _COM_H_
#define _COM_H_

#pragma warning(disable: 4530)
#pragma warning(disable: 4786)
#pragma warning(disable: 4800)

#include <cassert>
#include <strstream>
#include <algorithm>
#include <exception>
#include <iomanip>
using namespace std;
#include <windows.h>
#include <tchar.h>
class _base_com   //����� �������ڽӿ�
{
protected:
    volatile int _port;  //���ں�
    volatile HANDLE _com_handle;//���ھ��
    char _com_str[20];
    DCB _dcb;     //�����ʣ�ֹͣλ����
    COMMTIMEOUTS _co;  // ��ʱʱ��
    virtual bool open_port() = 0;
    void init() //��ʼ��
    {
        memset(_com_str, 0, 20);
        memset(&_co, 0, sizeof(_co));
        memset(&_dcb, 0, sizeof(_dcb));
        _dcb.DCBlength = sizeof(_dcb);
        _com_handle = INVALID_HANDLE_VALUE;
    }
    virtual bool setup_port()
    {
        if (!is_open())
            return false;

        if (!SetupComm(_com_handle, 8192, 8192))
            return false; //�����Ƽ�������

        if (!GetCommTimeouts(_com_handle, &_co))
            return false;
        _co.ReadIntervalTimeout = 0xFFFFFFFF;
        _co.ReadTotalTimeoutMultiplier = 0;
        _co.ReadTotalTimeoutConstant = 0;
        _co.WriteTotalTimeoutMultiplier = 0;
        _co.WriteTotalTimeoutConstant = 2000;
        if (!SetCommTimeouts(_com_handle, &_co))
            return false; //���ó�ʱʱ��

        if (!PurgeComm(_com_handle, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR))
            return false; //��մ��ڻ�����

        return true;
    }
    inline void set_com_port(int port)
    {
        char p[12];
        _port = port;
        strcpy_s(_com_str, "\\\\.\\COM");
        _ltoa_s(_port, p, 10);
        strcat_s(_com_str, p);
    }
public:
    _base_com()
    {
        init();
    }
    virtual ~_base_com()
    {
        close();
    }
    //���ô��ڲ����������ʣ�ֹͣλ���� ֧�������ַ��� "115200, 8, n, 1"
    bool set_state(char* set_str)
    {
        if (is_open())
        {
            if (!GetCommState(_com_handle, &_dcb))
                return false;
            if (!BuildCommDCBA(set_str, &_dcb))
                return false;
            return SetCommState(_com_handle, &_dcb) == TRUE;
        }
        return false;
    }
    //�������ýṹ���ڲ����������ʣ�ֹͣλ
    bool set_state(int BaudRate, int ByteSize = 8, int Parity = NOPARITY, int StopBits = ONESTOPBIT)
    {
        if (is_open())
        {
            if (!GetCommState(_com_handle, &_dcb))
                return false;
            _dcb.BaudRate = BaudRate;
            _dcb.ByteSize = ByteSize;
            _dcb.Parity = Parity;
            _dcb.StopBits = StopBits;
            return SetCommState(_com_handle, &_dcb) == TRUE;
        }
        return false;
    }
    //�򿪴��� ȱʡ 115200, 8, n, 1
    inline bool open(int port)
    {
        return open(port, 115200);
    }
    //�򿪴��� ȱʡ baud_rate, 8, n, 1
    inline bool open(int port, int baud_rate)
    {
        if (port < 1 || port > 1024)
            return false;

        set_com_port(port);

        if (!open_port())
            return false;

        if (!setup_port())
            return false;

        return set_state(baud_rate);
    }
    //�򿪴���
    inline bool open(int port, char* set_str)
    {
        if (port < 1 || port > 1024)
            return false;

        set_com_port(port);

        if (!open_port())
            return false;

        if (!setup_port())
            return false;

        return set_state(set_str);

    }
    inline bool set_buf(int in, int out)
    {
        return is_open() ? SetupComm(_com_handle, in, out) : false;
    }
    //�رմ���
    inline virtual void close()
    {
        if (is_open())
        {
            CloseHandle(_com_handle);
            _com_handle = INVALID_HANDLE_VALUE;
        }
    }
    //�жϴ����ǻ��
    inline bool is_open()
    {
        return _com_handle != INVALID_HANDLE_VALUE;
    }
    //��ô��ھ��
    HANDLE get_handle()
    {
        return _com_handle;
    }
    operator HANDLE()
    {
        return _com_handle;
    }
};

class _sync_com : public _base_com
{
protected:
    //�򿪴���
    virtual bool open_port()
    {
        if (is_open())
            close();

        _com_handle = CreateFileA(
            _com_str,
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );
        assert(is_open());
        return is_open();//��⴮���Ƿ�ɹ���
    }

public:

    _sync_com()
    {
    }
    //ͬ����
    int read(char* buf, int buf_len)
    {
        if (!is_open())
            return 0;

        buf[0] = '\0';

        COMSTAT  stat;
        DWORD error;

        if (ClearCommError(_com_handle, &error, &stat) && error > 0) //�������
        {
            PurgeComm(_com_handle, PURGE_RXABORT | PURGE_RXCLEAR); /*������뻺����*/
            return 0;
        }

        unsigned long r_len = 0;

        buf_len = min(buf_len - 1, (int)stat.cbInQue);
        if (!ReadFile(_com_handle, buf, buf_len, &r_len, NULL))
            r_len = 0;
        buf[r_len] = '\0';

        return r_len;
    }
    //ͬ��д
    int write(char* buf, int buf_len)
    {
        if (!is_open() || !buf)
            return 0;

        DWORD    error;
        if (ClearCommError(_com_handle, &error, NULL) && error > 0) //�������
            PurgeComm(_com_handle, PURGE_TXABORT | PURGE_TXCLEAR);

        unsigned long w_len = 0;
        if (!WriteFile(_com_handle, buf, buf_len, &w_len, NULL))
            w_len = 0;

        return w_len;
    }
    //ͬ��д
    inline int write(char* buf)
    {
        assert(buf);
        return write(buf, strlen(buf));
    }
    //ͬ��д, ֧�ֲ������͵������
    template<typename T>
    _sync_com& operator << (T x)
    {
        strstream s;

        s << x;
        write(s.str(), s.pcount());

        return *this;
    }
};

class _asyn_com : public _base_com
{
protected:

    OVERLAPPED _ro, _wo; // �ص�I/O

    virtual bool open_port()
    {
        if (is_open())
            close();

        _com_handle = CreateFileA(
            _com_str,
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, //�ص�I/O
            NULL
        );
        assert(is_open());
        return is_open();//��⴮���Ƿ�ɹ���
    }

public:

    _asyn_com()
    {
        memset(&_ro, 0, sizeof(_ro));
        memset(&_wo, 0, sizeof(_wo));

        _ro.hEvent = CreateEvent(NULL, true, false, NULL);
        assert(_ro.hEvent != INVALID_HANDLE_VALUE);

        _wo.hEvent = CreateEvent(NULL, true, false, NULL);
        assert(_wo.hEvent != INVALID_HANDLE_VALUE);
    }
    virtual ~_asyn_com()
    {
        close();

        if (_ro.hEvent != INVALID_HANDLE_VALUE)
            CloseHandle(_ro.hEvent);

        if (_wo.hEvent != INVALID_HANDLE_VALUE)
            CloseHandle(_wo.hEvent);
    }
    //�첽��
    int read(char* buf, int buf_len, int time_wait = 20)
    {
        if (!is_open())
            return 0;
        buf[0] = '\0';
        COMSTAT  stat;
        DWORD error;
        if (ClearCommError(_com_handle, &error, &stat) && error > 0) //�������
        {
            PurgeComm(_com_handle, PURGE_RXABORT | PURGE_RXCLEAR); /*������뻺����*/
            return 0;
        }

        if (!stat.cbInQue)// ������������
            return 0;

        unsigned long r_len = 0;

        buf_len = min((int)(buf_len - 1), (int)stat.cbInQue);

        if (!ReadFile(_com_handle, buf, buf_len, &r_len, &_ro)) //2000 �� ReadFile ʼ�շ��� True
        {
            if (GetLastError() == ERROR_IO_PENDING) // �����첽I/O
            {
                WaitForSingleObject(_ro.hEvent, time_wait); //�ȴ�20ms
                if (!GetOverlappedResult(_com_handle, &_ro, &r_len, false))
                {
                    if (GetLastError() != ERROR_IO_INCOMPLETE)//��������
                        r_len = 0;
                }
            }
            else
                r_len = 0;
        }

        buf[r_len] = '\0';
        return r_len;
    }
    //�첽д
    int write(char* buf, int buf_len)
    {
        if (!is_open())
            return 0;

        DWORD    error;
        if (ClearCommError(_com_handle, &error, NULL) && error > 0) //�������
            PurgeComm(_com_handle, PURGE_TXABORT | PURGE_TXCLEAR);

        unsigned long w_len = 0, o_len = 0;
        if (!WriteFile(_com_handle, buf, buf_len, &w_len, &_wo))
            if (GetLastError() != ERROR_IO_PENDING)
                w_len = 0;

        return w_len;
    }
    //�첽д
    inline int write(char* buf)
    {
        assert(buf);
        return write(buf, strlen(buf));
    }
    //�첽д, ֧�ֲ������͵������
    template<typename T>
    _asyn_com& operator << (T x)
    {
        strstream s;

        s << x;
        write(s.str(), s.pcount());

        return *this;
    }
};

//�����ܵ������͵����ڵ���Ϣ
#define ON_COM_RECEIVE WM_USER + 618  //  WPARAM �˿ں�

class _thread_com : public _asyn_com
{
protected:
    volatile HANDLE _thread_handle; //�����߳�
    volatile HWND _notify_hwnd; // ֪ͨ����
    volatile long _notify_num;//���ܶ����ֽ�(>_notify_num)����֪ͨ��Ϣ
    volatile bool _run_flag; //�߳�����ѭ����־
    void (*_func)(int port);

    OVERLAPPED _wait_o; //WaitCommEvent use

    //�߳��յ���Ϣ�Զ�����, �細�ھ����Ч, �ͳ���Ϣ, �������ڱ��
    virtual void on_receive()
    {
        if (_notify_hwnd)
            PostMessage(_notify_hwnd, ON_COM_RECEIVE, WPARAM(_port), LPARAM(0));
        else
        {
            if (_func)
                _func(_port);
        }
    }
    //�򿪴���,ͬʱ�򿪼����߳�
    virtual bool open_port()
    {
        if (_asyn_com::open_port())
        {
            _run_flag = true;
            DWORD id;
            _thread_handle = CreateThread(NULL, 0, com_thread, this, 0, &id); //�����߳�
            assert(_thread_handle);
            if (!_thread_handle)
            {
                CloseHandle(_com_handle);
                _com_handle = INVALID_HANDLE_VALUE;
            }
            else
                return true;
        }
        return false;
    }

public:
    _thread_com()
    {
        _notify_num = 0;
        _notify_hwnd = NULL;
        _thread_handle = NULL;
        _func = NULL;

        memset(&_wait_o, 0, sizeof(_wait_o));
        _wait_o.hEvent = CreateEvent(NULL, true, false, NULL);
        assert(_wait_o.hEvent != INVALID_HANDLE_VALUE);
    }
    ~_thread_com()
    {
        close();

        if (_wait_o.hEvent != INVALID_HANDLE_VALUE)
            CloseHandle(_wait_o.hEvent);
    }
    //�趨����֪ͨ, �����ַ���Сֵ
    void set_notify_num(int num)
    {
        _notify_num = num;
    }
    int get_notify_num()
    {
        return _notify_num;
    }
    //����Ϣ�Ĵ��ھ��
    inline void set_hwnd(HWND hWnd)
    {
        _notify_hwnd = hWnd;
    }
    inline HWND get_hwnd()
    {
        return _notify_hwnd;
    }
    inline void set_func(void (*f)(int))
    {
        _func = f;
    }
    //�ر��̼߳�����
    virtual void close()
    {
        if (is_open())
        {
            _run_flag = false;
            SetCommMask(_com_handle, 0);
            SetEvent(_wait_o.hEvent);

            if (WaitForSingleObject(_thread_handle, 100) != WAIT_OBJECT_0)
                TerminateThread(_thread_handle, 0);

            CloseHandle(_com_handle);
            CloseHandle(_thread_handle);

            _thread_handle = NULL;
            _com_handle = INVALID_HANDLE_VALUE;
            ResetEvent(_wait_o.hEvent);
        }
    }
    /*�����߳̿���*/
    //����߳̾��
    HANDLE get_thread()
    {
        return _thread_handle;
    }
    //��ͣ�����߳�
    bool suspend()
    {
        return _thread_handle != NULL ? SuspendThread(_thread_handle) != 0xFFFFFFFF : false;
    }
    //�ָ������߳�
    bool resume()
    {
        return _thread_handle != NULL ? ResumeThread(_thread_handle) != 0xFFFFFFFF : false;
    }
    //�ؽ������߳�
    bool restart()
    {
        if (_thread_handle) /*ֻ�����д����߳�ʱ*/
        {
            _run_flag = false;
            SetCommMask(_com_handle, 0);
            SetEvent(_wait_o.hEvent);

            if (WaitForSingleObject(_thread_handle, 100) != WAIT_OBJECT_0)
                TerminateThread(_thread_handle, 0);

            CloseHandle(_thread_handle);

            _run_flag = true;
            _thread_handle = NULL;

            DWORD id;
            _thread_handle = CreateThread(NULL, 0, com_thread, this, 0, &id);
            return (_thread_handle != NULL); //�����߳�
        }
        return false;
    }

private:
    //�����߳�
    static DWORD WINAPI com_thread(LPVOID para)
    {
        _thread_com* pcom = (_thread_com*)para;


        if (!SetCommMask(pcom->_com_handle, EV_RXCHAR | EV_ERR))
            return 0;

        COMSTAT  stat;
        DWORD error;

        for (DWORD length, mask = 0; pcom->_run_flag && pcom->is_open(); mask = 0)
        {
            if (!WaitCommEvent(pcom->_com_handle, &mask, &pcom->_wait_o))
            {
                if (GetLastError() == ERROR_IO_PENDING)
                {
                    GetOverlappedResult(pcom->_com_handle, &pcom->_wait_o, &length, true);
                }
            }

            if (mask & EV_ERR) // == EV_ERR
                ClearCommError(pcom->_com_handle, &error, &stat);

            if (mask & EV_RXCHAR) // == EV_RXCHAR
            {
                ClearCommError(pcom->_com_handle, &error, &stat);
                if (stat.cbInQue > pcom->_notify_num)
                    pcom->on_receive();
            }
        }

        return 0;
    }

};
typedef _thread_com _com; //���Ƽ�

#endif //_COM_H_

//int _tmain(int argc, _TCHAR* argv[])
//{
//    printf("kmbox����ͨ��demo\r\n");
//    char buff[1024];		        //�����շ����ݻ�����
//    _com myserial;				      //��������Ҫ�Ĵ�����
//    myserial.open(3, 115200);	  //�򿪴��ں�COM33 ������115200
//    sprintf_s(buff, "%s", "km.move(100,0)\r\n");//�򴮿�Ҫ��������(�������10����λ)
//    printf("PC->kmbox:%s", buff);//
//    myserial.write(buff);		    //���ڷ�������
//    Sleep(10);				          //�ȴ�10ms(���ڴ�������Ҫʱ�䣬kmbox�ڲ�����ִ��Ҳ��Ҫʱ�䡣)
//    myserial.read(buff, 20);		  //���մ������ݣ���ʱʱ��Ϊ20ms���еĺ�������Ҫ���룬���Բ���Ҫ���ա�����뾫ȷ�շ����Զ��崮��Э�飩
//    printf("kmbox->pc:%s", buff);
//    getchar();					        //��ͣ ���������ַ��˳�����
//    myserial.close();			      //�رմ���
//    return 0;
//}