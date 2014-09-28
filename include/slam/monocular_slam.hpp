/*! \file	monocular_slam.hpp
 *  \brief	Declarations for monocular slam.
*/

#ifndef _THERMALVIS_MONOCULAR_SLAM_H_
#define _THERMALVIS_MONOCULAR_SLAM_H_

#include "core/program.hpp"

#include "core/general_resources.hpp"
#include "core/ros_resources.hpp"
#include "core/opencv_resources.hpp"

#include "core/tracks.hpp"
	
#include "core/launch.hpp"
#include "core/timing.hpp"

#include "slam/geometry.hpp"
#include "slam/keyframes.hpp"
#include "slam/reconstruction.hpp"
#include "slam/visualization.hpp"

#ifdef _BUILD_FOR_ROS_
#include "slamConfig.h"
#endif

#include "slam_config.hpp"

#ifdef _USE_BOOST_
#include "boost/thread/mutex.hpp" 
#include "boost/thread.hpp"
#else
#include <mutex>
#include <thread>
#endif

#ifdef _USE_PCL_
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#endif

/// \brief		Stores configuration information for the monocular slam routine
class slamData : public slamSharedData, public commonData, public slamLaunchOnlyData {
	friend class xmlParameters;
	friend class featureTrackerNode;
#ifndef _BUILD_FOR_ROS_
	friend class slamConfig;
#endif

protected:
	
	// ...

public:
	slamData();

	bool assignFromXml(xmlParameters& xP);

#ifdef _BUILD_FOR_ROS_
	bool obtainStartingData(ros::NodeHandle& nh);   
#endif	
};

#ifndef _BUILD_FOR_ROS_
/// \brief		Substitute for ROS live configuration adjustments
class slamConfig : public slamRealtimeData {
public:
	slamConfig() { }
	bool assignStartingData(slamData& startupData);
};
#endif

/// \brief		Manages the monocular slam procedure
class slamNode : public GenericOptions {
private:
	
#ifdef _BUILD_FOR_ROS_
	ros::NodeHandle private_node_handle;
	dynamic_reconfigure::Server<thermalvis::slamConfig> server;
	dynamic_reconfigure::Server<thermalvis::slamConfig>::CallbackType f;
#endif

	slamData configData;

	bool isTracking, f1, evaluationCompleted;
	unsigned int baseConnectionNum;
	double **scorecardParams;
	ofstream evaluationStream;
	unsigned int nextFrame;
	unsigned int putativelyEstimatedFrames;

	cv::Mat eye4;
	bool repetitionNoted;
	ofstream error_file;
	char error_filename[256];

	double bestInitializationScore; //!< Stores the best initialization score achieved between potential initialization frames so far
	
	bool firstIteration;
	
	cv::Mat blank;
	
	bool structureFormed;
	
	int latestFrame;
	
	bool infoProcessed;
	
	struct timeval cycle_timer;
	double elapsedTime;
	
	double distanceConstraint;

#ifdef _BUILD_FOR_ROS_
	ros::Timer timer;
	ros::Subscriber tracks_sub, info_sub, pose_sub;
	ros::Publisher path_pub, camera_pub, points_pub, pose_pub, confidence_pub;
	dynamic_reconfigure::Server<thermalvis::monoslamConfig> server;
	dynamic_reconfigure::Server<thermalvis::monoslamConfig>::CallbackType f;
	char pose_pub_name[256];
	
	
	sensor_msgs::PointCloud2 pointCloud_message;
	
#endif

	ros::Header frameHeaderHistoryBuffer[MAX_HISTORY];

	geometry_msgs::PoseStamped poseHistoryBuffer[MAX_HISTORY];
	
	geometry_msgs::PoseStamped keyframePoses[MAX_POSES_TO_STORE];
	geometry_msgs::PoseStamped savedPose, currentPose, pnpPose;

#ifdef _USE_SBA_
	SysSBA sys, display_sys;
#endif
	
	cv::Point3d cloudCentroid[MAX_SEGMENTS], cloudStdDeviation[MAX_SEGMENTS];
	keyframeStore keyframe_store;
	cv::Mat F_arr[MAX_FRAMES], H_arr[MAX_FRAMES];
	
	cv::Mat ACM[MAX_FRAMES], ACM2[MAX_FRAMES];
	
	cv::Mat keyframeTestScores, keyframeTestFlags;

	int currentPoseIndex;
	
	double minimumKeyframeScore;
	
	vector<unsigned int> framesReceived;
	
	unsigned int lastBasePose;
	
	unsigned int startingTracksCount;
	
	timeAnalyzer trackHandlingTime;
	timeAnalyzer triangulationTime;
	timeAnalyzer poseEstimationTime;
	timeAnalyzer bundleAdjustmentTime;

	// Thread-protection
	boost::mutex cam_mutex;
	boost::mutex tracks_mutex;
	boost::mutex keyframes_mutex;
	boost::mutex main_mutex;
	
	

	// From videoslam:
	
	std::ofstream lStream;
	std::streambuf* lBufferOld;
	
	cv::Mat extrinsicCalib_T, extrinsicCalib_R, extrinsicCalib_P;
	//QuaternionDbl extrinsicCalib_quat;
	
	unsigned int frameProcessedCounter;
	
	bool hasTerminatedFeed;
	
	double distanceTravelled;
	
	int framesArrived, framesProcessed, pnpSuccesses, baSuccesses;
	double baAverage, dsAverage;
	
#ifdef _USE_PCL_
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_ptr_;
#endif

	unsigned int frameHeaderHistoryCounter, poseHistoryCounter;
	
	bool keyframeTypes[MAX_POSES_TO_STORE];
	unsigned int storedPosesCount;
	
	
	
	double latestTracksTime, pointShift, pnpError, pnpInlierProp;
	
	int lastTestedFrame, usedTriangulations;	
	
	double predictiveError, bundleTransShift, bundleRotShift;
	
	bool latestReceivedPoseProcessed;
	
public:

	vector<featureTrack> *featureTrackVector;

	///brief	Processes online changes in node configuration and applies them.
#ifdef _BUILD_FOR_ROS_
	void serverCallback(thermalvis::slamConfig &config, uint32_t level);
#else
	void serverCallback(slamConfig &config);
#endif

	///brief	Constructor. Must receive configuration data.
#ifdef _BUILD_FOR_ROS_
	slamNode(ros::NodeHandle& nh, slamData startupData);
#else
	slamNode(slamData startupData);
#endif

	///brief	Sets the debug mode - if true, images and data for visualization will be displayed to the user. 
	void setDebugMode(bool val) { 
		debugMode = val;
		configData.debugMode = val; 
	}

	///brief	Performs single loop/iteration?
#ifdef _BUILD_FOR_ROS_
	void main_loop(const ros::TimerEvent& event);
#else
	void main_loop(sensor_msgs::CameraInfo *info_msg, const vector<featureTrack>* msg);
#endif
	
	///brief	Prepares node for termination.
	void prepareForTermination();

	void assignPose(geometry_msgs::PoseStamped& pPose, cv::Mat& C);

	void estimatePose(vector<unsigned int>& basisNodes, unsigned int idx);
	
	void getBasisNodes(vector<unsigned int>& basisNodes, unsigned int idx);
	
#ifdef _BUILD_FOR_ROS_
	void handle_tracks(const thermalvis::feature_tracksConstPtr& msg);
#else
	void handle_tracks(const vector<featureTrack>* msg);
#endif

#ifdef _BUILD_FOR_ROS_
	void integrateNewTrackMessage(const thermalvis::feature_tracksConstPtr& msg);
#else
	void integrateNewTrackMessage(const vector<featureTrack>* msg);
#endif

	bool evaluationSummaryAndTermination();

	void videoslamPoseProcessing();

#ifdef _BUILD_FOR_ROS_
	void handle_info(const sensor_msgs::CameraInfoConstPtr& info_msg);
#else
	void handle_info(sensor_msgs::CameraInfo *info_msg);
#endif
	
	void handle_pose(const geometry_msgs::PoseStamped& pose_msg);

	bool updateKeyframePoses(const geometry_msgs::PoseStamped& pose_msg, bool fromICP = true);

	// Routines
	bool formInitialStructure();
	bool performKeyframeEvaluation();
	void refreshPoses();
	bool checkForKeyframe();
	void processNextFrame();
	void update_display();
	void show_poses();
	void update_cameras_to_pnp();
	double assignStartingFrames(unsigned int best_iii, unsigned int best_jjj, double* keyframe_scores, cv::Mat& startingTrans);
	
	void getGuidingPose(cv::Mat *srcs, cv::Mat& dst, unsigned int idx);

	///brief	Tests whether latest received frame has sufficient motion from other potential initialization frames to also be considered
	bool isCurrentFramePotentialInitializor();

	///brief	Using current frame, test initialization performance with existing potential initialization frames
	void testInitializationWithCurrentFrame();

	///brief	Chooses the indices of the best frames for structure initialization that have so far been found
	void selectBestInitializationPair();
	
	void clearSystem();
	
	bool processScorecard();
	
	///brief	Gets current tracking status
	bool stillTracking() { return isTracking; }

	// FROM VIDEOSLAM

	
	bool updateLocalPoseEstimates();
	
#ifdef _BUILD_FOR_ROS_
	//void publishPoints(const geometry_msgs::PoseStamped& pose_msg);
	void publishPoints(ros::Time stamp, unsigned int seq);
#endif

	bool determinePose();
	
	void publishPose();
	
	bool findNearestPoses(int& index1, int& index2, ros::Time& targetTime);
	
	void triangulatePoints();
	

	bool checkConnectivity(unsigned int seq);
	
	
	
	void trimFeatureTrackVector();

	
};

#endif
