//http://en.cppreference.com/w/c/header
//1.1C89头文件(15)
#include <assert.h> //诊断库
#include <ctype.h>  //字符处理函数库
#include <errno.h>  //错误定义
#include <float.h>  //浮点数处理
#include <limits.h> //定义各种数据类型最值的常量
#include <locale.h> //定义本地化C函数
#include <math.h>   //定义数学函数//21: fabs_sqrt_exp_frexp_ldexp_log_log10_pow_modf_fmod_ceil_floor; sin_cos_sinh_cosh_tanh_asin_acos_atan_atan2; 
#include <setjmp.h> //异常处理支持
#include <signal.h> //信号机制支持
#include <stdarg.h> //可变参数列表支持
#include <stddef.h> //常用常量
#include <stdio.h>  //定义输入输出函数
#include <stdlib.h> //定义杂项函数及内存分配函数//malloc_free
#include <string.h> //字符串处理	//memset_memcpy
#include <time.h>   //定义关于时间的函数
//1.2C95头文件(3)
#include <iso646.h> //对应各种运算符的宏
#include <wchar.h>  //宽字符处理及输入输出
#include <wctype.h> //宽字符分类
//1.3C99头文件(6)
#ifdef C99
#include <complex.h> //复数处理
#include <fenv.h>	 //浮点环境
#include <inttypes.h>//整型转换
#include <stdbool.h> //布尔环境
#include <stdint.h>	 //整型环境
#include <tgmath.h>  //通用数学宏
#endif
//1.4C11头文件(5)
#ifdef C11
#include <stdalign.h>    //对齐方式
#include <stdatomic.h>	 //原子类型支持
#include <stdnoreturn.h> //noreturn宏
#include <threads.h>     //多线程支持
#include <uchar.h>	     //Uni字符支持
#endif

//http://en.cppreference.com/w/cpp/header
//2.1C++98与C++03头文件(50)
#include <cassert>     //a诊断库
#include <cctype>      //a字符处理函数库
#include <cerrno>      //a定义错误码
#include <cfloat>      //a浮点数处理
#include <ciso646>     //a对应各种运算符的宏
#include <climits>     //a定义各种数据类型最值的常量
#include <clocale>     //a定义本地化函数
#include <cmath>       //a定义数学函数
#include <csetjmp>     //a异常处理支持
#include <csignal>     //a信号机制支持
#include <cstdarg>     //a可变参数列表支持
#include <cstddef>     //a常用常量
#include <cstdio>      //a定义输入输出函数
#include <cstdlib>     //a定义杂项函数及内存分配函数
#include <cstring>     //a字符串处理
#include <ctime>       //a定义关于时间的函数
#include <cwchar>      //a宽字符处理及输入输出
#include <cwctype>     //a宽字符类型---18---18
#include <deque>       //bSTL双端队列容器
#include <list>        //bSTL线性列表容器
#include <vector>      //bSTL动态数组容器
#include <set>         //bSTL集合容器
#include <map>         //bSTL映射容器
#include <queue>       //bSTL队列容器
#include <stack>       //bSTL堆栈容器---7---25
#include <algorithm>   //cSTL通用算法
#include <string>      //c字符串类
#include <iterator>    //cSTL迭代器---3---28
#include <memory>      //dSTL通过分配器进行的内存分配
#include <new>         //d动态内存分配---2---30
#include <ios>         //e基本输入输出支持
#include <iosfwd>      //e输入输出系统使用的前置声明
#include <iomanip>     //e参数化输入输出
#include <iostream>    //e数据流输入输出
#include <istream>     //e基本输入流
#include <ostream>     //e基本输出流
#include <fstream>     //e文件输入输出
#include <sstream>     //e基于字符串的流
#include <streambuf>   //e底层输入输出支持---9---39      #include <strstream>//4C风格的字符串流的输入输出操作。(C++03中标明为deprecated而改用<sstream>中的stringstream)
#include <exception>   //f异常处理类
#include <stdexcept>   //f标准异常类---2---41
#include <complex>     //g复数类(注意前面没有c)
#include <limits>      //g定义各种数据类型最值常量(注意前面没有c)
#include <numeric>     //gSTL常用的数字操作
#include <valarray>    //g对包含值的数组的操作---4---45
#include <locale>      //h本地化特定信息(注意前面没有c)---1---46		
#include <bitset>      //iSTL位集容器
#include <functional>  //iSTL定义运算函数（代替运算符）
#include <typeinfo>    //i运行期间类型信息
#include <utility>     //iSTL通用模板类---4---50
//2.2C++11与C++14头文件(25)
#include <typeindex>       //a
#include <type_traits>     //a
#include <chrono>          //a
#include <initializer_list>//a
#include <tuple>           //a
#include <scoped_allocator>//b
#include <cstdint>         //c
#include <cinttypes>       //c
#include <system_error>    //d      //#include <cuchar>//e
#include <array>           //f
#include <forward_list>    //f
#include <unordered_map>   //f
#include <unordered_set>   //f
#include <random>          //g
#include <ratio>           //g
#include <cfenv>           //g      //#include <codecvt>//h
#include <regex>           //i
#include <atomic>          //j
#include <thread>          //k 
#include <mutex>           //k      //#include <shared_mutex>//kC++14
#include <future>          //k
#include <condition_variable>//k
//2.3C++17与C++20头文件(7)
#ifdef CXX17
#include <any>
#include <optional>
#include <variant>
#include <memory_resource>
#include <string_view>
#include <execution>
#include <filesystem>
#endif
//2.4System头文件
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
//2.5消除系统预定义宏
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
#ifdef ROS
#include <ros/ros.h>
#endif

//4.1命名空间
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
//4.2深度类型
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef long long llong;
typedef unsigned long long ullong;
//4.3简单函数宏
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
#define __limituchar(v)  (uchar)((uint)(v) <= UCHAR_MAX/*是正数且小于等于最大*/ ? (v) : (v) > 0 /*是正数但大于最大*/? UCHAR_MAX : 0/*是负数*/)
#define __limitushort(v)  (uchar)((uint)(v) <= USHRT_MAX/*是正数且小于等于最大*/ ? (v) : (v) > 0 /*是正数但大于最大*/? USHRT_MAX : 0/*是负数*/)
#define __limitchar(v) (char)((uint)((v) - CHAR_MIN) <= UCHAR_MAX ? (v) : (v) > 0 ? CHAR_MAX : CHAR_MIN)
#define __limitshort(v) (short)((uint)((v) - SHRT_MIN) <= USHRT_MAX ? (v) : (v) > 0 ? SHRT_MAX : SHRT_MIN)
//4.4地址访问宏
#define DI(src, i, colsMULchns) ((src) + (i) * (colsMULchns))
#define DIJ(src, i, j, cols) ((src) + (i) * (cols) + (j))
#define DIJK(src, i, j, k, cols, chns) ((src) + (i) * (cols) * (chns) + (j) * (chns) + (k))
#define DIJ2(src, i, j, cols, chns) ((src) + (i) * (cols) * (chns) + (j) * (chns)
#define IJK(srcI, j, k, chns) ((srcI) + (j) * (chns) + (k))
#define IJ(srcI, j, chns) ((srcI) + (j) * (chns))
