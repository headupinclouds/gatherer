/**
 * Copyright (c) 2012 Nokia Corporation.
 */

#ifndef PROFILINGDATA_H
#define PROFILINGDATA_H

#include <qglobal.h>
#include <EGL/egl.h>

// Function pointer to query the graphics memory information.
typedef EGLBoolean (*PFNEGLQUERYPROFILINGDATANOK)(EGLDisplay, EGLint, EGLint *,
                                                  EGLint, EGLint *);

/*!
  \class ProfilingData
  \brief Gets graphics memory information by using eglQueryProfilingDataNOK
         extension.
*/

class ProfilingData {
public:

    /*!
      Constructor.
    */
    ProfilingData()
        : m_totalMemory(0),
          m_usedMemory(0),
          m_privateMemory(0),
          m_sharedMemory(0)
    {
    }

    /*!
      Resolves profiling data information about the available graphics memory.
      Returns false if the function fails. After succesful call to this function
      the data can be read from member variables.
    */
    bool resolveData()
    {
        PFNEGLQUERYPROFILINGDATANOK query;
        EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

        if (display != EGL_NO_DISPLAY) {
            query = reinterpret_cast<PFNEGLQUERYPROFILINGDATANOK>(
                        eglGetProcAddress("eglQueryProfilingDataNOK"));

            if (!query) {
                return false;
            }

            eglInitialize(display, 0, 0);

            QByteArray buffer;
            EGLint count = 0;

            query(display,
                  EGL_PROF_QUERY_MEMORY_USAGE_BIT_NOK |
                  EGL_PROF_QUERY_GLOBAL_BIT_NOK,
                  NULL, 0, &count);

            buffer.resize(count * sizeof(EGLint));
            buffer.fill(0);

            EGLint *data = reinterpret_cast<EGLint *>(buffer.data());

            query(display,
                  EGL_PROF_QUERY_MEMORY_USAGE_BIT_NOK |
                  EGL_PROF_QUERY_GLOBAL_BIT_NOK,
                  data, count, &count);

            for (int i=0; i<count; i++) {
                switch (data[i]) {
                case EGL_PROF_TOTAL_MEMORY_NOK: {
                    m_totalMemory = data[++i];
                    qDebug() << "Total gfx memory: "
                             << m_totalMemory / 1024 / 1024
                             << " MB";
                    break;
                }
                case EGL_PROF_USED_MEMORY_NOK: {
                    m_usedMemory = data[++i];
                    qDebug() << "Used memory: "
                             << m_usedMemory / 1024 / 1024
                             << " MB";
                    break;
                }
                case EGL_PROF_PROCESS_USED_PRIVATE_MEMORY_NOK: {
                    m_privateMemory = data[++i];
                    break;
                }
                case EGL_PROF_PROCESS_USED_SHARED_MEMORY_NOK: {
                    m_sharedMemory = data[++i];
                    break;
                }
                default:
                    // Unknown data
                    break;
                }
            }
        }
        else {
            return false;
        }

        return true;
    }

public:
    qint64 m_totalMemory;
    qint64 m_usedMemory;
    qint64 m_privateMemory;
    qint64 m_sharedMemory;
};

#endif // PROFILINGDATA_H
