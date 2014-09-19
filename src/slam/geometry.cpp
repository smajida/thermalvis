/*! \file	geometry.cpp
 *  \brief	Definitions for geometry-related calculations.
*/

#ifdef _USE_EIGEN_

#include "slam/geometry.hpp"

/// \brief 		Minimum number of projections required to achieve the specified number of pairs
int minProjections(int pairs) {
	double retVal = ( -1.0 + pow(1.0 + 8.0 * double(pairs), 0.5) ) / 2.0;
	//printf("%s << retVal(%d) = (%f)\n", __FUNCTION__, pairs, retVal);
	return int(ceil(retVal+1.0));
}

/// \brief 		Maximum possible number of pairs able to be achieved with specified number of projections
int possiblePairs(int projections) {
	double retVal = 0.5 * double(std::max(((int)projections-1),0)) * ( double(std::max(((int)projections-1),0)) + 1 );
	return int(retVal);
}

void transformPoints(std::vector<cv::Point3d>& pts, int *options) {
	
	int signs[3];
	
	signs[0] = (options[2] == 0) - (options[2] == 1);
	signs[1] = (options[3] == 0) - (options[3] == 1);
	signs[2] = (options[4] == 0) - (options[4] == 1);
	
	for (unsigned int iii = 0; iii < pts.size(); iii++) {
		
		double x, y, z;
		
		if (options[0] == 0) {
			x = pts.at(iii).x;
			
			if (options[1] == 0) {
				y = pts.at(iii).y;
				z = pts.at(iii).z;
			} else {
				y = pts.at(iii).z;
				z = pts.at(iii).y;
			}
			
		} else if (options[0] == 1) {
			x = pts.at(iii).y;
			
			if (options[1] == 0) {
				y = pts.at(iii).x;
				z = pts.at(iii).z;
			} else {
				y = pts.at(iii).z;
				z = pts.at(iii).x;
			}
			
		} else  {
			
			if (options[0] != 2) { printf("%s << ERROR!\n", __FUNCTION__); }
			x = pts.at(iii).z;
			
			if (options[1] == 0) {
				y = pts.at(iii).x;
				z = pts.at(iii).y;
			} else {
				y = pts.at(iii).y;
				z = pts.at(iii).x;
			}
			
		}
		
		pts.at(iii).x = signs[0] * x;
		pts.at(iii).y = signs[1] * y;
		pts.at(iii).z = signs[2] * z;
			
	}
	
}

void transformPoints(std::vector<cv::Point3d>& pts, unsigned int option) {
	
	
	for (unsigned int iii = 0; iii < pts.size(); iii++) {
		
		double x, y, z;
		x = pts.at(iii).x;
		y = pts.at(iii).y;
		z = pts.at(iii).z;
		
		if (option == 0) {
			pts.at(iii).x = x;
			pts.at(iii).y = -y;
			pts.at(iii).z = z;
		} else if (option == 1) {	// This one seems to match what you do to pose!
			pts.at(iii).x = -y;
			pts.at(iii).y = -z;
			pts.at(iii).z = x;
		} else if (option == 2) {
			pts.at(iii).x = z;
			pts.at(iii).y = -x;
			pts.at(iii).z = y;
		} else if (option == 3) {
			pts.at(iii).x = y;
			pts.at(iii).y = -z;
			pts.at(iii).z = x;
		} else if (option == 4) {
			pts.at(iii).x = -y;
			pts.at(iii).y = z;
			pts.at(iii).z = x;

		}
	}
	
}

#ifdef _BUILD_FOR_ROS_
void assignPose(geometry_msgs::PoseStamped& pPose, cv::Mat& C, int idx, ros::Time timestamp, int mode) {
	
	pPose.header.seq = idx;
	pPose.header.stamp = timestamp;
   
	cv::Mat R, t;
	Eigen::Quaterniond Q;
		
	decomposeTransform(C, R, t);
	matrixToQuaternion(R, Q);
	
	if (mode == MAPPER_ASSIGN_MODE) {
		QuaternionDbl Qrot;
		
		/*
		Qrot.x() = 0;
		Qrot.y() = -0.70711;
		Qrot.z() = 0;
		Qrot.w() = 0.70711;
		
		Q = Q * Qrot;
		*/
		
	}
   
	// tried: 1,0,2; 1,2,0; 0,2,1; 2,0,1; 2,1,0; 0,1,2
	// x-corresponds to graph -x; y to graph -z; z to graph -y
	
	// TRIED: 
	// 2,-0,-1
	// 2, 0, 1 (better)
	// 2, 0,-1
	// 2,-0, 1
	// 0, 1, 2
   
	if (mode == MAPPER_ASSIGN_MODE) {
		pPose.pose.position.x = t.at<double>(0,0); //;
		pPose.pose.position.y = t.at<double>(1,0); //t.at<double>(1,0);
		pPose.pose.position.z = t.at<double>(2,0); //t.at<double>(2,0);
	
	} else {
		
		pPose.pose.position.x = t.at<double>(2,0); //;
		pPose.pose.position.y = -t.at<double>(0,0); //t.at<double>(1,0);
		pPose.pose.position.z = -t.at<double>(1,0); //t.at<double>(2,0);
	   
	}
   
	if (abs(pPose.pose.position.x) > MAX_RVIZ_DISPLACEMENT) {
			pPose.pose.position.x = 0.0;
	}

	if (abs(pPose.pose.position.y) > MAX_RVIZ_DISPLACEMENT) {
			pPose.pose.position.y = 0.0;
	}
   
	if (abs(pPose.pose.position.z) > MAX_RVIZ_DISPLACEMENT) {
			pPose.pose.position.z = 0.0;
	}
   
	//printf("%s << QUAT = (%f, %f, %f, %f)", __FUNCTION__, Q.x(), Q.y(), Q.z(), Q.w());
   
	// tried x,y,z,w
	
	if (mode == MAPPER_ASSIGN_MODE) {
		pPose.pose.orientation.x = Q.x();
		pPose.pose.orientation.y = Q.y();
		pPose.pose.orientation.z = Q.z();
		pPose.pose.orientation.w = Q.w();
	} else {
		pPose.pose.orientation.x = Q.z();
		pPose.pose.orientation.y = -Q.x();
		pPose.pose.orientation.z = -Q.y();
		pPose.pose.orientation.w = Q.w();
	}
}

void convertPoseFormat(const cv::Mat& t, const Eigen::Quaternion<double>& Q, geometry_msgs::Pose& pose) {
	
	pose.position.x = t.at<double>(0,0);
	pose.position.y = t.at<double>(1,0);
	pose.position.z = t.at<double>(2,0);
	
	pose.orientation.w = Q.w();
	pose.orientation.x = Q.x();
	pose.orientation.y = Q.y();
	pose.orientation.z = Q.z();
	
}

void convertPoseFormat(const geometry_msgs::Pose& pose, cv::Mat& t, Eigen::Quaternion<double>& Q) {
	
	t = cv::Mat(3,1, CV_64FC1);
	
	t.at<double>(0,0) = pose.position.x;
	t.at<double>(1,0) = pose.position.y;
	t.at<double>(2,0) = pose.position.z;
	
	Q = Eigen::Quaternion<double>(pose.orientation.w, pose.orientation.x, pose.orientation.y, pose.orientation.z);
	
}

void convertAndShiftPoseFormat(const geometry_msgs::Pose& pose, cv::Mat& t, Eigen::Quaternion<double>& Q) {
	
	t = cv::Mat(3,1, CV_64FC1);
	
	t.at<double>(2,0) = pose.position.x;
	t.at<double>(0,0) = -pose.position.y;
	t.at<double>(1,0) = -pose.position.z;
	
	Q = Eigen::Quaternion<double>(pose.orientation.w, pose.orientation.x, -pose.orientation.y, -pose.orientation.z);
	
}

void convertAndShiftPoseFormat(const cv::Mat& t, const Eigen::Quaternion<double>& Q, geometry_msgs::Pose& pose) {
	
	pose.position.x = t.at<double>(2,0);
	pose.position.y = -t.at<double>(0,0);
	pose.position.z = -t.at<double>(1,0);
	
	pose.orientation.x = Q.z();
	pose.orientation.y = -Q.x();
	pose.orientation.z = -Q.y();
	pose.orientation.w = Q.w();
	
}
#endif

void convertRmatTo3frm(const cv::Mat& R_src, Matrix3frm& R_dst) {
	R_dst(0,0) = float(R_src.at<double>(0,0));
	R_dst(0,1) = float(R_src.at<double>(0,1));
	R_dst(0,2) = float(R_src.at<double>(0,2));
	
	R_dst(1,0) = float(R_src.at<double>(1,0));
	R_dst(1,1) = float(R_src.at<double>(1,1));
	R_dst(1,2) = float(R_src.at<double>(1,2));
	
	R_dst(2,0) = float(R_src.at<double>(2,0));
	R_dst(2,1) = float(R_src.at<double>(2,1));
	R_dst(2,2) = float(R_src.at<double>(2,2));
}

void convertTvecToEigenvec(const cv::Mat& T_src, Eigen::Vector3f& T_dst) {
	T_dst[0] = float(T_src.at<double>(0,0));
	T_dst[1] = float(T_src.at<double>(1,0));
	T_dst[2] = float(T_src.at<double>(2,0));
}

void convertEigenvecToTvec(const Eigen::Vector3f& T_src, cv::Mat& T_dst) {
	T_dst = cv::Mat::zeros(3,1,CV_64FC1);
	
	T_dst.at<double>(0,0) = T_src[0];
	T_dst.at<double>(1,0) = T_src[1];
	T_dst.at<double>(2,0) = T_src[2];
}

void convert3frmToRmat(const Matrix3frm& R_src, cv::Mat& R_dst) {
	R_dst = cv::Mat::eye(3,3,CV_64FC1);
	
	R_dst.at<double>(0,0) = R_src(0,0);
	R_dst.at<double>(0,1) = R_src(0,1);
	R_dst.at<double>(0,2) = R_src(0,2);
	
	R_dst.at<double>(1,0) = R_src(1,0);
	R_dst.at<double>(1,1) = R_src(1,1);
	R_dst.at<double>(1,2) = R_src(1,2);
	
	R_dst.at<double>(2,0) = R_src(2,0);
	R_dst.at<double>(2,1) = R_src(2,1);
	R_dst.at<double>(2,2) = R_src(2,2);
}

void projectionToTransformation(const cv::Mat& proj, cv::Mat& trans) {
	trans = cv::Mat::eye(4, 4, CV_64FC1);
	
	trans.at<double>(0,0) = proj.at<double>(0,0);
	trans.at<double>(0,1) = proj.at<double>(0,1);
	trans.at<double>(0,2) = proj.at<double>(0,2);
	trans.at<double>(0,3) = proj.at<double>(0,3);
	
	trans.at<double>(1,0) = proj.at<double>(1,0);
	trans.at<double>(1,1) = proj.at<double>(1,1);
	trans.at<double>(1,2) = proj.at<double>(1,2);
	trans.at<double>(1,3) = proj.at<double>(1,3);
	
	trans.at<double>(2,0) = proj.at<double>(2,0);
	trans.at<double>(2,1) = proj.at<double>(2,1);
	trans.at<double>(2,2) = proj.at<double>(2,2);
	trans.at<double>(2,3) = proj.at<double>(2,3);
	
	trans.at<double>(3,0) = 0.0;
	trans.at<double>(3,1) = 0.0;
	trans.at<double>(3,2) = 0.0;
	trans.at<double>(3,3) = 1.0;
	
}

void transformationToProjection(const cv::Mat& trans, cv::Mat& proj) {
	proj = cv::Mat::zeros(3, 4, CV_64FC1);
	
	proj.at<double>(0,0) = trans.at<double>(0,0);
	proj.at<double>(0,1) = trans.at<double>(0,1);
	proj.at<double>(0,2) = trans.at<double>(0,2);
	proj.at<double>(0,3) = trans.at<double>(0,3);
	
	proj.at<double>(1,0) = trans.at<double>(1,0);
	proj.at<double>(1,1) = trans.at<double>(1,1);
	proj.at<double>(1,2) = trans.at<double>(1,2);
	proj.at<double>(1,3) = trans.at<double>(1,3);
	
	proj.at<double>(2,0) = trans.at<double>(2,0);
	proj.at<double>(2,1) = trans.at<double>(2,1);
	proj.at<double>(2,2) = trans.at<double>(2,2);
	proj.at<double>(2,3) = trans.at<double>(2,3);
		
}

void projectionToRotation(const cv::Mat& src, cv::Mat& dst) {
	dst = cv::Mat(3, 3, CV_64FC1);
	
	dst.at<double>(0,0) = src.at<double>(0,0);
	dst.at<double>(0,1) = src.at<double>(0,1);
	dst.at<double>(0,2) = src.at<double>(0,2);

	dst.at<double>(1,0) = src.at<double>(1,0);
	dst.at<double>(1,1) = src.at<double>(1,1);
	dst.at<double>(1,2) = src.at<double>(1,2);
	
	dst.at<double>(2,0) = src.at<double>(2,0);
	dst.at<double>(2,1) = src.at<double>(2,1);
	dst.at<double>(2,2) = src.at<double>(2,2);
	
}

void rotationToProjection(const cv::Mat& src, cv::Mat& dst) {
	dst = cv::Mat(3, 4, CV_64FC1);
	
	dst.at<double>(0,0) = src.at<double>(0,0);
	dst.at<double>(0,1) = src.at<double>(0,1);
	dst.at<double>(0,2) = src.at<double>(0,2);
	
	dst.at<double>(1,0) = src.at<double>(1,0);
	dst.at<double>(1,1) = src.at<double>(1,1);
	dst.at<double>(1,2) = src.at<double>(1,2);
	
	dst.at<double>(2,0) = src.at<double>(2,0);
	dst.at<double>(2,1) = src.at<double>(2,1);
	dst.at<double>(2,2) = src.at<double>(2,2);
	
	dst.at<double>(0,3) = 0.0;	
	dst.at<double>(1,3) = 0.0;
	dst.at<double>(2,3) = 0.0;
	
}

void composeTransform(const cv::Mat& R, const cv::Mat& t, cv::Mat& c) {
	
	c = cv::Mat::zeros(4, 4, CV_64FC1);
	
	c.at<double>(0,0) = R.at<double>(0,0);
	c.at<double>(0,1) = R.at<double>(0,1);
	c.at<double>(0,2) = R.at<double>(0,2);

	c.at<double>(1,0) = R.at<double>(1,0);
	c.at<double>(1,1) = R.at<double>(1,1);
	c.at<double>(1,2) = R.at<double>(1,2);

	c.at<double>(2,0) = R.at<double>(2,0);
	c.at<double>(2,1) = R.at<double>(2,1);
	c.at<double>(2,2) = R.at<double>(2,2);

	c.at<double>(0,3) = t.at<double>(0,0);
	c.at<double>(1,3) = t.at<double>(1,0);
	c.at<double>(2,3) = t.at<double>(2,0);
	
	c.at<double>(3,0) = 0.0;
	c.at<double>(3,1) = 0.0;
	c.at<double>(3,2) = 0.0;
	c.at<double>(3,3) = 1.0;
}

void decomposeTransform(const cv::Mat& c, cv::Mat& R, cv::Mat& t) {
	
	R = cv::Mat::zeros(3, 3, CV_64FC1);
	t = cv::Mat::zeros(3, 1, CV_64FC1);

	R.at<double>(0,0) = c.at<double>(0,0);
	R.at<double>(0,1) = c.at<double>(0,1);
	R.at<double>(0,2) = c.at<double>(0,2);

	R.at<double>(1,0) = c.at<double>(1,0);
	R.at<double>(1,1) = c.at<double>(1,1);
	R.at<double>(1,2) = c.at<double>(1,2);

	R.at<double>(2,0) = c.at<double>(2,0);
	R.at<double>(2,1) = c.at<double>(2,1);
	R.at<double>(2,2) = c.at<double>(2,2);

	t.at<double>(0,0) = c.at<double>(0,3);
	t.at<double>(1,0) = c.at<double>(1,3);
	t.at<double>(2,0) = c.at<double>(2,3);

}


// http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/ethan.htm
void matrixToQuaternion(const cv::Mat& mat, Eigen::Quaternion<double>& quat) {

	// convert mat to Eigen::Matrix3d m;
	/*
	Eigen::Matrix3d m;

	m(0,0) = mat.at<double>(0,0);
	m(0,1) = mat.at<double>(0,1);
	m(0,2) = mat.at<double>(0,2);

	m(1,0) = mat.at<double>(1,0);
	m(1,1) = mat.at<double>(1,1);
	m(1,2) = mat.at<double>(1,2);

	m(2,0) = mat.at<double>(2,0);
	m(2,1) = mat.at<double>(2,1);
	m(2,2) = mat.at<double>(2,2);

	quat = Quaterniond(m);

	quat.normalize();

	return;
	*/
	// printf("%s << Entered...\n", __FUNCTION__);

	double m00 = mat.at<double>(0,0);
	double m01 = mat.at<double>(0,1);
	double m02 = mat.at<double>(0,2);

	double m10 = mat.at<double>(1,0);
	double m11 = mat.at<double>(1,1);
	double m12 = mat.at<double>(1,2);

	double m20 = mat.at<double>(2,0);
	double m21 = mat.at<double>(2,1);
	double m22 = mat.at<double>(2,2);

	double qw, qx, qy, qz;


	// ALT 3
	// J.M.P. van Waveren
	/*
	float m[9], q[4];
	m[0] = m00; m[1] = m01; m[2] = m02;
	m[3] = m10; m[4] = m11; m[5] = m12;
	m[6] = m20; m[7] = m21; m[8] = m22;

	//const float *m = jointMats[i].mat;
	if ( m[0 * 4 + 0] + m[1 * 4 + 1] + m[2 * 4 + 2] > 0.0f ) {
	float t = + m[0 * 4 + 0] + m[1 * 4 + 1] + m[2 * 4 + 2] + 1.0f;
	float s = ReciprocalSqrt( t ) * 0.5f;
	q[3] = s * t;
	q[2] = ( m[0 * 4 + 1] - m[1 * 4 + 0] ) * s;
	q[1] = ( m[2 * 4 + 0] - m[0 * 4 + 2] ) * s;
	q[0] = ( m[1 * 4 + 2] - m[2 * 4 + 1] ) * s;
	} else if ( m[0 * 4 + 0] > m[1 * 4 + 1] && m[0 * 4 + 0] > m[2 * 4 + 2] ) {
	float t = + m[0 * 4 + 0] - m[1 * 4 + 1] - m[2 * 4 + 2] + 1.0f;
	float s = ReciprocalSqrt( t ) * 0.5f;
	q[0] = s * t;
	q[1] = ( m[0 * 4 + 1] + m[1 * 4 + 0] ) * s;
	q[2] = ( m[2 * 4 + 0] + m[0 * 4 + 2] ) * s;
	q[3] = ( m[1 * 4 + 2] - m[2 * 4 + 1] ) * s;
	} else if ( m[1 * 4 + 1] > m[2 * 4 + 2] ) {
	float t = - m[0 * 4 + 0] + m[1 * 4 + 1] - m[2 * 4 + 2] + 1.0f;
	float s = ReciprocalSqrt( t ) * 0.5f;
	q[1] = s * t;
	q[0] = ( m[0 * 4 + 1] + m[1 * 4 + 0] ) * s;
	q[3] = ( m[2 * 4 + 0] - m[0 * 4 + 2] ) * s;
	q[2] = ( m[1 * 4 + 2] + m[2 * 4 + 1] ) * s;
	} else {
	float t = - m[0 * 4 + 0] - m[1 * 4 + 1] + m[2 * 4 + 2] + 1.0f;
	float s = ReciprocalSqrt( t ) * 0.5f;
	q[2] = s * t;
	q[3] = ( m[0 * 4 + 1] - m[1 * 4 + 0] ) * s;
	q[0] = ( m[2 * 4 + 0] + m[0 * 4 + 2] ) * s;
	q[1] = ( m[1 * 4 + 2] + m[2 * 4 + 1] ) * s;
	}

	qx = (double) q[0];
	qy = (double) q[1];
	qz = (double) q[2];
	qw = (double) q[3];
	*/

	// ALT 2
	// http://www.koders.com/cpp/fid38F83F165ABF728D6046FD59CF21ECF65E30E2D4.aspx
	/*
	double tmp = abs(m00 + m11 + m22 + 1);
	double s = sqrt(tmp);

	if (s > 0.0001) {
	qx = (m21 - m12) / 4*s;
	qy = (m02 - m20) / 4*s;
	qx = (m10 - m01) / 4*s;
	} else {
	int s_iNext[3] = { 2, 3, 1 };
	int i = 1;
	if ( m11 > m00 ) {
	i = 2;
	}

	if ( m22 > m11 ) {
	i = 3;
	}

	int j = s_iNext[i-1];
	int k = s_iNext[j-1];

	double fRoot = sqrt(mat.at<double>(i-1,i-1) - mat.at<double>(j-1,j-1) - mat.at<double>(k-1,k-1) + 1.0);

	double *tmp[3] = { &qx, &qy, &qz };
	*tmp[i-1] = 0.5 * fRoot;
	s = (mat.at<double>(k-1,j-1)-mat.at<double>(j-1,k-1))*fRoot;
	*tmp[j-1] = (mat.at<double>(j-1,i-1)+mat.at<double>(i-1,j-1))*fRoot;
	*tmp[k-1] = (mat.at<double>(k-1,i-1)+mat.at<double>(i-1,k-1))*fRoot;
	}

	if ((qx*qx + qy*qy + qz*qz) != 1.0) {
	qw = sqrt(1 - qx*qx - qy*qy - qz*qz);
	} else {
	qw = 0.0;
	}
	*/

	// ALT 1

	double tr1 = 1.0 + m00 - m11 - m22;
	double tr2 = 1.0 - m00 + m11 - m22;
	double tr3 = 1.0 - m00 - m11 + m22;



	//printf("%s << tr1, tr2, tr3 = (%f, %f, %f)\n", __FUNCTION__, tr1, tr2, tr3);

	if ((tr1 > tr2) && (tr1 > tr3)) {
	double S = sqrt(tr1) * 2.0; // S=4*qx
	//printf("%s << Case 1: %f\n", __FUNCTION__, S);
	qw = (m21 - m12) / S;
	qx = 0.25 * S;
	qy = (m01 + m10) / S;
	qz = (m02 + m20) / S;
	} else if ((tr2 > tr1) && (tr2 > tr3)) {
	double S = sqrt(tr2) * 2.0; // S=4*qy
	//printf("%s << Case 2: %f\n", __FUNCTION__, S);
	qw = (m02 - m20) / S;
	qx = (m01 + m10) / S;
	qy = 0.25 * S;
	qz = (m12 + m21) / S;
	} else if ((tr3 > tr1) && (tr3 > tr2)) {
	double S = sqrt(tr3) * 2.0; // S=4*qz
	//printf("%s << Case 3: %f\n", __FUNCTION__, S);
	qw = (m10 - m01) / S;
	qx = (m02 + m20) / S;
	qy = (m12 + m21) / S;
	qz = 0.25 * S;
	} else {
	//printf("%s << Case 4\n", __FUNCTION__);
	qw = 1.0;
	qx = 0.0;
	qy = 0.0;
	qz = 0.0;
	}

	



	quat.x() = qx;
	quat.y() = qy;
	quat.z() = qz;
	quat.w() = qw;

	//printf("%s << Quat = (%f, %f, %f, %f)\n", __FUNCTION__, qx, qy, qz, qw);

	quat.normalize();

	//printf("%s << Completed.\n", __FUNCTION__);
}

void quaternionToMatrix(const Eigen::Quaternion<double>& quat, cv::Mat& mat, bool handedness) {
	
	//QuaternionDbl eQuat;
	
	// eQuat.w()

	Eigen::Matrix3d eMat = quat.matrix();
	
	mat = cv::Mat::zeros(3, 3, CV_64FC1);
	
	if (handedness) {
		mat.at<double>(0,0) = eMat(0,0);
		mat.at<double>(0,1) = eMat(0,2);
		mat.at<double>(0,2) = eMat(0,1);
		
		mat.at<double>(1,0) = eMat(2,0);
		mat.at<double>(1,1) = eMat(2,2);
		mat.at<double>(1,2) = eMat(2,1);
		
		mat.at<double>(2,0) = eMat(1,0);
		mat.at<double>(2,1) = eMat(1,2);
		mat.at<double>(2,2) = eMat(1,1);
	} else {
		// Original
		mat.at<double>(0,0) = eMat(0,0);
		mat.at<double>(0,1) = eMat(0,1);
		mat.at<double>(0,2) = eMat(0,2);
		
		mat.at<double>(1,0) = eMat(1,0);
		mat.at<double>(1,1) = eMat(1,1);
		mat.at<double>(1,2) = eMat(1,2);
		
		mat.at<double>(2,0) = eMat(2,0);
		mat.at<double>(2,1) = eMat(2,1);
		mat.at<double>(2,2) = eMat(2,2);
	}
	
	
	// Quaternion.toRotationMatrix()

}

void getWandZ(cv::Mat& W, cv::Mat& Winv, cv::Mat& Z) {
	W = cv::Mat::zeros(3, 3, CV_64FC1);
	
	W.at<double>(0,0) = 0.0;
	W.at<double>(0,1) = -1.0;
	W.at<double>(0,2) = 0.0;
	
	W.at<double>(1,0) = 1.0;
	W.at<double>(1,1) = 0.0;
	W.at<double>(1,2) = 0.0;
	
	W.at<double>(2,0) = 0.0;
	W.at<double>(2,1) = 0.0;
	W.at<double>(2,2) = 1.0;
	
	Winv = cv::Mat::zeros(3, 3, CV_64FC1);
	
	Winv.at<double>(0,0) = 0.0;
	Winv.at<double>(0,1) = 1.0;
	Winv.at<double>(0,2) = 0.0;
	
	Winv.at<double>(1,0) = -1.0;
	Winv.at<double>(1,1) = 0.0;
	Winv.at<double>(1,2) = 0.0;
	
	Winv.at<double>(2,0) = 0.0;
	Winv.at<double>(2,1) = 0.0;
	Winv.at<double>(2,2) = 1.0;	
	
	Z = cv::Mat::zeros(3, 3, CV_64FC1);
	
	Z.at<double>(0,1) = 1.0;
	Z.at<double>(1,0) = -1.0;
}

void findP1Matrix(cv::Mat& P1, const cv::Mat& R, const cv::Mat& t) {
	//cv::Mat W, Winv, Z;
	
	//getWandZ(W, Z);
	//Winv = W.inv();

	//R = svd.u * W * svd.vt; //HZ 9.19
	//t = svd.u.col(2); //u3
	
	P1 = cv::Mat(3, 4, CV_64FC1);
			
	P1.at<double>(0,0) = R.at<double>(0,0);
	P1.at<double>(0,1) = R.at<double>(0,1);
	P1.at<double>(0,2) = R.at<double>(0,2);
	P1.at<double>(0,3) = t.at<double>(0,0);
	
	P1.at<double>(1,0) = R.at<double>(1,0);
	P1.at<double>(1,1) = R.at<double>(1,1);
	P1.at<double>(1,2) = R.at<double>(1,2);
	P1.at<double>(1,3) = t.at<double>(1,0);
	
	P1.at<double>(2,0) = R.at<double>(2,0);
	P1.at<double>(2,1) = R.at<double>(2,1);
	P1.at<double>(2,2) = R.at<double>(2,2);
	P1.at<double>(2,3) = t.at<double>(2,0);
}

void getTranslationBetweenCameras(cv::Mat& C1, cv::Mat& C2, double *translations) {
	cv::Mat CD = C2 - C1;
	translations[0] = CD.at<double>(0,3);
	translations[1] = CD.at<double>(1,3);
	translations[2] = CD.at<double>(2,3);
}

void initializeP0(cv::Mat& P) {
	P = cv::Mat::zeros(3, 4, CV_64FC1);
	P.at<double>(0,0) = 1.0;
	P.at<double>(0,1) = 0.0;
	P.at<double>(0,2) = 0.0;
	P.at<double>(0,3) = 0.0;
	P.at<double>(1,0) = 0.0;
	P.at<double>(1,1) = 1.0;
	P.at<double>(1,2) = 0.0;
	P.at<double>(1,3) = 0.0;
	P.at<double>(2,0) = 0.0;
	P.at<double>(2,1) = 0.0;
	P.at<double>(2,2) = 1.0;
	P.at<double>(2,3) = 0.0;
}

void transfer3dPoint(const cv::Point3d& src, cv::Point3d& dst, const cv::Mat& C) {
	
	cv::Mat pt1, pt2;
	
	pt1 = cv::Mat::zeros(4, 1, CV_64FC1);
	pt2 = cv::Mat::zeros(4, 1, CV_64FC1);
	
	pt1.at<double>(3, 0) = 1.0;
	pt2.at<double>(3, 0) = 1.0;

	
	pt1.at<double>(0,0) = src.x;
	pt1.at<double>(1,0) = src.y;
	pt1.at<double>(2,0) = src.z;
	
	pt2 = C * pt1;
	
	dst = cv::Point3d(pt2.at<double>(0,0), pt2.at<double>(1,0), pt2.at<double>(2,0));
	
}

void transfer3DPoints(const std::vector<cv::Point3d>& src, std::vector<cv::Point3d>& dst, const cv::Mat& C) {
	
	cv::Mat pt1, pt2;
	
	pt1 = cv::Mat::zeros(4, 1, CV_64FC1);
	pt2 = cv::Mat::zeros(4, 1, CV_64FC1);
	
	pt1.at<double>(3, 0) = 1.0;
	pt2.at<double>(3, 0) = 1.0;
	
	cv::Point3d shiftedPoint;
		
	for (unsigned int iii = 0; iii < src.size(); iii++) {
		pt1.at<double>(0,0) = src.at(iii).x;
		pt1.at<double>(1,0) = src.at(iii).y;
		pt1.at<double>(2,0) = src.at(iii).z;
		
		pt2 = C * pt1;		// pt2 = C.inv() * pt1;
		
		//printf("%s << pt2.at<double>(3,0) = %f\n", __FUNCTION__, pt2.at<double>(3,0));
		
		shiftedPoint.x = pt2.at<double>(0,0);
		shiftedPoint.y = pt2.at<double>(1,0);
		shiftedPoint.z = pt2.at<double>(2,0);
		
		dst.push_back(shiftedPoint);
	}
	
	
	
}

double getQuaternionAngle(const Quaterniond& q1, const Quaterniond& q2) {
	double angle, dot_prod;
	
	dot_prod = dotProduct(q1, q2);
	
	angle = 2.0 * acos(abs(dot_prod));
	
	return angle;
}

double dotProduct(const Quaterniond& q1, const Quaterniond& q2) {
	double prod;
	
	prod = q1.x()*q2.x() + q1.y()*q2.y() + q1.z()*q2.z() + q1.w()*q2.w();
	
	return prod;
}

void combineTransforms(cv::Mat& CN, const cv::Mat& C0, const cv::Mat& C1) {
	CN = cv::Mat::eye(4, 4, CV_64FC1);
	
	CN = C0 * C1;
}

double dotProduct(const cv::Mat& vec1, const cv::Mat& vec2) {
	double prod;
	
	prod = vec1.at<double>(0,0)*vec2.at<double>(0,0) + vec1.at<double>(0,0)*vec2.at<double>(0,0) + vec1.at<double>(0,0)*vec2.at<double>(0,0);

	return prod;

}

double getRotationInDegrees(const cv::Mat& R) {
	Quaterniond rotation;
	matrixToQuaternion(R, rotation);
	Quaterniond dq = defaultQuaternion();
	double qa = getQuaternionAngle(rotation, dq) * 180.0 / M_PI;
	
	return qa;

}

double getDistanceInUnits(const cv::Mat& t) {
	double retVal = 0.0;
	
	retVal += pow(t.at<double>(0,0), 2.0);
	retVal += pow(t.at<double>(1,0), 2.0);
	retVal += pow(t.at<double>(2,0), 2.0);
	retVal = pow(retVal, 0.5);
	
	return retVal;
}

void convertVec4dToMat(const Vector4d& vec4, cv::Mat& mat) {
	mat = cv::Mat::zeros(3, 1, CV_64FC1);
	
	mat.at<double>(0,0) = vec4.x();
	mat.at<double>(1,0) = vec4.y();
	mat.at<double>(2,0) = vec4.z();
	
}
	
#endif
