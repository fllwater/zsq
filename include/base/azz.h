#include "add.h"

#ifndef __azz_h__
#define __azz_h__

namespace azz
{
//CvSeq说明
//每个结点占空: 基本成员占空(CvSeq成员)+扩展成员占空(CvXXX=CvSet&CvGraph&CvChain&CvContour&CvDIY等的附加成员)+数据块占空(=元素总数*元素占空), 前两项之和就是headerSize的值(headerSize必须大于等于sizeof(CvSeq)的原因)
//操作结点成员: 基本成员用CvSeq或CvCXX指针直访(可将CvSeq和CvXXX看作基类与子类的关系), 扩展成员使用CvXXX指针直访(直接将CvSeq指针强制转换为CvXXX指针即可)
//操作结点数据: 基于CvSeq指针的函数(可看作基类的成员函数), 基于CvXXX指针的函数(可看作子类的成员的函数), OpenCV已经实际了可能的所有基类函数和关于CvSet&CvGraph&CvChain&CvContour可能的所有子类函数
//使用中的谨记: 看作vector的扩展版本, 当不使用h_pre+h_next+v_pre+v_next可看一个动态双端序列, 否则看作树中的一个结点, 但此点是一个动态双端序列

//操作CvSeq序列

//操作CvSeq树
	void cvNextTreeNode(CvTreeNodeIterator *it)
	{	//与OpenCV实现差异
		//1.无返回值, 因为OpenCV中返回值是传入的it->node, 所以返回值意义不大
		//2.无if(node)判断, 而要求外部判断并限制传入的it->node不能为空
		//3.没用CvTreeNode而直接使用CvSeq, 因为OpenCV的源码中使用的最基类是CvSeq, 所以无需使用根基类CvTreeNode
		CvSeq *node = (CvSeq*)it->node;
		//1.先走子级
		if (node->v_next)
		{
			it->node = node = node->v_next;
			++(it->level);
		}
		//2.再走同级
		else
		{
			while (node->h_next == 0)//2.1若同级不存在
			{
				node = node->v_prev;//2.2则返回到父级
				if (--(it->level) < 0)//表示已遍历完成
				{
					it->node = node = 0;
					return;
				}
			}
			it->node = node = node->h_next;//2.3定位到同级或父级的同级
		}
	}

	void cvPrevTreeNode(CvTreeNodeIterator *it)
	{	//与OpenCV实现差异
		//1.无返回值, 因为OpenCV中返回值是传入的it->node, 所以返回值意义不大
		//2.无if(node)判断, 而要求外部判断并限制传入的it->node不能为空
		//3.没用CvTreeNode而直接使用CvSeq, 因为OpenCV的源码中使用的最基类是CvSeq, 所以无需使用根基类CvTreeNode
		CvSeq *node = (CvSeq*)it->node;
		//1.先走父级
		if (node->h_prev == NULL)
		{
			it->node = node = node->v_prev;
			if (--(it->level) < 0)
			{
				it->node = node = 0;
				return;
			}
		}
		//2.再走同级
		else
		{
			node = node->h_prev;
			while (node->v_next)
			{
				node = node->v_next;
				++(it->level);

				while (node->h_next)
					node = node->h_next;
			}
			it->node = node;
		}
	}

	void cvRemoveNodeFromTree(CvSeq *node, CvSeq *addParent)
	{	//功能: 移除node结点及其子结点
		//差异: 没用CvTreeNode而直接使用CvSeq, 因为OpenCV的源码中使用的最基类是CvSeq, 所以无需使用根基类CvTreeNode
		if (node->h_next)//同级右处理
			node->h_next->h_prev = node->h_prev;
		if (node->h_prev)//同级左处理
			node->h_prev->h_next = node->h_next;
		else//若无同级左则处理其父
		{
			if (node->v_prev == NULL) node->v_prev = addParent; //无父则使用提供的父
			if (node->v_prev) node->v_prev->v_next = node->h_next; //更改父的第一子结点
		}
	}

	void cvInsertNodeIntoTree(CvSeq *node, CvSeq *parent)
	{	//功能: 插入node结点并作为parent的第一结点
		//差异: 没用CvTreeNode而直接使用CvSeq, 因为OpenCV的源码中使用的最基类是CvSeq, 所以无需使用根基类CvTreeNode
		node->v_prev = parent;
		node->h_next = parent->v_next;//原第一子结点变为node的同级右结点
		if (parent->v_next) parent->v_next->h_prev = node;//node变为原第一子结点的同级左结点
		parent->v_next = node;//第一子结点变为node
	}

	void cvTreeToNodeSeq(CvSeq *first, CvSeq **addrSeq)
	{
		CvTreeNodeIterator it = { (void*)first, 0, INT_MAX };
		while (1)
		{
			if (it.node == NULL) return;
			cvSeqPush(*addrSeq, &(it.node));
			azz::cvNextTreeNode(&it);
		}
	}

	void cvTreeToNodeSeq(CvSeq *first, vector<CvSeq*> &addrVec)
	{
		int level = 0;
		while (1)
		{
			//0.结点入库
			addrVec.push_back(first);
			//1.先走子级
			if (first->v_next)
			{
				first = first->v_next;
				++level;
			}
			//2.再走同级
			else
			{
				while (first->h_next == 0)//2.1若同级不存在
				{
					first = first->v_prev;//2.2则返回到父级
					if (--level < 0)//表示已遍历完成
						return;
				}
				first = first->h_next;//2.3定位到同级或父级的同级
			}
		}
	}

//轮廓查找
	class FindContour
	{
	public:
		typedef struct _CvContourInfo
		{
			int flags;
			struct _CvContourInfo *next;        /* next contour with the same mark value */
			struct _CvContourInfo *parent;      /* information about parent contour */
			CvSeq *contour;             /* corresponding contour (may be 0, if rejected) */
			CvRect rect;                /* bounding rectangle */
			CvPoint origin;             /* origin point (where the contour was traced from) */
			int is_hole;                /* hole flag */
		}
		_CvContourInfo;

		typedef struct _CvContourScanner
		{
			CvMemStorage *storage1;     /* contains fetched contours */
			CvMemStorage *storage2;     /* contains approximated contours
										(!=storage1 if approx_method2 != approx_method1) */
			CvMemStorage *cinfo_storage;        /* contains _CvContourInfo nodes */
			CvSet *cinfo_set;           /* set of _CvContourInfo nodes */
			CvMemStoragePos initial_pos;        /* starting storage pos */
			CvMemStoragePos backup_pos; /* beginning of the latest approx. contour */
			CvMemStoragePos backup_pos2;        /* ending of the latest approx. contour */
			schar *img0;                /* image origin */
			schar *img;                 /* current image row */
			int img_step;               /* image step */
			CvSize img_size;            /* ROI size */
			CvPoint offset;             /* ROI offset: coordinates, added to each contour point */
			CvPoint pt;                 /* current scanner position */
			CvPoint lnbd;               /* position of the last met contour */
			int nbd;                    /* current mark val */
			_CvContourInfo *l_cinfo;    /* information about latest approx. contour */
			_CvContourInfo cinfo_temp;  /* temporary var which is used in simple modes */
			_CvContourInfo frame_info;  /* information about frame */
			CvSeq frame;                /* frame itself */
			int approx_method1;         /* approx method when tracing */
			int approx_method2;         /* final approx method */
			int mode;                   /* contour scanning mode:
										0 - external only
										1 - all the contours w/o any hierarchy
										2 - connected components (i.e. two-level structure -
										external contours and holes),
										3 - full hierarchy;
										4 - connected components of a multi-level image
										*/
			int subst_flag;
			int seq_type1;              /* type of fetched contours */
			int header_size1;           /* hdr size of fetched contours */
			int elem_size1;             /* elem size of fetched contours */
			int seq_type2;              /*                                       */
			int header_size2;           /*        the same for approx. contours  */
			int elem_size2;             /*                                       */
			_CvContourInfo *cinfo_table[128];
		}
		_CvContourScanner;

	public://轮廓查找: 未完成, 参考Suzuki S, Be K. Topological structural analysis of digitized binary images by border following[J]. Computer Vision Graphics & Image Processing, 1985, 30(1):32-46.
		static void findContours1(Mat_<uchar> src, vector<vector<Point>> &contours, vector<Vec4i> &hiers, int mode, int method, Point offset)
		{
			Mat srcEx;
			copyMakeBorder(src, srcEx, 1, 1, 1, 1, cv::BORDER_CONSTANT | cv::BORDER_ISOLATED, Scalar(0)); 
			CvMemStorage *storage = cvCreateMemStorage();

			//1.初始化扫描器(cvStartFindContours + cvFindNextContour + cvEndFindContours)
			_CvContourScanner *scanner = (_CvContourScanner*)cvAlloc(sizeof(*scanner));
			memset(scanner, 0, sizeof(*scanner));
			scanner->storage1 = storage;//存储提取的轮廓
			scanner->storage2 = storage;//存储压缩的轮廓
			scanner->cinfo_storage = cvCreateChildMemStorage(scanner->storage2);//存储_CvContourInfo结点
			scanner->cinfo_set = cvCreateSet(0, sizeof(CvSet), sizeof(_CvContourInfo), scanner->cinfo_storage);//存储_CvContourInfo结点
			cvSaveMemStoragePos(storage, &(scanner->initial_pos));//起始存储位置
																  //scanner->backup_pos;//提取的最新轮廓的起点
																  //scanner->backup_pos2;//提取的最新轮廓的终点
			scanner->img0 = (schar*)srcEx.data;
			scanner->img = (schar*)(srcEx.data + srcEx.cols);
			scanner->img_step = srcEx.cols;
			scanner->img_size = CvSize(srcEx.cols - 1, srcEx.rows - 1); /* exclude rightest column and bottomost row*/
			scanner->offset = Point(-1, -1);
			scanner->pt = cvPoint(1, 1);//当前扫描位置
			scanner->lnbd = cvPoint(0, 1);//上一次提取的轮廓的位置
			scanner->nbd = 2;//当前mark值
			scanner->l_cinfo = 0;//提取的最新压缩轮廓的贪睡 
								 //scanner->cinfo_temp;//临时轮廓的信息
			scanner->frame.flags = CV_SEQ_FLAG_HOLE;
			scanner->frame_info.next = 0;
			scanner->frame_info.parent = 0;
			scanner->frame_info.contour = &(scanner->frame);
			scanner->frame_info.rect = cvRect(0, 0, srcEx.cols, srcEx.rows);
			//scanner->frame_info.origin;
			scanner->frame_info.is_hole = 1;
			//scanner->frame;//框架
			scanner->approx_method1 = method;//轮廓的初始压缩方法
			scanner->approx_method2 = method;//轮廓的最终压缩方法
			scanner->mode = mode;//如何组织轮廓间的关系
			scanner->subst_flag = 0;//
			scanner->seq_type1 = CV_SEQ_POLYGON;
			scanner->seq_type2 = CV_SEQ_POLYGON;
			scanner->header_size1 = sizeof(CvContour);
			scanner->header_size2 = sizeof(CvContour);
			scanner->elem_size1 = sizeof(Point);
			scanner->elem_size2 = sizeof(Point);
			//scanner->cinfo_table;
			threshold(srcEx, srcEx, 0, 1, CV_THRESH_BINARY);

			//2.执行轮廓查找
			int count = 0; CvContourScanner scan = (CvContourScanner)scanner;
			while (cvFindNextContour(scan) != 0) ++count;
			CvSeq *root = cvEndFindContours(&scan);
			cout << endl << count;

			//3.提取每个轮廓首地址(遍历树)	
			vector<CvSeq*> addrVec;
			CvSeq *addrSeq = cvCreateSeq(0, sizeof(CvSeq), sizeof(root), storage);
			azz::cvTreeToNodeSeq(root, addrVec);
			azz::cvTreeToNodeSeq(root, &addrSeq);

			//4.提取每个轮廓边界点
			for (int i = 0; i < count; ++i)
			{
				CvSeq *node = *((CvSeq**)cvGetSeqElem(addrSeq, i));//addr[i]
				CvContour *contour = (CvContour*)node;
				contour->color = i;

				vector<Point> pts;
				for (int j = 0; j < node->total; ++j) //cvCvtSeqToArray(node, ci.ptr());
					pts.push_back(*((Point*)cvGetSeqElem(node, j)));
				contours.push_back(pts);
			}

			//5.提取轮廓间相互关系
			for (int i = 0; i < count; ++i)
			{
				CvSeq *node = *((CvSeq**)cvGetSeqElem(addrSeq, i));//addr[i]
				int h_next = node->h_next ? ((CvContour*)node->h_next)->color : -1;
				int h_prev = node->h_prev ? ((CvContour*)node->h_prev)->color : -1;
				int v_next = node->v_next ? ((CvContour*)node->v_next)->color : -1;
				int v_prev = node->v_prev ? ((CvContour*)node->v_prev)->color : -1;

				hiers.push_back(Vec4i(h_next, h_prev, v_next, v_prev));
			}

			cvReleaseMemStorage(&storage);
		}

		static void testFindContour()
		{
			//预处理
			Mat_<uchar> src = imread("./../data/nao.jpg", 0);
			blur(src, src, Size(3, 3));

			//计算边缘
			Mat_<uchar> can; int thresh = 100;//max=255
			Canny(src, can, thresh, 100 * 2, 3);

			Mat_<uchar> bin(21, 21, (uchar)0);
			bin(Rect(1, 1, 13, 13)) = 1;
			bin(Rect(2, 2, 5, 5)) = 0;
			bin(Rect(3, 3, 3, 3)) = 1;
			bin(Rect(8, 8, 5, 5)) = 0;
			bin(Rect(9, 9, 3, 3)) = 1;

			bin(Rect(15, 15, 5, 5)) = 1;
			bin(Rect(16, 16, 3, 3)) = 0;

			//cout << endl << bin;
			//getchar();

			//查找轮廓

			vector<Vec4i> hiers;
			vector<vector<Point>> contours;
			findContours1(can, contours, hiers, RETR_TREE, CHAIN_APPROX_NONE, Point(0, 0));


			//for (int i = 0; i < contours.size(); ++i)
			//{
			//	cout << endl << i << ": " << contours[i].size();
			//	cout << endl << contours[i];
			//}

			FileStorage fs("./../data/contours.yml", FileStorage::WRITE);
			fs << "contours" << contours << "hiers" << hiers;
			fs.release();
			return;

			//绘制结果
			RNG rng;
			Mat_<Vec3b> show(can.rows, can.cols);
			for (uint i = 0; i < contours.size(); ++i)
			{
				Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
				drawContours(show, contours, i, color, 2, 8, hiers, 0, Point());
			}
			imshow("testFindContour", show);
			waitKey(0);

			return;
		}

	public: //不变矩计算: 完成拟稿
		static void completeMomentState(Moments *moments)
		{
			double cx = 0, cy = 0;
			double mu20, mu11, mu02;
			double inv_m00 = 0.0;
			//assert(moments != 0);

			if (__abs(moments->m00) > DBL_EPSILON)
			{
				inv_m00 = 1. / moments->m00;
				cx = moments->m10 * inv_m00;
				cy = moments->m01 * inv_m00;
			}

			// mu20 = m20 - m10*cx
			mu20 = moments->m20 - moments->m10 * cx;
			// mu11 = m11 - m10*cy
			mu11 = moments->m11 - moments->m10 * cy;
			// mu02 = m02 - m01*cy
			mu02 = moments->m02 - moments->m01 * cy;

			moments->mu20 = mu20;
			moments->mu11 = mu11;
			moments->mu02 = mu02;

			// mu30 = m30 - cx*(3*mu20 + cx*m10)
			moments->mu30 = moments->m30 - cx * (3 * mu20 + cx * moments->m10);
			mu11 += mu11;
			// mu21 = m21 - cx*(2*mu11 + cx*m01) - cy*mu20
			moments->mu21 = moments->m21 - cx * (mu11 + cx * moments->m01) - cy * mu20;
			// mu12 = m12 - cy*(2*mu11 + cy*m10) - cx*mu02
			moments->mu12 = moments->m12 - cy * (mu11 + cy * moments->m10) - cx * mu02;
			// mu03 = m03 - cy*(3*mu02 + cy*m01)
			moments->mu03 = moments->m03 - cy * (3 * mu02 + cy * moments->m01);


			double inv_sqrt_m00 = std::sqrt(__abs(inv_m00));
			double s2 = inv_m00*inv_m00, s3 = s2*inv_sqrt_m00;

			moments->nu20 = moments->mu20*s2; moments->nu11 = moments->mu11*s2; moments->nu02 = moments->mu02*s2;
			moments->nu30 = moments->mu30*s3; moments->nu21 = moments->mu21*s3; moments->nu12 = moments->mu12*s3; moments->nu03 = moments->mu03*s3;
		}

		static Moments contourMoments(vector<Point> &ptsi)
		{
			Moments m;
			int lpt = (int)ptsi.size();// ptscontour.checkVector(2);

			double a00 = 0, a10 = 0, a01 = 0, a20 = 0, a11 = 0, a02 = 0, a30 = 0, a21 = 0, a12 = 0, a03 = 0;
			double xi, yi, xi2, yi2, xi_1, yi_1, xi_12, yi_12, dxy, xii_1, yii_1;

			xi_1 = ptsi[lpt - 1].x;
			yi_1 = ptsi[lpt - 1].y;

			xi_12 = xi_1 * xi_1;
			yi_12 = yi_1 * yi_1;

			for (int i = 0; i < lpt; i++)
			{
				xi = ptsi[i].x;
				yi = ptsi[i].y;

				xi2 = xi * xi;
				yi2 = yi * yi;
				dxy = xi_1 * yi - xi * yi_1;
				xii_1 = xi_1 + xi;
				yii_1 = yi_1 + yi;

				a00 += dxy;
				a10 += dxy * xii_1;
				a01 += dxy * yii_1;
				a20 += dxy * (xi_1 * xii_1 + xi2);
				a11 += dxy * (xi_1 * (yii_1 + yi_1) + xi * (yii_1 + yi));
				a02 += dxy * (yi_1 * yii_1 + yi2);
				a30 += dxy * xii_1 * (xi_12 + xi2);
				a03 += dxy * yii_1 * (yi_12 + yi2);
				a21 += dxy * (xi_12 * (3 * yi_1 + yi) + 2 * xi * xi_1 * yii_1 +
					xi2 * (yi_1 + 3 * yi));
				a12 += dxy * (yi_12 * (3 * xi_1 + xi) + 2 * yi * yi_1 * xii_1 +
					yi2 * (xi_1 + 3 * xi));
				xi_1 = xi;
				yi_1 = yi;
				xi_12 = xi2;
				yi_12 = yi2;
			}

			if (fabs(a00) > FLT_EPSILON)
			{
				double db1_2, db1_6, db1_12, db1_24, db1_20, db1_60;

				if (a00 > 0)
				{
					db1_2 = 0.5;
					db1_6 = 0.16666666666666666666666666666667;
					db1_12 = 0.083333333333333333333333333333333;
					db1_24 = 0.041666666666666666666666666666667;
					db1_20 = 0.05;
					db1_60 = 0.016666666666666666666666666666667;
				}
				else
				{
					db1_2 = -0.5;
					db1_6 = -0.16666666666666666666666666666667;
					db1_12 = -0.083333333333333333333333333333333;
					db1_24 = -0.041666666666666666666666666666667;
					db1_20 = -0.05;
					db1_60 = -0.016666666666666666666666666666667;
				}

				// spatial moments
				m.m00 = a00 * db1_2;
				m.m10 = a10 * db1_6;
				m.m01 = a01 * db1_6;
				m.m20 = a20 * db1_12;
				m.m11 = a11 * db1_24;
				m.m02 = a02 * db1_12;
				m.m30 = a30 * db1_20;
				m.m21 = a21 * db1_60;
				m.m12 = a12 * db1_60;
				m.m03 = a03 * db1_20;

				completeMomentState(&m);
			}
			return m;
		}

		static void testMoments()
		{
			//预处理
			Mat_<uchar> src = imread("./../data/nao.jpg", 0);
			blur(src, src, Size(3, 3));

			//计算边缘
			Mat_<uchar> can; int thresh = 100;//max=255
			Canny(src, can, thresh, 100 * 2, 3);

			//查找轮廓
			vector<Vec4i> hiers;
			vector<vector<Point>> contours;
			findContours(can, contours, hiers, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

			//计算不变矩
			vector<Moments> mmts(contours.size());
			for (uint i = 0; i < contours.size(); ++i)
				mmts[i] = FindContour::contourMoments(contours[i]);
			//mmts[i] = moments(contours[i], false);

			vector<Mat_<double>> mmtsMat;
			for (uint i = 0; i < contours.size(); ++i)
				mmtsMat.push_back(Mat_<double>(24, 1, (double*)(&(mmts[i]))));
			FileStorage fs("./../data/mmts.yml", FileStorage::WRITE);
			fs << "mmtsMat" << mmtsMat;
			fs.release();
			return;

			//计算质心
			vector<Point2f> mcs(contours.size());
			for (uint i = 0; i < contours.size(); ++i)
				mcs[i] = Point2f(static_cast<float>(mmts[i].m10 / mmts[i].m00), static_cast<float>(mmts[i].m01 / mmts[i].m00));

			//计算面积
			for (uint i = 0; i < contours.size(); ++i)
				printf("\nContour[%d] Area: m00=%-10.3f, contourArea=%-10.3f, arcLength=%-10.3f", (int)i,
					mmts[i].m00,
					contourArea(contours[i]),
					arcLength(contours[i], true));

			//绘制结果
			RNG rng;
			Mat_<Vec3b> show(can.rows, can.cols);
			for (uint i = 0; i < contours.size(); ++i)
			{
				Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
				drawContours(show, contours, i, color, 2, 8, hiers, 0, Point());
				circle(show, mcs[i], 4, color, -1, 8, 0);
			}
			imshow("testMoments", show);
			waitKey(0);

			return;
		}

	public://凸包计算: 完成拟稿
		static bool CHullCmpPoints1(Point *p1, Point *p2)
		{
			return p1->x < p2->x || (p1->x == p2->x && p1->y < p2->y);
		}

		static int Sklansky_(Point **array, int start, int end, int *stack, int nsign, int sign2)
		{
			int incr = end > start ? 1 : -1;
			// prepare first triangle
			int pprev = start, pcur = pprev + incr, pnext = pcur + incr;
			int stacksize = 3;

			if (start == end ||
				(array[start]->x == array[end]->x &&
					array[start]->y == array[end]->y))
			{
				stack[0] = start;
				return 1;
			}

			stack[0] = pprev;
			stack[1] = pcur;
			stack[2] = pnext;

			end += incr; // make end = afterend

			while (pnext != end)
			{
				// check the angle p1,p2,p3
				int cury = array[pcur]->y;
				int nexty = array[pnext]->y;
				int by = nexty - cury;

				if (CV_SIGN(by) != nsign)
				{
					int ax = array[pcur]->x - array[pprev]->x;
					int bx = array[pnext]->x - array[pcur]->x;
					int ay = cury - array[pprev]->y;
					int convexity = ay*bx - ax*by; // if >0 then convex angle

					if (CV_SIGN(convexity) == sign2 && (ax != 0 || ay != 0))
					{
						pprev = pcur;
						pcur = pnext;
						pnext += incr;
						stack[stacksize] = pnext;
						stacksize++;
					}
					else
					{
						if (pprev == start)
						{
							pcur = pnext;
							stack[1] = pcur;
							pnext += incr;
							stack[2] = pnext;
						}
						else
						{
							stack[stacksize - 2] = pnext;
							pcur = pprev;
							pprev = stack[stacksize - 4];
							stacksize--;
						}
					}
				}
				else
				{
					pnext += incr;
					stack[stacksize - 1] = pnext;
				}
			}

			return --stacksize;
		}

		static void convexHull(vector<Point> &_points, OutputArray _hull, bool clockwise, bool returnPoints)
		{
			//CV_INSTRUMENT_REGION()

			//Mat points = _points.getMat();
			int i, total = (int)_points.size(), nout = 0;
			int miny_ind = 0, maxy_ind = 0;


			int _stack[640 * 2 + 480 * 2];// AutoBuffer<int> _stack(total + 2);
			int _hullbuf[640 * 2 + 480 * 2];//AutoBuffer<int> _hullbuf(total);
			Point *_pointer[640 * 2 + 480 * 2];//AutoBuffer<Point*> _pointer(total);
			Point *data0 = &_points[0];// points.ptr<Point>();
			for (i = 0; i < total; i++)
				_pointer[i] = &_points[i];

			// sort the point set by x-coordinate, find min and max y

			std::sort(_pointer, _pointer + total, CHullCmpPoints1);
			for (i = 1; i < total; i++)
			{
				int y = _pointer[i]->y;
				if (_pointer[miny_ind]->y > y)
					miny_ind = i;
				if (_pointer[maxy_ind]->y < y)
					maxy_ind = i;
			}


			if (_pointer[0]->x == _pointer[total - 1]->x && _pointer[0]->y == _pointer[total - 1]->y)
			{
				_hullbuf[nout++] = 0;
			}
			else
			{
				// upper half
				int *tl_stack = _stack;
				int tl_count =
					Sklansky_(_pointer, 0, maxy_ind, tl_stack, -1, 1);
				int *tr_stack = _stack + tl_count;
				int tr_count =
					Sklansky_(_pointer, total - 1, maxy_ind, tr_stack, -1, -1);

				// gather upper part of convex hull to output
				if (!clockwise)
				{
					std::swap(tl_stack, tr_stack);
					std::swap(tl_count, tr_count);
				}

				for (i = 0; i < tl_count - 1; i++)
					_hullbuf[nout++] = int(_pointer[tl_stack[i]] - data0);
				for (i = tr_count - 1; i > 0; i--)
					_hullbuf[nout++] = int(_pointer[tr_stack[i]] - data0);
				int stop_idx = tr_count > 2 ? tr_stack[1] : tl_count > 2 ? tl_stack[tl_count - 2] : -1;

				// lower half
				int *bl_stack = _stack;
				int bl_count =
					Sklansky_(_pointer, 0, miny_ind, bl_stack, 1, -1);
				int *br_stack = _stack + bl_count;
				int br_count =
					Sklansky_(_pointer, total - 1, miny_ind, br_stack, 1, 1);

				if (clockwise)
				{

					std::swap(bl_stack, br_stack);
					std::swap(bl_count, br_count);
				}

				if (stop_idx >= 0)
				{
					int check_idx = bl_count > 2 ? bl_stack[1] :
						bl_count + br_count > 2 ? br_stack[2 - bl_count] : -1;
					if (check_idx == stop_idx || (check_idx >= 0 &&
						_pointer[check_idx]->x == _pointer[stop_idx]->x &&
						_pointer[check_idx]->y == _pointer[stop_idx]->y))
					{
						// if all the points lie on the same line, then the bottom part of the convex hull is the mirrored top part (except the exteme points).
						bl_count = __min(bl_count, 2);
						br_count = __min(br_count, 2);
					}
				}

				for (i = 0; i < bl_count - 1; i++)
					_hullbuf[nout++] = int(_pointer[bl_stack[i]] - data0);
				for (i = br_count - 1; i > 0; i--)
					_hullbuf[nout++] = int(_pointer[br_stack[i]] - data0);
			}

			if (!returnPoints)
			{
				Mat(nout, 1, CV_32S, _hullbuf).copyTo(_hull);
				cout << endl << "aaa";
			}
			else
			{
				cout << endl << "bbb";
				_hull.create(nout, 1, CV_MAKETYPE(CV_32S, 2));
				Mat hull = _hull.getMat();
				size_t step = !hull.isContinuous() ? hull.step[0] : sizeof(Point);
				for (i = 0; i < nout; i++)
					*(Point*)(hull.ptr() + i*step) = data0[_hullbuf[i]];
			}
		}

		static void testConvexHull()
		{
			//预处理
			Mat_<uchar> src = imread("./../data/nao.jpg", 0);
			blur(src, src, Size(3, 3));

			//阀值化
			Mat_<uchar> thh; int thresh = 100;
			threshold(src, thh, thresh, 255, THRESH_BINARY);//max = 255;

															//查找轮廓
			vector<Vec4i> hiers;
			vector<vector<Point>> contours;
			findContours(thh, contours, hiers, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

			//计算凸包
			vector<vector<Point>> hull(contours.size());
			for (uint i = 0; i < contours.size(); ++i)
				convexHull(contours[i], hull[i], true, true);
			//cv::convexHull(Mat(contours[i]), hull[i], true, true);

			FileStorage fs("./../data/hull.yml", FileStorage::WRITE);
			fs << "hull" << hull;
			fs.release();
			return;

			//绘制结果
			RNG rng;
			Mat_<Vec3b> show(thh.rows, thh.cols);
			for (uint i = 0; i < contours.size(); ++i)
			{
				Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
				drawContours(show, contours, i, color, 1, 8, noArray(), 0, Point());
				drawContours(show, hull, i, color, 1, 8, noArray(), 0, Point());
			}
			imshow("testConvexHull", show);
			waitKey(0);

		}
	};
}
#endif
