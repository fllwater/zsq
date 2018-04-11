#include "000.h"

#ifndef __aaa_h__
#define __aaa_h__

namespace aaa
{
	template<typename depth> string num2string(depth v, int decimalDigits = 0)
	{
		char format[10]; sprintf(format, "%%.%dlf", decimalDigits);
		char result[50]; sprintf(result, format, (double)v);
		return string(result);
	}
	void integral(Mat_<uchar> src, Mat_<int> dst)
	{	//每点的值包含此点所在行列的值
		int rows = src.rows, cols = src.cols, total = (int)src.total();
		int i, j;

		dst(0, 0) = src(0, 0);//行0, 列0

		for (j = 1; j < cols; ++j)//行0, 列[1, cols)
			dst(0, j) = src(0, j) + dst(0, j - 1);

		for (i = 1; i < rows; ++i)//行[1, rows), 列0
			dst(i, 0) = src(i, 0) + dst(i - 1, 0);

		for (i = 1; i < rows; ++i)//行[1,rows), 列[1,cols)
			for (j = 1; j < cols; ++j)
				dst(i, j) = src(i, j) + dst(i - 1, j) + dst(i, j - 1) - dst(i - 1, j - 1);

#if 0//For test
		void integralTest()
		{
			Mat_<uchar> src(480, 640); randu(src, 0, 255);

			Mat_<int> dst0(src.rows + 1, src.cols + 1);
			ullong t1 = clock();
			integral(src, dst0, CV_32S);
			cout << endl << "time cost cv: " << (clock() - t1) << endl;

			Mat_<int> dst1(src.rows, src.cols);
			t1 = clock();
			aaa::integral(src, dst1);
			cout << endl << "time cost: " << (clock() - t1) << endl;

			abb::calcErr(dst0(Rect(1, 1, src.cols, src.rows)), dst1);
		}
#endif
	}
	void resize(Mat_<uchar> src, Mat_<uchar> dst)
	{
		int rows = src.rows, cols = src.cols, total = (int)src.total(), dstrows = dst.rows, dstcols = dst.cols, dsttotal = (int)dst.total();
		int i, j, ii, jj, rows_ = rows - 1, cols_ = cols - 1, lastrow = rows_ * cols;
		int idist1, idist2, idist3, idist4, jdist1, jdist2, jdist3, jdist4;
		float x, y, fx1, fy1, fx2, fy2, fx3, fy3, fx4, fy4;
		float scalex = cols / (float)dstcols;
		float scaley = rows / (float)dstrows;

		for (i = 0; i < dstrows; ++i)
			for (j = 0; j < dstcols; ++j)
			{
				x = j * scalex; jj = (int)x;//与remap的区别是计算x而不是直接读取
				y = i * scaley; ii = (int)y;//与remap的区别是计算y而不是直接读取

				fx1 = fx4 = x - jj; fx3 = fx2 = 1.f - fx1;
				fy1 = fy2 = y - ii; fy3 = fy4 = 1.f - fy1;
#if 1//若编译器对非连续地址访问无优化
				jdist1 = jj;        jdist3 = jdist1 + 1;
				idist1 = ii * cols; idist3 = idist1 + cols;

				if (jdist1 < 0) jdist1 = 0; else if (jdist1 > cols_) jdist1 = cols_;
				if (jdist3 < 0) jdist3 = 0; else if (jdist3 > cols_) jdist3 = cols_;
				if (idist1 < 0) idist1 = 0; else if (idist1 > lastrow) idist1 = lastrow;
				if (idist3 < 0) idist3 = 0; else if (idist3 > lastrow) idist3 = lastrow;

				jdist2 = jdist3; jdist4 = jdist1;
				idist2 = idist1; idist4 = idist3;

				dst(i, j) = (uchar)(
					*(src.data + idist1 + jdist1) * fx3 * fy3 +
					*(src.data + idist3 + jdist3) * fx1 * fy1 +
					*(src.data + idist2 + jdist2) * fx4 * fy4 +
					*(src.data + idist4 + jdist4) * fx2 * fy2);//不可能为负数所以直接强制转换即可
#else//若编译器对非连续地址访问有优化
				jdist1 = jj; jdist3 = jdist1 + 1;
				idist1 = ii; idist3 = idist1 + 1;

				if (jdist1 < 0) jdist1 = 0; else if (jdist1 > cols_) jdist1 = cols_;
				if (jdist3 < 0) jdist3 = 0; else if (jdist3 > cols_) jdist3 = cols_;
				if (idist1 < 0) idist1 = 0; else if (idist1 > rows_) idist1 = rows_;
				if (idist3 < 0) idist3 = 0; else if (idist3 > rows_) idist3 = rows_;

				jdist2 = jdist3; jdist4 = jdist1;
				idist2 = idist1; idist4 = idist3;

				dst(i, j) = (uchar)(
					src(idist1, jdist1) * fx3 * fy3 +
					src(idist3, jdist3) * fx1 * fy1 +
					src(idist2, jdist2) * fx4 * fy4 +
					src(idist4, jdist4) * fx2 * fy2);//不可能为负数所以直接强制转换即可
#endif
			}
#if 0
		void resizeTest()
		{
			Mat_<uchar> src = imread("./../data/nao.jpg", 0);
			Mat_<uchar> dst(src.rows * 3, src.cols * 3);

			ullong t1 = clock();
			aaa::resize(src, dst);
			cout << endl << "time cost: " << (clock() - t1) << endl;

			imshow("src", src);
			imshow("dst", dst);
			waitKey(0);
		}
#endif
	}
	void remap(Mat_<uchar> src, Mat_<uchar> dst, Mat_<float> mapx, Mat_<float> mapy)
	{
		int rows = dst.rows, cols = dst.cols, total = (int)dst.total();
		int i, j, ii, jj, rows_ = rows - 1, cols_ = cols - 1, lastrow = rows_ * cols;
		int idist1, idist2, idist3, idist4, jdist1, jdist2, jdist3, jdist4;
		float x, y, fx1, fy1, fx2, fy2, fx3, fy3, fx4, fy4;
		for (i = 0; i < rows; ++i)
			for (j = 0; j < cols; ++j)
			{
				x = mapx(i, j); jj = (int)x;//与resize的区别是直接读取x而不是计算
				y = mapy(i, j); ii = (int)y;//与resize的区别是直接读取y而不是计算

				fx1 = fx4 = x - jj; fx3 = fx2 = 1.f - fx1;
				fy1 = fy2 = y - ii; fy3 = fy4 = 1.f - fy1;
#if 1//若编译器对非连续地址访问无优化
				jdist1 = jj;        jdist3 = jdist1 + 1;
				idist1 = ii * cols; idist3 = idist1 + cols;

				if (jdist1 < 0) jdist1 = 0; else if (jdist1 > cols_) jdist1 = cols_;
				if (jdist3 < 0) jdist3 = 0; else if (jdist3 > cols_) jdist3 = cols_;
				if (idist1 < 0) idist1 = 0; else if (idist1 > lastrow) idist1 = lastrow;
				if (idist3 < 0) idist3 = 0; else if (idist3 > lastrow) idist3 = lastrow;

				jdist2 = jdist3; jdist4 = jdist1;
				idist2 = idist1; idist4 = idist3;

				dst(i, j) = (uchar)(
					*(src.data + idist1 + jdist1) * fx3 * fy3 +
					*(src.data + idist3 + jdist3) * fx1 * fy1 +
					*(src.data + idist2 + jdist2) * fx4 * fy4 +
					*(src.data + idist4 + jdist4) * fx2 * fy2);//不可能为负数所以直接强制转换即可
#else//若编译器对非连续地址访问有优化
				jdist1 = jj; jdist3 = jdist1 + 1;
				idist1 = ii; idist3 = idist1 + 1;

				if (jdist1 < 0) jdist1 = 0; else if (jdist1 > cols_) jdist1 = cols_;
				if (jdist3 < 0) jdist3 = 0; else if (jdist3 > cols_) jdist3 = cols_;
				if (idist1 < 0) idist1 = 0; else if (idist1 > rows_) idist1 = rows_;
				if (idist3 < 0) idist3 = 0; else if (idist3 > rows_) idist3 = rows_;

				jdist2 = jdist3; jdist4 = jdist1;
				idist2 = idist1; idist4 = idist3;

				dst(i, j) = (uchar)(
					src(idist1, jdist1) * fx3 * fy3 +
					src(idist3, jdist3) * fx1 * fy1 +
					src(idist2, jdist2) * fx4 * fy4 +
					src(idist4, jdist4) * fx2 * fy2);//不可能为负数所以直接强制转换即可
#endif
			}
#if 0
		//参见resize的验证
#endif
	}

	//rule1: 惯用变量名halfkrows=krows>>1, krows_=krows-1, rows_=rows-1, trueRows_=rows-krows, trueRows=rows-krows_, halfkrowsEx = halfkrows + 1, trueRowsEx = trueRows + halfkrows, rowsEx = rows + halfkrows
	//rule2：对于局部滤波或相同原理的操作, 若行(列类似)之间无依赖则索引区域为[0, trueRows)且idst = isrc + halfrows
	//rule3: 对于局部滤波或相同原理的操作, 若有依赖则初始化索引区域为[0, krows)而主循环索引区域为[krows, rows)且idst = isrc - halfkrows
	//rule4: 面指针命名src, 线指针命名srcI, 点指针命名为srcJ, 指针长度为单维时用[]访问, 否则通过DIJ访问
	//tips1: 若行坐标i, 则i之前有i行(不含i), i之后有rows-i行(含i)
	//tips2: 若行坐标i, 则i±k在以i为0开始计数的第k个坐标

	void sepFilter(Mat_<double> src, Mat_<double> dst, Mat_<double> kernelY, Mat_<double> kernelX)
	{
		//维度变量(根据实际情况清理)
		int rows = src.rows, cols = src.cols, total = (int)src.total(), krows = (int)kernelY.total(), kcols = (int)kernelX.total();
		int halfkrows = krows >> 1, krows_ = krows - 1, rows_ = rows - 1, trueRows_ = rows - krows, trueRows = rows - krows_, halfkrowsEx = halfkrows + 1, trueRowsEx = trueRows + halfkrows, rowsEx = rows + halfkrows;
		int halfkcols = kcols >> 1, kcols_ = kcols - 1, cols_ = cols - 1, trueCols_ = cols - kcols, trueCols = cols - kcols_, halfkcolsEx = halfkcols + 1, trueColsEx = trueCols + halfkcols, colsEx = cols + halfkcols;
		//步长变量(根据实际情况清理)
		int i, j, k, ii, jj, kk;
		//中间变量
		double tmp;
		Mat_<double> buf(1, kcols);
#if 0//不处理边界
		for (i = 0; i < trueRows; ++i)
		{
			//列初始化
			for (j = 0; j < kcols; ++j)
			{
				buf(j) = 0;
				for (ii = 0; ii < krows; ++ii)
					buf(j) += kernelY(ii) * src(i + ii, j);
			}

			//列初始化对应的执行
			j = kcols_;
			dst(i + halfkrows, j - halfkcols) = 0;
			for (jj = 0; jj < kcols; ++jj)
				dst(i + halfkrows, j - halfkcols) += kernelX(jj) * buf(jj);

			//列主循环	
			for (j = kcols; j < cols; ++j)
			{
				for (jj = 0; jj < kcols_; ++jj)
					buf(jj) = buf(jj + 1);

				buf(kcols_) = 0;
				for (ii = 0; ii < krows; ++ii)
					buf(kcols_) += kernelY(ii) * src(i + ii, j);

				dst(i + halfkrows, j - halfkcols) = 0;
				for (jj = 0; jj < kcols; ++jj)
					dst(i + halfkrows, j - halfkcols) += kernelX(jj) * buf(jj);
			}
		}
#else//处理边界
		for (i = -halfkrows; i < trueRowsEx; ++i)
		{
			//列初始化
			for (j = 0; j < halfkcolsEx; ++j)
			{
				buf(j) = buf(j + halfkcols) = 0;
				for (ii = 0; ii < krows; ++ii)
				{
					buf(j) += kernelY(ii) * src(__max(__min(i + ii, rows_), 0), 0);
					buf(j + halfkcols) += kernelY(ii) * src(__max(__min(i + ii, rows_), 0), j);
				}
			}

			//列初始化对应的执行
			j = halfkcols;
			dst(i + halfkrows, j - halfkcols) = 0;
			for (jj = 0; jj < kcols; ++jj)
				dst(i + halfkrows, j - halfkcols) += kernelX(jj) * buf(jj);

			//列主循环	
			for (j = halfkcolsEx; j < colsEx; ++j)
			{
				for (jj = 0; jj < kcols_; ++jj)
					buf(jj) = buf(jj + 1);

				buf(kcols_) = 0;
				for (ii = 0; ii < krows; ++ii)
					buf(kcols_) += kernelY(ii) * src(__max(__min(i + ii, rows_), 0), __min(j, cols_));

				dst(i + halfkrows, j - halfkcols) = 0;
				for (jj = 0; jj < kcols; ++jj)
					dst(i + halfkrows, j - halfkcols) += kernelX(jj) * buf(jj);
			}
		}
#endif

#if 0 //For test
		void sepFilterTest()
		{
			int rows = 480;
			int cols = 640;
			int krows = 17;
			int kcols = 21;
			double sigmaY = 0.3*((krows - 1)*0.5 - 1) + 0.8;
			double sigmaX = 0.3*((kcols - 1)*0.5 - 1) + 0.8;
			Mat_<double> kernelY = getGaussianKernel(krows, sigmaY);
			Mat_<double> kernelX = getGaussianKernel(kcols, sigmaX);

			Mat_<double> src(rows, cols);  randu(src, 0, 255);

			Mat_<double> dst0(rows, cols);
			ullong t1 = clock();
			GaussianBlur(src, dst0, Size(kcols, krows), sigmaX, sigmaY, cv::BORDER_REPLICATE);
			cout << endl << "time cost cv: " << (clock() - t1) << endl;

			Mat_<double> dst1(rows, cols, (double)0);
			t1 = clock();
			aaa::sepFilter(src, dst1, kernelY, kernelX);
			cout << endl << "time cost cv: " << (clock() - t1) << endl;

			//Rect roi(kcols / 2, krows / 2, cols - kcols + 1, rows - krows + 1);
			Rect roi(0, 0, cols, rows);
			abb::calcErr(dst0(roi), dst1(roi));
		}
#endif
	}
	void minFilter(Mat_<uchar> src, Mat_<uchar> dst, int krows, int kcols)
	{
		//维度变量(根据实际情况清理)
		int rows = src.rows, cols = src.cols, total = (int)src.total();
		int halfkrows = krows >> 1, krows_ = krows - 1, rows_ = rows - 1, trueRows_ = rows - krows, trueRows = rows - krows_, halfkrowsEx = halfkrows + 1, trueRowsEx = trueRows + halfkrows, rowsEx = rows + halfkrows;
		int halfkcols = kcols >> 1, kcols_ = kcols - 1, cols_ = cols - 1, trueCols_ = cols - kcols, trueCols = cols - kcols_, halfkcolsEx = halfkcols + 1, trueColsEx = trueCols + halfkcols, colsEx = cols + halfkcols;
		//步长变量(根据实际情况清理)
		int i, j, k, ii, jj, kk;
		//中间变量
		uchar tmp;
		Mat_<uchar> buf(1, kcols);
#if 0//不处理边界
		for (i = 0; i < trueRows; ++i)
		{
			//列初始化
			for (j = 0; j < kcols; ++j)
			{
				buf(j) = UCHAR_MAX;
				for (ii = 0; ii < krows; ++ii)
					buf(j) = __min(buf(j), src(i + ii, j));
			}

			//列初始化对应的执行
			j = kcols_;
			dst(i + halfkrows, j - halfkcols) = UCHAR_MAX;
			for (jj = 0; jj < kcols; ++jj)
				dst(i + halfkrows, j - halfkcols) = __min(dst(i + halfkrows, j - halfkcols), buf(jj));

			//列主循环	
			for (j = kcols; j < cols; ++j)
			{
				for (jj = 0; jj < kcols_; ++jj)
					buf(jj) = buf(jj + 1);

				buf(kcols_) = UCHAR_MAX;
				for (ii = 0; ii < krows; ++ii)
					buf(kcols_) = __min(buf(kcols_), src(ii + i, j));

				dst(i + halfkrows, j - halfkcols) = UCHAR_MAX;
				for (jj = 0; jj < kcols; ++jj)
					dst(i + halfkrows, j - halfkcols) = __min(dst(i + halfkrows, j - halfkcols), buf(jj));
			}
		}
#else
		for (i = -halfkrows; i < trueRowsEx; ++i)
		{
			//列初始化
			for (j = 0; j < halfkcolsEx; ++j)
			{
				buf(j) = buf(j + halfkcols) = UCHAR_MAX;
				for (ii = 0; ii < krows; ++ii)
				{
					buf(j) = __min(buf(j), src(__max(__min(i + ii, rows_), 0), 0));
					buf(j + halfkcols) = __min(buf(j + halfkcols), src(__max(__min(i + ii, rows_), 0), j));
				}
			}

			//列初始化对应的执行
			j = halfkcols;
			dst(i + halfkrows, j - halfkcols) = UCHAR_MAX;
			for (jj = 0; jj < kcols; ++jj)
				dst(i + halfkrows, j - halfkcols) = __min(dst(i + halfkrows, j - halfkcols), buf(jj));

			//列主循环	
			for (j = halfkcolsEx; j < colsEx; ++j)
			{
				for (jj = 0; jj < kcols_; ++jj)
					buf(jj) = buf(jj + 1);

				buf(kcols_) = UCHAR_MAX;
				for (ii = 0; ii < krows; ++ii)
					buf(kcols_) = __min(buf(kcols_), src(__max(__min(ii + i, rows_), 0), __min(j, cols_)));

				dst(i + halfkrows, j - halfkcols) = UCHAR_MAX;
				for (jj = 0; jj < kcols; ++jj)
					dst(i + halfkrows, j - halfkcols) = __min(dst(i + halfkrows, j - halfkcols), buf(jj));
			}
		}
#endif
#if 0 //For test
		void minFilterTest()
		{
			int rows = 480;
			int cols = 640;
			int krows = 17;
			int kcols = 21;

			Mat_<uchar> src(rows, cols, uchar(0)); randu(src, 0, 255);//cout << endl << src << endl;

			Mat_<uchar> dst0(rows, cols);
			ullong t1 = clock();
			erode(src, dst0, Mat_<uchar>(krows, kcols, (uchar)1), Point(-1, -1), 1, cv::BORDER_REPLICATE); //cout << endl << dstcv << endl;
			cout << endl << "time cost: " << (clock() - t1) << endl;

			Mat_<uchar> dst1(rows, cols, (uchar)0);
			t1 = clock();
			aaa::minFilter(src, dst1, krows, kcols); //cout << endl << dst << endl;
			cout << endl << "time cost: " << (clock() - t1) << endl;

			//Rect roi(kcols / 2, krows / 2, cols - kcols + 1, rows - krows + 1);
			Rect roi(0, 0, cols, rows);
			abb::calcErr(dst0(roi), dst1(roi));
		}
#endif
	}
	void boxFilter(Mat_<uchar> src, Mat_<int> dst, int krows, int kcols)
	{
		//维度变量(根据实际情况清理)
		int rows = src.rows, cols = src.cols, total = (int)src.total();
		int halfkrows = krows >> 1, krows_ = krows - 1, rows_ = rows - 1, trueRows_ = rows - krows, trueRows = rows - krows_, halfkrowsEx = halfkrows + 1, trueRowsEx = trueRows + halfkrows, rowsEx = rows + halfkrows;
		int halfkcols = kcols >> 1, kcols_ = kcols - 1, cols_ = cols - 1, trueCols_ = cols - kcols, trueCols = cols - kcols_, halfkcolsEx = halfkcols + 1, trueColsEx = trueCols + halfkcols, colsEx = cols + halfkcols;
		//步长变量(根据实际情况清理)
		int i, j, k, ii, jj, kk;
		//中间变量
		int sum;
		Mat_<int> buf(1, cols);
#if 0//不处理边界
		//1.行初始化
		for (j = 0; j < cols; ++j)
		{
			buf(j) = 0;
			for (ii = 0; ii < krows; ++ii)
				buf(j) += src(ii, j);
		}

		//2.行初始化对应的执行
		i = krows_;
		{
			//列初始化
			sum = 0;
			for (j = 0; j < kcols; ++j)
				sum += buf(j);

			//列初始化对应的执行
			j = kcols_;
			dst(i - halfkrows, j - halfkcols) = sum;

			//列主循环
			for (j = kcols; j < cols; ++j)
			{
				sum += buf(j) - buf(j - kcols);
				dst(i - halfkrows, j - halfkcols) = sum;
			}
		}

		//3.行主循环	
		for (i = krows; i < rows; ++i)
		{
			//列初始化
			sum = 0;
			for (j = 0; j < kcols; ++j)
			{
				buf(j) += src(i, j) - src(i - krows, j);
				sum += buf(j);
			}

			//列初始化对应的执行
			j = kcols_;
			dst(i - halfkrows, j - halfkcols) = sum;

			//列主循环
			for (j = kcols; j < cols; ++j)
			{
				buf(j) += src(i, j) - src(i - krows, j);
				sum += buf(j) - buf(j - kcols);
				dst(i - halfkrows, j - halfkcols) = sum;
			}
		}
#else//处理边界
		//1.行初始化
		for (j = 0; j < cols; ++j)
		{
			buf(j) = 0;
			for (ii = 0; ii < halfkrowsEx; ++ii)
				ii == 0 ?
				buf(j) += src(ii, j) * halfkrowsEx :
				buf(j) += src(ii, j);
		}

		//2.行初始化对应的执行
		i = halfkrows;
		{
			//列初始化
			sum = 0;
			for (j = 0; j < halfkcolsEx; ++j)
				j == 0 ?//亦可采用sepFilter方式
				sum += buf(j) * halfkcolsEx :
				sum += buf(j);

			//列初始化对应的执行
			j = halfkcols;
			dst(i - halfkrows, j - halfkcols) = sum;

			//列主循环
			for (j = halfkcolsEx; j < colsEx; ++j)
			{
				sum += buf(__min(j, cols_)) - buf(__max(j - kcols, 0));
				dst(i - halfkrows, j - halfkcols) = sum;
			}
		}

		//3.行主循环	
		for (i = halfkrowsEx; i < rowsEx; ++i)
		{
			//列初始化
			sum = 0;
			for (j = 0; j < halfkcolsEx; ++j)
			{
				buf(j) += src(__min(i, rows_), j) - src(__max(i - krows, 0), j);
				j == 0 ?//亦可采用sepFilter方式
					sum += buf(j) * halfkcolsEx :
					sum += buf(j);
			}

			//列初始化对应的执行
			j = halfkcols;
			dst(i - halfkrows, j - halfkcols) = sum;

			//列主循环
			for (j = halfkcolsEx; j < colsEx; ++j)
			{
				j < cols ? buf(j) += src(__min(i, rows_), j) - src(__max(i - krows, 0), j) : 0;
				sum += buf(__min(j, cols_)) - buf(__max(j - kcols, 0));
				dst(i - halfkrows, j - halfkcols) = sum;
			}
		}

#endif
#if 0 //For test
		void boxFilterTest()
		{
			int rows = 480;
			int cols = 640;
			int krows = 17;
			int kcols = 21;

			Mat_<uchar> src(rows, cols); randu(src, 0, 255);

			Mat_<int> dst0(rows, cols);
			ullong t1 = clock();
			boxFilter(src, dst0, CV_32S, Size(kcols, krows), Point(-1, -1), false, cv::BORDER_REPLICATE);//BORDER_CONSTANT//BORDER_REPLICATE//BORDER_REFLECT_101
			cout << endl << "time cost cv: " << (clock() - t1) << endl;

			Mat_<int> dst1(rows, cols, 0);
			t1 = clock();
			aaa::boxFilter(src, dst1, krows, kcols);
			cout << endl << "time cost: " << (clock() - t1) << endl;

			//Rect roi(kcols / 2, krows / 2, cols - kcols + 1, rows - krows + 1);
			Rect roi(0, 0, cols, rows);
			abb::calcErr(dst0(roi), dst1(roi));
		}
#endif
	}
	void midFilterEx(Mat_<uchar> src, Mat_<uchar> dst, int krows, int kcols)
	{
		//维度变量(根据实际情况清理)
		int rows = src.rows, cols = src.cols, total = (int)src.total();
		int halfkrows = krows >> 1, krows_ = krows - 1, rows_ = rows - 1, trueRows_ = rows - krows, trueRows = rows - krows_, halfkrowsEx = halfkrows + 1, trueRowsEx = trueRows + halfkrows, rowsEx = rows + halfkrows;
		int halfkcols = kcols >> 1, kcols_ = kcols - 1, cols_ = cols - 1, trueCols_ = cols - kcols, trueCols = cols - kcols_, halfkcolsEx = halfkcols + 1, trueColsEx = trueCols + halfkcols, colsEx = cols + halfkcols;
		//步长变量(根据实际情况清理)
		int i, j, k, ii, jj, kk, halfCount = (krows * kcols) >> 1;
		//中间变量
		int hed, end, cnt1, cnt2, flag;
		int hist16[272], *hist256 = hist16 + 16;//记录每个灰度组和灰度级的数量
#if 0//不处理边界
		for (j = 0; j < trueCols; ++j)
		{
			flag = j & 1;

			//1.行初始化
			memset(hist16, 0, 1088);
			for (i = 0; i < krows; ++i)
				for (jj = 0; jj < kcols; ++jj)
				{
					hed = src(flag == 0 ? i : rows_ - i, j + jj);
					++hist256[hed];
					++hist16[hed >> 4];
				}

			//2.行初始化对应的执行
			i = krows_;
			{
				cnt1 = cnt2 = 0;
				for (k = 0;; ++k)
				{
					cnt2 += hist16[k];
					if (cnt2 > halfCount) break;
					cnt1 = cnt2;
				}

				for (k <<= 4;; ++k)
				{
					cnt1 += hist256[k];
					if (cnt1 > halfCount) break;
				}
				dst(flag == 0 ? i - halfkrows : rows_ - i + halfkrows, j + halfkcols) = k;
			}

			//3.行主循环
			for (i = krows; i < rows; ++i)
			{
				for (jj = 0; jj < kcols; ++jj)
				{
					hed = src(flag == 0 ? i - krows : rows_ - i + krows, j + jj);
					end = src(flag == 0 ? i : rows_ - i, j + jj);
					--hist256[hed];
					--hist16[hed >> 4];
					++hist256[end];
					++hist16[end >> 4];
				}

				cnt1 = cnt2 = 0;
				for (k = 0;; ++k)
				{
					cnt2 += hist16[k];
					if (cnt2 > halfCount) break;
					cnt1 = cnt2;
				}

				for (k <<= 4;; ++k)
				{
					cnt1 += hist256[k];
					if (cnt1 > halfCount) break;
				}
				dst(flag == 0 ? i - halfkrows : rows_ - i + halfkrows, j + halfkcols) = k;
			}
		}
#else //处理边界
		for (j = -halfkcols; j < trueColsEx; ++j)
		{
			flag = __max(__min(j, cols_), 0) & 1;

			//1.行初始化
			memset(hist16, 0, 1088);
			for (i = 0; i < halfkrowsEx; ++i)
				for (jj = 0; jj < kcols; ++jj)
				{
					hed = src(flag == 0 ? i : rows_ - i, __max(__min(j + jj, cols_), 0));
					i == 0 ?
						(hist256[hed] += halfkcolsEx, hist16[hed >> 4] += halfkcolsEx) :
						(++hist256[hed], ++hist16[hed >> 4]);
				}

			//2.行初始化对应的执行
			i = halfkrows;
			{
				cnt1 = cnt2 = 0;
				for (k = 0;; ++k)
				{
					cnt2 += hist16[k];
					if (cnt2 > halfCount) break;
					cnt1 = cnt2;
				}

				for (k <<= 4;; ++k)
				{
					cnt1 += hist256[k];
					if (cnt1 > halfCount) break;
				}
				dst(flag == 0 ? i - halfkrows : rows_ - i + halfkrows, j + halfkcols) = k;
			}

			//3.行主循环
			for (i = halfkrowsEx; i < rowsEx; ++i)
			{
				for (jj = 0; jj < kcols; ++jj)
				{
					hed = src(flag == 0 ? __max(i - krows, 0) : __min(rows_ - i + krows, rows_), __max(__min(j + jj, cols_), 0));
					end = src(flag == 0 ? __min(i, rows_) : __max(rows_ - i, 0), __max(__min(j + jj, cols_), 0));
					--hist256[hed];
					--hist16[hed >> 4];
					++hist256[end];
					++hist16[end >> 4];
				}

				cnt1 = cnt2 = 0;
				for (k = 0;; ++k)
				{
					cnt2 += hist16[k];
					if (cnt2 > halfCount) break;
					cnt1 = cnt2;
				}

				for (k <<= 4;; ++k)
				{
					cnt1 += hist256[k];
					if (cnt1 > halfCount) break;
				}
				dst(flag == 0 ? i - halfkrows : rows_ - i + halfkrows, j + halfkcols) = k;
			}
		}
#endif
#if 0 //For test
		void midFilterExTest()
		{
			int rows = 480;
			int cols = 640;
			int krows = 21;
			int kcols = krows;

			Mat_<uchar> src(rows, cols);  randu(src, 0, 255);

			Mat_<uchar> dst0(rows, cols);
			ullong t1 = clock();
			medianBlur(src, dst0, krows);
			cout << endl << "time cost cv: " << (clock() - t1) << endl;

			Mat_<uchar> dst1(rows, cols, (uchar)0);
			t1 = clock();
			aaa::midFilterEx(src, dst1, krows, kcols);
			cout << endl << "time cost cv: " << (clock() - t1) << endl;

			//Rect roi(kcols / 2, krows / 2, cols - kcols + 1, rows - krows + 1);
			Rect roi(0, 0, cols, rows);
			abb::calcErr(dst0(roi), dst1(roi));
		}
#endif
	}
	void midFilterEx3(Mat_<uchar> src, Mat_<uchar> dst)
	{	//适合有并行处理
		int rows = src.rows, cols = src.cols, total = (int)src.total();
		int i, j, t, a11, a12, a13, a21, a22, a23, a31, a32, a33;
		int rows_ = rows - 1, cols_ = cols - 1;

		for (i = 1; i < rows_; ++i)
			for (j = 1; j < cols_; ++j)
			{
				a11 = src(i - 1, j - 1), a12 = src(i - 1, j), a13 = src(i - 1, j + 1);
				a21 = src(i, j - 1), a22 = src(i, j), a23 = src(i, j + 1);
				a31 = src(i + 1, j - 1), a32 = src(i + 1, j), a33 = src(i + 1, j + 1);

				__minmax(a12, a13, t); __minmax(a22, a23, t); __minmax(a32, a33, t); __minmax(a11, a12, t);
				__minmax(a21, a22, t); __minmax(a31, a32, t); __minmax(a12, a13, t); __minmax(a22, a23, t);
				__minmax(a32, a33, t); __minmax(a11, a21, t); __minmax(a23, a33, t); __minmax(a22, a32, t);
				__minmax(a21, a31, t); __minmax(a12, a22, t); __minmax(a13, a23, t); __minmax(a22, a32, t);
				__minmax(a22, a13, t); __minmax(a31, a22, t); __minmax(a22, a13, t);
				dst(i, j) = a22;
			}

		for (i = 0; i < rows; i += rows_)
			for (j = 0; j < cols; ++j)
			{
				a11 = src(__max(i - 1, 0), __max(j - 1, 0)), a12 = src(__max(i - 1, 0), j), a13 = src(__max(i - 1, 0), __min(j + 1, cols_));
				a21 = src(i, __max(j - 1, 0)), a22 = src(i, j), a23 = src(i, __min(j + 1, cols_));
				a31 = src(__min(i + 1, rows_), __max(j - 1, 0)), a32 = src(__min(i + 1, rows_), j), a33 = src(__min(i + 1, rows_), __min(j + 1, cols_));

				__minmax(a12, a13, t); __minmax(a22, a23, t); __minmax(a32, a33, t); __minmax(a11, a12, t);
				__minmax(a21, a22, t); __minmax(a31, a32, t); __minmax(a12, a13, t); __minmax(a22, a23, t);
				__minmax(a32, a33, t); __minmax(a11, a21, t); __minmax(a23, a33, t); __minmax(a22, a32, t);
				__minmax(a21, a31, t); __minmax(a12, a22, t); __minmax(a13, a23, t); __minmax(a22, a32, t);
				__minmax(a22, a13, t); __minmax(a31, a22, t); __minmax(a22, a13, t);
				dst(i, j) = a22;
			}

		for (i = 0; i < rows; ++i)
			for (j = 0; j < cols; j += cols_)
			{
				a11 = src(__max(i - 1, 0), __max(j - 1, 0)), a12 = src(__max(i - 1, 0), j), a13 = src(__max(i - 1, 0), __min(j + 1, cols_));
				a21 = src(i, __max(j - 1, 0)), a22 = src(i, j), a23 = src(i, __min(j + 1, cols_));
				a31 = src(__min(i + 1, rows_), __max(j - 1, 0)), a32 = src(__min(i + 1, rows_), j), a33 = src(__min(i + 1, rows_), __min(j + 1, cols_));

				__minmax(a12, a13, t); __minmax(a22, a23, t); __minmax(a32, a33, t); __minmax(a11, a12, t);
				__minmax(a21, a22, t); __minmax(a31, a32, t); __minmax(a12, a13, t); __minmax(a22, a23, t);
				__minmax(a32, a33, t); __minmax(a11, a21, t); __minmax(a23, a33, t); __minmax(a22, a32, t);
				__minmax(a21, a31, t); __minmax(a12, a22, t); __minmax(a13, a23, t); __minmax(a22, a32, t);
				__minmax(a22, a13, t); __minmax(a31, a22, t); __minmax(a22, a13, t);
				dst(i, j) = a22;
			}
#if 0
		void midFilterEx3Test()
		{
			int rows = 480;
			int cols = 640;

			Mat_<uchar> src(rows, cols);  randu(src, 0, 255);

			Mat_<uchar> dst0(rows, cols);
			ullong t1 = clock();
			medianBlur(src, dst0, 3);
			cout << endl << "time cost cv: " << (clock() - t1) << endl;

			Mat_<uchar> dst1(rows, cols, (uchar)0);
			t1 = clock();
			aaa::midFilterEx3(src, dst1);
			cout << endl << "time cost cv: " << (clock() - t1) << endl;

			Rect roi(0, 0, cols, rows);
			abb::calcErr(dst0(roi), dst1(roi));
	}
#endif
	}

	void sobelXFilter(Mat_<uchar> src, Mat_<short> dst)
	{	//原理同SepFilter
		int rows = src.rows, cols = src.cols, total = (int)src.total();
		int i, j, rows_ = rows - 1, cols_ = cols - 1, trueRows = rows - 2, trueCols = cols - 2;
		short s1, s2, s3;

		for (i = 0; i < trueRows; ++i)
		{
			//列初始化
			j = 0; s1 = src(i, j) + (src(i + 1, j) << 1) + src(i + 2, j);
			++j; s2 = src(i, j) + (src(i + 1, j) << 1) + src(i + 2, j);
			++j; s3 = src(i, j) + (src(i + 1, j) << 1) + src(i + 2, j);

			//列初始化对应的执行
			j = 2;
			dst(i + 1, j - 1) = s3 - s1;

			//列主循环
			for (j = 3; j < cols; ++j)
			{
				s1 = s2; s2 = s3;
				s3 = src(i, j) + (src(i + 1, j) << 1) + src(i + 2, j);
				dst(i + 1, j - 1) = s3 - s1;
			}
		}
#if 1
		//边界处理
		for (i = 0, j = 1; j < cols_; ++j) dst(i, j) =
			-src(i/*-1*/, j - 1) + src(i/*-1*/, j + 1) +
			((-src(i, j - 1) + src(i, j + 1)) << 1) +
			-src(i + 1, j - 1) + src(i + 1, j + 1);
		for (i = rows_, j = 1; j < cols_; ++j) dst(i, j) =
			-src(i - 1, j - 1) + src(i - 1, j + 1) +
			((-src(i, j - 1) + src(i, j + 1)) << 1) +
			-src(i/*+1*/, j - 1) + src(i/*+1*/, j + 1);
		for (j = 0, i = 1; i < rows_; ++i) dst(i, j) =
			-src(i - 1, j/*-1*/) + src(i - 1, j + 1) +
			((-src(i, j/*-1*/) + src(i, j + 1)) << 1) +
			-src(i + 1, j/*-1*/) + src(i + 1, j + 1);
		for (j = cols_, i = 1; i < rows_; ++i) dst(i, j) =
			-src(i - 1, j - 1) + src(i - 1, j/*+1*/) +
			((-src(i, j - 1) + src(i, j/*+1*/)) << 1) +
			-src(i + 1, j - 1) + src(i + 1, j/*+1*/);
		i = 0; j = 0; dst(i, j) =
			-src(i /*-1*/, j/*-1*/) + src(i/*-1*/, j + 1) +
			((-src(i, j/*-1*/) + src(i, j + 1)) << 1) +
			-src(i + 1, j/*-1*/) + src(i + 1, j + 1);
		i = 0; j = cols_; dst(i, j) =
			-src(i/*-1*/, j - 1) + src(i/*-1*/, j/*+1*/) +
			((-src(i, j - 1) + src(i, j/*+1*/)) << 1) +
			-src(i + 1, j - 1) + src(i + 1, j/*+1*/);
		i = rows_; j = 0; dst(i, j) =
			-src(i - 1, j/*-1*/) + src(i - 1, j + 1) +
			((-src(i, j/*-1*/) + src(i, j + 1)) << 1) +
			-src(i/*+1*/, j/*-1*/) + src(i/*+1*/, j + 1);
		i = rows_; j = cols_; dst(i, j) =
			-src(i - 1, j - 1) + src(i - 1, j/*+1*/) +
			((-src(i, j - 1) + src(i, j/*+1*/)) << 1) +
			-src(i/*+1*/, j - 1) + src(i/*+1*/, j/*+1*/);
#endif		

#if 0 //For test
		void sobelXFilterTest()
		{
			int rows = 480;
			int cols = 640;
			Mat_<uchar> src(rows, cols); randu(src, 0, 255);

			Mat_<short> dst0(rows, cols);
			ullong t1 = clock();
			Sobel(src, dst0, CV_16S, 1, 0, 3, 1.0, 0, cv::BORDER_REPLICATE);
			cout << endl << "time cost: " << (clock() - t1) << endl;

			Mat_<short> dst1(rows, cols, (short)0);
			t1 = clock();
			aaa::sobelXFilter(src, dst1);
			cout << endl << "time cost: " << (clock() - t1) << endl;

			Rect roi(0, 0, cols, rows);
			abb::calcErr(dst0(roi), dst1(roi));
		}
#endif
	}
	void sobelYFilter(Mat_<uchar> src, Mat_<short> dst)
	{	//原理同SepFilter
		int rows = src.rows, cols = src.cols, total = (int)src.total();
		int i, j, rows_ = rows - 1, cols_ = cols - 1, trueRows = rows - 2, trueCols = cols - 2;
		short s1, s2, s3;

		for (i = 0; i < trueRows; ++i)
		{
			//列初始化
			j = 0; s1 = src(i + 2, j) - src(i, j);
			++j; s2 = src(i + 2, j) - src(i, j);;
			++j; s3 = src(i + 2, j) - src(i, j);

			//列初始化对应的执行
			j = 2;
			dst(i + 1, j - 1) = s1 + (s2 << 1) + s3;

			//列主循环
			for (j = 3; j < cols; ++j)
			{
				s1 = s2; s2 = s3;
				s3 = src(i + 2, j) - src(i, j);
				dst(i + 1, j - 1) = s1 + (s2 << 1) + s3;
			}
		}
#if 1
		//边界处理
		for (i = 0, j = 1; j < cols_; ++j) dst(i, j) =
			-src(i/*-1*/, j - 1) - (src(i/*-1*/, j) << 1) - src(i/*-1*/, j + 1)
			+ src(i + 1, j - 1) + (src(i + 1, j) << 1) + src(i + 1, j + 1);
		for (i = rows_, j = 1; j < cols_; ++j) dst(i, j) =
			-src(i - 1, j - 1) - (src(i - 1, j) << 1) - src(i - 1, j + 1)
			+ src(i/*+1*/, j - 1) + (src(i/*+1*/, j) << 1) + src(i/*+1*/, j + 1);
		for (j = 0, i = 1; i < rows_; ++i) dst(i, j) =
			-src(i - 1, j/*-1*/) - (src(i - 1, j) << 1) - src(i - 1, j + 1)
			+ src(i + 1, j/*-1*/) + (src(i + 1, j) << 1) + src(i + 1, j + 1);
		for (j = cols_, i = 1; i < rows_; ++i) dst(i, j) =
			-src(i - 1, j - 1) - (src(i - 1, j) << 1) - src(i - 1, j/*+1*/)
			+ src(i + 1, j - 1) + (src(i + 1, j) << 1) + src(i + 1, j/*+1*/);
		i = 0; j = 0; dst(i, j) =
			-src(i/*-1*/, j/*-1*/) - (src(i/*-1*/, j) << 1) - src(i/*-1*/, j + 1)
			+ src(i + 1, j/*-1*/) + (src(i + 1, j) << 1) + src(i + 1, j + 1);
		i = 0; j = cols_; dst(i, j) =
			-src(i/*-1*/, j - 1) - (src(i/*-1*/, j) << 1) - src(i/*-1*/, j/*+1*/)
			+ src(i + 1, j - 1) + (src(i + 1, j) << 1) + src(i + 1, j/*+1*/);
		i = rows_; j = 0; dst(i, j) =
			-src(i - 1, j/*-1*/) - (src(i - 1, j) << 1) - src(i - 1, j + 1)
			+ src(i/*+1*/, j/*-1*/) + (src(i/*+1*/, j) << 1) + src(i/*+1*/, j + 1);
		i = rows_; j = cols_; dst(i, j) =
			-src(i - 1, j - 1) - (src(i - 1, j) << 1) - src(i - 1, j/*+1*/)
			+ src(i/*+1*/, j - 1) + (src(i/*+1*/, j) << 1) + src(i/*+1*/, j/*+1*/);
#endif

#if 0 //For test
		void sobelYFilterTest()
		{
			int rows = 480;
			int cols = 640;
			Mat_<uchar> src(rows, cols); randu(src, 0, 255);

			Mat_<short> dst0(rows, cols);
			ullong t1 = clock();
			Sobel(src, dst0, CV_16S, 0, 1, 3, 1.0, 0, cv::BORDER_REPLICATE);
			cout << endl << "time cost: " << (clock() - t1) << endl;

			Mat_<short> dst1(rows, cols, (short)0);
			t1 = clock();
			aaa::sobelYFilter(src, dst1);
			cout << endl << "time cost: " << (clock() - t1) << endl;

			Rect roi(0, 0, cols, rows);
			abb::calcErr(dst0(roi), dst1(roi));
		}
#endif
	}

	void fprintfTest()
	{
		char path[50] = "./../data/txtfile.txt";
		int index[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		float value[9] = { 1.1f, 2.2f, 3.3f, 4.4f, 5.5f, 6.6f, 7.7f, 8.8f, 9.9f };

		FILE *out = fopen(path, "w");
		for (int i = 0; i < 9; ++i) fprintf(out, "index = %-9d \t value = %-9.3f\n", index[i], value[i]);//除\t左右的空格外其它空格都必须以方便fscanf读取, 格式取左对齐且宽度9以方便查看
		fclose(out);
	}
	void fscanfTest()
	{
		char path[50] = "./../data/txtfile.txt";
		int index[18]; memset(index, 0, 18 * sizeof(int));
		float value[18]; memset(value, 0, 18 * sizeof(float));
		char str[50];
		int i = 0, cnt = 0;

		FILE *in = fopen(path, "r");
		while (fscanf(in, "%s%s%d%s%s%f", str, str, index + i, str, str, value + i) != EOF) ++i;//自动判断是否读到最后行, 也可以通过在文件首存储行数来执行固定的循环
		fclose(in);

		for (cnt = i, i = 0; i < cnt; ++i) printf("index = %-9d \t value = %-9.3f\n", index[i], value[i]);
		getchar();
	}
	void fwriteTest()
	{
		char path[50] = "./../data/binfile.bin";
		int src[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

		FILE *out = fopen(path, "w");
		fprintf(out, "depth: %d\ntotal: %d\n", (int)sizeof(int), 9);//之间的空格一定要有以方便fscanf读取
		fwrite(src, sizeof(int), 9, out);//fwrite(value, sizeof(float)*9, 1, out);//for (int i = 0; i < 9; ++i) fwrite(index + i, sizeof(int), 1, out);//不连续分配的图像可通过循环逐行保存
		fclose(out);
	}
	void freadTest()
	{
		char path[50] = "./../data/binfile.bin";
		int src[18]; memset(src, 0, 18 * sizeof(int));
		char str[50];
		int depth, total;

		FILE *in = fopen(path, "r");
		fscanf(in, "%s%d%s%d", str, &depth, str, &total);
		fgetc(in);//跳过回车(fread不会自动跳过空字符)
		fread(src, depth, total, in);
		fclose(in);

		for (int i = 0; i < total; ++i) printf("vec = %-10d\n", src[i]);
		getchar();
	}
#ifdef WIN32
#define aaa_ns (chrono::system_clock::now().time_since_epoch().count() * 100)
#define aaa_us (chrono::system_clock::now().time_since_epoch().count() / 10)
#define aaa_ms (chrono::system_clock::now().time_since_epoch().count() / 10000)
#else
#define aaa_ns (chrono::system_clock::now().time_since_epoch().count())
#define aaa_us (chrono::system_clock::now().time_since_epoch().count() / 1000)
#define aaa_ms (chrono::system_clock::now().time_since_epoch().count() / 1000000)
#endif
}

#endif
