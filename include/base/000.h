//http://www.cplusplus.com/reference/clibrary/
//1.1C99before headers(18)
#include <assert.h>     //Conditionally compiled macro that compares its argument to zero
#include <ctype.h>      //Functions to determine the type contained in character data
#include <errno.h>      //Macros reporting error conditions
#include <float.h>      //Limits of float types
#include <iso646.h>     //Alternative operator spellings(since C95)
#include <limits.h>     //Sizes of basic types
#include <locale.h>     //Localization utilities
#include <math.h>       //Common mathematics functions  //21 functions: fabs_sqrt_exp_frexp_ldexp_log_log10_pow_modf_fmod_ceil_floor; sin_cos_sinh_cosh_tanh_asin_acos_atan_atan2; 
#include <setjmp.h>     //Nonlocal jumps
#include <signal.h>     //Signal handling
#include <stdarg.h>     //Variable arguments
#include <stddef.h>     //Common macro definitions
#include <stdio.h>      //Input/output
#include <stdlib.h>     //General utilities : memory management, program utilities, string conversions, random numbers
#include <string.h>     //String handling
#include <time.h>       //Time/date utilities
#include <wchar.h>      //Extended multibyte and wide character utilities(since C95)
#include <wctype.h>     //Functions to determine the type contained in wide character data(since C95)
//1.2C99 headers(6)
//#include <complex.h>    //Complex number arithmetic(since C99) --- conflict with boost
//#include <fenv.h>       //Floating-point environment(since C99)
//#include <inttypes.h>   //Format conversion of integer types(since C99)
//#include <stdbool.h>    //Boolean type(since C99)
//#include <stdint.h>     //Fixed-width integer types(since C99)
//#include <tgmath.h>     //Type-generic math(macros wrapping math.h and complex.h)(since C99)
//1.3C11 headers(5)
//#include <stdalign.h>   //alignas and alignof convenience macros(since C11)
//#include <stdatomic.h>  //Atomic types(since C11)
//#include <stdnoreturn.h>//noreturn convenience macros(since C11)
//#include <threads.h>    //Thread library(since C11)
//#include <uchar.h>      //UTF-16 and UTF-32 character utilities(since C11)

//http://en.cppreference.com/w/cpp/header
//2.1C++11before headers(50)
#include <cassert>
#include <cctype>
#include <cerrno>
#include <cfloat>
#include <ciso646>
#include <climits>
#include <clocale>
#include <cmath>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cwchar>
#include <cwctype>
#include <deque>
#include <list>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <stack>        //---7---25
#include <algorithm>
#include <string>
#include <iterator>     //---3---28
#include <memory>
#include <new>          //---2---30
#include <ios>
#include <iosfwd>
#include <iomanip>
#include <iostream>
#include <istream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <streambuf>    //---9---39
#include <exception>
#include <stdexcept>    //---2---41
#include <complex>
#include <limits>
#include <numeric>
#include <valarray>     //---4---45
#include <locale>       //---1---46		
#include <bitset>
#include <functional> 
#include <typeinfo>
#include <utility>      //---4---50
//2.2C++11 and C++14 headers(25)
#include <typeindex>
#include <type_traits>
#include <chrono>
#include <initializer_list>
#include <tuple>
#include <scoped_allocator>
#include <cstdint>
#include <cinttypes>
#include <system_error>
//#include <cuchar>      //Gcc not supported yet
#include <array>
#include <forward_list>
#include <unordered_map>
#include <unordered_set>
#include <random>
#include <ratio>
#include <cfenv>
//#include <codecvt>    //Deprecated in C++17
#include <regex>
#include <atomic>
#include <thread>
#include <mutex>
//#include <shared_mutex>//Gcc not supported yet
#include <future>
#include <condition_variable>


//2.4System headers
#ifdef WIN32
#include <Windows.h>
#include <Windowsx.h>
#include <CommCtrl.h>
#include <atlimage.h>//CImage
#include <conio.h>//kbhit,getch
#include <io.h>//access
#pragma warning(disable:4996)//disable sprintf to sprintf_s, getch to _getch, and so on
#pragma warning(disable:4101)//disable unreferenced local variable
#pragma warning(disable:4819)//disable save code-936 to code-unicode
#pragma warning(disable:4800)//disable force int to bool
#pragma warning(disable:4305)//disable double to float
#pragma warning(disable:4244)//disable double to float
#pragma warning(disable:4267)//disable size_t to int
#pragma warning(disable:4251)//disable for g2o
#else
#include <dirent.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#endif
//2.5Remove system's predefined macros
#undef max
#undef min

//3.1Boost_Eigen_Sophus_Ceres
#include <boost/filesystem.hpp>
#include <boost/bind.hpp>
#include <Eigen/Eigen>
#include <sophus/so2.hpp>
#include <sophus/so3.hpp>
#include <sophus/se2.hpp>
#include <sophus/se3.hpp>
#include <sophus/rxso2.hpp>
#include <sophus/rxso3.hpp>
#include <sophus/sim2.hpp>
#include <sophus/sim3.hpp>
#include <ceres/ceres.h>
//3.2OpenCV_PCL
#include <opencv2/opencv.hpp>
#include <opencv2/core/eigen.hpp>
#include <opencv2/viz.hpp>
#include <opencv2/aruco/charuco.hpp>
#include <opencv2/ximgproc.hpp>
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/filters/statistical_outlier_removal.h>
//3.3QT_ROS
#include <QtGui/QtGui>
#include <QtCore/QtCore>
#include <QtWidgets/QtWidgets>
#include <QtNetwork/QtNetwork>
#include <QtSerialPort/QtSerialPort>
#include <QtMultimedia/QtMultimedia>
#include <QtMultimediaWidgets/QtMultimediaWidgets>
#define QT_WIDGETS_LIB //For finding QSqlRelationalDelegate
#include <QtSql/QtSql>
#include <QtCharts/QtCharts>
#include <QtDataVisualization/QtDataVisualization>
#ifdef ROS
#include <ros/ros.h>
#endif

//4.1Namespaces
using namespace std;
using namespace Eigen;
using Sophus::SO2f;
using Sophus::SO2d;
using Sophus::SO3f;
using Sophus::SO3d;
using Sophus::RxSO2f;
using Sophus::RxSO2d;
using Sophus::RxSO3f;
using Sophus::RxSO3d;
using Sophus::SE2f;
using Sophus::SE2d;
using Sophus::SE3f;
using Sophus::SE3d;
using Sophus::Sim2f;
using Sophus::Sim2d;
using Sophus::Sim3f;
using Sophus::Sim3d;
using Sophus::Vector6f;
using Sophus::Vector6d;
using namespace cv;
using namespace pcl;
//4.2Simplify certain types
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef long long llong;
typedef unsigned long long ullong;
//4.3Short functions's macros
#ifndef __min
#define __min(a,b) ((a) < (b) ? (a) : (b))
#endif
#ifndef __max
#define __max(a,b) ((a) > (b) ? (a) : (b))
#endif
#define __abs(x) ((x) > 0 ? (x): (-(x)))
#define __ceil(x) (int)((x) + 0.999999999999)
#define __floor(x) (int)(x)
#define __round(x) (((x) + 0.5) >= (int)(x) + 1 ? (int)(x) + 1 : (int)(x))
#define __swap(a, b, tmp) {(tmp) = (a); (a) = (b); (b) = (tmp);}
#define __minmax(a, b, t)  ((a) > (b) ? ((t) = (a), (a) = (b), (b) = (t)): 0)
#define __limituchar(v)  (uchar)((uint)(v) <= UCHAR_MAX/*positive and <= max*/ ? (v) : (v) > 0 /*postive but > max*/? UCHAR_MAX : 0/*negative*/)
#define __limitushort(v)  (uchar)((uint)(v) <= USHRT_MAX/*positive and <= max*/ ? (v) : (v) > 0 /*postive but > max*/? USHRT_MAX : 0/*negative*/)
#define __limitchar(v) (char)((uint)((v) - CHAR_MIN) <= UCHAR_MAX ? (v) : (v) > 0 ? CHAR_MAX : CHAR_MIN)
#define __limitshort(v) (short)((uint)((v) - SHRT_MIN) <= USHRT_MAX ? (v) : (v) > 0 ? SHRT_MAX : SHRT_MIN)
//4.4Addresses' macros 
#define DI(src, i, colsMULchns) ((src) + (i) * (colsMULchns))
#define DIJ(src, i, j, cols) ((src) + (i) * (cols) + (j))
#define DIJK(src, i, j, k, cols, chns) ((src) + (i) * (cols) * (chns) + (j) * (chns) + (k))
#define DIJ2(src, i, j, cols, chns) ((src) + (i) * (cols) * (chns) + (j) * (chns)
#define IJK(srcI, j, k, chns) ((srcI) + (j) * (chns) + (k))
#define IJ(srcI, j, chns) ((srcI) + (j) * (chns))
