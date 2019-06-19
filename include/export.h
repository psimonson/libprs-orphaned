#define PRS_EXPORT_H

#ifdef _WIN32
#define PRS_EXPORT __declspec(dllexport)
#else
#define PRS_EXPORT extern
#endif
