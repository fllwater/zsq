#include "aaa.h"

#ifndef __abb_h__
#define __abb_h__

namespace abb
{
#define abb_cap_params int deviceId = 0, int autoSize = 1, int capRows = 480, int capCols = 640, bool isShow = true, int waitTime = 100

	void readBin(void *data, int total, int depth = 1, const char *path = "./../data/abb/dat.bin")
	{
		FILE *in = fopen(path, "rb");
		fread(data, total, depth, in);
		fclose(in);
	}
	void saveBin(void *data, int total, int depth = 1, const char *path = "./../data/abb/dat.bin")
	{
		FILE *out = fopen(path, "wb");
		fwrite(data, total, depth, out);
		fclose(out);
	}
	template<typename type> void readXML(Mat_<type> *mat, const char *nodename = "mat", const char *path = "./../data/abb/dat.yml")
	{
		FileStorage fs;
		fs.open(path, FileStorage::READ);
		fs[nodename] >> *mat;
		fs.release();
	}
	template<typename type> void saveXML(Mat_<type> *mat, const char *nodename = "mat", const char *path = "./../data/abb/dat.yml")
	{
		FileStorage fs;
		fs.open(path, FileStorage::WRITE);
		fs.write(nodename, *mat);
		fs.release();
	}

	template<typename depth> void calcErr(depth *data1, depth *data2, int total)
	{
		double tmp = 0.;
		double sum = 0.;
		double max = 0.;
		double min = DBL_MAX;
		int num = 0;
		for (int i = 0; i < total; ++i)
		{
			tmp = (double)data1[i] - (double)data2[i];
			if (tmp != 0) ++num;
			if (tmp < 0) tmp = -tmp;
			if (min > tmp) min = tmp;
			if (max < tmp) max = tmp;
			sum += tmp;
		}
		printf("\nDifNum: %d", num);
		printf("\nMinErr: %.2lf", min);
		printf("\nMaxErr: %.2lf", max);
		printf("\nSumErr: %.2lf", sum);
		printf("\nAvgErr: %.2lf ", sum / total);
		getchar();
	}
	template<typename depth> void calcErr(Mat_<depth> mat1, Mat_<depth> mat2)
	{
		double tmp = 0.;
		double sum = 0.;
		double max = 0.;
		double min = DBL_MAX;
		int num = 0;
		for (int i = 0; i < mat1.rows; ++i)
		{
			for (int j = 0; j < mat1.cols; ++j)
			{
				tmp = (double)mat1(i, j) - (double)mat2(i, j);
				if (tmp != 0) ++num;
				if (tmp < 0) tmp = -tmp;
				if (min > tmp) min = tmp;
				if (max < tmp) max = tmp;
				sum += tmp;
			}
		}
		printf("\nDifNum: %d", num);
		printf("\nMinErr: %.2lf", min);
		printf("\nMaxErr: %.2lf", max);
		printf("\nSumErr: %.2lf", sum);
		printf("\nAvgErr: %.2lf ", sum / mat1.total());
		getchar();
	}

	vector<string> splitString(string str, string sep)
	{
		size_t preInd = -1;
		size_t curInd = 0;
		vector<string> listResult;

		while (1)
		{
			++preInd;
			curInd = str.find_first_of(sep, preInd);

			if (curInd == string::npos)
			{
				listResult.push_back(str.substr(preInd, str.length() - preInd));
				break;
			}
			else
			{
				listResult.push_back(str.substr(preInd, curInd - preInd));
				preInd = curInd;
			}
		}

		return listResult;
	}

	int getPathState(string path, int mode = 0)
	{
		if (mode == 0) return access(path.c_str(), 0);//是否存在
		else if (mode == 1) return access(path.c_str(), 2);//是否只写
		else if (mode == 2) return access(path.c_str(), 4);//是否只读
		else if (mode == 3) return access(path.c_str(), 6);//是否读写
		return -1;//以上状态成功都返回0否则返回-1
	}
	string getPathInfo(string path, int mode = 0)
	{
		int ind1 = (int)path.find_last_of('/');
		int ind2 = (int)path.find_last_of('.');
		if (mode == 0) return path.substr(0, ind1);//目录
		else if (mode == 1) return path.substr(ind1 + 1, ind2 - ind1 - 1);//文件名
		else if (mode == 2) return path.substr(ind2 + 1);//扩展名
		else if (mode == 3) return path.substr(0, ind2);//目录 + 文件名
		else return path.substr(ind1 + 1);//文件名 + 扩展名
	}
	
	vector<string> getAllPaths(string dir, bool getdir = 0, string prefix = "")
	{
		//判断否存
		namespace fs = boost::filesystem;
		vector<string> outpath;
		fs::path pathBoost(dir);
		if (!fs::exists(pathBoost))
			return outpath;

		//执行遍历
		fs::directory_iterator end;
		for (fs::directory_iterator it(pathBoost); it != end; ++it)
		{
			if (getdir)//目录
			{
				if (fs::is_directory(it->status()))
					if (it->path().filename().string().find(prefix) == 0)
					{
						string str = it->path().string();
						long ind = (long)str.find_last_of('\\');
						if (ind != -1) str.replace(ind, 1, "/");
						outpath.push_back(str);
					}
					
			}
			else//文件
			{
				if (fs::is_regular_file(it->status()))
					if (it->path().filename().string().find(prefix) == 0)
					{
						string str = it->path().string();
						long ind = (long)str.find_last_of('\\');
						if (ind != -1) str.replace(ind, 1, "/");
						outpath.push_back(str);
					}
			}
		}
		stable_sort(outpath.begin(), outpath.end(), less<string>());
		stable_sort(outpath.begin(), outpath.end(), [](string str1, string str2) {return str1.length() < str2.length(); });
		return outpath;
	}
	void createDirectory(string dir, bool create = true)
	{
		if (create)
		{
			boost::filesystem::path dirBoost(dir);
			boost::filesystem::create_directories(dirBoost);
		}
		else
		{
			boost::filesystem::remove_all(dir);
		}
	}

	void renameWithPrefix(string dir = "./../data/abb", string prefix = "dzy")
	{
		vector<string> paths = getAllPaths(dir);
		for (uint i = 0; i < paths.size(); ++i)
		{
			string newpath = getPathInfo(paths[i], 0) + "/" + prefix + aaa::num2string(i + 1) + "." + getPathInfo(paths[i], 2);
			rename(paths[i].c_str(), newpath.c_str());
			cout << endl << "重命名: " << paths[i] << " 为 " << newpath;
		}
	}
	void renameWithTimeline(string dir = "./../data/abb", int mode = 0)
	{
		vector<string> paths = getAllPaths(dir);
		for (uint i = 0; i < paths.size(); ++i)
		{
			string newname;
			if (mode == 0) newname = aaa::num2string(aaa_ns);
			else if (mode == 1) newname = aaa::num2string(aaa_us);
			else newname = aaa::num2string(aaa_ms);

			string newpath = getPathInfo(paths[i], 0) + "/" + newname + "." + getPathInfo(paths[i], 2);
			rename(paths[i].c_str(), newpath.c_str());
			cout << endl << "重命名: "<< paths[i] << " 为 " << newpath;
		}
	}
	
	void extractFrame(string path = "./../data/abb/vid.mp4", bool timelineName = false, bool isShow = false)
	{
		VideoCapture cap(path);
		int rows, cols, fps, nframe, code, cspace, id;
		cout << endl << "帧高: " << (rows = (int)cap.get(CAP_PROP_FRAME_HEIGHT));
		cout << endl << "帧宽: " << (cols = (int)cap.get(CAP_PROP_FRAME_WIDTH));
		cout << endl << "帧率: " << (fps = (int)cap.get(CAP_PROP_FPS));
		cout << endl << "帧数: " << (nframe = (int)cap.get(CAP_PROP_FRAME_COUNT));
		cout << endl << "编码方式: " << (code = (int)cap.get(CAP_PROP_FOURCC));
		cout << endl << "颜色空间: " << (cspace = (int)cap.get(CAP_PROP_MODE)) << " (0→BGR, 1→RGB, 2→GRAY, 3=→YUYV)";
		if (isShow) namedWindow("extractFrame", WINDOW_NORMAL);

		Mat frame;
		while (cap.read(frame))
		{
			cout << endl << "当前进度: " << (id = (int)cap.get(CAP_PROP_POS_FRAMES)) << "/" << nframe;

			if(timelineName) imwrite(abb::getPathInfo(path) + "/" + aaa::num2string(aaa_ns) + string(".tif"), frame);
			else imwrite(abb::getPathInfo(path, 3) + aaa::num2string(id) + string(".tif"), frame);

			if (!isShow) continue;
			imshow("extractFrame", frame);
			waitKey(10);
		}if (isShow) destroyWindow("extractFrame");
	}
	void composeVideo(string dir = "./../data/abb", string vidname = "vid.avi", int fps = 25, bool isShow = false)
	{
		namedWindow("composeVideo", WINDOW_NORMAL);
		vector<string> paths = getAllPaths(dir);
		VideoWriter wrt(dir + "/" + vidname, VideoWriter::fourcc('D', 'I', 'V', '3'), fps, imread(paths[0], -1).size(), true);
		for (uint i = 0; i < paths.size(); ++i)
		{
			cout << endl << "加入: " << paths[i];
			Mat frame = imread(paths[i], -1);
			wrt.write(frame);

			if (!isShow) continue;
			imshow("composeVideo", frame);
			waitKey(10);
		}if (isShow) destroyWindow("composeVideo");
	}
	
	void formatVideo(string path = "./../data/abb/vid.mp4", string format = "avi", bool isShow = false)
	{
		VideoCapture cap(path);
		int rows, cols, fps, nframe, code, cspace, id;
		cout << endl << "帧高: " << (rows = (int)cap.get(CAP_PROP_FRAME_HEIGHT));
		cout << endl << "帧宽: " << (cols = (int)cap.get(CAP_PROP_FRAME_WIDTH));
		cout << endl << "帧率: " << (fps = (int)cap.get(CAP_PROP_FPS));
		cout << endl << "帧数: " << (nframe = (int)cap.get(CAP_PROP_FRAME_COUNT));
		cout << endl << "编码方式: " << (code = (int)cap.get(CAP_PROP_FOURCC));
		cout << endl << "颜色空间: " << (cspace = (int)cap.get(CAP_PROP_MODE)) << " (0→BGR, 1→RGB, 2→GRAY, 3=→YUYV)";
		if (isShow) namedWindow("formatVideo", WINDOW_NORMAL);

		Mat frame;
		VideoWriter wrt(getPathInfo(path, 3) + string(".") + format, code = VideoWriter::fourcc('D', 'I', 'V', '3'), fps, Size(cols, rows), true);
		while (cap.read(frame))
		{
			cout << endl << "当前进度: " << (id = (int)cap.get(CAP_PROP_POS_FRAMES)) << "/" << nframe;

			wrt.write(frame);

			if (!isShow) continue;
			imshow("formatVideo", frame);
			waitKey(10);
		}if(isShow) destroyWindow("formatVideo");
	}	
	void formatFrame(string dir = "./../data/abb", string format = "tif", bool replace = false)
	{
		vector<string> paths = getAllPaths(dir);
		for (uint i = 0; i < paths.size(); ++i)
		{
			cout << endl << "格式化: "<< paths[i];
			imwrite(abb::getPathInfo(paths[i], 3) + "." + format, imread(paths[i], -1));
			if (replace) remove(paths[i].c_str());
		}
	}

	void splitFrame(string binoDir = "./../data/bino", string leftDir = "./../data/left", string rightDir = "./../data/right")
	{
		//创建目录
		createDirectory(leftDir);
		createDirectory(rightDir);

		//执行分割
		vector<string> binoPaths = getAllPaths(binoDir);
		for (uint i = 0; i < binoPaths.size(); ++i)
		{
			cout << endl << "分割: " << binoPaths[i];
			Mat frame = imread(binoPaths[i], -1);
			Mat frame1 = frame.colRange(0, frame.cols >> 1);
			Mat	frame2 = frame.colRange(frame.cols >> 1, frame.cols);
			imwrite(leftDir + "/left" + aaa::num2string(i + 1) + ".tif", frame1);
			imwrite(rightDir + "/right" + aaa::num2string(i + 1) + ".tif", frame2);
		}
	}
	void mergeFrame(string leftDir = "./../data/left", string rightDir = "./../data/right", string binoDir = "./../data/bino")
	{
		//创建目录
		createDirectory(binoDir);

		//执行合并
		vector<string> leftPaths = getAllPaths(leftDir);
		vector<string> rightPaths = getAllPaths(rightDir);
		for (uint i = 0; i < leftPaths.size(); ++i)
		{
			cout << endl << "合并: " << leftPaths[i] << "和" << rightPaths[i];
			Mat frame1 = imread(leftPaths[i], -1);
			Mat frame2 = imread(rightPaths[i], -1);
			Mat frame; hconcat(frame1, frame2, frame);
			imwrite(binoDir + "/bino" + aaa::num2string(i + 1) + ".tif", frame);
		}
	}

	void takeCamera(string dir = "./../data/abb", abb_cap_params)
	{
		//0.创建目录
		createDirectory(dir);

		//1.设置抓取
		VideoCapture cap(deviceId);
		if (autoSize == 1)
		{
			capRows = (int)cap.get(CAP_PROP_FRAME_HEIGHT);
			capCols = (int)cap.get(CAP_PROP_FRAME_WIDTH);
		}
		else
		{
			cap.set(CAP_PROP_FRAME_HEIGHT, capRows);
			cap.set(CAP_PROP_FRAME_WIDTH, capCols);
		}
		namedWindow("takeCamera", WINDOW_NORMAL);

		//2.开始抓取
		Mat frame; int nframe = 0; cout << endl << "按S键抓取...按Q结束";
		while (cap.read(frame))
		{
			imshow("takeCamera", frame);
			char c = waitKey(3);
			if (c == 's' || c == 'S') { imwrite(dir + string("/") + aaa::num2string(aaa_ns) + string(".tif"), frame); cout << endl << "已抓取帧数: " << ++nframe; }
			if (c == 'q' || c == 'Q') break;
		}destroyWindow("takeCamera");
	}
	void recordCamera(string dir = "./../data/abb", abb_cap_params)
	{
		//0.创建目录
		createDirectory(dir);

		//1.设置抓取
		VideoCapture cap(deviceId);
		if (autoSize == 1)
		{
			capRows = (int)cap.get(CAP_PROP_FRAME_HEIGHT);
			capCols = (int)cap.get(CAP_PROP_FRAME_WIDTH);
		}
		else
		{
			cap.set(CAP_PROP_FRAME_HEIGHT, capRows);
			cap.set(CAP_PROP_FRAME_WIDTH, capCols);
		}
		namedWindow("recordCamera", WINDOW_NORMAL);

		//2.预备抓取
		Mat frame; int nframe = 0; cout << endl << "按S开始录制...按Q键结束录制";
		while (cap.read(frame))
		{
			imshow("recordCamera", frame);
			char c = waitKey(3);
			if (c == 's' || c == 'S') break;
		}

		//3.开始抓取
		VideoWriter wrt(dir + string("/") + aaa::num2string(aaa_ns) + string(".avi"), VideoWriter::fourcc('D', 'I', 'V', '3'), 25, Size(capCols, capRows), true);
		while (cap.read(frame))
		{
			wrt.write(frame); cout << endl << "已录制帧数: " << ++nframe;
			imshow("recordCamera", frame);
			char c = waitKey(3);	
			if (c == 'q' || c == 'Q') break;
		}destroyWindow("recordCamera");
	}
	void recordScreen(string dir = "./../data/abb")
	{
#ifdef WIN32
		//1.获取屏幕信息
		HDC hdcScreen = GetDC(GetDesktopWindow());
		int elemSize = GetDeviceCaps(hdcScreen, BITSPIXEL);
		int rows = GetDeviceCaps(hdcScreen, VERTRES);
		int cols = GetDeviceCaps(hdcScreen, HORZRES);

		//2.分配空间
		CImage cimage; int nframe = 0, nframe_ = 0;
		cimage.Create(cols, rows, 24);//不用elemSize
		HDC hdcCIM = cimage.GetDC();
		Mat_<Vec3b> frame(rows, cols, (Vec3b*)cimage.GetPixelAddress(0, rows - 1), -cimage.GetPitch());

		//3.开始录制
		cout << endl << "按任意键并回车开始录制...按S键抓取...按Q键结束录制"; getchar();
		VideoWriter wrt(dir + string("/") + aaa::num2string(aaa_ns) + string(".avi"), VideoWriter::fourcc('D', 'I', 'V', '3'), 25, Size(cols, rows), true);
		while (1)
		{
			BitBlt(hdcCIM, 0, 0, cols, rows, hdcScreen, 0, 0, SRCCOPY);
			flip(frame, frame, 0);
			wrt.write(frame); cout << endl << "已录制帧数:: " << ++nframe;

			imshow("recordScreen", frame);
			char c = waitKey(3);
			if (c == 's' || c == 'S') { imwrite(dir + string("/") + aaa::num2string(aaa_ns) + string(".tif"), frame); cout << "已抓取帧数: " << ++nframe_; }
			if (c == 'q' || c == 'Q') break;
		}

		//4.释放资源
		cimage.ReleaseDC();
		cimage.Destroy();
#else
		cout << endl << "待实现。。。" << endl;
#endif
	}

#ifndef WIN32
	int kbhit(void)
	{
		struct termios oldt, newt;
		int ch;
		int oldf;

		tcgetattr(STDIN_FILENO, &oldt);
		newt = oldt;
		newt.c_lflag &= ~(ICANON | ECHO);
		tcsetattr(STDIN_FILENO, TCSANOW, &newt);
		oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
		fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

		ch = getchar();

		tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
		fcntl(STDIN_FILENO, F_SETFL, oldf);

		if (ch != EOF)
		{
			ungetc(ch, stdin);
			return 1;
		}

		return 0;
	}
	int getch(void)
	{	//此函数回车返回13, 而Windows下的getch()回车返回10
		int c = 0;
		struct termios org_opts, new_opts;
		int res = 0;
		//-----  store old settings -----------
		res = tcgetattr(STDIN_FILENO, &org_opts);
		assert(res == 0);
		//---- set new terminal parms --------
		memcpy(&new_opts, &org_opts, sizeof(new_opts));
		new_opts.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL);
		tcsetattr(STDIN_FILENO, TCSANOW, &new_opts);
		c = getchar();
		//------  restore old settings ---------
		res = tcsetattr(STDIN_FILENO, TCSANOW, &org_opts); assert(res == 0);
		return c;
	}
#endif
}

#endif
