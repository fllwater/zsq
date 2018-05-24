//0
#include "base/000.h"
#include "base/aaa.h"
#include "base/abb.h"
#include "base/acc.h"
#include "base/add.h"
#include "base/azz.h"
//1
#include "calib/calibaaa.h"
#include "calib/calibzzz.h"
//2
#include "stereo/stereoaaa.h"
#include "stereo/stereoibm.h"
#include "stereo/stereoisgbm.h"
#include "stereo/stereoielas.h"
//3
#include "vslam/vslamaaa.h"
//9
#include "window/windowaaa.h"

#ifndef __zemozzz_h__
#define __zemozzz_h__

namespace demozzz
{
	int use_eigen_sophus_test = 0;
	int use_suitesparse_octomap_test = 0;
	int use_vtk_opencv_test = 0;
	int use_pangolin_test = 0;
	int use_fbow_test = 0;
	int use_ceres_test = 0;
	int use_g2o_test = 0;
	int use_gtsam_test = 0;
	int use_flann_qhull_pcl_test = 0;

	void eigen_sophus_test()
	{
		AngleAxisd Ra(3.14159265358979323846 / 2, Eigen::Vector3d(0, 0, 1));//旋转角量
		Quaterniond Rq(Ra);//旋转四元数
		SO3d Rv(Rq);//旋转向量
		Matrix3d R_a = Ra.toRotationMatrix();//旋转矩阵
		Matrix3d R_q = Rq.toRotationMatrix();//旋转矩阵
		Matrix3d R_v = Rv.matrix();//旋转矩阵

		cout << "旋转角量到旋转矩阵" << endl << Ra.toRotationMatrix() << endl;
		cout << "旋转四元到旋转矩阵" << endl << Rq.toRotationMatrix() << endl;
		cout << "旋转向量到旋转矩阵" << endl << SO3d::exp(Vector3d(0, 0, 3.14159265358979323846 / 2)).matrix() << endl;
		cout << "旋转矩阵到旋转向量" << endl << SO3d::log(Rv.matrix()).matrix() << endl;
		cout << endl << "Finish eigen_sophus_test, press any key!"; getchar();
	}

	void vtk_opencv_test()
	{
		Mat_<Vec3b> rgb(480, 640); randu(rgb, 1, 255);
		Mat_<Vec3f> xyz(480, 640); randu(xyz, 1, 255);

		//imshow("rgb", rgb);
		//waitKey(0);

		viz::Viz3d viz3d;
		viz::WCloud cloud(xyz, rgb);
		viz::WCoordinateSystem sys(255);
		sys.setRenderingProperty(viz::OPACITY, 0.2);
		viz3d.showWidget("cloud", cloud);
		viz3d.showWidget("system", sys);
		cv::Affine3d pose(Vec3d(0, 0, 0), Vec3d(0, 0, 0));
		viz3d.setViewerPose(pose);
		viz3d.spin();
		cout << endl << "Finish vtk_opencv_test, press any key!"; getchar();
	}

#if 0
	namespace pangolintest
	{
		struct RotationMatrix
		{
			Matrix3d matrix = Matrix3d::Identity();
		};

		ostream& operator << (ostream& out, const RotationMatrix& r)
		{
			out.setf(ios::fixed);
			Matrix3d matrix = r.matrix;
			out << '=';
			out << "[" << setprecision(2) << matrix(0, 0) << "," << matrix(0, 1) << "," << matrix(0, 2) << "],"
				<< "[" << matrix(1, 0) << "," << matrix(1, 1) << "," << matrix(1, 2) << "],"
				<< "[" << matrix(2, 0) << "," << matrix(2, 1) << "," << matrix(2, 2) << "]";
			return out;
		}

		istream& operator >> (istream& in, RotationMatrix& r)
		{
			return in;
		}

		struct TranslationVector
		{
			Vector3d trans = Vector3d(0, 0, 0);
		};

		ostream& operator << (ostream& out, const TranslationVector& t)
		{
			out << "=[" << t.trans(0) << ',' << t.trans(1) << ',' << t.trans(2) << "]";
			return out;
		}

		istream& operator >> (istream& in, TranslationVector& t)
		{
			return in;
		}

		struct QuaternionDraw
		{
			Quaterniond q;
		};

		ostream& operator << (ostream& out, const QuaternionDraw quat)
		{
			auto c = quat.q.coeffs();
			out << "=[" << c[0] << "," << c[1] << "," << c[2] << "," << c[3] << "]";
			return out;
		}

		istream& operator >> (istream& in, const QuaternionDraw quat)
		{
			return in;
		}

		void pangolin_test()
		{
			pangolin::CreateWindowAndBind("visualize geometry", 1000, 600);
			glEnable(GL_DEPTH_TEST);
			pangolin::OpenGlRenderState s_cam(pangolin::ProjectionMatrix(1000, 600, 420, 420, 500, 300, 0.1, 1000), pangolin::ModelViewLookAt(3, 3, 3, 0, 0, 0, pangolin::AxisY));

			const int UI_WIDTH = 500;
			pangolin::View& d_cam = pangolin::CreateDisplay().SetBounds(0.0, 1.0, pangolin::Attach::Pix(UI_WIDTH), 1.0, -1000.0f / 600.0f).SetHandler(new pangolin::Handler3D(s_cam));

			// ui
			pangolin::Var<RotationMatrix> rotation_matrix("ui.R", RotationMatrix());
			pangolin::Var<TranslationVector> translation_vector("ui.t", TranslationVector());
			pangolin::Var<TranslationVector> euler_angles("ui.rpy", TranslationVector());
			pangolin::Var<QuaternionDraw> quaternion("ui.q", QuaternionDraw());
			pangolin::CreatePanel("ui").SetBounds(0.0, 1.0, 0.0, pangolin::Attach::Pix(UI_WIDTH));

			while (!pangolin::ShouldQuit())
			{
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				d_cam.Activate(s_cam);

				pangolin::OpenGlMatrix matrix = s_cam.GetModelViewMatrix();
				Matrix<double, 4, 4> m = matrix;
				// m = m.inverse();
				RotationMatrix R;
				for (int i = 0; i < 3; i++)
					for (int j = 0; j < 3; j++)
						R.matrix(i, j) = m(j, i);
				rotation_matrix = R;

				TranslationVector t;
				t.trans = Vector3d(m(0, 3), m(1, 3), m(2, 3));
				t.trans = -R.matrix*t.trans;
				translation_vector = t;

				TranslationVector euler;
				euler.trans = R.matrix.transpose().eulerAngles(2, 1, 0);
				euler_angles = euler;

				QuaternionDraw quat;
				quat.q = Quaterniond(R.matrix);
				quaternion = quat;

				glColor3f(1.0, 1.0, 1.0);

				pangolin::glDrawColouredCube();
				// draw the original axis
				glLineWidth(3);
				glColor3f(0.8f, 0.f, 0.f);
				glBegin(GL_LINES);
				glVertex3f(0, 0, 0);
				glVertex3f(10, 0, 0);
				glColor3f(0.f, 0.8f, 0.f);
				glVertex3f(0, 0, 0);
				glVertex3f(0, 10, 0);
				glColor3f(0.2f, 0.2f, 1.f);
				glVertex3f(0, 0, 0);
				glVertex3f(0, 0, 10);
				glEnd();

				pangolin::FinishFrame();
			}
		}
	}
#endif

	namespace cerestest
	{
		struct CostAll
		{
			template <typename T> bool operator()(const T* const params, T* residual) const
			{
				residual[0] = params[0] + 10.0 * params[1];//f1 = x1 + 10 * x2;
				residual[1] = sqrt(5.0) * (params[2] - params[3]);//f2 = sqrt(5) (x3 - x4)
				residual[2] = (params[1] - 2.0 * params[2]) * (params[1] - 2.0 * params[2]);//f3 = (x2 - 2 x3)^2
				residual[3] = sqrt(10.0) * (params[0] - params[3]) * (params[0] - params[3]);//f4 = sqrt(10) (x1 - x4)^2
				return true;
			}
		};

		void ceres_test()
		{
			//
			double params[4] = { 3.0, -1.0, 0.0, 1.0 };

			//
			ceres::Problem::Options problemOptions;
			problemOptions.cost_function_ownership = ceres::Ownership::DO_NOT_TAKE_OWNERSHIP;
			problemOptions.loss_function_ownership = ceres::Ownership::DO_NOT_TAKE_OWNERSHIP;
			problemOptions.local_parameterization_ownership = ceres::Ownership::DO_NOT_TAKE_OWNERSHIP;
			problemOptions.enable_fast_removal = false;
			problemOptions.disable_all_safety_checks = false;

			//
			ceres::Solver::Options solverOptions;
			solverOptions.max_num_iterations = 100;
			solverOptions.linear_solver_type = ceres::DENSE_QR;
			solverOptions.minimizer_progress_to_stdout = true;

			//
			ceres::AutoDiffCostFunction<CostAll, 4, 4> diffS(new CostAll);//因为默认Diff都占有Cost所以直接在构造函数中分配

			//
			ceres::Problem problem(problemOptions);
			problem.AddResidualBlock(&diffS, NULL, params);//前面已经设置Problem不占有Diff所以才能如此

			//
			ceres::Solver::Summary summary;
			ceres::Solve(solverOptions, &problem, &summary);

			//
			cout << endl << summary.FullReport();
			cout << endl << "Final results: " << Mat_<double>(1, 4, params);
			cout << endl << "Finish ceres_test, press any key!"; getchar();
		}
	}

	namespace g2otest
	{
		using namespace g2o;

		class CameraParametersDZY : public Parameter
		{
		public:
			double focal_length;
			Vector2D principle_point;
			double baseline;
		public:
			CameraParametersDZY() :focal_length(1.), principle_point(Vector2D(0., 0.)), baseline(0.5) {}
			CameraParametersDZY(double focal_length, Vector2D principle_point, double baseline) : focal_length(focal_length), principle_point(principle_point), baseline(baseline) {}
			Vector2D cam_map(Vector3D trans_xyz)
			{
				Vector2D res;//Vector2D proj = project2d(trans_xyz);
				res[0] = trans_xyz[0] / trans_xyz[2] * focal_length + principle_point[0];
				res[1] = trans_xyz[1] / trans_xyz[2] * focal_length + principle_point[1];
				return res;
			}
			Vector3D stereocam_uvu_map(Vector3D trans_xyz)
			{
				Vector2D uv_left = cam_map(trans_xyz);
				double proj_x_right = (trans_xyz[0] - baseline) / trans_xyz[2];
				double u_right = proj_x_right*focal_length + principle_point[0];
				return Vector3D(uv_left[0], uv_left[1], u_right);
			}
		public:
			void *operator new(size_t size) { return Eigen::internal::conditional_aligned_malloc<true>(size); }
			void *operator new[](size_t size) { return Eigen::internal::conditional_aligned_malloc<true>(size); }
			bool read(istream& is) { return true; }
			bool write(ostream& os) const { return true; }
		};

		class VertexSE3ExpmapDZY : public BaseVertex<6, SE3Quat>
		{
		public:
			VertexSE3ExpmapDZY() {}
			virtual void setToOriginImpl() { _estimate = SE3Quat(); }
			virtual void oplusImpl(const double* update_) { _estimate = SE3Quat::exp(g2o::Vector6d(update_)) * _estimate; }
		public:
			void *operator new(size_t size) { return Eigen::internal::conditional_aligned_malloc<true>(size); }
			void *operator new[](size_t size) { return Eigen::internal::conditional_aligned_malloc<true>(size); }
			bool read(istream& is) { return true; }
			bool write(ostream& os) const { return true; }
		};

		class VertexSBAPointXYZDZY : public BaseVertex<3, Vector3D>
		{
		public:
			VertexSBAPointXYZDZY() {};
			virtual void setToOriginImpl() { _estimate.setZero(); }
			virtual void oplusImpl(const double* update) { _estimate += Vector3D(update); }
		public:
			void *operator new(size_t size) { return Eigen::internal::conditional_aligned_malloc<true>(size); }
			void *operator new[](size_t size) { return Eigen::internal::conditional_aligned_malloc<true>(size); }
			bool read(istream& is) { return true; }
			bool write(ostream& os) const { return true; }
		};

		class EdgeProjectXYZ2UVDZY : public  BaseBinaryEdge<2, Vector2D, VertexSBAPointXYZDZY, VertexSE3ExpmapDZY>
		{
		public:
			CameraParametersDZY * _cam;
		public:
			EdgeProjectXYZ2UVDZY()
			{
				_cam = 0;
				resizeParameters(1);
				installParameter(_cam, 0);
			}

			void computeError()
			{
				CameraParametersDZY *KObj = (CameraParametersDZY*)parameter(0);
				VertexSE3ExpmapDZY *TObj = (VertexSE3ExpmapDZY*)_vertices[1];
				VertexSBAPointXYZDZY *XYZObj = (VertexSBAPointXYZDZY*)_vertices[0];
				_error = _measurement - KObj->cam_map(TObj->estimate().map(XYZObj->estimate()));
			}
			virtual void linearizeOplus()
			{
				CameraParametersDZY *KObj = (CameraParametersDZY*)parameter(0);
				VertexSE3ExpmapDZY *TvObj = (VertexSE3ExpmapDZY*)_vertices[1];
				VertexSBAPointXYZDZY *XYZObj = (VertexSBAPointXYZDZY*)_vertices[0];
				SE3Quat TvObjQ(TvObj->estimate());
				Vector3D xyz = XYZObj->estimate();
				Vector3D xyz_trans = TvObjQ.map(xyz);

				double x = xyz_trans[0];
				double y = xyz_trans[1];
				double z = xyz_trans[2];
				double z_2 = z*z;

				Matrix<double, 2, 3> tmp;
				tmp(0, 0) = KObj->focal_length;
				tmp(0, 1) = 0;
				tmp(0, 2) = -x / z*KObj->focal_length;
				tmp(1, 0) = 0;
				tmp(1, 1) = KObj->focal_length;
				tmp(1, 2) = -y / z*KObj->focal_length;
				_jacobianOplusXi = -1. / z * tmp * TvObjQ.rotation().toRotationMatrix();

				_jacobianOplusXj(0, 0) = x*y / z_2 *KObj->focal_length;
				_jacobianOplusXj(0, 1) = -(1 + (x*x / z_2)) *KObj->focal_length;
				_jacobianOplusXj(0, 2) = y / z *KObj->focal_length;
				_jacobianOplusXj(0, 3) = -1. / z *KObj->focal_length;
				_jacobianOplusXj(0, 4) = 0;
				_jacobianOplusXj(0, 5) = x / z_2 *KObj->focal_length;
				_jacobianOplusXj(1, 0) = (1 + y*y / z_2) *KObj->focal_length;
				_jacobianOplusXj(1, 1) = -x*y / z_2 *KObj->focal_length;
				_jacobianOplusXj(1, 2) = -x / z *KObj->focal_length;
				_jacobianOplusXj(1, 3) = 0;
				_jacobianOplusXj(1, 4) = -1. / z *KObj->focal_length;
				_jacobianOplusXj(1, 5) = y / z_2 *KObj->focal_length;
			}
		public:
			void *operator new(size_t size) { return Eigen::internal::conditional_aligned_malloc<true>(size); }
			void *operator new[](size_t size) { return Eigen::internal::conditional_aligned_malloc<true>(size); }
			bool read(istream& is) { return true; }
			bool write(ostream& os) const { return true; }
		};

		void bundleAdjustment(vector<Point3f> point3D, vector<Point2f> point2D, Mat_<double> R, Mat_<double> t, double fx, double cx, double cy)
		{

			//0.初始化图模型
			//typedef BlockSolver<BlockSolverTraits<6, 3>> Block;  //pose维度为 6, landmark维度为3
			BlockSolver<BlockSolverTraits<6, 3>>::LinearSolverType *linearSolver = new LinearSolverCSparse<BlockSolver< BlockSolverTraits<6, 3>>::PoseMatrixType>();
			BlockSolver<BlockSolverTraits<6, 3>> *blockSolver = new BlockSolver<BlockSolverTraits<6, 3>>(linearSolver);
			OptimizationAlgorithmLevenberg *lmSolver = new OptimizationAlgorithmLevenberg(blockSolver);
			SparseOptimizer sparseOptimizer;
			sparseOptimizer.setAlgorithm(lmSolver);

			//1.设置参数: 仅相机矩阵
			CameraParametersDZY *KObj = new CameraParametersDZY(fx, Vector2d(cx, cy), 0);
			KObj->setId(0);
			sparseOptimizer.addParameter(KObj);

			//2.设置顶点: 位姿和坐标
			Mat_<double> Rtrans = R.t();
			VertexSE3ExpmapDZY *TvObj = new VertexSE3ExpmapDZY();
			TvObj->setId(0);
			TvObj->setEstimate(SE3Quat(Matrix3d((double*)Rtrans.data), Vector3d((double*)t.data)));
			sparseOptimizer.addVertex(TvObj);

			for (int i = 0; i < point3D.size(); ++i)
			{
				VertexSBAPointXYZDZY *XYZ = new VertexSBAPointXYZDZY();
				XYZ->setId(i + 1);
				XYZ->setEstimate(Vector3d(point3D[i].x, point3D[i].y, point3D[i].z));
				XYZ->setMarginalized(true); //G2O中必须设置Marg(参见第十讲内容)
				sparseOptimizer.addVertex(XYZ);
			}

			//3.设置边: 重投影误差
			for (uint i = 0; i < point2D.size(); ++i)
			{
				EdgeProjectXYZ2UVDZY* edge = new EdgeProjectXYZ2UVDZY();
				edge->setId(i + 1);
				edge->setParameterId(0, 0);
				edge->setVertex(1, dynamic_cast<VertexSE3ExpmapDZY*>(sparseOptimizer.vertex(0)));//连接的顶点(可连接多个顶点)
				edge->setVertex(0, dynamic_cast<VertexSBAPointXYZDZY*>(sparseOptimizer.vertex(i + 1)));//连接的顶点
				edge->setMeasurement(Vector2d(point2D[i].x, point2D[i].y));
				edge->setInformation(Matrix2d::Identity());
				sparseOptimizer.addEdge(edge);
			}

			//4.执行优化
			sparseOptimizer.setVerbose(true);
			sparseOptimizer.initializeOptimization();
			sparseOptimizer.optimize(100);

			//5.保存结果
			ofstream out("./../data/o.txt");
			out << endl << "after optimization:" << endl;
			out << "T=" << endl << Isometry3d(TvObj->estimate()).matrix() << endl;
		}

		void g2o_test()
		{
			double fx = 520.9;
			double fy = 521.0;
			double cx = 325.1;
			double cy = 249.7;
			Mat_<double> M(3, 3); M << fx, 0, cx, 0, fy, cy, 0, 0, 1;

			//1.读取源数据
			Mat ima1 = imread("./../data/1.png", 1);
			Mat ima2 = imread("./../data/2.png", 1);
			Mat_<ushort> dep = imread("./../data/1_depth.png", -1);       // 深度图为16位无符号数，单通道图像

			//2.1计算特征点
			Ptr<ORB> orb = ORB::create(100, 2, 8, 31, 0, 2, ORB::HARRIS_SCORE, 31, 20);
			vector<KeyPoint> keypts1;
			vector<KeyPoint> keypts2;
			orb->detect(ima1, keypts1);
			orb->detect(ima2, keypts2);

			//2.2计算描述符
			orb = ORB::create();
			Mat_<uchar> desc1;
			Mat_<uchar> desc2;
			orb->compute(ima1, keypts1, desc1);
			orb->compute(ima2, keypts2, desc2);

			//2.3寻找匹配对
			Ptr<BFMatcher> bfMatcher = BFMatcher::create(NORM_HAMMING);
			vector<DMatch> matchesOrg;
			bfMatcher->match(desc1, desc2, matchesOrg);

			//2.4寻找最相似与最不相似匹配对
			float minD = 10000;
			float maxD = 0;
			for (int i = 0; i < desc1.rows; i++)
			{
				float dist = matchesOrg[i].distance;
				if (dist < minD) minD = dist;
				if (dist > maxD) maxD = dist;
			}
			cout << endl << "最相似匹配对之间的距离: " << minD;
			cout << endl << "最不相似匹配对之间的距离: " << maxD;

			//2.5剔除一些不太相似的匹配对
			vector<DMatch> rawMatches;
			for (int i = 0; i < desc1.rows; ++i)
				if (matchesOrg[i].distance <= __max(2 * minD, 30.0)) rawMatches.push_back(matchesOrg[i]);
			cout << endl << "所有匹配对数: " << matchesOrg.size();
			cout << endl << "优化后的匹配对数: " << rawMatches.size();

			//3.建立3D-2D匹配对
			vector<Point3f> point3D;
			vector<Point2f> point2D;
			for (int i = 0; i < rawMatches.size(); ++i)
			{
				ushort d = dep(int(keypts1[rawMatches[i].queryIdx].pt.y), int(keypts1[rawMatches[i].queryIdx].pt.x));
				if (d == 0) continue;
				float z = d / 1000.0;
				float y = (keypts1[rawMatches[i].queryIdx].pt.y - cy) / fy * z;
				float x = (keypts1[rawMatches[i].queryIdx].pt.x - cx) / fx * z;

				point3D.push_back(Point3f(x, y, z));
				point2D.push_back(keypts2[rawMatches[i].trainIdx].pt);
			}
			cout << endl << "3D-2D匹配对数: " << point3D.size();

			//
			Mat imgMatchesOrg;
			Mat imgMatches;
			drawMatches(ima1, keypts1, ima2, keypts2, matchesOrg, imgMatchesOrg);
			drawMatches(ima1, keypts1, ima2, keypts2, rawMatches, imgMatches);
			vconcat(imgMatchesOrg, imgMatches, imgMatches);
			//imshow("Matches", imgMatches);
			//waitKey(0);

			//4.求解PnP
			Mat_<double> Rv;
			Mat_<double> t;
			Mat_<double> R;
			solvePnP(point3D, point2D, M, Mat(), Rv, t, false); // 调用OpenCV 的 PnP 求解，可选择EPNP，DLS等方法  
			Rodrigues(Rv, R); // r为旋转向量形式，用Rodrigues公式转换为矩阵
			cout << endl << "PnP R = " << R;
			cout << endl << "PnP t = " << t;

			//5.全局优化
			bundleAdjustment(point3D, point2D, R, t, fx, cx, cy);
			cout << endl << "Finish g2o_test, press any key!"; getchar();
		}
	}

	void flann_qhull_pcl_test()
	{
		double fx = 518.0;
		double fy = 519.0;
		double cx = 325.5;
		double cy = 253.5;

		FILE *in;
		double dataPose[7];
		PointCloud<PointXYZRGB>::Ptr allPointClound(new PointCloud<PointXYZRGB>);
		for (int k = 0; k < 5; ++k)
		{
			cout << endl << "转换为PCLMAP: " << k + 1;
			//1.读取图像及深度
			Mat_<Vec3b> rgb = imread("./../data/color/" + aaa::num2string(k + 1) + ".png");
			Mat_<ushort> depth = imread("./../data/depth/" + aaa::num2string(k + 1) + ".pgm", -1);

			//2.读取位姿矩阵
			if (k == 0) in = fopen("./../data/pose.txt", "r");
			for (int i = 0; i < 7; ++i)
			{
				fscanf(in, "%lf", dataPose + i);
				cout << "   " << *(dataPose + i);
			}
			if (k == 5) fclose(in);
			Isometry3d T(Quaterniond(dataPose[6], dataPose[3], dataPose[4], dataPose[5]));
			T.pretranslate(Vector3d(dataPose[0], dataPose[1], dataPose[2]));

			//3.获取点云数据
			PointCloud<PointXYZRGB>::Ptr pointCloud(new PointCloud<PointXYZRGB>);
			for (int i = 0; i < rgb.rows; ++i)
				for (int j = 0; j < rgb.cols; ++j)
				{
					Vector3d xyz;
					Vector3d XYZ;
					PointXYZRGB XZYRGB;
					ushort d = depth.ptr<ushort>(i)[j];
					if (d == 0) continue; //没有测量到
					if (d >= 7000) continue; //太大不可靠

					xyz[2] = double(d) / 1000.;
					xyz[0] = (j - cx)*xyz[2] / fx;
					xyz[1] = (i - cy)*xyz[2] / fy;
					XYZ = T*xyz;

					XZYRGB.x = XYZ[0];
					XZYRGB.y = XYZ[1];
					XZYRGB.z = XYZ[2];
					XZYRGB.b = rgb(i, j)[0];
					XZYRGB.g = rgb(i, j)[1];
					XZYRGB.r = rgb(i, j)[2];
					pointCloud->points.push_back(XZYRGB);
				}

			//4.滤除点云噪点
			PointCloud<PointXYZRGB>::Ptr tmpPointCloud(new PointCloud<PointXYZRGB>);
			StatisticalOutlierRemoval<PointXYZRGB> stttFilter;
			stttFilter.setMeanK(50);
			stttFilter.setStddevMulThresh(1.0);
			stttFilter.setInputCloud(pointCloud);
			stttFilter.filter(*tmpPointCloud);
			(*allPointClound) += *tmpPointCloud;
		}
		allPointClound->is_dense = false;

		//5.降采样点云
		cout << endl << "降采样前点云数: " << allPointClound->size();
		VoxelGrid<PointXYZRGB> voxelFilter;
		voxelFilter.setLeafSize(0.01, 0.01, 0.01);
		PointCloud<PointXYZRGB>::Ptr tmpPointCloud(new PointCloud<PointXYZRGB>);
		voxelFilter.setInputCloud(allPointClound);
		voxelFilter.filter(*tmpPointCloud);
		tmpPointCloud->swap(*allPointClound);
		io::savePCDFileBinary("./../data/pclmap.pcd", *allPointClound);
		cout << endl << "降采样后点云数: " << allPointClound->size();
		cout << endl << "Finish flann_qhull_pcl_test, press any key!"; getchar();
	}

	void suitesparse_octomap_test()
	{
		double fx = 518.0;
		double fy = 519.0;
		double cx = 325.5;
		double cy = 253.5;

		FILE *in;
		double dataPose[7];
		octomap::OcTree tree(0.05);
		for (int k = 0; k < 5; ++k)
		{
			cout << endl << "转换为OCTOMAP: " << k + 1;
			//1.读取图像及深度
			Mat_<Vec3b> rgb = imread("./../data/color/" + aaa::num2string(k + 1) + ".png");
			Mat_<ushort> depth = imread("./../data/depth/" + aaa::num2string(k + 1) + ".pgm", -1);

			//2.读取位姿矩阵
			if (k == 0) in = fopen("./../data/pose.txt", "r");
			for (int i = 0; i < 7; ++i)
			{
				fscanf(in, "%lf", dataPose + i);
				cout << "   " << *(dataPose + i);
			}
			if (k == 5) fclose(in);
			Isometry3d T(Quaterniond(dataPose[6], dataPose[3], dataPose[4], dataPose[5]));
			T.pretranslate(Vector3d(dataPose[0], dataPose[1], dataPose[2]));

			//3.获取点云数据
			octomap::Pointcloud pointCloud;
			for (int i = 0; i < rgb.rows; ++i)
				for (int j = 0; j < rgb.cols; ++j)
				{
					Vector3d xyz;
					Vector3d XYZ;
					ushort d = depth.ptr<ushort>(i)[j];
					if (d == 0) continue; //没有测量到
					if (d >= 7000) continue; //太大不可靠

					xyz[2] = double(d) / 1000.;
					xyz[0] = (j - cx)*xyz[2] / fx;
					xyz[1] = (i - cy)*xyz[2] / fy;
					XYZ = T*xyz;

					pointCloud.push_back(XYZ[0], XYZ[1], XYZ[2]);
				}

			//4.转为八叉树地图(这里要给定原点)
			tree.insertPointCloud(pointCloud, octomap::point3d(T(0, 3), T(1, 3), T(2, 3)));
		}

		//5.更新中间节点的占据信息
		tree.updateInnerOccupancy();
		tree.writeBinary("./../data/octomap.bt");
		cout << endl << "Finish suitesparse_octomap_test, press any key!"; getchar();
	}

}

#endif
