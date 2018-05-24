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
		cout << endl << "Different num: " + aaa::num2string(num);
		cout << endl << "Minimum error: " + aaa::num2string(min, 2);
		cout << endl << "Maximum error: " + aaa::num2string(max, 2);
		cout << endl << "All error sum: " + aaa::num2string(sum, 2);
		cout << endl << "All error avg: " + aaa::num2string(sum / total, 2);
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
		cout << endl << "Different num: " + aaa::num2string(num);
		cout << endl << "Minimum error: " + aaa::num2string(min, 2);
		cout << endl << "Maximum error: " + aaa::num2string(max, 2);
		cout << endl << "All error sum: " + aaa::num2string(sum, 2);
		cout << endl << "All error avg: " + aaa::num2string(sum / num, 2);
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
		if (mode == 0) return access(path.c_str(), 0);//Existent
		else if (mode == 1) return access(path.c_str(), 2);//Only readable
		else if (mode == 2) return access(path.c_str(), 4);//Only writable
		else if (mode == 3) return access(path.c_str(), 6);//Both readable and writable
		return -1;//Return 0 for success and -1 for failure
	}
	string getPathInfo(string path, int mode = 0)
	{
		int ind1 = (int)path.find_last_of('/');
		int ind2 = (int)path.find_last_of('.');
		if (mode == 0) return path.substr(0, ind1);//directory
		else if (mode == 1) return path.substr(ind1 + 1, ind2 - ind1 - 1);//filename
		else if (mode == 2) return path.substr(ind2 + 1);//extension
		else if (mode == 3) return path.substr(0, ind2);//directory and filename
		else return path.substr(ind1 + 1);//filename and extension
	}
	
	vector<string> getAllPaths(string dir, bool getdir = 0, string prefix = "")
	{
		//Check existence
		namespace fs = boost::filesystem;
		vector<string> outpath;
		fs::path pathBoost(dir);
		if (!fs::exists(pathBoost))
			return outpath;

		//Do traversal
		fs::directory_iterator end;
		for (fs::directory_iterator it(pathBoost); it != end; ++it)
		{
			if (getdir)//For directory
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
			else//For file
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
			cout << endl << "Rename: " << paths[i] << " as " << newpath;
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
			cout << endl << "Rename: "<< paths[i] << " as " << newpath;
		}
	}
	
	void extractFrame(string path = "./../data/abb/vid.mp4", bool timelineName = false, bool isShow = false)
	{
		VideoCapture cap(path);
		int rows, cols, fps, nframe, code, cspace, id;
		cout << endl << "Frame heiht: " << (rows = (int)cap.get(CAP_PROP_FRAME_HEIGHT));
		cout << endl << "Frame width: " << (cols = (int)cap.get(CAP_PROP_FRAME_WIDTH));
		cout << endl << "Frame rate: " << (fps = (int)cap.get(CAP_PROP_FPS));
		cout << endl << "Frame count: " << (nframe = (int)cap.get(CAP_PROP_FRAME_COUNT));
		cout << endl << "Video codec: " << (code = (int)cap.get(CAP_PROP_FOURCC));
		cout << endl << "Color space: " << (cspace = (int)cap.get(CAP_PROP_MODE)) << " (0→BGR, 1→RGB, 2→GRAY, 3=→YUYV)";
		if (isShow) namedWindow("extractFrame", WINDOW_NORMAL);

		Mat frame;
		while (cap.read(frame))
		{
			cout << endl << "Current progress: " << (id = (int)cap.get(CAP_PROP_POS_FRAMES)) << "/" << nframe;

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
			cout << endl << "Merging: " << paths[i];
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
		cout << endl << "Frame heiht: " << (rows = (int)cap.get(CAP_PROP_FRAME_HEIGHT));
		cout << endl << "Frame width: " << (cols = (int)cap.get(CAP_PROP_FRAME_WIDTH));
		cout << endl << "Frame rate: " << (fps = (int)cap.get(CAP_PROP_FPS));
		cout << endl << "Frame count: " << (nframe = (int)cap.get(CAP_PROP_FRAME_COUNT));
		cout << endl << "Video codec: " << (code = (int)cap.get(CAP_PROP_FOURCC));
		cout << endl << "Color space: " << (cspace = (int)cap.get(CAP_PROP_MODE)) << " (0→BGR, 1→RGB, 2→GRAY, 3=→YUYV)";
		if (isShow) namedWindow("formatVideo", WINDOW_NORMAL);

		Mat frame;
		VideoWriter wrt(getPathInfo(path, 3) + string(".") + format, code = VideoWriter::fourcc('D', 'I', 'V', '3'), fps, Size(cols, rows), true);
		while (cap.read(frame))
		{
			cout << endl << "Current progress: " << (id = (int)cap.get(CAP_PROP_POS_FRAMES)) << "/" << nframe;

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
			cout << endl << "Formating: "<< paths[i];
			imwrite(abb::getPathInfo(paths[i], 3) + "." + format, imread(paths[i], -1));
			if (replace) remove(paths[i].c_str());
		}
	}

	void splitFrame(string binoDir = "./../data/bino", string leftDir = "./../data/left", string rightDir = "./../data/right")
	{
		//Create directory
		createDirectory(leftDir);
		createDirectory(rightDir);

		//Split frame
		vector<string> binoPaths = getAllPaths(binoDir);
		for (uint i = 0; i < binoPaths.size(); ++i)
		{
			cout << endl << "Splitting: " << binoPaths[i];
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
		//0.Create directory
		createDirectory(dir);

		//1.Capture setting 
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

		//2.Capture frame
		Mat frame; int nframe = 0; cout << endl << "Press s/S to capture and q/Q to exit";
		while (cap.read(frame))
		{
			imshow("takeCamera", frame);
			char c = waitKey(3);
			if (c == 's' || c == 'S') { imwrite(dir + string("/") + aaa::num2string(aaa_ns) + string(".tif"), frame); cout << endl << "Caputured frame count: " << ++nframe; }
			if (c == 'q' || c == 'Q') break;
		}destroyWindow("takeCamera");
	}
	void recordCamera(string dir = "./../data/abb", abb_cap_params)
	{
		//0.Create directory
		createDirectory(dir);

		//1.Record setting
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

		//2.Ready for record
		Mat frame; int nframe = 0; cout << endl << "Press s/S to record and q/Q to exit";
		while (cap.read(frame))
		{
			imshow("recordCamera", frame);
			char c = waitKey(3);
			if (c == 's' || c == 'S') break;
		}

		//3.Record frame
		VideoWriter wrt(dir + string("/") + aaa::num2string(aaa_ns) + string(".avi"), VideoWriter::fourcc('D', 'I', 'V', '3'), 25, Size(capCols, capRows), true);
		while (cap.read(frame))
		{
			wrt.write(frame); cout << endl << "Recorded frame count: " << ++nframe;
			imshow("recordCamera", frame);
			char c = waitKey(3);	
			if (c == 'q' || c == 'Q') break;
		}destroyWindow("recordCamera");
	}
	void recordScreen(string dir = "./../data/abb")
	{
#ifdef WIN32
		//1.Get screen infomation
		HDC hdcScreen = GetDC(GetDesktopWindow());
		int elemSize = GetDeviceCaps(hdcScreen, BITSPIXEL);
		int rows = GetDeviceCaps(hdcScreen, VERTRES);
		int cols = GetDeviceCaps(hdcScreen, HORZRES);

		//2.Allocate space
		CImage cimage; int nframe = 0, nframe_ = 0;
		cimage.Create(cols, rows, 24);//not use elemSize
		HDC hdcCIM = cimage.GetDC();
		Mat_<Vec3b> frame(rows, cols, (Vec3b*)cimage.GetPixelAddress(0, rows - 1), -cimage.GetPitch());

		//3.Start to record
		cout << endl << "Press any key and Enter key to record; Press s/S to capture; Press q/Q to exit"; getchar();
		VideoWriter wrt(dir + string("/") + aaa::num2string(aaa_ns) + string(".avi"), VideoWriter::fourcc('D', 'I', 'V', '3'), 25, Size(cols, rows), true);
		while (1)
		{
			BitBlt(hdcCIM, 0, 0, cols, rows, hdcScreen, 0, 0, SRCCOPY);
			flip(frame, frame, 0);
			wrt.write(frame); cout << endl << "Recorded frame count:: " << ++nframe;

			imshow("recordScreen", frame);
			char c = waitKey(3);
			if (c == 's' || c == 'S') { imwrite(dir + string("/") + aaa::num2string(aaa_ns) + string(".tif"), frame); cout << "Captured frame count: " << ++nframe_; }
			if (c == 'q' || c == 'Q') break;
		}

		//4.Release source
		cimage.ReleaseDC();
		cimage.Destroy();
#else
		cout << endl << "Waiting..." << endl;
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
	{	//Return 13 here, but 10 for window's getch() function
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
